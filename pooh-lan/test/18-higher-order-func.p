# Higher order functions fold/filter/map implemented as script functions.

sub foldleft2( array , func, initval )

   acc = initval

   i = size( ~arg array )
   while  i > 0
     acc = func( ~a array[ i ] ~b acc )
     i = i - 1
  end
  return acc
end

sub foldright2( array, func, initval )
    acc = initval
    i = 1
    while  i <= size( ~arg array )
      acc = func( ~a array[ i ] ~b acc )
      i = i + 1
    end 
    return acc
end

sub filter2( array, func )
   ret = []

   for v array
     if func(~arg v )
        push( ~array ret ~top v )
     end    
   end

   return ret
end


sub map2( array, func )
   ret = []

   for v array
     push( ~array ret ~top func( ~arg v ) )
   end

   return ret
end

# --------------------------------

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

# --------------------------------

a = [ 1, 2, 3, 4, 5 ]

println( ~msg 'fold left - sum: ' .. foldleft2( ~array a ~func sum ~initval 0 ) ) 
println( ~msg 'fold right - sum: ' .. foldright2( ~array a ~func sum ~initval 0 ) ) 

println( ~msg 'fold left - sum of squares: ' .. foldleft2( ~array a ~func sumsqr ~initval 0 ) ) 
println( ~msg 'fold right - sum of squares: ' .. foldright2( ~array a ~func sumsqr ~initval 0 ) ) 

println( ~msg 'filter - filter odd numbers: ' .. filter2( ~array a ~func even ) ) 

println( ~msg '*** map - square root of numbers ***' )

sqa = map2( ~array a ~func square )

for v sqa 
  println( ~msg v )
end

