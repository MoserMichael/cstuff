
println( ~msg '*** list *.c files ***' )
for f listdir( ~pattern '../cutils/*.c' )
  println ( ~msg 'file name [f]' )
end

println( ~msg '*** list *.c and *.h files ***' )

for f listdir( ~pattern [ '../cutils/*.h' , '../cutils/*.c' ] )
  println ( ~msg 'file name [f]' )
end

println( ~msg '*** list *.c and *.h files (2) ***' )
ofile = listdir( ~pattern [ '../cutils/*.h' , '../cutils/*.c' ] )
for f ofile
  println ( ~msg 'file name [f]' )
end
 
println( ~msg '*** eof files ***' )

