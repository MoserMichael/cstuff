# function call by reference , call by value

a = 'ccc'

println( ~msg a )

funbyref( ~arg a )

println( ~msg a )

funbyarg( ~arg a )

println( ~msg a )


sub funbyref( arg byref )
    arg = 'aaa'
end

sub funbyarg( arg )
    arg = 'bbb'
end


