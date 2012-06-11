

a = 1
b = 2

[a , b] = [b, a]

println( ~msg 'shold have swapped values a=[ a ] (2)  b =[ b ] (1)' )

[a , b] = [b + a, a - b]

println( ~msg ' a=[ a ] (3) b =[ b ] (1)' )
