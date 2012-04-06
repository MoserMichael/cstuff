

a = 1

b := a

print ( "a is reference to b " .. a .. " " .. b .. "\n" )

a = 10

print ( "both values changed " .. a .. " " .. b .. "\n" )


b = a

print ( "a is copy of b " .. a .. " " .. b .. "\n" )

a = 20

print ( "both are different " .. a .. " " .. b .. "\n" )




