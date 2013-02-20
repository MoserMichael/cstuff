
sub make_slist_node( data optional )
  
  # this ugly construct forces to return a node in heap memory. 
  # i think the notion of a clear language has to be revised a bit ;-)
  l = [ [ Null, data ] ]
  return l[1]
end

# constructs a single linked list . 
# Each node of the list has a link to the next node ; one can move forward among the links of the list, but not backward.
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
      newnode := make_slist_node( ~data data )
      if defined( ~arg pos[ 1 ] )
        newnode[ 1 ] := pos[ 1 ]
      end
      pos[ 1 ] := newnode
      this . ncount = this . ncount + 1

      return newnode
    end,

    'remove' : sub( pos )
       if ! defined( ~arg pos )
	  return false
       end	  
       
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

    'mapcopy' : sub( func )
       rt := make_slist()
       rpos := rt . headp

       pos := this . headp[ 1 ]
       while defined( ~arg pos )
         fres := func( ~arg pos [ 2 ] )
	 rt . count()
	 rpos := rt . insert( ~pos rpos ~data fres )
         pos := pos[ 1 ]
       end
       return rt
    end,

    'mapreplace' : sub( func )
       pos := this . headp[ 1 ]
       while defined( ~arg pos )
         pos[ 2 ] := func( ~arg pos [ 2 ] )
         pos := pos[ 1 ]
       end
    end,

    'filtercopy' : sub( func )
       rt = make_slist()
       rpos := rt . head()
       pos := this . headp[ 1 ]
       while defined( ~arg pos )
         if func( ~arg pos [ 2 ] )
	    data := pos[ 2 ]
	    rpos := rt . insert( ~pos rpos ~data data )
	 end
         pos := pos[ 1 ]
       end
       return rt
    end,

    'foldfirst2last' : sub( func, initval )
       pos := this . headp[ 1 ]
       first = 1
       rt = Null
       while defined( ~arg pos )
         if first
            rt = func( ~a initval ~b pos[ 2 ] )
            first = 0
         else
            rt = func( ~a rt ~b pos[ 2 ] )
         end
         pos := pos[ 1 ]
       end
       return rt
     end
  }
end




