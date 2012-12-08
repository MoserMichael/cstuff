# test where object has a finalizer - it is called when object is freed.


a := { 'field' : 42, 
       'finalizer' : sub ()
                        println( ~msg 'finalizer is called' )
                     end
     }  



# assigning a different value to a; the previous value is now freed and finalizer is called.
a = {}

