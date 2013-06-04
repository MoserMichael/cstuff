


sub foo()
  return 42
end

a  = 1

# string template with statement; this translates into an anonymous function
println( ~msg '  aaa [ global.a = 2 return 'a' ]' )


# string template with function call:
println( ~msg '  bbb [ 5 * foo() ] ' )

# the first template changed value of a global as a side effect.
println( ~msg a )
