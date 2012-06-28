
#-----------------------------------------

# function with ... can receive any number of arguments.
sub threadcurry( ... )

   # without parameters info on calling function parametersis returned
   params = functionparameters( )

   # first parameter is supposed to be the function.
   if params[0]{'label'} eq 'func'
     exit( ~msg 'makecurry: first parameter must have label func' )
   end
   if params[0]{'type'} ne 'function'
     exit( ~msg 'makecurry: first parameter must be a function' )
   end

   func = params[0]{'valueref'}

   shift( ~array params)

   return sub ( method )
     if method eq 'apply'
        makethread( ~thread outer . func)

        # perform the call
        return applyfunc( ~func outer . func  ~params outer . params)
     elsif method eq 'resume'
        return resume( ~func outer . func)
     elsif method eq 'isactive'
        return isactive( ~thread outer . func )
     end
   end
end


#-----------------------------------------
# now lets use map and reduce, when the source data is a thread (instead of array).
# the thread is represented as thread curry - the thread call arguments have been stored in thread curry.
# when the curry is called, the intended thread is called;

sub map2array( threadcurry, func )
 value = threadcurry( ~method 'apply' )
 ret = []

     while value != Nill
   
   push( ~array ret ~top func( ~arg value ) )

   if not threadcurry( ~method 'isactive' ) 
     break 
   end
   value = threadcurry( ~method 'resume' )   
 end
 return ret
end


sub mapthread( threadcurry, func )

 value = threadcurry( ~method 'apply' )

 while value != Nill
   
   yield( ~message func( ~arg value ) )

   if not threadcurry( ~method 'isactive' ) 
     break
   end
   value = threadcurry( ~method 'resume' )   
 end
end
`
sub reduce_thread( threadcurry, func, initval )
 value = threadcurry( ~method 'apply' )
 
 s = initval
 while value != Nill
   
   s = func( ~first s ~second value )

   if not threadcurry( ~method 'isactive' ) 
     break
   end
   value = threadcurry( ~method 'resume' )   
 end
 return s 
end

sub reduce_vector( vector, func, initval )
  s = initval
  for r vector
    s = func( ~first s ~second r )
  end
  return s
end

sub reduce( arg , func , initval )
  if isfunction( ~value arg )
    return reduce_thread( ~threadcurry arg ~func func ~initval initval ) 
  end
  if isarray( ~value arg )
    return reduce_vector( ~vector arg ~func func ~initval initval )
  end
end


#-----------------------------------------
sub sum( first, second )
  return first + second
end

sub square( arg )
  return arg * arg
end

# produce the sum of the squares of numbers in range from 1 to 100
# here the map step produces an array; reduce step iterates over array

r = reduce( ~arg map2array( ~arg threadcurry( ~func range ~first 1 ~second 100 )  ~func square ) ~func sum ~initval 0 )
println( ~msg 'sum of squares ' .. r )

# produce the sum of the squares of numbers in range from 1 to 100
# here the map step produces a thread; reduce step sees that argument is a function, the argument is treated as a threadcurry;
# so that reduce obtains its value from a thread

r = reduce( ~arg threadcurry( ~func mapthread( ~arg threadcurry( ~func range ~first 1 ~second 100 ) ~func square ) ) ~func sum ~initval 0 )

println( ~msg 'sum of squares ' .. r )


