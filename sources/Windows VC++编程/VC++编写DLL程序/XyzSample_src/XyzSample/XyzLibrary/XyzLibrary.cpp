// XyzLibrary.cpp : Defines the Xyz implementation.
//

#include "stdafx.h"
#include "XyzLibrary.h"


////////////////////////////////////////////////////////////////////////////////
// Implementation of the IXyz interface.

class XyzImpl : public IXyz
{
    int Foo(int n);
    void Release();
};

int XyzImpl::Foo(int n)
{
    return n * n;
}

void XyzImpl::Release()
{
    delete this;
}

////////////////////////////////////////////////////////////////////////////////
// Factory function that creates instances if the Xyz object.

// Export both decorated and undecorated names.
//   GetXyz     - Undecorated name, which can be easily used with GetProcAddress
//                Win32 API function.
//   _GetXyz@0  - Common name decoration for __stdcall functions in C language.
//
// For more information on name decoration see here:
// "Format of a C Decorated Name"
// http://msdn.microsoft.com/en-us/library/x7kb4e2f.aspx

#if !defined(_WIN64)
// This pragma is required only for 32-bit builds. In a 64-bit environment,
// C functions are not decorated.
#pragma comment(linker, "/export:GetXyz=_GetXyz@0")
#endif  // _WIN64

XYZAPI XYZHANDLE APIENTRY GetXyz()
{
	return new XyzImpl;
}

////////////////////////////////////////////////////////////////////////////////
// Regular C++ class implementatin.
// The class is exported.

int CXyz::Foo(int n)
{
    return n * n;
}

////////////////////////////////////////////////////////////////////////////////
// C interface implementation.
// Internally these functions are written in the C++ language and
// use XyzImpl instances.

XYZAPI INT APIENTRY XyzFoo(XYZHANDLE handle, INT n)
{
    INT nResult = -1;

    if(handle)
    {
        nResult = handle->Foo(n);
    }

    return nResult;
}

XYZAPI VOID APIENTRY XyzRelease(XYZHANDLE handle)
{
    if(handle)
    {
        handle->Release();
    }
}

////////////////////////////////////////////////////////////////////////////////