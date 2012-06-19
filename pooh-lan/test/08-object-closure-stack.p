

# ------------------------------------------
# stack object made out of closures.

sub makestack()

   # instance data of the stack object.
   stack = []

   dopop_ =  sub ()
                  if size( ~arg outer.stack ) == 0
                         println( ~msg 'Error: empty stack' )
                     return Nill
                  else 
                     println( ~msg 'pop: has [ size( ~arg outer.stack ) ] elements' )
                     return pop ( ~array outer . stack )
                  end
              end
   dopush_ = sub ( top )
                  push( ~array outer . stack ~top top )
                  println( ~msg 'push: has [ size( ~arg outer.stack ) ] elements' )
               end

   # constructor returns the whole interface.
   return [ dopush_, dopop_ ]
end

# ------------------------------------------


# make stack returns a list of function objects, by calling these objects we modify the object data which is the data of the closure
[dopush, dopop ] = makestack( )

dopush( ~arg 1 )
dopush( ~arg 2 )
dopush( ~arg 3 )


for i range( ~from 1 ~to 4 )
  println ( ~msg 'pop: result is [ dopop() ]' )
end

