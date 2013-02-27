# working with files

# read the whole file into a buffer

f = open( ~filename 'test/11-file-test' )

buf = newline()
f.read( ~toread buf ~numread 1000 )

println( ~msg buf )

f.close()


# read the whole file line by line

f = open( ~filename 'test/11-file-test-lines' )

for ln readlines( ~file f )
    println( ~msg ln )
end

f.close()



