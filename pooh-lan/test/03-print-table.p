

days =  {
          'sun'  : 'Sunday',
          'mon'  : 'Monday' ,
          'tue'  : 'Tuesday' ,
          'wed'  : 'Wednesday' ,
          'thur' : 'Thursday' ,
          'fri'  : 'Friday' ,
          'sat'  : 'Shabbat'
        }

daykeys = keys( ~table days )
i = 1

println( ~msg '**** value { keys  } ***' )

while daykeys[ i ] != Nill
    println( ~msg  days{ daykeys[ i ] }   )
    i = i + 1
end

println( ~msg '*** value ***' )

daykeys = values( ~table days )
i = 1

while daykeys[ i ] != Nill
    println( ~msg  daykeys[ i ]    )
    i = i + 1
end

println( ~msg '*** each ***' )
v = each(  ~table days )

i = 1
while i <= size( ~arg v )
  e = v[ i ] 
  println( ~msg e[ 1 ] )
  println( ~msg e[ 2 ] )
  i = i + 1
end
