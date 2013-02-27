
sub make_queue_node( data optional )
  
  # this ugly construct forces to return a node in heap memory. 
  # i think the notion of a clear language has to be revised a bit ;-)
  l = [ [ Null, data ] ]
  return l[1]
end

# constructs a linked list stack
sub make_queue()
  e := make_queue_node( )
  return {
   'head' : e,
   'tail' : e,
   'ncount' : 0,
  
   # add new element to the top of the stack
   'push' : sub( data )
        newnode := make_queue_node( ~data data )
        this . tail[ 1 ] := newnode
        this . ncount = this . ncount + 1

	#this . tail := Null
	this . tail := newnode
        
        #dump( ~arg this . head )
        #println( ~msg '***' )
     end,

    # removes an element from the stack; the last value inserted with push is the first one returned by pop.
   'pop' : sub( )
        if ! defined( ~arg this . head[ 1 ] )
          return Null
        end
        rt := this . head[ 1 ]
        this . head[ 1 ]   := rt[ 1 ]
        this . ncount = this . ncount - 1

	if this . ncount == 0
	   this . tail := this . head 
	end
        
        #dump( ~arg this . head )
        #println( ~msg '***' )

        return rt[ 2 ]
    end,

    'count' : sub() 
        return this . ncount
    end        
 
  }
end


