# show stack trace of deep nested function (showstack function)



println( ~msg recurse( ~num 5 ) )

sub recurse( num )
  loc = num * num
  if num > 0
     return loc + recurse( ~num  num - 1 )
  else
     showstack()
     return 1
  end   
end


