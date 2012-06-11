

a = 1

b := a

println ( ~msg 'a is reference to b ' .. a .. ' ' .. b  )

a = 10

println ( ~msg 'both values changed ' .. a .. ' ' .. b  )


b = a

println ( ~msg 'a is copy of b ' .. a .. ' '  .. b  )

a = 20

print ( ~msg 'both are different ' .. a .. ' ' .. b )




