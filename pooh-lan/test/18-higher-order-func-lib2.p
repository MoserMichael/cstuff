# Higher order functions fold/filter/map - use standard library implementation.


sub recsum( num )
  if (num > 0)
     return recsum( ~num num - 1 ) + num
  else
     return 0
  end
end


sub sum( a, b )
  return recsum( ~num a ) + recsum( ~num b )
end

sub sumsqr( a, b )
  a = recsum(~num a )
  b = recsum(~num b )
  return a * a + b
end

sub even( arg )
 # recsum( ~num arg * arg ) 
  return arg % 2 == 0
end

sub square( arg )
   arg = recsum( ~num arg )
   return arg * arg
end

# --------------------------------

a = [ 1, 2, 3]

println( ~msg 'fold right - sum: ' .. foldlast2first( ~array a ~func sum ~initval 0 ) ) 
println( ~msg 'fold left - sum: ' .. foldfirst2last( ~array a ~func sum ~initval 0 ) ) 

println( ~msg 'filter - filter odd numbers: ' .. filtercopy( ~array a ~func even ) ) 

println( ~msg '*** map - square root of numbers ***' )

sqa = mapcopy( ~array a ~func square )

for v sqa 
  println( ~msg v )
end

println( ~msg '*** mapreplace - add one to array ***' )


mapreplace( ~array sqa ~func sub (arg) 
                                return 1 + arg 
                             end )

for v sqa 
  println( ~msg v )
end



