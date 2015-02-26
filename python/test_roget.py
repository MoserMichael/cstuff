import pd
import sys
import traceback
import roget				 


def lookupWord( rogetThesaurus, word ):
    idx = rogetThesaurus.senseIndex
    wordSenses = idx[ word ]
    if wordSenses != None:	
	for s in wordSenses:
	    while s != None:
		print s.toString(), " / ",
		s = s.parent
	    print "\n"		
    else:
	print "no word senses found\n"


def do_main():
    parser = roget.RogetBuilder( 1 )
#   rogetThesaurus = parser.load( 'roget-binary' )
    rogetThesaurus = parser.parse()

    lookupWord(rogetThesaurus, "fact")
    lookupWord(rogetThesaurus, "fiction")
    lookupWord(rogetThesaurus, "love")
    lookupWord(rogetThesaurus, "hate")

    with open( 'roget-out.txt', 'w' ) as f:
	format = roget.RogetThesaususFormatterText()
	format.show(rogetThesaurus, f)
  
    with open( 'roget-struct-out.txt', 'w' ) as f:
	format = roget.RogetThesaususFormatterText()
	format.show(rogetThesaurus, f, roget.ROGET_NODE_CATEGORY )

    with open( 'roget-struct-with-headw.txt', 'w' ) as f:
	format = roget.RogetThesaususFormatterText()
	format.show(rogetThesaurus, f, roget.ROGET_NODE_CATEGORY  | roget.ROGET_NODE_HEADWORD )


    with open( 'roget-out.xml', 'w' ) as f:
	format = roget.RogetThesaurusFormatterXML() 
	format.show(rogetThesaurus, f)

    print "*** test completed ***"	

def main():
    try:
	do_main()
    except BaseException:
	pd.print_exception_ex()	
	
if __name__ == "__main__":
    main()
