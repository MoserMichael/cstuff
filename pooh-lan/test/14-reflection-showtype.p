
# --------------------------------------------------------

# kind of values 

val_scalar_n = 42
val_scalar_s = 'hello world'
val_vector = [ 1, 2, 3, 4 ]
val_table = { 'mon' : 'monday', 'tue' : 'tuesday' }
val_lambda = sub () return 42 end

println( ~msg 'type of val_scalar_n ' .. type( ~arg val_scalar_n ) )
println( ~msg 'type of val_scalar_s ' .. type( ~arg val_scalar_s ) )
println( ~msg 'type of val_vector '   .. type( ~arg val_vector ) )
println( ~msg 'type of val_table '    .. type( ~arg val_table ) )
println( ~msg 'type of val_lambda '   .. type( ~arg val_lambda ) )



