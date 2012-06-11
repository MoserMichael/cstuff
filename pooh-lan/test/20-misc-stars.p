
# print some snow on the screen and watch it scroll down
# ... long long time ago this was cool ...


srand( ~seed 10 )

for i range( ~from 1 ~to 200) 
   pos = rand( ~max 60 )
   println( ~msg spaces( ~num pos ) .. '*' )
end


sub spaces( num )
   i = 0
   ret = "
   while( i < num )
     ret = ret .. ' '
   end
   return ret
end
