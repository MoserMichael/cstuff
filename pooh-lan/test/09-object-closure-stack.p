
# ------------------------------------------
# poor mans stack object made out of closures.

sub makestack( )

   # instance data of the stack object.
   stack = []

   # work by currying (poor mans currying in javascript that is). 
   # return the constructor function
   # the constructor gets capture of the instance data, so do the methods returned by constructor.

   # .. or in OO terminology: the object costructor returns the factory method for the object interface (now factory gets object state as closure)
   
   # now all this is not very educational per see ;-( but its a good test case to look if closures are working.

   return sub ( method )
     if method eq 'pop'
       return sub ()
                  if size( ~arg outer.stack ) == 0
                     println( ~msg 'Error: empty stack' )
                     return Nill
                  else 
                     println( ~msg 'pop: has [ size( ~arg outer.stack ) ] elements' )
                     return pop ( ~array outer.stack )
                  end
              end
     elsif method eq 'push'
       return sub ( top )
                  push( ~array outer . stack ~top top )
                  println( ~msg 'push: has [ size( ~arg outer.stack ) ] elements' )
               end
     end 
   end  
end

# ------------------------------------------


# make stack returns a list of function objects, by calling these objects we modify the object data which is the data of the closure
stack = makestack( )

dopush = stack( ~method 'push' )
dopop  = stack( ~method 'pop' )

dopush( ~top 1 )
dopush( ~top 2 )
dopush( ~top 3 )


for i range( ~from 1 ~to 4 )
  println ( ~msg 'pop: result is [ dopop() ]' )
end

