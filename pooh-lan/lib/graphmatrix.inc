

sub make_graph_matrix( keepinedge )
  return {
    'nullnode' : [],
    'nodes' : [],
    'deletednods' : [],

    'links' : [],
    'linksreverse' : [],
    'keepinedge' : defined( ~arg keepinedge ),

    # add a new node to the graph; the index of the new node is returned;
    # you can attach data to the node (if data argument is not Null)
    # you can set the node index with optional index parameter
    'addnode' : 
      sub( data optional , index optional )
      	if ! defined( ~arg index )
          if size( ~arg this . deletednodes ) == 0
            num = size( ~arg this . nodes ) + 1
          else
            num = pop( ~array this . deletednodes )
          end
	else
	  num = index
	  if defined( ~arg this . nodes[ num ] )
	    return false
	  end
	end
    
	if defined( ~arg data )
	  this . nodes[ num ] := data
	else
	  this . nodes[ num ] := this . nullnode
	end
      end,

    # return a reference to the data attached to node with index node
    'nodedata' :
      sub( node )
	if ! defined( ~arg this . nodes[ node ] )
	  return Null
	end

	rt := this . nodes[ node ]
	if issame( ~a rt ~b this . nullnode )
	  return Null
	else
	  return this . nodes[ node ]
	end
      end,
       
    # delete a node with given index.
    'delnode' :
      sub (node)
	if ! defined( ~arg this . nodes[ node ] )
	  return false
	end
	this . nodes[ node ] = Null 
	
	if this . keepinedge == 1
	  this . linksreverse[ node ] = Null
	end

	push( ~array this . deletednods ~top node )
	return true
      end, 

    # iterator, returns the index of each node in the graph.
    'eachnode' : 
      sub ()
        rt = []
	for i range( ~from 1 ~to size( ~arg this . nodes ) )
	    rt := this . nodes[ i ]

	    if defined( ~arg rt ) and not issame( ~a rt ~b this . nullnode )
		if isthreadmain()
		  threadyield0( ~yieldval i )
		else
		  push( ~array rt ~top i )
		end
	    end
	end
	if not isthreadmain()
	  return rt
	end
      end,

    # iterator, returns the index of each node in the graph and the data of the node
    'eachnodewithdata' : 
      sub ()
        rt = []
	for i range( ~from 1 ~to size( ~arg this . nodes ) )
	    rt := this . nodes[ i ]

	    if defined( ~arg rt ) and not issame( ~a rt ~b this . nullnode )
		if isthreadmain()
		  threadyield0( ~yieldval [ i , this . nodedata( ~node i ) ] )
		else
		  push( ~array rt ~top  [ i , this . nodedata( ~node i ) ] )
		end
	    end
	end
	if not isthreadmain()
	  return rt
	end
      end,

    # add a new edge to the graph.
    'addedge' :
      sub (from,to,linkdata optional)  
      	if ! defined( ~arg this . nodes[ from ] )
	  return false
	end
      	if ! defined( ~arg this . nodes[ to ] )
	  return false
	end

     
    	if defined( ~arg linkdata )
	  d := linkdata	    
	else
	  d := this . nullnode
	end
	
	this . links [ from ] [ to ] := d
	if this . keepinedge == true
	  this . linksreverse [ to ] [ from ] := d
	end
      end,

    # returns true if edge exists from node with index from to node with index to
    'hasedge' :
       sub (from, to )

      	if ! defined( ~arg this . nodes[ from ] )
	  return false
	end
      	if ! defined( ~arg this . nodes[ to ] )
	  return false
	end

	if  defined( ~arg this . links [ from ] [ to ] )
	  return true
	end
	return false
     end,

    # returns data associated to edge from node with index from to node with index to
    'edgedata' : 
      sub( from, to)
      	if ! defined( ~arg this . nodes[ from ] )
	  return Null
	end
      	if ! defined( ~arg this . nodes[ to ] )
	  return Null
	end

	rt := this . links [ from ] [ to ] 
	if ! defined( ~arg rt ) or issame( ~a rt ~b this . nullnode )
	  return Null
	end
        return rt
      end,

    # delete edge that leads from node with index from to node with index to
    'deledge' :
      sub (from, to )
       	if ! defined( ~arg this . nodes[ from ] )
	  return false
	end
      	if ! defined( ~arg this . nodes[ to ] )
	  return false
	end
	
	this . links [ from ] [ to ] := Null
	if this . keepinedge == true
	    this . linksreverse [ from ] [ to ] := Null
	end
	return false
      end,      

    # iterator - returns the index of all edges that lead out of node with index from
    'outedges' :
      sub (from)
	if ! defined( ~arg this . nodes[ from ] )
	  return Null
	end

	rt = []
	for i range( ~from 1 ~to size( ~arg this . links[ from ] ) )
	  
          if ! defined( ~arg this . links [ from ] [ i ]  ) 
	     skip
	  end

	  if isthreadmain()
	     threadyield0( ~yieldval i )
	  else
	     push( ~array rt ~top i )
	  end
	end
        if  not isthreadmain()
	  return rt
	end
      end,

    # iterator - returns edges that lead into node with index from
    'inedges':
       sub (from)
	if ! defined( ~arg this . nodes[ from ] )
	  return Null
	end

	if  this . keepinedge  == true
	  rt = []
	  for i range( ~from 1 ~to size( ~arg this . linksreverse[ from ] ) )
	    v := this . linksreverse [ from ] [ i ]
	    if not defined( ~arg v ) 
	      skip
	    end	

	    if isthreadmain()
	      threadyield0( ~yieldval i )
	    else
	      push( ~array rt ~top i )
	    end
	  end
          if  not isthreadmain()
	    return rt
	  end
       	else
          rt = []
          for n this . eachnode() 
            if this . hasedge( ~from n ~to from )
                if isthreadmain()
                  threadyield0( ~yieldval n )
                else
                  push( ~array rt ~top n )
                end
              end
            end
            if not isthreadmain()
              return rt
            end
 	end
     end
  }      
end
