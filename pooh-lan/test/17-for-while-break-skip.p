# for loop ; test keywords: break - finishing the iteration and skip - continue to next iteration. 

println( ~msg 'while loop with skip and break' )

i = 1
while i < 11
  if i == 10 
    break
  end    
  if i % 2 == 0
    i = i + 1
    skip
  end
  println( ~msg i )
  i = i + 1
end  

println( ~msg 'for loop with skip and break' )

for i range( ~from 1 ~to 11 )
  if i == 10 
    break
  end    

  if i % 2 == 0
    skip
  end
  println( ~msg i )
end

println( ~msg '*** eof ***' )
