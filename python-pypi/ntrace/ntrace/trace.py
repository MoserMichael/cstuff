#!/usr/bin/python

""" utilities for tracing of python scripts; works similar to trace.py, it can also print out function parameters and return values
as well as modification of global and local variables.

can be invoked as a script:

./ntrace [options] <python-file-to-trace> [command line of python file to trace]

By default it shows function parameters and function return values

Options:
    -f		    trace function parameter calls (default: on)
    -F		    do not trace function parameters

    -r		    trace function return values  (default: on)
    -R		    do not trace function return values

    -l		    trace source lines before they are executed (default: on)
    -L		    do not trace source lines before they are executed

    -s		    trace setting of variablesa (default: off)
    -S		    do not trace setting of variables

    -g		    trace getting of variables (default: off)
    -G		    do not trace getting of variables

    -o		    print out object content (default: off)
    -O		    do not print out object content

    -t		    show the current thread id in prefix for each source line (default on)
    -T		    hide the current thread id	

    -n		    show the nesting level, how many levels down the call stack (default on)
    -N		    hide the nesting level

By default the following options are on: -l -f -r

can be invoked as an api; example:


    import ntrace

    t = ntrace.NTracer(ntrace.TO_FUNCTION_ARGUMENTS | ntrace.TO_FUNCTION_RVALUES | ntrace.TO_TRACE_SRC_LINES )
    t.trace( 'test_ntrace2.py' , False)

Written by Michael Moser (c) 2015
"""

import sys
import os
import linecache
import pprint
import inspect
import types
import collections

try:
    import thread
    import threading
except ImportError:
    _settrace = sys.settrace
    _THREADING = 0
else:
    _THREADING = 1
    def _settrace(func):
        threading.settrace(func)
        sys.settrace(func)

try:
    import ntracenative
except ImportError:
    _NTRACEACTIVE = 0
else:
    _NTRACEACTIVE = 1
 
# exported symbols
__all__ = [ 'enabletrace', 'NTracer', 'NTraceFromCommandLine', 'TO_FUNCTION_ARGUMENTS', 'TO_FUNCTION_RVALUES', 'TO_SET_VARS', 'TO_GET_VARS', 'TO_TRACE_SRC_LINES', 'TO_SHOW_PREFIX_TID', 'TO_SHOW_PREFIX_NESTING' ]

# trace flags

''' argument flag: print function arguments'''
TO_FUNCTION_ARGUMENTS = 0x1
''' argument flag: print function return values'''
TO_FUNCTION_RVALUES = 0x2
''' argument flag: print modification of variables'''
TO_SET_VARS = 0x4
''' argument flag: print variable access'''
TO_GET_VARS = 0x8
''' argument flag: show souce lines before they are executed'''
TO_TRACE_SRC_LINES=0x10
''' show thread id in prefix string of lines traced '''
TO_SHOW_PREFIX_TID=0x20
''' show call strack nesting in prefix string of lines traced '''
TO_SHOW_PREFIX_NESTING = 0x40

# trace settings
_ISTRACEON = 0
_TRACEOPTS = 0
_FOLLOW_OBJECTS = 0

# these modules are not traced
#_BLACKLIST = set([ ])

# internal
_LocalDictType =  dict
_LocalTupleType = type( () )
_LocalListType = type( [] )
_ThreadLocal = threading.local()

def _transformToTraceObj( key, value ):
    global _ISTRACEON
    _ISTRACEON = 0	    
    if isinstance( value, dict) and not isinstance( value, DictTracer) :
	    #print "-> interceptDict: ", key
	    value = DictTracer( value , key ) 

    if (isinstance( value, list) or isinstance( value, tuple) ) and not isinstance( value, SequenceTracer):
	    #print "-> interceptSeq: ", key
	    value = SequenceTracer( value , key ) 
    _ISTRACEON = 1	   
    return value

def _transformToTraceObjSeq( key, index, value ):
    global _ISTRACEON
    _ISTRACEON = 0	    
    if isinstance( value, dict) and not isinstance( value, DictTracer) :
	    #print "-> interceptDict: ", key
	    key = key + '[' + str(index) + ']'
	    value = DictTracer( value , key ) 

    if (isinstance( value, list) or isinstance( value, tuple) ) and not isinstance( value, SequenceTracer):
	    #print "-> interceptSeq: ", key
	    key = key + '[' + str(index) + ']'
	    value = SequenceTracer( value , key ) 
    _ISTRACEON = 1	   
    return value

def _clearTraceName( value ):
    global _ISTRACEON
    _ISTRACEON = 0	    
    if isinstance( value, SequenceTracer) or isinstance( value, DictTracer):
	value.name = ''
    _ISTRACEON = 1	   

def _getrep( value ):
    global _ISTRACEON
    global _FOLLOW_OBJECTS
 
    if _FOLLOW_OBJECTS:
	_ISTRACEON = 0	    
	r = getattr(value, '__dict__', value)
	#try:
	#    r = vars( value ) 	
	#except TypeError:
	#    r = value
	_ISTRACEON = 1	   
	return r
    else:
	return value

def _getPrefix():
	global _TRACEOPTS
	global _ThreadLocal  

	hasPrev = 0
	if _TRACEOPTS & (TO_SHOW_PREFIX_TID | TO_SHOW_PREFIX_NESTING):
	    r = '('
	    if _TRACEOPTS & TO_SHOW_PREFIX_TID:
		r += 'tid=' + str( thread.get_ident() )  
		hasPrev = 1
	    if _TRACEOPTS & TO_SHOW_PREFIX_NESTING: 
		if hasPrev:
		    r += ' '
		r += 'level=' + str( _ThreadLocal.ntraceLevelOfNesting ) 
	    r += ') '	    
	    return r	    
	return ''

# derived from UserList
class SequenceTracer(collections.MutableSequence):
    def __init__(self, data, name):
        self.data = data
	self.name = name

    def __setitem__(self, i, item): 
	global _ISTRACEON
	global _TRACEOPTS 
	
	if _ISTRACEON and _TRACEOPTS & TO_SET_VARS:
	    print "==> set ", self.name, " [", i, "] = ", _getrep( item )
		    
	item = _transformToTraceObjSeq( self.name, i , item )
	self.data.__setitem__( i, item )
    
    def append(self, item):	
	item = _transformToTraceObjSeq( self.name, len(self.data) , item )
	self.data.append(item)

    def insert(self, i, item): 
	item = _transformToTraceObjSeq( self.name, len(i), item ) #now all the other items got their index invalidated ;-)
	self.data.insert(i, item)
	_updateIndexes(i+1, len(self.data)) # this is getting silly
 
    def __getitem__(self, i): 
	r = self.data[i]
	global _ISTRACEON
	global _TRACEOPTS 

	if _ISTRACEON and _TRACEOPTS & TO_GET_VARS:
	    print "==> get ", self.name, " [", i, "] = ", _getrep( r )
	return r

    def __add__(self, other):
       	global _ISTRACEON
        _ISTRACEON = 0	    
	if isinstance(other, SequenceTracer):
            r = self.__class__('',self.data + other.data)
        elif isinstance(other, type(self.data)):
            r = self.__class__('',self.data + other)
        else:
            r = self.__class__('',self.data + list(other))
        _ISTRACEON = 1
	return r  

    def __radd__(self, other):
       	global _ISTRACEON
        _ISTRACEON = 0	    
	if isinstance(other, SequenceTracer):
            r = self.__class__('',other.data + self.data)
        elif isinstance(other, type(self.data)):
            r = self.__class__('',other + self.data)
        else:
            r = self.__class__('',list(other) + self.data)
        _ISTRACEON = 1
	return r  

    def __iadd__(self, other):
       	global _ISTRACEON
        _ISTRACEON = 0	    
	if isinstance(other, Sequencetracer):
            self.data += other.data
        elif isinstance(other, type(self.data)):
            self.data += other
        else:
	    self.data += list(other)
        _ISTRACEON = 1
	return self.data.__iadd__( other )

    def __cast(self, other):
	global _ISTRACEON
        _ISTRACEON = 0	    
        if isinstance(other, SequenceTracer):
	    rt = other.data
        else: 
	    rt = other
        _ISTRACEON = 1	    
	return rt
	
    def __repr__(self): 
	return self.data.__repr__()

    def __lt__(self, other): 
	return self.data <  self.__cast(other)
    def __le__(self, other): 
	return self.data <= self.__cast(other)
    def __eq__(self, other): 
	return self.data == self.__cast(other)
    def __ne__(self, other): 
	return self.data != self.__cast(other)
    def __gt__(self, other): 
	return self.data >  self.__cast(other)
    def __ge__(self, other): 
	return self.data >= self.__cast(other)
    def __cmp__(self, other):
        return cmp(self.data, self.__cast(other))
    
    def __hash__(self):
	return self.data.__hash__()
    
    def __contains__(self, item): 
	return item in self.data

    def __len__(self): 
	return len(self.data)

    def __delitem__(self, i): 
	self.data.__delitem__[i]

    def __getslice__(self, i, j):
        i = max(i, 0); j = max(j, 0)
        r = self.__class__(self.data[i:j])
	global _ISTRACEON
        _ISTRACEON = 0	    
        if not isinstance(r, SequenceTracer):
	    r = SequenceTracer( name + '[' + str(i) + ':' + str(j) + ']', r )
        _ISTRACEON = 1	    
	
	return r

    def __setslice__(self, i, j, other):
        i = max(i, 0); j = max(j, 0)

       	global _ISTRACEON
	_ISTRACEON = 0	    
        if isinstance(other, SequenceTracer):
            self.data[i:j] = other.data
        elif isinstance(other, type(self.data)):
            self.data[i:j] = other
        else:
            self.data[i:j] = list(other)
        _ISTRACEON = 1	   

    def __delslice__(self, i, j):
        i = max(i, 0); j = max(j, 0)
        del self.data[i:j]

    def __mul__(self, n):
        return self.__class__(self.data*n)

    def __iter__(self):
	return self.data.__iter__()

    def __rmul_(self):
	return self.data.__rmul__()

    def __imul__(self, n):
        self.data *= n
        return self
   
    def pop(self, i=-1): 
	r = self.data.pop(i)
	_clearTraceName( r )
	return r
    
    def remove(self, item): 
	self.data.remove(item)

    def count(self, item): 
	return self.data.count(item)
    
    def index(self, item, *args): 
	return self.data.index(item, *args)

    def reverse(self): 
	self.data.reverse()
	_updateIndexes(0, len(self.data)) # this is getting silly

    def sort(self, *args, **kwds): 
	self.data.sort(*args, **kwds)
	_updateIndexes(0, len(self.data)) # this is getting silly

    def extend(self, other):
	for s in other:
	    self.append( s )

    def _updateIndexes(self,nfrom,nto):
	global _ISTRACEON
	_ISTRACEON = 0	    
	while nfrom < nto:
	    value = self.data[nfrom]
	    if isinstance( value, SequenceTracer) or isinstance( value, DictTracer):
		idx = value.name.index(' ')
		if idx != -1:
		    value.name = value[ 0 : idx ] + ' [' + str(idx) + ']'
	    nfrom += 1
        _ISTRACEON = 1	    

# derived from UserDict
class DictTracer:
    def __init__(self, data, name):
	self.name = name
        self.data = data

    def __getitem__(self, key):
	r = self.data[key]
	global _ISTRACEON
	global _TRACEOPTS
	if _ISTRACEON and _TRACEOPTS & TO_GET_VARS:
	    print "==> get ", self.name, "[", key, "] = ", _getrep( r )
	return r

    def __setitem__(self, key, item): 
	global _ISTRACEON
	global _TRACEOPTS
	
	if _ISTRACEON and _TRACEOPTS & TO_SET_VARS:
	    print "==> set ", self.name, "[", key, "] = ", _getrep( item )
	
	item = _transformToTraceObjSeq( self.name,  key , item )
	self.data[key] = item

    def __repr__(self): 
	return self.data.__repr__()
    
    def __cmp__(self, dict):
	global _ISTRACEON
	_ISTRACEON = 0	    
	rt = isinstance(dict,SequenceTracer)
        _ISTRACEON = 1	    
    
        if rt:
            return cmp(self.data, dict.data)
        else:
            return cmp(self.data, dict)

    def __hash__(self):
	return self.data.__hash__()

    def __len__(self): 
	return len(self.data)
    
    def __delitem__(self, key): 
	del self.data[key]

    def clear(self): 
	self.data.clear()

    def copy(self):
        return DictTracer(self.data.copy(), self.name)

    def keys(self): 
	return self.data.keys()

    def items(self): 
	return self.data.items()

    def iteritems(self): 
	return self.data.iteritems()

    def iterkeys(self): 
	return self.data.iterkeys()

    def itervalues(self): 
	return self.data.itervalues()

    def values(self): 
	return self.data.values()

    def has_key(self, key): 
	return key in self.data

    def update(self, dict=None, **kwargs):
	if dict is None:
            pass
	global _ISTRACEON	    
	_ISTRACEON = 0	    
	if isinstance(dict, DictTracer):
            self.data.update(dict.data)
        elif isinstance(dict, type({})) or not hasattr(dict, 'items'):
            self.data.update(dict)
        else:
            for k, v in dict.items():
                self[k] = v
        if len(kwargs):
            self.data.update(kwargs)
	_ISTRACEON = 1	   

    def get(self, key, failobj=None):
        return self.data.get( key, failobj )

    def setdefault(self, key, failobj=None):
	return self.data.setdefualt( key, failobj )

    def pop(self, key, *args):
        r = self.data.pop(key, *args)
	_clearTraceName( r )
	return r

    def popitem(self):
        r = self.data.popitem()
	_clearTraceName( r[1] )	
	return r

    def __contains__(self, key):
        return key in self.data

    def __iter__(self):
	return self.data.__iter__()


    @classmethod
    def fromkeys(cls, iterable, value=None):
        d = cls()
        for key in iterable:
            d[key] = value
        return d


# dictionary represents a namespace (local or global)
# intercepts get/set methods; get is a value lookup from namespace; set is a value modification


class TraceDict(dict):

	def __init__(self, dict=None, **kwargs):
	    self.data = {}
	    if dict is not None:
		self.update(dict)
	    if len(kwargs):
		self.update(kwargs)

	def __getitem__(self,key):
	    r = super(TraceDict,self).__getitem__(key)
	    global _ISTRACEON
	    global _TRACEOPTS
	    if _ISTRACEON and _TRACEOPTS & TO_GET_VARS:
		if key != 'self':
		    print "-> get: ", key, ' = ', _getrep( r ) #, ' type: ', type( r )
	    return r

	def __setitem__(self, key, value):
	    global _ISTRACEON
	    global _TRACEOPTS
	    if _ISTRACEON and _TRACEOPTS & TO_SET_VARS:
		print "-> set: ", key, " = ", _getrep( value )

	    value = _transformToTraceObj( key, value )
	    r = super(TraceDict,self).__setitem__(key, value)
	    return r


	def __delitem__(self, key ):
	    #print "-> del: ", key
	    return  super(TraceDict,self).__delitem__(self, key)

def enabletrace(on):
    ''' enable/disable tracing, can be called from the traced program '''
    global _ISTRACEON
    _ISTRACEON = on

class NTracer:
    ''' the tracer object; invoke this class to run a python file with tracing
    '''

    def __init__(self, traceOptions = TO_FUNCTION_ARGUMENTS | TO_FUNCTION_RVALUES | TO_SET_VARS | TO_GET_VARS | TO_TRACE_SRC_LINES , follow_objects = False):
	''' 
	    object constructor; options are passed as bit mask values via traceOptions parameter, available options:

		TO_FUNCTION_ARGUMENTS	    argument flag: print function arguments
		TO_FUNCTION_RVALUES	    argument flag: print function return values
	        TO_SET_VARS		    argument flag: print modification of variables
		TO_GET_VARS		    argument flag: print variable access
		TO_TRACE_SRC_LINES	    argument flag: show souce lines before they are executed
		TO_SHOW_PREFIX_TID	    show thread id in prefix string of lines traced 
		TO_SHOW_PREFIX_NESTING	    show call strack nesting in prefix string of lines traced 
 
	    follow_objects parameter is true if object references are to be printed		
	'''	    
	self.firstLineOfFunction = 0

	if _NTRACEACTIVE == 0 and traceOptions & (TO_SET_VARS | TO_GET_VARS):
	    print "Native library not available, so can't watch setting/getting of variables on function frames"
	    traceOptions &= (TO_FUNCTION_ARGUMENTS | TO_FUNCTION_RVALUES)  
	
	global _ThreadLocal  
	_ThreadLocal.ntraceLevelOfNesting = 0

	global _TRACEOPTS
	global _FOLLOW_OBJECTS
	_TRACEOPTS = traceOptions
	_FOLLOW_OBJECTS = follow_objects

	enabletrace(0)
    

    def trace(self, progname, tracingenabled = True):
	''' tracer invocation
	    progname - python file to be traced
	    tracingenabled - if tracing is initially on (True by default)
	'''
	global _TRACEOPTS
	try:
	    with open(progname) as fp:
                code = compile(fp.read(), progname, 'exec' )

	    if _TRACEOPTS & (TO_SET_VARS | TO_GET_VARS):
		td = TraceDict()
	    else:
		td = {}

	    td[ '__file__' ] = progname
	    td[ '__name__' ] = '__main__'
	    td[ '__package__' ] = None
	    td[ '__cached__' ] = None

	    _settrace( self._tracerCB )

	    try:
		enabletrace( tracingenabled )
		exec code in td, td
	    finally:
		enabletrace(0)
		_settrace( None )

	except BaseException, err:
	 	raise   
		#pd.print_exception_ex( )

 

    # called for each function; if we need to trace function then return local tracer
    def _tracerCB(self, frame, why, arg):

	global _ISTRACEON
	global _ThreadLocal 

	#print "\n??",why,frame.f_code.co_filename,':',frame.f_lineno, arg, frame.f_code.co_name

	if why == 'call':
	    
	    if _ISTRACEON == 0:
	        return None

	    filename = frame.f_code.co_filename
	    if filename == __file__:
	    	return None

	    if _ThreadLocal.ntraceLevelOfNesting != 0 and frame.f_code.co_name == '<module>':
	    	return None
	    
	    #print "->tracerCB(", filename, '=', frame.f_code.co_name , ") frame.line_no: ",frame.f_lineno, "frame.f_code.co_firstline ", frame.f_code.co_firstlineno
	    if frame.f_lineno == frame.f_code.co_firstlineno:
	    	self.firstLineOfFunction = 1
 
 
	    # if we want to print local variables, then need to set our tracing dictionary for the local namespace (modify current frame)
	    # frame.f_locals = TraceDict() # read only attribute
	    # frame.f_locals.__class__ = TraceDict # only for heap types
	    # setattr(frame.f_locals.__class__,'__getitem__', types.MethodType(frame.f_locals.__class__,TraceGet)) # can't set type of built-in/extension type dict
	    # native methods can do everything.
	    global _TRACEOPTS
	    if _TRACEOPTS & (TO_SET_VARS | TO_GET_VARS):  
		ntracenative.frameSetLocals( frame, TraceDict() )


            lineno = frame.f_lineno
            bname = os.path.basename(filename)
	    #if bname in _BLACKLIST:
	    # 	return None

	    return self._localtrace_traceCB

	return None

    def _localtrace_traceCB(self, frame, why, arg):
	global _ISTRACEON
	global _TRACEOPTS
	
	if _ISTRACEON == 0:
	    return None
        
	if why == "line":


	    # record the file name and line number of every trace
            filename = frame.f_code.co_filename
	    if filename == __file__:
	    	return None

            lineno = frame.f_lineno
            bname = os.path.basename(filename)
	    
	    if self.firstLineOfFunction == 1:
		_ThreadLocal.ntraceLevelOfNesting  += 1
		self.firstLineOfFunction = 0
		if _TRACEOPTS & TO_FUNCTION_ARGUMENTS:  
		    ret = self._format_frame( frame )
		    print "%s:%d %s" % (bname, frame.f_code.co_firstlineno, ret ),

	    if _TRACEOPTS & TO_TRACE_SRC_LINES:
		ret = linecache.getline(filename, lineno) 
		print _getPrefix() + "%s:%d %s" % (bname, lineno, ret ),
	elif why == 'return':
	    if _TRACEOPTS & TO_FUNCTION_RVALUES:  
		print  '-> def ', frame.f_code.co_name, ' returns ' ,  _getrep( arg ) # pprint.pformat( _getrep(arg) ) #_getrep( arg )
	    _ThreadLocal.ntraceLevelOfNesting  -= 1

        return self._localtrace_traceCB




    def _format_frame(self,frame):
	ret = ' def ' + frame.f_code.co_name + "("
	for i in range(0, frame.f_code.co_argcount):
	    if i >0:
		ret += ", "
	    argname = frame.f_code.co_varnames[ i ]  		    
	    try:
		ret += argname  + " = " + pprint.pformat( _getrep( frame.f_locals[ argname  ] ) )
	    except Exception as e:
		ret += '<error while formatting>' + str( e ) # str( frame.f_locals[ argname ]  )
	ret += ")\n"
	return ret
	
	#at " + frame.f_code.co_filename + ":" + str( frame.f_lineno ) #str( frame.f_code.co_firstlineno )
		

class NTraceFromCommandLine:
    ''' command line parser for ntrace
    '''

    def __init__(self, argv): 
	import getopt

	try:
	    opts, prog_argv = getopt.getopt( argv, "fFrRsSgGlLoOtTnN")

	except getopt.error, msg:
	    sys.stderr.write("%s: %s\n" % (argv, msg))
	    self._usage(sys.stderr)
	    sys.exit(1)

	optionsVal = TO_FUNCTION_ARGUMENTS | TO_FUNCTION_RVALUES | TO_TRACE_SRC_LINES
	followObjects = False

	for opt, val in opts:
	    if opt == '-f':
		optionsVal |= TO_FUNCTION_ARGUMENTS  
	    elif opt == '-F':
		optionsVal &= ~TO_FUNCTION_ARGUMENTS  
	    elif opt == '-r':
		optionsVal |= TO_FUNCTION_RVALUES 
	    elif opt == '-R':
		optionsVal &= ~TO_FUNCTION_RVALUES 
	    elif opt == '-s':	    
		optionsVal |= TO_SET_VARS
	    elif opt == '-S':	    
		optionsVal &= ~TO_SET_VARS
	    elif opt == '-g':
		optionsVal |= TO_GET_VARS
	    elif opt == '-G':
		optionsVal &= ~TO_GET_VARS
	    elif opt == '-l':
		optionsVal |= TO_TRACE_SRC_LINES 
	    elif opt == '-L':
		optionsVal &= ~TO_TRACE_SRC_LINES 
	    elif opt == '-o':
		followObjects = True
	    elif opt == '-O':
		followObjects = False
	    elif opt == '-t':
		optionsVal |= TO_SHOW_PREFIX_TID 
	    elif opt == '-T':
		optionsVal &= ~TO_SHOW_PREFIX_TID  
	    elif opt == '-n':
		optionsVal |= TO_SHOW_PREFIX_NESTING
	    elif opt == '-N':
		optionsVal &= ~TO_SHOW_PREFIX_NESTING 

	if len( prog_argv ) == 0:
	    self._usage()

	sys.argv = prog_argv

	tracer = NTracer( optionsVal, followObjects )
	tracer.trace( sys.argv[ 0 ] )


    def _usage(self):
	sys.stderr.write("""Usage: %s [OPTIONS] <file> [ARGS}

Traces execution of python program; 

By default it shows function parameters and function return values

Options:
    -f		    trace function parameter calls (default: on)
    -F		    do not trace function parameters

    -r		    trace function return values  (default: on)
    -R		    do not trace function return values

    -l		    trace source lines before they are executed (default: on)
    -L		    do not trace source lines before they are executed

    -s		    trace setting of variablesa (default: off)
    -S		    do not trace setting of variables

    -g		    trace getting of variables (default: off)
    -G		    do not trace getting of variables

    -o		    print out object content (default: off)
    -O		    do not print out object content

    -t		    show the current thread id in prefix for each source line (default on)
    -T		    hide the current thread id	

    -n		    show the nesting level, how many levels down the call stack (default on)
    -N		    hide the nesting level

By default the following options are on: -l -f -r

""" % sys.argv[0] )   
	sys.exit(1)


def main():
    NTraceFromCommandLine( sys.argv[1:] )

if __name__=='__main__':
    main()
