
#-----------------------------------------------------

# example of invoking a thread;
# when a function is called from a for loop, then an interpreter thread is created behind the scenes.
# now lets do it explicitly


sub myrange( from, to )
   i = from
   while i < to

     # yield function returns the value to the thread.
     # yield will put this thread to sleep, it will sleep untill somebody will resume this thread.
     
     yield( ~message i )

     i = i + 1
   end
end


# a function object is turned into a thread; a thread is a special kind of function object.
th = makethread( ~thread myrange )

# thread is created when the function object returned by makethread is called.
# if the thread wants to tell us something, it calls yield; 
# return value of the function is the argument to yield, when yield is called for the first time ever.

val = th( ~from 1 ~to 10)
 
loop
 
  # the return value is Nill if the thread just stops, or does not return a valid value.
  if val != Nill
    println( ~msg 'thread returned val' )
  end

  # if the thread 
  if not isactive( ~thread th )
    break
  end

  # resume will tell the thread to wake up right after it called yield.
  val = resume( ~thread th )
end

 
