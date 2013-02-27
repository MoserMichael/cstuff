

sub foo()
  return {
    'arr' : [],
    'add' : sub( data )
       this . arr[ 1 ] := data
     end,
     'show' :sub( )
        dump( ~arg this . arr )
     end
   }
end   

r := foo()

r . add( ~data [ 1, 2] )
r . show()
