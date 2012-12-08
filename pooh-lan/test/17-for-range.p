# co-routines / generators as used with for statement

sub myrange( from, to )
   i = from
   while i <= to

     # yield function puts this thread to sleep and will wake up the caling thread
     # it also returns a value to the calling thread.

     threadyield0( ~yieldval i )

     i = i + 1
   end
end

#
# Invoke the co-routine via the for loop syntax
#

for value myrange( ~from 1 ~to 10 )
  println( ~msg 'thread returned [ value ]' )
end

