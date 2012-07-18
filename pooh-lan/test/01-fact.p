


sub fact( num )
  if (num > 0)
     return fact( ~num num - 1 ) * num
  else
     return 1
  end
end


println( ~msg 'the factorial of 10 is ' .. fact( ~num 10 ) )
