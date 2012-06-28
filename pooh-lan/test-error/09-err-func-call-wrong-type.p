

sub triangle(a , b , c)
  return sqr( ~num a * a + b * b + c * c )
end

println( ~msg 'square of triangle' .. triangle( ~a 1 ~b 2 ~c 3 ) )


msg='a string'
arr=[]
c = 2
println( ~msg 'wrong arguments' .. triangle( ~a msg ~b arr ~c c ))

arr = triangle( ~a 1 ~b 2 ~c 3 )
