

Parses the roget thesaurus and makes it accessible through an API.

the text of the Roget thesaurus was downloaded from here https://archive.org/details/rogetsthesauruso10681gut

Written by Michael Moser (c) 2015

----

class RogetBuilder
   	The main entry point of this library; builds an instances of RogetThesaurus
 
 	Methods defined here:
		
	    __init__(self, verbose=0)
	    
	    parse(self)
		parse the roget thesaursus
		returns an instance of RogetThesaurus 
		 
		Note that that file 10681-body.txt  must be in the same directory as the script roget.py

	    load(self, file)
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

----

class RogetThesaurus
    Methods defined here:

	__init__(self, rootNode=None, headWordIndex=None, senseIndex=None)


	semanticSimilarity(self, seq1, seq2)
	    computes the semantic similarity between two terms, 
	     
	    returns the following tuple (similarity-score, common-node-in-roget-thesaurus)
	     
	     
	    the similarity score:
		100 - both terms appear in the same SenseGroup node
		 90 - both terms he the same head word
		 80 - both terms appear in the same leaf category
		  0 - everything else
	     
	    common-node-in-roget-thesaurus: is None if the score is 0; 
	    otherwise it is the common node that the score is based on

    Data descriptors defined here:

	headWordIndex
	    the index of head words - maps a head word to its node in the ontology
	    
	rootNode
	    the root node of the ontology

	senseIndex
	    the index of word senses - maps the word sense to a list of nodes in the ontology

----

class RogetNode
    RogetNode - the base class of all nodes maintained by Roget thesaurus
 
    Methods defined here:
	__init__(self, type, description, parent=None)

	toString(self)

	typeToString(self)
	    returns the type o this node as a string

    Data descriptors defined here:


	child
	    returns the array of child nodes

	description
	    returns an optional description (in the text this appears as [ .... ] )

	internalId
	    each node has its own internal id

	key
	    the meaning/key of this node
	
	parent
	    returns the parent node (one up in the ontology)

	type
	    returns the type of this node as a integer

----

    class RogetThesaurusFormatterXML
   	class for formatting of Roget thesaurus as xml
 
 	Methods defined here:
	    show(self, roget, file)

----
class Sense(RogetNode)
    a single sense (the leaf node of the Roget Thesaurus
 
    Methods defined here:
	    __init__(self, type, parent)

	    toString(self)
    
    Data descriptors defined here:

	    comment
		an optional comment (in the text this is the text that appears in brackets )


	    link
		optional link to a node of type HeadWord (in the text this appears as "&amp;c; 111" - link to headword with id 111


	    linkComment
		optional comment on a link

	    wordType
		optional word type annotation

    Methods inherited from RogetNode:
	    typeToString(self)
		returns the type o this node as a string

    Data descriptors inherited from RogetNode:
	    child
		returns the array of child nodes

	    description
		returns an optional description (in the text this appears as [ .... ] )
	
	    internalId
		each node has its own internal id

	    key
		the meaning/key of this node

	    parent
		returns the parent node (one up in the ontology)

	    type
		returns the type of this node as a integer


----


class HeadWord(Sense)
    A headword
 
 	
    Method resolution order:
	HeadWord
	Sense
	RogetNode


    Methods defined here:
	__init__(self, HeadIndex, parent)

	toString(self)

    Data descriptors defined here:
	
	index
	    the string id that identifies the headword in the Roget thesaurus

    Data descriptors inherited from Sense:
	comment
	    an optional comment (in the text this is the text that appears in brackets )

	link
	    optional link to a node of type HeadWord (in the text this appears as "&amp;c; 111" - link to headword with id 111

	linkComment
	    optional comment on a link

	wordType
	    optional word type annotation

    Methods inherited from RogetNode:
	typeToString(self)
	    returns the type o this node as a string

    Data descriptors inherited from RogetNode:
	child
	    returns the array of child nodes

	description
	    returns an optional description (in the text this appears as [ .... ] )

	internalId
	    each node has its own internal id

	key
	    the meaning/key of this node

	parent
	    returns the parent node (one up in the ontology)

	type
	    returns the type of this node as a integer	

----
    class RogetThesaususFormatterText
	class for formatting of Roget thesaurus as text report
 
 	Methods defined here:
	    show(self, roget, file, mask=15)
	    
