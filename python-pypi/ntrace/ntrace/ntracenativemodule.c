#include <python2.7/Python.h>
#include <python2.7/frameobject.h>
#include <python2.7/dictobject.h>

static PyObject * ntracenative_frameSetLocals(PyObject *self, PyObject *args)
{
    PyObject *frameobj = NULL,*dict = NULL;
    PyFrameObject *frame;

    (void) self;

    
//printf("before parse\n");
    if (!PyArg_ParseTuple( args, "OO", &frameobj, &dict))
    {
	return NULL;
    }	

//printf("before framecheck frameobj=%p dict=%p\n", frameobj, dict);

    if ( frameobj == NULL || !PyFrame_Check(frameobj) ) // check if frame tame
    {
	return NULL;
    }

//printf("before dictcheck dict=%p\n", dict);
                             
    if ( !PyDict_Check(dict) )
    {
	return NULL;
    }	

//printf("allchecks ok\n");

    frame = (PyFrameObject *) frameobj;
    if (PyDict_CheckExact(frame->f_locals))    
    {
	PyObject *oldLocals;


//printf("set f_locals\n");

	PyDict_Merge(dict, frame->f_locals, 1); // 1 - add reference to keys and values
    	Py_INCREF(dict);


	oldLocals = frame->f_locals;

    	frame->f_locals = dict;

	if (oldLocals != NULL)
		Py_DECREF( oldLocals );
    }


#if 0
    if (PyDict_CheckExact(frame->f_globals))    
    {
	PyObject *oldLocals;

	PyDict_Merge(dict, frame->f_globals, 1); // 1 - add reference to keys and values
    	Py_INCREF(dict);


	oldLocals = frame->f_globals;

    	frame->f_globals = dict;

	if (oldLocals != NULL)
		Py_DECREF( oldLocals );
    }
#endif

    return Py_BuildValue("i", 0);
} 


static PyMethodDef methods_def[] = {
	{"frameSetLocals", ntracenative_frameSetLocals, METH_VARARGS, "set local object in a python frame"},
	{0,0,0,0}   
};

PyMODINIT_FUNC
initntracenative(void)
{
	Py_InitModule("ntracenative", methods_def);
}
