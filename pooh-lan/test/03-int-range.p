# function fills array with number range, array is the return value.


nums = myrange( ~from 1 ~to 10)
i = 1
while i <= size( ~arg nums )
   println( ~msg nums[ i ] )
   i = i + 1
end

sub myrange(from,to)
  ret = []

  while from <= to
     push( ~array ret ~top from)
     from = from + 1
  end
  return ret
end
