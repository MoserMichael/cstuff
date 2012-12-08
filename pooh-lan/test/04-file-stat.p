# check properties of a file

fileinfo = stat( ~filename 'aaa.aaa' )
if size( ~arg fileinfo ) == 0
  println( ~msg 'the file aaa.aaa does not exist' )
end

fileinfo = stat( ~filename ARGV[ 1 ] )

for k keys( ~table fileinfo )
  println( ~msg k .. ' = ' .. fileinfo{ k } )
end

testfile( ~filename ARGV[ 1 ] )

sub testfile( filename )
  println( ~msg 'file [ filename ] is [ filesize( ~filename filename ) ] bytes long' )

  if canread( ~filename filename )
    println( ~msg 'can read [ filename ]' )
  else
    println( ~msg ''can not read [ filename ]'' )
  end
  
  if canwrite( ~filename filename )
    println( ~msg 'can write [ filename ]' )
  else
    println( ~msg ''can not write [ filename ]'' )
  end
 
  if canrun( ~filename filename )
    println( ~msg 'can run [ filename ]' )
  else
    println( ~msg ''can't run [ filename ]'' )
  end
end
