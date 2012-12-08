# a quick sort written in pooh

test = [ 5, 1, 4, 2, 3, 10, 9, 7, 8, 6 ]
sorted = qsort( ~array test )

for n sorted
  println( ~msg n )
end


sub qsort( array )
  if size( ~arg array ) <= 1
    return array
  end

  pivot = array[1]
  small=[]
  large=[]

  i = 2
  while i <= size( ~arg array )
    if array[ i ] < pivot 
      push( ~array small ~top array[i] )
    else
      push( ~array large ~top array[i] )
    end
    i = i + 1
  end

  small = qsort( ~array small )
  large = qsort( ~array large )

  push( ~array small ~top pivot )
  append( ~array small ~add large )

  return small
end
