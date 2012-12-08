ff := 42 
gg := ff
println( ~msg ff )
println( ~msg gg )
ff = 43
println( ~msg ff )
println( ~msg gg )

##############################################

aaa = []

aaa[ 1 ] = 42

aaa[ 2 ] := aaa[ 1 ]

println( ~msg aaa[ 1 ] )

println( ~msg aaa[ 2 ] )

aaa[ 1 ] = 43

println( ~msg aaa[ 1 ] )

println( ~msg aaa[ 2 ] )

##############################################

a = 42

println( ~msg a )

b := a

println( ~msg b )

a = 43

println( ~msg a )
println( ~msg b )

##############################################

aa = 'a'

println( ~msg aa )

bb := aa

println( ~msg bb )

aa = 'aa'

println( ~msg aa )
println( ~msg bb )







