# Higher order functions fold/filter/map - use built in library implementation.

sub sum( a, b )
  return a + b
end

sub sumsqr( a, b )
  return a * a + b
end

sub even( arg )
  return arg % 2 == 0
end

sub square( arg )
   return arg * arg
end

sub inc( arg )
   return arg + 1
end

# --------------------------------

a = [ 1, 2, 3, 4, 5 ]

println( ~msg 'fold left - sum: ' .. foldfirst2last( ~array a ~func sum ~initval 0 ) ) 
println( ~msg 'fold right - sum: ' .. foldlast2first( ~array a ~func sum ~initval 0 ) ) 

println( ~msg 'fold left - sum of squares: ' .. foldfirst2last( ~array a ~func sumsqr ~initval 0 ) ) 
println( ~msg 'fold right - sum of squares: ' .. foldlast2first( ~array a ~func sumsqr ~initval 0 ) ) 

println( ~msg 'filter - filter odd numbers: ' .. filtercopy( ~array a ~func even ) ) 

println( ~msg '*** mapcopy - square root of numbers ***' )

sqa = mapcopy( ~array a ~func square )

for v sqa 
  println( ~msg v )
end

println( ~msg '*** mapreplace - add one to array ***' )


mapreplace( ~array sqa ~func inc )

for v sqa 
  println( ~msg v )
end



