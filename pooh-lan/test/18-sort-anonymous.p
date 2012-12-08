# use standard library sort procedure - with anonymous function as callback for comparison

a = [ 10 , 3, 1, 5, 4, 9, 6, 7, 8, 2 ]


sort( ~array a ~func sub (a,b) 
  if a < b
    return -1
  elsif a > b
    return 1
  else 
    return 0;
  end
end
)
    

println( ~msg 'sorted numbers' )

for v a
  println( ~msg v )
end
