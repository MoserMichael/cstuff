

arr = []
karr = [ 1, 2, 3 ]
hash = {}

str = 'aaa'


# array - can't have string index
arr[ str ] = 1

# array - can't have array as index
arr[ karr ] = 1

# array - can't have hash as index
arr[ hash ] = 1
