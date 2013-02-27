# get current date / time; functions: localtime, gmtime, currenttime

daysofweek = [ 'Sun', 'Mon', 'Tue', 'Wed', 'Thur', 'Fri', 'Sat' ]

ctime = currenttime()
println( ~msg 'The seconds passed since Jan 1 1970 UTC time [ ctime ] ' )

[ secs, mins, hour, dayofmonth, month, year, dayofweek, _ ] = localtime( ~epoch ctime )

println( ~msg 'UTC time: Today is [daysofweek[dayofweek+1]] the time is [hour]:[mins]:[secs]  [dayofmonth]/[month]/[year]' )
[ secs, mins, hour, dayofmonth, month, year, dayofweek, _ ] = gmtime( ~epoch ctime )

println( ~msg 'UTC time: Today is [daysofweek[dayofweek+1]] the time is [hour]:[mins]:[secs]  [dayofmonth]/[month]/[year]' )
