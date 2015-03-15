
import ntrace



def main():
    #t = ntrace.NTracer(ntrace.TO_FUNCTION_ARGUMENTS | ntrace.TO_FUNCTION_RVALUES | ntrace.TO_SET_VARS | ntrace.TO_GET_VARS | ntrace.TO_TRACE_SRC_LINES )
    t = ntrace.NTracer(ntrace.TO_FUNCTION_ARGUMENTS | ntrace.TO_FUNCTION_RVALUES | ntrace.TO_TRACE_SRC_LINES )
    t.trace( 'test_ntrace2.py' , False)

main()


