# run process and script send data to it via stdin and read stdout

f = openpipe( ~cmdline 'tr a A' )

f.write( ~towrite 'aaaaaaa' ~numwrite 7 )

f.closewrite()

output = emptystring()

f.read( ~toread output ~numread 7 )

status = f.wait()

println( ~msg 'output: [ output ] status: [ status ]' )




