

text = 'the quick brown fox jumped over the lazy dog'

println( ~msg '*** split in for loop ***' )
for f split( ~string text )
  println( ~msg f )
end

text = 'the quick brown fox jumped over the lazy dog'
tokens = split( ~string text )

println( ~msg '*** split returning an array of tokens ***' )

for f tokens 
   println( ~msg f )
end

text = 'the:quick:brown:fox:jumped:over:the:lazy:dog twice'

println( ~msg '*** split with the : separator ***' )
for f split( ~string text ~separator ':' )
  println( ~msg f )
end
