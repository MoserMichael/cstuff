
sub make_stack_node( data optional )
  
  # this ugly construct forces to return a node in heap memory. 
  # i think the notion of a clear language has to be revised a bit ;-)
  l = [ [ Null, data ] ]
  return l[1]
end

# constructs a linked list stack
sub make_stack()
  e := make_stack_node( )
  return {
   'head' : e,
   'ncount' : 0,
  
   # add new element to the top of the stack
   'push' : sub( data )
        newnode := make_stack_node( ~data data )
        if defined( ~arg this . head[ 1 ] )
          newnode[ 1 ]  := this . head [ 1 ]
        end
        this . head[ 1 ] := newnode
        this . ncount = this . ncount + 1
        
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
        
        #dump( ~arg this . head )
        #println( ~msg '***' )

        return rt[ 2 ]
    end,

    'count' : sub() 
        return this . ncount
    end        
 
  }
end


