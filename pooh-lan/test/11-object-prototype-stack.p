
# ---- objects by means of cloning (prototypes) -----


sub makestack()

 # constructor of object makes a hash table; the methods of the object are has table entries where the value is a function without name (aka lambda)
 # data members are also table entries; it is good practice to declare data members in the hash.

 return { 
          'stack' : {},
          

          # when a function is inserted into a collection (hash or vector) then a function object is created.
          # the function object has a this symbol that points to the enclosing collection; so a function can access
          # members of the collection as 'member data' via this . name_of_data_member

          push : sub ( top ) 

                  push( ~array this.stack ~top top )
                  println( ~msg 'push: has [ size( ~arg outer.stack ) ] elements' )
             
          end,

          pop : sub () 

                if size( ~arg this.stack ) == 0
                     println( ~msg 'Error: empty stack' )
                         return Nill
                  else 
                     println( ~msg 'pop: has [ size( ~arg this.stack ) ] elements' )
                     return pop ( ~array this.data )
                  end
   
          end
  }

end


stack = makestack()

stack.push( ~top 1 )
stack.push( ~top 2 )
stack.push( ~top 3 )


for i range( ~from 1 ~to 4 )
  println ( ~msg 'pop: result is [ stack.pop() ]' )
end
