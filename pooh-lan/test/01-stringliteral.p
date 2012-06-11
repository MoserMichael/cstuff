# example of string literals

# that's how you say 'empty string' .
empty = "

println( ~msg 'simple 
multiline string' );

println( ~msg ''another simple 
multiline string'' );

println( ~msg '''yet another simple 
multiline string''' );

a=[1,2,3]


# text within [ and ] is code, result is inserted into the string. (quoted code)
println( ~msg 'quotes with arrays [ a[1] + a[2] + a[3] ] ok' )

# double '' means that the embedded code is also stated within [[ ]]
println( ~msg ''quotes, ten times ten euros is [[ 10 * 10 ]] euros'' )



