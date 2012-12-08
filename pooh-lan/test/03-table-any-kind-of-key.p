# table with any kind of keys

a = { [1,2,3] : 'a' , [2,3,4] : 'b', [3, 4, 5] : 'c', { 'a' : [1, 2, 3], 'b' : 'c' } : 'd' }

println( ~msg a{ [1, 2, 3] } )
println( ~msg a{ [2, 3, 4] } )
println( ~msg a{ [3, 4, 5] } )

println( ~msg a{  { 'a' : [1, 2, 3], 'b' : 'c' } } )
 
