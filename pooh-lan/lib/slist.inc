
sub make_slist_node( data optional )
  
  # this ugly construct forces to return a node in heap memory. 
  # i think the notion of a clear language has to be revised a bit ;-)
  l = [ [ Null, data ] ]
  return l[1]
end

# constructs a linked list stack
sub make_slist()
  e := make_slist_node( )
  return {
   'headp' : e,
   'ncount' : 0,

   'head' : sub()
      rt := this . headp
      return rt
   end,

   'data' : sub( pos )
      return pos[ 2 ]
   end,

   'next' : sub( pos )
      rt := pos[ 1 ]
      return rt
   end,

   'nth' : sub( num )
      pos := this . headp
      while num > 0
        if not defined( ~arg pos )
	  return Null
        end	  
        num = num - 1
	pos := pos [ 1 ]
      end
      return pos
    end,

    'insert' : sub ( pos, data )
      new := make_slist_node( ~data data )
      new[ 1 ] := pos[ 1 ]
      pos[ 1 ] := new
      this . ncount = this . ncount + 1
    end,

    'remove' : sub( pos )
       rt := pos[ 1 ]

       if defined( ~arg rt )
         pos[ 1 ] := rt[ 1 ]
	 this . ncount = this . ncount - 1
	 return true
       end	 
       return false       
    end,

    'count' : sub() 
        return this . ncount
    end,

    'map' : sub()
       pos := this . head[ 1 ]
       
       while defined( ~arg pos )
         println( ~msg pos[ 2 ] )
         pos := pos[ 1 ]
       end

    end
 
  }
end




