
f = openpipe( ~cmdline 'tr a A' )

f.write( ~towrite 'aaaaaaa' ~numwrite 7 )

f.closewrite()

output = emptystring()

f.read( ~toread output ~numread 7 )

status = f.wait()

println( ~msg 'output: [ output ] status: [ status ]' )




