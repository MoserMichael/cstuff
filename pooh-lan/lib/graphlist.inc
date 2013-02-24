include 'slist.inc'

# makes an adjacency graph - a graph where the links between nodes are stored in linked list
# The argument keepinedge says if fast lookup of reverse edges is enabled 
# this means that for each edge an entre for the reverse direction is maintained

sub make_graph_list( keepinedge optional )
  return {
    
    'nodes' : [],
    'deletednodes' : [],
    'keepinedge' : defined( ~arg keepinedge ),

    # add a new node to the graph; the index of the new node is returned;
    'addnode' : 
      sub ( data optional, index optional )
        links := make_slist()

	if !defined( ~arg index )
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

        this . nodes [ num ] := [ data, links ]
        return num
      end,

    # return a reference to the data attached to node with index node
    'nodedata' :
      sub( node )
        rt := this . nodes[ node ]
        if ! defined( ~arg rt )
          return Null
        end
        return rt[ 1 ]
      end,

    # delete a node with given index.
    'delnode' :
      sub (node)
        rt := this . nodes[ node ]
        if ! defined( ~arg rt )
          return false
        end
        
        # delete all reverse edges
        if  this . keepinedge 
          nlist := rt[ 2 ]
          for n nlist . range()
            to = n[ 1 ]
            if to < 0
              this . deledgeimp( ~from - to ~to node )
            end
          end
        end

        this . nodes[ node ] = Null
        push( ~array this . deletednodes ~top node )
        return true
      end,

    # iterator, returns the index of each node in the graph.
    'eachnode' : 
      sub ()
        if isthreadmain()
           for i range( ~from 1 ~to size( ~arg this . nodes ) )
              if defined( ~arg this . nodes[ i ] )
	        threadyield0( ~yieldval i )
              end
           end
        else
	   rt = []
           for i range( ~from 1 ~to size( ~arg this . nodes ) )
              if defined( ~arg this . nodes[ i ] )
	         push( ~array rt ~top i )
              end
           end
           return rt
        end
      end,

    # iterator, returns the index of each node in the graph and the data of the node
    'eachnodewithdata' : 
      sub ( )
        if isthreadmain()
           for i range( ~from 1 ~to size( ~arg this . nodes ) )
              if defined( ~arg this . nodes[ i ] )
		threadyield0( ~yieldval [ i , this.nodes[ i ] [ 2 ] ] )
              end
           end
        else
	   rt = []
           for i range( ~from 1 ~to size( ~arg this . nodes ) )
              if this . nodes[ i ] != Null
	        push( ~array rt ~top [ i , this.nodes[ i ] [ 2 ] ] )
              end
           end
           return rt
        end
      end,

    # add a new edge to the graph.
    'addedge' :
      sub (from,to,linkdata optional)  
         
        rt := this . nodes[ from ]
        if ! defined( ~arg rt )
          return false
        end

        if ! defined( ~arg this . nodes[ to ] )
          return false
        end
        
        nlist := rt [ 2 ]

        # check that this is a new edge
        for n nlist . range( )
          if n[ 1 ] == from
            return false
          end
        end 
    
        nlist . insert( ~pos nlist . head( ) ~data [ to , linkdata ] )
        if this . keepinedge == true
          rt := this . nodes[ to ]
          nlist := rt[ 2 ]
          nlist . insert( ~pos nlist . head( ) ~data [ - from , Null ] )
        end

        return true
      end,

    'hasedge' :
       sub (from, to )
        rt := this . nodes[ from ]
        if ! defined( ~arg rt )
          return false
        end
        nlist := rt [ 2 ]

        for n nlist . range( )
          if n[ 1 ] == to
            return true
          end
        end
        return false
       end,

    'edgedata' : 
      sub( from, to)
        rt := this . nodes[ from ]
        if ! defined( ~arg rt )
          return Null
        end
        nlist := rt [ 2 ]

        for n nlist . range( )
          if n[ 1 ] == to
            return n[ 2 ]
          end
        end

        return Null
      end,

    'deledge' :
      sub (from, to )
        if this . deledgeimp( ~from from ~to to ) and this . keepinedge == true
          this . deledgeimp( ~from to ~to - from )
        end
      end,

    'deledgeimp' : 
      sub( from, to )
        rt := this . nodes[ from ]
        if ! defined( ~arg rt )
          return Null
        end
        nlist := rt [ 2 ]

        prev := nlist . head()
        pos := nlist . nth( ~num 1 )
 
        while defined( ~arg pos )
          data := pos[ 2 ]
          if data[ 1 ] == to
              nlist . remove( ~pos prev )
              return true
          end
          pos := pos[ 1 ] 
          prev := prev[ 1 ]
        end
        return false
     end,

    'outedges' :
      sub (from)
        rt := this . nodes[ from ]
        if ! defined( ~arg rt )
          return Null
        end
        nlist := rt [ 2 ]
        if isthreadmain()
          for n nlist . range()
            if n[ 1 ]  > 0
	      threadyield0( ~yieldval n[ 1 ] )
            end
          end
        else
          rt = []
          for n nlist . each()
            if n[ 1 ]  > 0
	      push( ~array rt ~top n[ 1 ] )
            end
          end
          return rt
        end
      end,


    'inedges':
       sub (from)
         if this . keepinedge == true
            rt := this . nodes[ from ]
            if ! defined( ~arg rt )
                return Null
            end
            nlist := rt [ 2 ]

            if isthreadmain()
              for n nlist . range()
                if n[ 1 ]  < 0
                  threadyield0( ~yieldval - n[ 1 ] )
                end
              end
            else
              rt = []
              for n nlist . range()
                if n[ 1 ]  < 0
                  push( ~array rt ~top - n[ 1 ] )
                end
              end
              return rt
            end
         else
            rt = []
            for n this . eachnodes() 
              if this . hasedge( ~from n ~to from )
                if isthreadmain()
                  threadyield0( ~yieldval n )
                else
                  push( ~array rt ~top n )
                end
              end
            end
            if isthreadmain()
              return rt
            end
         end
       end
  }
end

