""" Parses the roget thesaurus and makes it accessible through an API.
"""
import sys
import re
import os
import time
import cgi

__all__ = [ 'RogetBuilder', 'RogetThesaurus', 'RogetNode', 'Sense', 'HeadWord', 'RogetThesaususFormatterText', 'RogetThesaurusFormatterXML'  ]


try:
    import cPickle as pickle
except Exception:
    print 'failed to load cPicle'


class RogetBuilder:
    '''
	The main entry point of this library; builds an instances of RogetThesaurus
    '''    
    _RECURSION_LIMIT = 4500
    _VERBOSE = 1
    
    #_wordGroupBoundaryRe = re.compile( '((\&amp;c\s+(\([^\)]+\))?\s*[^\s\,\;]+\.?|\[[^]]+\]|[^;])+)' )   
    _wordGroupBoundaryRe = re.compile( '((\&amp;c\s+(\([^\)]+\))?\s*[^\s^\,^\;^\.]+|\[[^]]+\]|\.(?!\n)|[^\;^\.])+)' )    
    _wordBoundaryRe = re.compile( '((\&amp;c\s+(\([^\)]+\))?\s*[^\s^\,^\;^\.]+\.?|\[[^]]+\]|[^,])+)' )  
    _startHeadWordRe = re.compile("^\s*([0-9]+[a-z]*)?.\s*(\[[^\]]*\])?([^\-]+)\-")
    _linkRe = re.compile( '\&amp;c\s+(\([^\)]+\))?\s*([^\s^\,^\;^\.]+)\.?' )
    _commentRe = re.compile('\[([^\]]*)\]')
    _attributeRe = re.compile('(N\.|Adj\.|Adv\.|V\.|Phr\.)')
    _cleanupRe = re.compile('[\^\n]')
    _cleanupRe2 = re.compile('\s\s+')
    _numRe = re.compile('(\d+)')

    _headWordIndex = {}
    _senseIndex = {}
    _lastHeadIndex  = None

    def __init__(self, verbose = 0):
	self._VERBOSE = verbose
 
    def _resolveReference( self, node ):
	if node.type == ROGET_NODE_HEADWORD or node.type == ROGET_NODE_SENSE:
	    if node.link != None:
		if not node.link in self._headWordIndex:
		    die("word: " + node.key   + " unresolved link: " + str( node.link ) )
		node.link = self._headWordIndex[ node.link ]		    

	if node.type == ROGET_NODE_HEADWORD or node.type == ROGET_NODE_SENSE:
	    if node.key == '' and node.link != None:
		node.key = node.link.key
	    if node.key not in self._senseIndex:
		self._senseIndex[ node.key ] = [] 
	    self._senseIndex[ node.key ].append( node )    

	for n  in node.child:
	    self._resolveReference( n )

    def _parseWord(self, word, text ):
	textCopy = text

	n = self._commentRe.search( text )
	if n:
	    commentText = n.group(1).strip()
	    if self._cleanupRe.search( commentText ):
		commentText = self._cleanupRe.sub( '', commentText )
	    word.comment = commentText.strip()
	    text = self._commentRe.sub( '', text )
	n = self._linkRe.search( text )
	if n:
	    attribValue = n.group(1)
	    linkValue = n.group(2)

	    if attribValue == None and linkValue == None:
		die('Bad link: ' + text)

	    if attribValue == None:
		attribValue = linkValue
		linkValue = None

	    if attribValue != None:
		lowerAttrib = attribValue.lower()
		if lowerAttrib == 'adj' or  lowerAttrib == 'adj.':
		    word.wordType = WORD_TYPE_ADJ 
		elif lowerAttrib == 'v' or lowerAttrib == 'v.':
		    word.wordType = WORD_TYPE_VERB 	    
		elif lowerAttrib == 'adv' or lowerAttrib == 'adv.':
		    word.wordType  = WORD_TYPE_ADVERB
		elif lowerAttrib == 'n':
		    word.wordType = WORD_TYPE_NOUN 	   
		elif lowerAttrib == 'phr':
		    word.wordType = WORD_TYPE_PHRASE
		elif lowerAttrib[0].isdigit():
		    linkValue = lowerAttrib

	    if linkValue != None:
		word.link = linkValue.strip() 
	    text = self._linkRe.sub( '', text)  
	
	n = self._attributeRe.search( text )
	if n:
	    val = n.group(1)
	    if val == 'V.':
		word.wordType = WORD_TYPE_VERB 
	    elif val == 'N.':
		word.wordType = WORD_TYPE_NOUN 
	    elif val == 'Adj.':
		word.wordType = WORD_TYPE_ADJ 
	    elif val == 'Adv.':
		word.wordType = WORD_TYPE_ADVERB
	    elif val == 'Phr.':
		word.wordType = WORD_TYPE_PHRASE
	    else:
		die(val + " -- " + text)
	    text = self._attributeRe.sub( '', text )

	#n = self.linkType2.search( text )
	#if n:
	#    # TODO: extract link type
	#    text = self.linkType2.sub( '', text )	

	if self._cleanupRe.search( text ):
	    text = self._cleanupRe.sub( ' ', text )
	
	if self._cleanupRe2.search( text ):
	    text = self._cleanupRe2.sub( ' ', text )

	word.key = text.strip()

	if word.key == '' and word.link == '':
	    die('empty word : ' + textCopy)
	#print word.key

    def _parseHeadWords(self, node, passage ):

	#match = self._startHeadWordRe.match( passage )
	matchPos = passage.find('--')  
	if matchPos != -1: 
	    try:
		matchPos += 1
		headDef = passage[ :matchPos ]
	        match = self._startHeadWordRe.match( headDef )
		if match:
		    headWord = HeadWord( match.group(1), node )
		    self._parseWord( headWord, match.group(3) )

		    if match.group(2) != None:
			headWord._linkComment = match.group(2).strip()
		    
		    matchPos += 1
		    passage = passage[ matchPos: ]

		    self._headWordIndex[ headWord.index ] = headWord
	    
		    n = self._numRe.match( headWord.index )
		    if n != None: 
			if self._lastHeadIndex != None and (int(n.group(1)) - int(self._lastHeadIndex)) > 1:
			    die('last index ' + self._lastHeadIndex + 'current index: ' + n.group(1) )
			self._lastHeadIndex = n.group(1)

		    #parse word groups
		    groups = self._wordGroupBoundaryRe.findall( passage )
		    for g in groups:
			gr = g[0].strip()
			if gr =='':
			    continue
			#print '->', gr
			wgroup = re.findall( self._wordBoundaryRe, gr )
			if  len(wgroup) > 1:
			    relatedWords = RogetNode( ROGET_NODE_SENSE_GROUP, None, headWord)  
			    for wg in wgroup:
				w = Sense( ROGET_NODE_SENSE, relatedWords )
				self._parseWord( w, wg[0] )
			else:
			    w  = Sense( ROGET_NODE_SENSE, headWord )
			    for wg in wgroup:
				self._parseWord( w, wg[0] )

			#for w in wgroup:
			#    print "\t\t$" , w[0] , "$"		    

	    except Exception:
		print  'Error during pasing: ', passage
		raise
  
    def parse(self):
	'''
	    parse the roget thesaursus
	    returns an instance of RogetThesaurus 

	    Note that that file 10681-body.txt  must be in the same directory as the script roget.py 
	'''	    
	fpath = os.path.split( __file__ )
	rpath = os.path.join( fpath[0], '10681-body.txt' )
	if os.access( rpath, os.F_OK | os.R_OK ) ==  0:
	    die("Roget thesaursus text file rpath has not been found")

	if self._VERBOSE != 0:
	   print "parsing file: ", rpath
	   tm = time.clock()

	root = RogetNode(ROGET_NODE_CATEGORY, 'root')  
	classMatch = re.compile("^CLASS");
	divisionMatch = re.compile("^DIVISION")
	sectionMatch = re.compile("^SECTION")
	subsectionMatch = re.compile("^[0-9]+\.? [A-Z][A-Z,\s]+")

	currentNode = None
	currentClass = None
	currentDivision = None
	currentSection = None
	currentSubSection = None
	
	passage = ''
	passageLines = 0
	
	with open( rpath ) as f:
	    for line in f:
		if line.strip() != '':
		    passage += line 
		    passageLines += 1
		elif line.strip() == '':
		    if 'End of of E-Thesaurus' in passage:
			break
		    
		    if classMatch.match( passage ):
			lines = passage.split( '\n' )

			currentNode = RogetNode( ROGET_NODE_CATEGORY, lines[0], root )
			currentNode.key = lines[1].strip()
			currentClass = currentNode
			currentDivision = None
			currentSection = None
			currentSubSection = None

		    elif divisionMatch.match( passage ):
			lines = passage.split( '\n' )
			currentNode = RogetNode( ROGET_NODE_CATEGORY, lines[0], currentClass )
			currentNode.key = lines[1].strip()
			currentDivision = currentNode
			currentSection = None
			currentSubSection = None

		    else:
			if currentNode != None:
			    if sectionMatch.match( passage ):
				lines = passage.split( '\n' )


				if currentDivision != None:
				    currentNode = RogetNode( ROGET_NODE_CATEGORY, lines[0], currentDivision )
				else:			
				    currentNode = RogetNode( ROGET_NODE_CATEGORY, lines[0], currentClass )
				currentNode.key = lines[1].strip()
				currentSection = currentNode
				currentSubSection = None

			    elif subsectionMatch.match( passage ):
				lines = passage.split( '\n' )
				currentNode = RogetNode( ROGET_NODE_CATEGORY, None, currentSection )
				currentSubSection = currentNode
				currentNode.key = lines[0].strip()
			    else:
				if passageLines == 1 and not '--' in passage:
				    if currentSubSection != None:
					currentNode = RogetNode( ROGET_NODE_CATEGORY, None, currentSubSection )
				    else:
				       currentNode = RogetNode( ROGET_NODE_CATEGORY, None, currentSection )
				    currentNode.key = passage.strip()
				else:
				    self._parseHeadWords( currentNode, passage )
		    passage = ''	
		    passageLines = 0

	self._resolveReference( root )
	
	if self._VERBOSE != 0:
	    tm = time.clock() - tm
	    print "time to parse file: ", tm

	return RogetThesaurus(root,self._headWordIndex, self._senseIndex)		

    def load(self, file ):
	'''
	loads an instance of roget thesaurus (if possible from pickled/serialized form)

	if file does not exist
	    parse roget thesaursus 
	    store pickled form to file
	else
	    load pickled form from file
	returns instance of RogetThesaurus		

	don't use this! surprisingly it takes less time to parse it from the text file.
	(even with this inefficient parser)

	Reason for this seems to be that pickled format is much larger then text file;
	pickle adds the type of the class as first element of sexpression - 
	so there is a lot of redundancy and pickled file is much larger than text file.		
	'''
	res = None
	if os.access( file, os.F_OK | os.R_OK ):
	    print 'file exists: ', file
	    res = self._loadFromFile( file )
	
	if res == None: 	    
	    print 'load from file; current recursion limit ', sys.getrecursionlimit() 
	    
	    if self._VERBOSE != 0:
		print "Parsing from text: ", file
		tm = time.clock()
	    res = self.parse()
	    if self._VERBOSE != 0:
		tm = time.clock() - tm
		print "time to parse from text: ", tm
	    self._storeToFile( file, res )
	
	return res	    

    def _loadFromFile( self, file ):
	try:
	    r = RogetThesaurus()
	    if self._VERBOSE != 0:
	       print "Load from file: ", file
	       tm = time.clock()
	    curRecursionLimit = sys.getrecursionlimit()
	    if curRecursionLimit < self.RECURSION_LIMIT:
		sys.setrecursionlimit( self.RECURSION_LIMIT )
	    with open( file ) as f:
		ret = pickle.load( f )
		if curRecursionLimit < self.RECURSION_LIMIT:
		    sys.setrecursionlimit( curRecursionLimit )	    
		if self._VERBOSE != 0:
		    tm = time.clock() - tm
		print "time to load from file: ", tm
		return ret
	except Exception:
	    print 'Error while loading thesaurus', e
	    return None

    def _storeToFile( self, file, r ):
	try:
	    if self._VERBOSE != 0:
	       print "Storing to file: ", file
	       tm = time.clock()
	    curRecursionLimit = sys.getrecursionlimit()
	    if curRecursionLimit < self.RECURSION_LIMIT:
		sys.setrecursionlimit( self.RECURSION_LIMIT )
	    print 'storing to ' +  file
	    with open( file, 'w') as f:
		pickle.dump( r, f )
	    if curRecursionLimit < self.RECURSION_LIMIT:
		sys.setrecursionlimit( curRecursionLimit )	    
	    if self._VERBOSE != 0:
		tm = time.clock() - tm
		print "time to store to file: ", tm
	except Exception:
	    print 'Error while storing thesaurus', e
	    os.remove( file )		
	    return None


ROGET_NODE_CATEGORY = 1
ROGET_NODE_HEADWORD = 2
ROGET_NODE_SENSE_GROUP = 4
ROGET_NODE_SENSE     = 8

class RogetNode:
    ''' 
	RogetNode - the base class of all nodes maintained by Roget thesaurus
    '''    
    def __init__(self, type, description, parent = None):
	self._type = type 
	if description != None:
	    self._description = description.strip()
	else:
	    self._description = None
	self._parent = parent
	self._child = []
	self._key = ''
	if parent != None:
	    parent._addChild( self )
	    #print "addChild ", description, "parent ",parent.description

    def _addChild(self, nchild):
	self._child.append( nchild )		

    def toString(self):
	ret = self.typeToString()
	if self._description != None:
	    ret += " - " + self.description
	ret += " ("  + self.key + ")"
	return ret
 
    def typeToString(self):
	''' returns the type o this node as a string '''
	if self._type == ROGET_NODE_CATEGORY:
	    return "Category"
	elif self._type == ROGET_NODE_HEADWORD:
	    return "Headword"
	elif self._type == ROGET_NODE_SENSE_GROUP:
	    return "SenseGroup"
	elif self._type == ROGET_NODE_SENSE:
	    return "Sense"

    @property
    def type(self):
	''' returns the type of this node as a integer '''
	return self._type

    @property
    def key(self):
	''' the meaning/key of this node '''
	return self._key

    @property
    def description(self):
	''' returns an optional description (in the text this appears as [ .... ] ) '''
	return self._description

    @property
    def parent(self):
	''' returns the parent node (one up in the ontology) '''
	return self._parent

    @property 
    def child(self):
	''' returns the array of child nodes '''
	return self._child

	

WORD_TYPE_NONE = 0
WORD_TYPE_VERB = 1
WORD_TYPE_NOUN = 2
WORD_TYPE_ADJ  = 3
WORD_TYPE_ADVERB  =  4
WORD_TYPE_PHRASE = 5


 
class Sense(RogetNode):
    '''
	a single sense (the leaf node of the Roget Thesaurus
    '''       
    def __init__(self, parent):
	self.__init__(self, ROGET_NODE_SENSE, parent)
	
    def __init__(self, type, parent):
	RogetNode.__init__( self, type, None, parent)
	self._comment = ''
	self._link = None
	self._linkComment = None
	self._wordType = WORD_TYPE_NONE

    def toString(self):
	return self._wordToString()

    def _wordToString(self):	
	ret = self.typeToString() + " ("  + self.key + ")"
	if self.wordType != WORD_TYPE_NONE:
	     ret += ' /'
	     if self.wordType ==  WORD_TYPE_ADJ:
		ret += 'Adj'
	     if self.wordType ==  WORD_TYPE_VERB:
		ret += 'V'
	     if self.wordType ==  WORD_TYPE_ADVERB:
		ret += 'Adv'
	     if self.wordType ==  WORD_TYPE_NOUN:
		ret += 'N'
	     if self.wordType ==  WORD_TYPE_PHRASE:
	    	ret += 'Phr'
	     ret += '/ '
	if self.comment != '':
	    ret += ' comment: ' + self.comment
	if self.link != None:
	    ret += " [link: #" + self.link.index	
	    if self.link.key != None:
		ret += " (" + self.link.key + ")"
	    ret += " ]"
	return ret

    @property
    def comment(self):	
	''' an optional comment (in the text this is the text that appears in brackets ) '''
	return self._comment

    @property
    def link(self):
	''' optional link to a node of type HeadWord (in the text this appears as "&amp;c; 111" - link to headword with id 111 '''
	return self._link

    @property
    def linkComment(self):    
	''' optional comment on a link '''
	return self._linkComment

    @property
    def wordType(self):    
	''' optional word type annotation '''
	return self._wordType



   
class HeadWord(Sense):
    '''
	A headword
    '''     
    def __init__(self, HeadIndex, parent):
	Sense.__init__( self, ROGET_NODE_HEADWORD, parent)
	self._index = HeadIndex.strip()

    def toString(self):
	return '#' + self._index  + ' ' + self._wordToString() 

    @property
    def index(self):
	''' the string id that identifies the headword in the Roget thesaurus '''
	return self._index

'''
    The Roget Thesaurus class
'''    
class RogetThesaurus:
    def __init__(self):
	self._rootNode = None
	self._headWordIndex = None
	self._senseIndex = None
     
    def __init__(self, rootNode = None, headWordIndex = None, senseIndex = None ):
	self._rootNode = rootNode
	self._headWordIndex = headWordIndex
	self._senseIndex = senseIndex
 
    @property 
    def rootNode(self):
	''' the root node of the ontology '''
	return self._rootNode

    @property
    def headWordIndex(self):
	''' the index of head words - maps a head word to its node in the ontology '''
	return self._headWordIndex

    @property
    def senseIndex(self):
	''' the index of word senses - maps the word sense to a list of nodes in the ontology '''
	return self._senseIndex


   
class RogetThesaususFormatterText:
    '''
	class for formatting of Roget thesaurus as text report
    '''     
    def show(self, roget, file, mask = 0xF ):
	self._showRogetTextImp( roget.rootNode, 1, file, mask)
	
    def _showRogetTextImp(self, node, depth, file, mask ):

	if node.type & mask != 0:
	    line = ''
	    for i in range(1,depth):
		line += '\t'
	    line += node.toString() + '\n'
	    file.write( line )

	for n  in node.child:
	    self._showRogetTextImp(n , depth + 1, file, mask )

def escape( str ):
    str = str.replace("<", "&lt;")
    str = str.replace(">", "&gt;")
    str = str.replace("\"", "&quot;")
    return str

   
class RogetThesaurusFormatterXML:
    '''
	class for formatting of Roget thesaurus as xml
    '''     
    def show(self, roget, file):
	
	file.write( "<?xml version='1.0'?>\n<rogetThesaurus>\n<ontology>" )
	self._showRogetXMLNodes( roget.rootNode, file )
	file.write( "</ontology>" )
	file.write( "</rogetThesaurus>" )
 
    def _showRogetXMLNodes( self, node, file ):

	txt = "<" + node.typeToString() + " " 
	
	if node.type == ROGET_NODE_CATEGORY:
	    txt += 'name="' + escape( node.key ) + '"' 
	#elif node.type == ROGET_NODE_SENSE_GROUP:
	    # nothing to do
	elif node.type == ROGET_NODE_SENSE or node.type == ROGET_NODE_HEADWORD:
	    if node.type == ROGET_NODE_HEADWORD:
		txt += 'id="' + escape( node.index ) + '" ' 
	    txt += 'sense="' + escape( node.key ) + '" '
	    if node.wordType != WORD_TYPE_NONE:
		 txt += ' wordType="'
		 if node.wordType ==  WORD_TYPE_ADJ:
		    txt += 'Adj'
		 if node.wordType ==  WORD_TYPE_VERB:
		    txt += 'V'
		 if node.wordType ==  WORD_TYPE_ADVERB:
		    txt += 'Adv'
		 if node.wordType ==  WORD_TYPE_NOUN:
		    txt += 'N'
		 if node.wordType ==  WORD_TYPE_PHRASE:
		    txt += 'Phr'
		 txt += '" '
	    if node.comment != '':
		txt += ' comment="' + escape( node.comment ) + '" '
	    if node.link != None:
		txt += ' link="' + escape( node.link.index ) + '" '	
		if node.link.key != None:
		    txt  += ' linkComment="' + escape( node.link.key )+ '" '
	txt += ">"
	file.write( txt + '\n' )
	
	for n  in node.child:
	    self._showRogetXMLNodes( n, file )
	    file.write( " " )
	
	file.write( "</" + node.typeToString() + ">" )


	
