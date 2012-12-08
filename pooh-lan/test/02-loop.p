# loops: function computes the sum of squares over integer rane. 

sub squares( from, to)
  i = from
  res = 0
  while i < to
    res = res + i * i
    i = i + 1
  end
  return res
end

println( ~msg 'the sum of numbers is ' .. squares( ~from 1 ~to 7 ) )
