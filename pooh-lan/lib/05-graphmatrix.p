include 'graphmatrix.inc'

r := make_graph_matrix( ~keepinedge 1 )

r . addnode( ~index 1 ~data [1, 2] )
r . addnode( ~index 2 ~data [2, 3] )
r . addnode( ~index 3 ~data [3, 4] )

r . addedge( ~from 1 ~to 2 )
r . addedge( ~from 1 ~to 3 )
r . addedge( ~from 2 ~to 1 )
r . addedge( ~from 2 ~to 3 )
r . addedge( ~from 3 ~to 1 )
r . addedge( ~from 3 ~to 2 )

r . addedge( ~from 4 ~to 6 )
r . addedge( ~from 5 ~to 6 )
r . addedge( ~from 5 ~to 6 )
r . addedge( ~from 5 ~to 7 )
r . addedge( ~from 6 ~to 7 )
r . addedge( ~from 7 ~to 1 )

println( ~msg '** the nodes **' )
for n  r . eachnode()
  d := r . nodedata( ~node n )
  println( ~msg ''node [[ n ]] data [ [[ join( ~array d ~separator ' ' ) ]] ]'' )
end

#println( ~msg '** the nodes with data  **' )
#for nd  r . eachnodewithdata()
#  println( ~msg nd[1] .. ' ' .. nd[2] )
#end


println( ~msg 'out edges for node 1' )
for e  r . outedges( ~from 1 )
   println( ~msg e )
end

println( ~msg 'in edges for node 1' )
for e  r . outedges( ~from 1 )
   println( ~msg e )
end

println( ~msg 'delete edge 1-2' )
r . deledge( ~from 1 ~to 2 )


println( ~msg 'out edges for node 1' )
for e  r . outedges( ~from 1 )
   println( ~msg e )
end

println( ~msg 'in edges for node 1' )
for e  r . outedges( ~from 1 )
   println( ~msg e )
end


















