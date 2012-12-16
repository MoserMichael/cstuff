
println( ~msg '*** list *.c files ***' )
for f listdir( ~pattern '../cutils/*.c' )
  println ( ~msg 'file name [f]' )
end
println( ~msg '*** eof files ***' )

println( ~msg '*** list *.c and *.h files ***' )

for f listdir( ~pattern [ '../cutils/*.h' , '../cutils/*.c' ] )
  println ( ~msg 'file name [f]' )
end
