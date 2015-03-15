romanSym = [(100, 1500, 'M'), (100, 500, 'D'), (10, 100, 'C'), (10, 50, 'L'), (1, 10, 'X'),  (1, 5, 'V'), (0, 1, 'I')]

def romanNumerals( num ):
    ret = ""
    if num <= 0:
	return ""
    while num > 0:
	for n in romanSym:
	    if n[1] <= num:
		num -= n[1]
		ret = ret + n[2]
		break
	    elif (n[1] - n[0]) <= num:
		letter = -1
		for m in romanSym:
		    if m[1] == n[0]:
			letter = m[2]
			break;
		num -= n[1] - n[0]
		ret = ret + letter + n[2]
    return ret		

def loopTest():
    for n in romanSym:
	print n[0], n[1]


#for i in range(1,101):
#    print romanNumerals(i)

print romanNumerals( 17 )



