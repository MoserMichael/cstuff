import sys
import traceback
import roget


def lookupWord( rogetThesaurus, word ):
    idx = rogetThesaurus.senseIndex
    wordSenses = idx[ word ]
    if wordSenses != None:
        for s in wordSenses:
            while s != None:
                print( s.toString(), " / " )
                s = s.parent
            print("\n")
    else:
        print("no word senses found\n")

def test_lookup(rogetThesaurus):
    print(' *** test lookup *** ')
    lookupWord(rogetThesaurus, "fact")
    lookupWord(rogetThesaurus, "fiction")
    lookupWord(rogetThesaurus, "love")
    lookupWord(rogetThesaurus, "hate")

def test_save(rogetThesaurus):
    print(' *** test save *** ')
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

def test_sim( rogetThesaurus, w1, w2 ):
    (score, s ) = rogetThesaurus.semanticSimilarity( w1, w2 )

    print("score: ", score, "words: '", w1, "', '", w2 , "' ")

    if s != None:
        print("node: ")
    while s != None:
        print(s.toString(), " / ")
        s = s.parent
    print("\n")

def test_similarity( rogetThesaurus):
    print(' *** test similarity *** ')
    test_sim( rogetThesaurus, 'being', 'entity' )
    test_sim( rogetThesaurus, 'fact', 'being' )
    test_sim( rogetThesaurus, 'at the very moment', 'just then' )
    test_sim( rogetThesaurus, 'being', 'nihility' )
    test_sim( rogetThesaurus, 'being', 'commit' )


def do_main():
    parser = roget.RogetBuilder( 1 )
#   rogetThesaurus = parser.load( 'roget-binary' )
    rogetThesaurus = parser.parse()

    test_lookup( rogetThesaurus )
    test_similarity( rogetThesaurus )
    test_save( rogetThesaurus )

    print("*** test completed ***")

def main():
    do_main()

if __name__ == "__main__":
    main()
