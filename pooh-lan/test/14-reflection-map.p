


# --------------------------------------

sub mapcopy( map , func )

  ret = []
  
  # like 06-map.p but now the function checks if the argument function has the required prototype
  pinfo = functionparameters( ~function func )
  
  if size( ~arg pinfo ) != 1 
    println( ~msg 'mapcopy: argument function does not receive exactly one parameter' )
  end
 
  if pinfo[0]{'label'} ne '~arg'
    println( ~msg 'mapfunction: argument function must have parameter with label arg' )
  end


  # because map is in for statement, the type of map is assumed to be 'vector'
  for cur map
     push( ~array ret ~top func( ~arg cur ) )
  end

  return ret
end


sub square ( arg )
  return arg * arg
end

for c mapcopy( ~map [ 1, 2, 3, 4, 5] ~func square )
   print( ~msg 'squared number is [ c ]' )
end




     
   
