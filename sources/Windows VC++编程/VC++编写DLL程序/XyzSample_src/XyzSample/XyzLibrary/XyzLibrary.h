// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the XYZLIBRARY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// XYZAPI functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef XYZLIBRARY_EXPORTS
#define XYZAPI __declspec(dllexport)
#else
#define XYZAPI __declspec(dllimport)
#endif

////////////////////////////////////////////////////////////////////////////////
// 

#ifdef __cplusplus

// COM-Like abstract interface.
// This interface doesn't require __declspec(dllexport/dllimport) specifier.
// Method calls are dispatched via virtual table.
// Any C++ compiler can use it.
// Instances are obtained via factory function.
struct IXyz
{
    virtual int Foo(int n) = 0;
    virtual void Release() = 0;
};

// Regular C++ class.
// The __declspec specifier is required, otherwise clients won't be able to
// call methods of the class.
// Both DLL and EXE must use the same version of a C++ compiler.
// No need to use factory to make an instance of this class.
class XYZAPI CXyz
{
public:
    int Foo(int n);
};

// Handle type. In C++ language the iterface type is used.
typedef IXyz* XYZHANDLE;

#else   // C

// Handle type. In C language there are no classes, so empty struct is used
// in order to ensure type safety.
typedef struct tagXYZHANDLE {} * XYZHANDLE;

#endif // __cplusplus

////////////////////////////////////////////////////////////////////////////////
// 
#ifdef __cplusplus
#   define EXTERN_C     extern "C"
#else
#   define EXTERN_C
#endif // __cplusplus

// Factory function that creates instances of the Xyz object.
EXTERN_C
XYZAPI
XYZHANDLE
WINAPI
GetXyz(
    VOID);

////////////////////////////////////////////////////////////////////////////////
// Classic C interface.
// These functions can be used with any compiler that knows how to call
// an exported C routine.

EXTERN_C
XYZAPI
INT
APIENTRY
XyzFoo(
    XYZHANDLE handle,
    INT n);

EXTERN_C
XYZAPI
VOID
APIENTRY
XyzRelease(
    XYZHANDLE handle);

////////////////////////////////////////////////////////////////////////////////
