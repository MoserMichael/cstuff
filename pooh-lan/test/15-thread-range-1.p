# create co-routine thread via api


# a most simple co-routine that yields all integers in range: from .. to

sub myrange( from, to )
   i = from
   while i <= to

     # yield function puts this thread to sleep and will wake up the caling thread
     # it also returns a value to the calling thread.

     threadyield0( ~yieldval i )

     i = i + 1
   end
end


sub testfun()

  #
  # Invoke the co-routine via the API.

  th := makethread( ~func myrange )

  th( ~from 1 ~to 10)

  [ running, value ] = threadyieldvalue( ~thread th )

  while( running != 0 )

    println( ~msg 'thread returned [ value ]' )

    [ running, value ] = resumethread( ~thread th )
  end

  stopthread( ~thread th )

end

testfun()
