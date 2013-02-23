
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

   # returns position of the head of the list - 
   # this is an empty node that is maintained for each list; also known as the 0th node.
   'head' : sub()
      rt := this . headp
      return rt
   end,

 
   # given a position (pos argument) returns the data entry for the node identified by the position
   'data' : sub( pos )
      return pos[ 2 ]
   end,

   # given a position (pos argument) returns the next entry following pos.
   'next' : sub( pos )
      rt := pos[ 1 ]
      return rt
   end,

   # returns position of the nth element in the list (0th element is the head of the list)
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

    # insert new node after node identified by pos argument , the new node will have data argument attached to it.
    'insert' : sub ( pos, data )
      newnode := make_slist_node( ~data data )
      if defined( ~arg pos[ 1 ] )
        newnode[ 1 ] := pos[ 1 ]
      end
      pos[ 1 ] := newnode
      this . ncount = this . ncount + 1

      return newnode
    end,

    # remove the node after the node identified by pos argument
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

    # returns the number of elements in the list; the list object has attached counter of elements.
    'count' : sub() 
        return this . ncount
    end,

    # apply argument function func to each element in the list; returns a new list made up of the results.
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

    # apply argument function func to each element of the list; the original list data entries are replaced with call results.
    'mapreplace' : sub( func )
       pos := this . headp[ 1 ]
       while defined( ~arg pos )
         pos[ 2 ] := func( ~arg pos [ 2 ] )
         pos := pos[ 1 ]
       end
    end,

    # apply argument function to each element of the list; if it returns true then add reference to list data to return list
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

    # fold the first to last element in the list
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
     end,

     # list iterator: 
     'range' : sub( from optional, to optional )
	
        
	if ! defined( ~arg from )
          n = 1 
        else
          n = from
	  if n < 1
	    n = 1
	  end
	end

	if ! defined( ~arg to )
	   m = -1
        else 
           m = to
	end
    
	pos := this . nth( ~num n )

        if isthreadmain()
	    while defined( ~arg pos ) 
		if m != -1 and n > m
		    break
		end
		threadyield0( ~yieldval pos[ 2 ] )
		pos := pos[ 1 ]
                n = n + 1
	    end
	else
	    rt = []
	    while defined( ~arg pos ) 
		if m != -1 and n > m
		    break
		end
		tmp := pos[ 2 ]
		push( ~array rt ~top tmp )
		pos := pos[ 1 ]
                n = n + 1
	    end	
	    return rt
	end
     end
  }
end




