include 'queue.inc'

sub make_graph_utils()
  return {

   # breadth first search iterator : walk over the graph
   'breadth_first_search' : 
     sub( graph, startnode )
      
      seen = []
      current := make_queue()
      rt = []

      current . push( ~data startnode )

      n = startnode
      seen[ n ] = 1

      while current . count() != 0
	 n = current . pop( )

	 if isthreadmain()
            threadyield0( ~yieldval n )
	 else    
	    push( ~array rt ~top n )
	 end	
	 
 
      	 for i graph . outedges( ~from n ) 
	    if ! defined( ~arg seen[ i ] )
	      seen[ i ] = 1
	      current . push( ~data i )
	    end
	 end
      end

      if ! isthreadmain()
        return rt
      end
     end,

   # depth first search iterator : walk over the graph
   'depth_first_search' : 
     sub( graph, startnode )
      
      seen = []
      current = []
      rt = []

      push( ~array current ~top startnode )

      n = startnode
      seen[ n ] = 1

      while size( ~arg current ) != 0
	 n = pop( ~array current )
 

         if isthreadmain()
            threadyield0( ~yieldval n )
         else    
            push( ~array rt ~top n )
	 end		 
 	 for i graph . outedges( ~from n ) 
	    if ! defined( ~arg seen[ i ] )
	      seen[ i ] = 1
	      push( ~array current ~top i )
	    end
	 end

     end

      if ! isthreadmain()
        return rt
      end
     end

   }

end
