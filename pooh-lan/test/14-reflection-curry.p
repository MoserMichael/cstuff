
#-----------------------------------------

# thre dots as parameter definition means that any combination of parameters goes.

sub makecurry( ... )

   # without parameters info on calling function parametersis returned
   params = functionparameters( )

   # first parameter is supposed to be the function.
   if params[0]{'label'} eq 'func'
     exit( ~msg 'makecurry: first parameter must have label func' )
   end
   if params[0]{'type'} ne 'function'
     exit( ~msg 'makecurry: first parameter must be a function')
   end

   func = params[0]{'valueref'}

   shift( ~array param)

   return sub ()
      makethread( ~thread func)
      return applyfunc( ~func func ~params params)
   end
end

#-----------------------------------------


# create curry - parameters of the call are recorded, the call is made later.
curry = makecurry( ~func range ~from 1 ~to 10 ) 

# now the function is called.
print( ~msg curry() )



