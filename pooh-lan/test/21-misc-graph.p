

sub get_data( func, from, to, step )
  ret = []

  for x range( ~from from ~to to ~step step)
    push( ~array ret ~top func( ~arg x ) )
  end
  return ret
end


sub draw_graph( data )
  min = maxfloat()
  max = minfloat()

  for x data
     if x < min
       min = x
     end
     if x > max
       min = x
     end
  end

  for x data 
    val = 80 * ( x - min ) / ( max - min )
    for i range( ~from 0 ~to val )
      print( ~msg ' ' )
    end
    println( ~msg '*' )
  end
end


data = get_data( ~func sin ~from 1 ~to 10 ~step 0.1 )
draw_graph( ~data data )

