
gl_table = {}


sub foo()

   arr = []
   tab = {}
   scl = 1

   if arr{ 'who-now' } == 1 
     println( ~msg 'array used as table' )
   end

   if tab[ 0 ] == 1 
     println( ~msg 'table used as array' )
   end

   if scl{ 1 } == 1
     println( ~msg  'scalar used as array' )
   end

   if global . gl_table == 1
     println( ~msg 'global table used as scalar' )
   end

end
