

# arguments passed by reference, by default they are ALL passed by value

sub swap( x byref, y byref )
  tmp = x
  x = y
  y = x
end


x = 10
y = 20

swap( ~x x ~y y )

println( ~msg 'after swap x [ x ] y [ y ]' )
