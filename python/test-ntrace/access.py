


def hashAccess():
    a = { 'a' : 1, 'b' : 2, 'c' : 3 }
    a[ 'c' ] = 42
    return a[ 'c' ]

def arrayAccess():
    a = [ 1, 2, 3, 4, 5]
    a[2] = 42
    return a[3]

def arrayAccess2():
    a = [ 1, 2, 3, 4, 5]
    a[2] = [2,3,4]

    return a[2][2]


print hashAccess()    

print arrayAccess()

print arrayAccess2()
	


