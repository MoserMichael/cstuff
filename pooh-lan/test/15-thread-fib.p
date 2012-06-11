#-----------------------------------------------------

sub fibonacciseries()
  n = 0
  m = 1

  # return the first fibonacci numbers to the caller.
  yield( ~message n)
  yield( ~message m)

  loop
    nextf = n + m

    # return next fibonacci number to the caller.
    yield( ~message nextf)

    m = n
    n = nextf
  end
end

th = makethread( ~thread fibonacciseries )

# get the first fibonacci number
num = th()

# ten fibonacci numbers please.
for c range( ~from 1 ~to 10)

   # the thread will not have stopped; skip check
   #if val == Nil
   #  break
   #end

   # print fibonacci number 
   println( ~msg num )

   # the thread will not have stopped; skip check
   #if not isactive( ~thread th )
   # break
   #end

   # resume the thread so that it computes the next number
   val = resume( ~thread th )
end

# don't need this thread any more (the thread has not stopped and can be resumed), so stop it.
stopthread( ~thread th )
  
 
