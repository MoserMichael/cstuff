

# get all prime numbers up to max
sub primes( max )

  mark = []
  ret = []

  n = 2 
  while n < max
    m = 2 * n
    loop 
        if m > max
            break;
        end
        mark[ m ] = 1
        m = m + n
    end
    n = n + 1
  end

  n = 2
  while n  < max
    if mark[ n ] != 1
       push( ~array ret ~top n )
    end
    n = n + 1
  end

  return ret
end


# get prime numbers up to 100
pr = primes( ~max 100 )

# print them out.
i = 1
while i <= size( ~arg pr)
# println( ~msg 'prime number [ pr[i] ]' )
  println( ~msg 'prime number ' ..  pr[i]  )

  i = i + 1
end
