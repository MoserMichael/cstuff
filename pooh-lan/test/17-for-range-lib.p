
println( ~msg '*** growing integer range ***' )
for c range( ~from 0 ~to 5 )
  println( ~msg c )
end
println( ~msg '*** growing float range ***' )
for c range( ~from 0.2 ~to 5 )
  println( ~msg c )
end 
println( ~msg '*** growing float range (float step) ***' )
for c range( ~from 1 ~to 5 ~step 0.5 )
  println( ~msg c )
end 
println( ~msg '*** decreasing integer range ***' )
for c range( ~from 5 ~to 0 ~step -1 )
  println( ~msg c )
end
println( ~msg '*** decreasing float range ***' )
for c range( ~from 4.2 ~to 0 ~step -1 )
  println( ~msg c )
end 
println( ~msg '*** decreasing float range (float step) ***' )
for c range( ~from 5 ~to 1 ~step -0.5 )
  println( ~msg c )
end 

