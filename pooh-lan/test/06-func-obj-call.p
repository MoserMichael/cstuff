# function invokes argument callback over range of numbers; prints result as ASCII graph

sub myfunc( num )
  return sqr( ~num num )  + num * num
end


sub get_data( func, from, to, step )
  ret = []

  i = from
  while i <= to
    push( ~array ret ~top func( ~num i ) )
    i = i + step
  end
  return ret
end


sub draw_graph( data )
  min = maxfloat()
  max = minfloat()

  i = 1
  while i <= size( ~arg data )
     x = data[ i ] 
     if x < min
       min = x
     end
     if x > max
       max = x
     end
     i = i + 1
  end

  i = 1
  while i <= size( ~arg data )
    x = data[ i ]
    val = 80 * ( x - min ) / ( max - min )
    j = 1
    while j <= val
      print( ~msg ' ' )
      j = j + 1
    end
    println( ~msg '*' )
    i = i + 1
  end
end


println( ~msg 'graph of cos function' )
data = get_data( ~func cos ~from 0 ~to 4  ~step 0.1 )
draw_graph(  ~data data )

println( ~msg 'graph of user defined function' )
data = get_data( ~func myfunc ~from 0 ~to 4 ~step 0.2 )
draw_graph(  ~data data )




