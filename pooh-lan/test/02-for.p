

arr = [ 1, 2, 3, 4]


# array counter is a reference to the array element;
# by assigning a value to the counter value the array element is modified.

for n arr
  n = n * 2
end

for n arr
 println( ~msg n )
end

for i range( ~from 1 ~to 10)
  if i % 2 == 0
    break
  end
  println( ~msg 'odd number [ i ]' )
end

for i range( ~from 1 ~to 10 ~step 2 )
  println( ~msg 'odd number [ i ]' )
end

i = 1
loop
  i = i + 1
  if i > 100
    break
  end
end
println( ~msg 'number is [ i ]' )
