# test complex number library - complex.inc
include 'lib/complex.inc'


n = make_rect_complex( ~re 0 ~im 1 )
m = make_rect_complex( ~re 0 ~im 1 )

res = n . mult( ~num m )

println( ~msg 'square of i is' )
res . print()


res = res . add( ~num res )

println( ~msg '
twice i is' )
res . print()


println( ~msg '
absolute is [ res . absolute () ]' )



