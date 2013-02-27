# test graph traversal class - graphutils.inc
include 'graphutils.inc'
include 'graphlist.inc'


r := make_graph_list()

r . addnode( ~index 1 )
r . addnode( ~index 2 )
r . addnode( ~index 3 )
r . addnode( ~index 4 )
r . addnode( ~index 5 )
r . addnode( ~index 6 )

r . addedge( ~from 1 ~to 2 )
r . addedge( ~from 1 ~to 3 )
r . addedge( ~from 2 ~to 4 )
r . addedge( ~from 2 ~to 5 )
r . addedge( ~from 5 ~to 6 )
r . addedge( ~from 5 ~to 1 )
r . addedge( ~from 2 ~to 3 )
r . addedge( ~from 3 ~to 1 )

u := make_graph_utils()

println( ~msg '*** breadth first search iterator ***' )
for i   u . breadth_first_search( ~graph r ~startnode 1 )
  println( ~msg 'node [ i ]' )
end

println( ~msg '*** depth first search iterator ***' )
for i   u . depth_first_search( ~graph r ~startnode 1 )
  println( ~msg 'node [ i ]' )
end


