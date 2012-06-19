

days = {
  'sun' : 'sunday',
  'mon' : 'monday',
  'tue' : 'tuesday',
  'wed' : 'wednesday',
  'thur' : 'thursday',
  'fri'  : 'friday',
  'sat'  : 'Shabat'
}


for k keys( ~table days ) 
  println( ~msg 'short names for days of week' .. k .. ' long names ' .. days{ k } )
end

for k values( ~table days ) 
  println( ~msg 'long names for days of week' .. k )
end

for v each( ~table days )
  println( ~msg 'both short name [ v[0] ] and long name [ v[1] ] for day of week' )
end

# i dont like tuesdays 
erase( ~table days ~key 'tue' )

println( ~msg 'no more tuesdays' )

for v each( ~table days )
  println( ~msg 'both short name [ v[0] ] and long name [ v[1] ] for day of week' )
end



