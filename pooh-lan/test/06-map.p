


# --------------------------------------

sub mapcopy( map , func )

  ret = []
  
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


# --------------------------------------

sub squarebyref ( arg byref )
  arg  = arg * arg
end


sub mapinplace( map , func )
  
  # another feature of for statement:
  # the index variable holds a reference to the collection entry (if iterating over a collection) 

  for cur map
      func( ~arg cur )
  end
end


arr = [ 2, 3, 4, 5, 6 ]

mapinplace( ~map arr ~func squarebyref )

for c arr
  print( ~msg 'squared number is [ c ]' )
end



     
   
