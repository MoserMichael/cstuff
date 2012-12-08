# co-routine threads: create thread via API, thread yields sequence of fibonaci numbers.


# a most simple co-routine that yields all integers in range: from .. to

sub myrange(  to )

   a = 1
   b = 1
   
   i = 1
   while i <= to

     # yield function puts this thread to sleep and will wake up the caling thread
     # it also returns a value to the calling thread.

     nextval = a + b

     threadyield0( ~yieldval nextval )

     a = b
     b = nextval
     i = i + 1
   end
end

#
# Invoke the co-routine via the API.
#

th := makethread( ~func myrange )

th( ~to 10)

[ running, value ] = threadyieldvalue( ~thread th )

while( running != 0 )

  println( ~msg 'thread returned [ value ]' )

  [ running, value ] = resumethread( ~thread th )
end

stopthread( ~thread th )

