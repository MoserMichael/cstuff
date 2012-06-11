

lambda = sub ( str_a, num_b, vect_c, hash_d, func_e )
  if hash_d{ 'action' } eq 'do' and str_a eq 'do'
    return func_e( ~arg num_b )
  end
end


# functionparameters returns information about parameters of a function object.

for pinfo functionparameters( ~function lambda )
   println( ~msg 'Argument label [ pinfo{'label'} ] Argument types [ pinfo{'type'} ] isoptional [ pinfo{'isoptional'} ] isbyref [ pinfo{'isbyref'} ]' )
end

println( ~msg 'return type ' .. functionreturntype( ~function lambda ) )



