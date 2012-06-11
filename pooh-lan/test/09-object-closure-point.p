
# -------------------------------------------------
# another test case for closures as objects.

sub makepoint( x, y )

  # the object state here are the constructor arguments (x, y)
  # very similar to object by closure

  # an advantage of object-by closure over object as hash (ala javascript)
  # when you type the code, the instance method has to be defined as local of enclosing scope;
  # so you can misspell the member names.

  # return factory method that returns object methods  - both factory and object methods get capture of the object state 
  return sub( method ) 
      if method == 'move'
        return sub( dx, dy )
            outer.x = outer.x + dx
            outer.y = outer.y + dy
          end
      elsif method == 'print'
        return sub()
           println( ~msg 'position x = [ outer.x ] y = [  outer.y ]' )
          end
      elsif method == 'x-pos'
         return sub()
            return outer.x
         end
      elsif method == 'y-pos'
         return sub()
             return outer.y
         end
      end
  end

end


# make point object
point = makepoint( ~x 42 ~y 10 )

# show position of point
point( ~method 'print') () 

point( ~method 'move') ( ~dx 10 ~dy 10 )

# show position of point after having moved it.
point( ~method 'print') () 

# call accessor methods.
println( ~msg 'horizontal postion [ point( ~method 'x-pos') () ] vertical position [ point( ~method 'y-pos') () ]' )


