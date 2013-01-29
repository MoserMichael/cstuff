include 'heap.inc'

h = make_min_heap()

for i range( ~from 3 ~to 1 ~step -1 )
    h . push( ~key i ~data  i ) # '[i]' )
end

while ! h.empty()
    [key, val ] = h.pop()

    println( ~msg 'key [ key ] value [ val ]' )
end
