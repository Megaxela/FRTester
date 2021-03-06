/* Generated by Cython 0.25.1 */

#ifndef __PYX_HAVE__driver
#define __PYX_HAVE__driver


#ifndef __PYX_HAVE_API__driver

#ifndef __PYX_EXTERN_C
  #ifdef __cplusplus
    #define __PYX_EXTERN_C extern "C"
  #else
    #define __PYX_EXTERN_C extern
  #endif
#endif

#ifndef DL_IMPORT
  #define DL_IMPORT(_T) _T
#endif

__PYX_EXTERN_C DL_IMPORT(PyObject) *createDriver(FRDriver *);
__PYX_EXTERN_C DL_IMPORT(PyObject) *createPhysicalInterface(PhysicalInterface *);

#endif /* !__PYX_HAVE_API__driver */

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initdriver(void);
#else
PyMODINIT_FUNC PyInit_driver(void);
#endif

#endif /* !__PYX_HAVE__driver */
