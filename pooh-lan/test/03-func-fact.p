
sub fact( n )
  if n > 0
    return 1 * fact(n)
  else
    return 1
  end
end

for i [ 1, 5, 15 ]
   print ( @"@Factorial of " .. i .. @"@ = " .. fact(i) .. @"@
" )
end

for i [ 1, 5, 15 ]
  print ( @"@Factorial of #{i} = #{ fact(i) }  
" )
end
