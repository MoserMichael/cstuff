

# function tells us if argument is odd number or even
sub iseven( arg )
   if arg % 2 == 0
     println( ~msg 'even number [ arg ]' )
   else
     println( ~msg 'odd number [ arg ]' )
   end
end

# call function that tells us if a number is odd or even
iseven( ~arg 2 )

iseven( ~arg 3 )

# function counts to three, all other numbers are said to be very big.  
sub show( arg )

  if arg < 0
     return
  end

  # these elsif nestings are instead of switch ...
  if arg == 1 
    println( ~msg 'one' )
  elsif arg == 2
    println( ~msg 'two' ) 
  elsif arg == 3
    println( ~msg 'three' ) 
  else
    println( ~msg 'very big value' )
  end

end


show( ~arg 1 )
show( ~arg 2 )
show( ~arg 3 )
show( ~a 1 ~arg 4 )
