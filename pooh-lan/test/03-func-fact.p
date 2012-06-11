
# recursive function call

sub fact( n )
  if n > 1
    return n * fact( ~n n - 1 )
  else 
    return 1
  end
end

for i [ 1, 5, 15 ]
   println ( ~msg 'Factorial of [ i ]  = [ fact( ~n i ) ] ' )
end

for i [ 1, 5, 15 ]
  println ( ~msg 'Factorial of ' ..  i .. '  =  ' .. fact( ~n i )  )
end
