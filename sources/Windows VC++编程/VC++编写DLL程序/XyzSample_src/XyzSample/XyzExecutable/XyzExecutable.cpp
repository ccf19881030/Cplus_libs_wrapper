// XyzExecutable.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "XyzLibrary.h"

#if _MSC_VER > 1200     // VC++7.0 and higher
#include "AutoClosePtr.h"
#elif _MSC_VER == 1200  // VC++6.0
#include "AutoClosePtrVC6.h"
#else                   // really old version
#error  Newer MS VC++ compiler required.
#endif // _MSC_VER

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
{
    // 1. COM-like usage.
    IXyz* pXyz = ::GetXyz();

    if(pXyz)
    {
        pXyz->Foo(42);
        pXyz->Release();
    }
    //

#if _MSC_VER >= 1600    // VC++10.0 and higher
    
    // 1a. Use smart pointers from the Standard C++ library.
    typedef std::shared_ptr<IXyz> IXyzPtr;

    IXyzPtr ptrXyz(::GetXyz(), std::mem_fn(&IXyz::Release));

    if(ptrXyz)
    {
        ptrXyz->Foo(42);
    }
    //

#elif _MSC_VER >= 1300 && _MSC_VER < 1600   // VC++7.0 - VC++9.0

    // 1b. COM-Like usage with smart pointer.
    // No need to call `IXyz::Release'; the instance will
    // be released automatically in destructor of the smart pointer.
    typedef AutoClosePtr<IXyz, void, &IXyz::Release> IXyzPtr;

    IXyzPtr ptrXyz(::GetXyz());

    if(ptrXyz)
    {
        ptrXyz->Foo(42);
    }
    //

#else   // VC++6.0

    // 1c. COM-Like usage with smart pointer.
    // No need to call `IXyz::Release'; the instance will
    // be released automatically in destructor of the smart pointer.
    typedef AutoClosePtr<IXyz> IXyzPtr;

    IXyzPtr ptrXyz(&IXyz::Release, ::GetXyz());

    if(ptrXyz)
    {
        ptrXyz->Foo(42);
    }
    //

#endif // _MSC_VER

    // 2. Regular C++ class.
    // The class must be exported.
    CXyz xyz;
    xyz.Foo(42);
    //

    // 3. Classic C language usage.
    XYZHANDLE hXyz = GetXyz();

    if(hXyz)
    {
        XyzFoo(hXyz, 42);
        XyzRelease(hXyz);
        hXyz = NULL;
    }
    //

	return 0;
}

