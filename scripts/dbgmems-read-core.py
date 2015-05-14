

# set to True if only the first error is to be reported
stopOnFirstError=False

# set to True if output of each gdb command is displayed.
do_debug=False
cacheLineLocation = {}

def safe_str(obj):
    try:
        return str(obj)
    except UnicodeEncodeError:
        # obj is unicode
        return unicode(obj).encode('unicode_escape')


def my_eval( expr ):
    if do_debug:
        gdb.write( '#' + safe_str( expr ) + '\n' )
    ret = gdb.parse_and_eval( expr )
    if do_debug:
        gdb.write('>' + safe_str(ret) + '\n')
    return ret


def my_eval_ptr( expr ):
        r = my_eval( expr )
        ret = safe_str( r ) . split() [ 0 ]
        if do_debug:
                gdb.write( '>' + safe_str( ret ) + '\n' )   
        return ret              

LogFile = 'memerrors.log'

arena_size = my_eval('(int) ARENA_SIZE')
root = my_eval_ptr( '(char *) &root' ) 
ptr_size = my_eval( 'sizeof(void *)' )

if int( ptr_size ) == 8:
	sentinel_valign = '13758495683704258285'
	sentinel =  '12456583691736362206'
else:	
	sentinel_valign = '3203399405'
	sentinel = '2900274398' 
	

cur_node = root 
First = True
num = 1
errors = 0

with open(LogFile, 'w') as of:
    while True:
            next_node = my_eval_ptr( '((MEM_ENTRY *) ' + cur_node + ' )->entry.next' ) 
            prev_node = my_eval_ptr( '((MEM_ENTRY *) ' + next_node + ' )->entry.prev' ) 
            uptr = my_eval_ptr( ' ((char *) ' + cur_node + ' + ' + str(arena_size) + ') ' ) 
            usize = my_eval( '((MEM_ENTRY*) ' + cur_node + ' )->size' )

            if not First:
                smagic = my_eval( ' * (size_t *) ((char *) ' + cur_node + ' + ' + str(arena_size) + ' - sizeof(size_t) ) ' ) 

                err = ''

                Valid = True
                if str(smagic) != sentinel and str(smagic) != sentinel_valign:
                        Valid = False
                        err = 'lower bound of ' + uptr + ' overwritten !!\n'

                smagic = my_eval( ' * (size_t *) ((char *) ' + cur_node + ' + ' + str(arena_size) + ' + ' + str(usize) + ' ) ' )                    
                if str(smagic) != sentinel:
                        Valid = False
                        err = err + 'upper bound of ' + uptr + ' overwritten !!\n'
                 
                if prev_node != cur_node:
                        Valid = False                     
                        err = err + 'invalid  node ' + cur_node + '\n'
                        if not First:
                                err = err + 'invalid pointer value: ' + uptr + ' (next arena does not point back at this arena)\n'
                
                if not Valid:
                    of.write( '*** bad pointer '  + str( uptr ) + ' size? ' + str(usize) + ' ***\n    ' + err  + '\n' )
                    stack_frames = my_eval('(int) STACK_FRAMES' )
                    for i in range( 0, int(str(stack_frames)) ):
                        frame = my_eval( ' ((MEM_ENTRY *) ' + cur_node + ' ).frames[ ' + str( i ) + ' ]' )
                        of.write(' frame #' + str( i ) + ' : ' + str( frame ) + '\n' )
 
                        addr = str( frame ) . split() [ 0 ] 
	
                        if not addr in cacheLineLocation:
                                l = 'info line *'  + addr 
                                s = gdb.execute( l, False, True )
                                cacheLineLocation[ addr ] = s
                                of.write( safe_str( s ) + '\n' )
                        else:
                                of.write( cacheLineLocation[ addr ] + '\n' )
                    errors += 1			
                    if stopOnFirstError:
                        break
                else:
                    num += 1
 			
            cur_node = next_node
            First = False       

            if cur_node == root:
                    break             
            
if errors == 0:
        gdb.write( '*** all memory is consistent (' + str( num ) + ' checked ) ***\n' )
else:
	gdb.write( '*** errors detected ( ' + str(errors) + ' ) errors; (' + str( num ) + ' ) good blocks ****\n*** please examine ' + LogFile + ' for more details ***\n' )
