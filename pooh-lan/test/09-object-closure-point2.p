
# -------------------------------------------------
# another test case for closures as objects.

sub makepoint( x, y )

  # the object state here are the constructor arguments (x, y)
  # very similar to object by closure



  domove = sub( dx, dy )
            outer.x = outer.x + dx
            outer.y = outer.y + dy
          end

  doprint = sub()
           println( ~msg 'position x = [ outer.x ] y = [  outer.y ]' )
          end
 
  dogetx = sub()
            return outer.x
         end

  dogety = sub()
             return outer.y
         end

  dofactory = sub( method ) 
      if method eq 'move'
        return outer . domove
      elsif method eq 'print'
        return outer . doprint
      elsif method eq 'x-pos'
        return outer . dogetx
      elsif method eq 'y-pos'
        return outer . dogety
      end
  end


  # return factory method that returns object methods  - both factory and object methods get capture of the object state (
  
  # now adding addition optimization: each time that the factory method returns a accessor method or constructor, then the same instance of the closure is returned.
  # lots of typing there is...

  return dofactory
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


