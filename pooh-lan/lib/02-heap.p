include 'heap.inc'

println( ~msg '*** test min heap ***' )
h = make_min_heap()

for i range( ~from 30 ~to 1 ~step -1 )
    h . push( ~key i ~data  i ) # '[i]' )
end

while ! h.empty()
    [key, val ] = h.pop()

    println( ~msg 'key [ key ] value [ val ]' )
end

println( ~msg '*** test max heap ***' )
h = make_max_heap()

for i range( ~from 1 ~to 30 )
    h . push( ~key i ~data  i ) # '[i]' )
end

while ! h.empty()
    [key, val ] = h.pop()

    println( ~msg 'key [ key ] value [ val ]' )
end
