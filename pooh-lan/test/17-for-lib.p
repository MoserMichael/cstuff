# co-routines / generators as used with for statement

println( ~msg '*** range of numbers ***' )

for i range( ~from 1 ~to 10 )
  println( ~msg 'value ' .. i )
end

println( ~msg '*** for each key and value in table ***' )

days =  {
          'sun'  : 'Sunday' ,
          'mon'  : 'Monday' ,
          'tue'  : 'Tuesday' ,
          'wed'  : 'Wednesday' ,
          'thur' : 'Thursday' ,
          'fri'  : 'Friday' ,
          'sat'  : 'Shabbat'
        }

for [ shortname, fullname ] each( ~table days )
   println( ~msg shortname .. ' => ' .. fullname )
end

println( ~msg '*** for each key and value in table ***' )

for dayval each( ~table days )
    println( ~msg dayval[ 1 ] .. ' => ' .. dayval[ 2 ] )
end

println( ~msg '*** for each key in table ***' )

for val keys( ~table days )
   println( ~msg val )
end

println( ~msg '*** for each value in table ***' )

for val values( ~table days )
   println( ~msg val )
end




