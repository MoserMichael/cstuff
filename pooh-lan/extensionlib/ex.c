
#include <pooh-lan/rtlib/rtlib.h>


static void x_addnumbers(XCALL_DATA *xcall)
{
  POOH_INT a,b;
  BINDING_DATA *arg;

  arg = XCALL_param( xcall, 0 ); 
  BINDING_DATA_get_int( arg, &a );

  arg = XCALL_param( xcall, 1 ); 
  BINDING_DATA_get_int( arg, &b );
  
  
  BINDING_DATA_set_int( XCALL_rvalue( xcall ), a + b );  
}

/*
 *  Each exported function -and its signature - must be defined in the interface definition.
 *  The definition of a function specifies the
 *	- name of the function
 *	- c function implementation; the function that is called when function is used from pooh.
 *	- return type of the function
 *	- parameter labels and types of parameters
 *
 *   Each function definition is defined by macro 
 *	DEFINE_XFUNC1 -	defines function with one parameter
 *	DEFINE_XFUNC2 -	defines function with two parameters
 *	DEFINE_XFUNC3 -	defines function with three parameters
 */
static AST_XFUNC_DECL xlib_iface[] = {

  DEFINE_XFUNC2( "addnumbers", x_addnumbers, S_VAR_INT,	"a", S_VAR_INT, "b", S_VAR_INT ),

 
/* this must be the last entry in interface definition */ 
  DEFINE_XFUNC1( 0, 0, 0, "", 0 )

};


/*
 * Pooh extension library must define exported function get_honey_jar_interface
 * this function returns the interface definition.
 */
AST_XFUNC_DECL * get_honey_jar_interface()
{
  return xlib_iface;
}
