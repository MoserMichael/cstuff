


sub primes( max )

  mark = []
  ret = []

  for n range( ~from 2 ~to max )
    m = 2 * n
    loop 
        if m > max
            break;
        end
        mark[ m ] = 1
        m = m + n
    end
  end

  for n range( ~from 2 ~to max)
#   if mark[ n ] == Nil
    if mark[ n ] != 1
       push( ~array ret ~top n )
    end
  end

  return ret
end



for m primes( ~max 100 )
  println( ~msg 'prime number [ m ]' )
end
