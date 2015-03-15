import collections
import pprint

a = (1, 2, 3)

def showinfo( a ):
	print "type: ", a.__name__ 
	for n in a.__dict__:
		print '->', n
	for f in a.__bases__:
		print "baseclass: ", f.__name__ , " of ", a.__name__
		showinfo( f )


def showtype( a ):
	print ' ******************* '
	showinfo( type( a ) )


showtype( () )
showtype( {} )
showtype( [] )


showtype( 1 )
showtype( 1.123 )
showtype( 'aaa' )

