# tests string library functions mid left right

p = 'the quick brown fox jumped'
q = 'fox'
n = find( ~hay p ~needle q )
println( ~msg 'Position "[ n ]" of "[ q ]" in "[ p ]"' )

q = 'bee'
n = find( ~hay p ~needle q )
println( ~msg 'Position "[ n ]" of "[ q ]" in "[ p ]"' )

a = left( ~string 'abcdefg' ~length 3 )
println( ~msg a )

a = left( ~string 'abcdefg' ~length 200 )
println( ~msg a )

a = right( ~string 'abcdefg' ~length 3 )
println( ~msg a )

a = right( ~string 'abcdefg' ~length 200 )
println( ~msg a )

a = mid( ~string 'abcdefg' ~offset 2 ~length 3 )
println( ~msg a ) 

a = mid( ~string 'abcdefg' ~offset 2 )
println( ~msg a )

a = mid( ~string 'abcdefg' ~length 3 )
println( ~msg a ) 

a = mid( ~string 'abcdefg' ~offset -1 ~length 4 )
println( ~msg a ) 



