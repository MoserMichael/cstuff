# load an extension library written in C and call exported function

loadextension 'libextlibexample'


c = addnumbers( ~a 42 ~b 3 )
println( ~msg c )
