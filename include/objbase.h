/*
 * Copyright (C) 1998-1999 Fran�ois Gouget
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rpc.h"
#include "rpcndr.h"

#ifndef _OBJBASE_H_
#define _OBJBASE_H_

/*****************************************************************************
 * define ICOM_MSVTABLE_COMPAT
 * to implement the microsoft com vtable compatibility workaround for g++.
 *
 * NOTE: Turning this option on will produce a winelib that is incompatible
 * with the binary emulator.
 *
 * If the compiler supports the com_interface attribute, leave this off, and
 * define the ICOM_USE_COM_INTERFACE_ATTRIBUTE macro below. This may also
 * require the addition of the -vtable-thunks option for g++.
 *
 * If you aren't interested in Winelib C++ compatibility at all, leave both
 * options off.
 *
 * The preferable method for using ICOM_USE_COM_INTERFACE_ATTRIBUTE macro
 * would be to define it only for your Winelib application. This allows you
 * to have both binary and Winelib compatibility for C and C++ at the same
 * time :)
 */
/* #define ICOM_MSVTABLE_COMPAT 1 */
/* #define ICOM_USE_COM_INTERFACE_ATTRIBUTE 1 */

#include "objidl.h"

#ifndef RC_INVOKED
/* For compatibility only, at least for now */
#include <stdlib.h>
#endif

#ifndef INITGUID
#include "cguid.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NONAMELESSSTRUCT
#define LISet32(li, v)   ((li).HighPart = (v) < 0 ? -1 : 0, (li).LowPart = (v))
#define ULISet32(li, v)  ((li).HighPart = 0, (li).LowPart = (v))
#else
#define LISet32(li, v)   ((li).s.HighPart = (v) < 0 ? -1 : 0, (li).s.LowPart = (v))
#define ULISet32(li, v)  ((li).s.HighPart = 0, (li).s.LowPart = (v))
#endif

/*****************************************************************************
 *	Standard API
 */
DWORD WINAPI CoBuildVersion(void);

typedef enum tagCOINIT
{
    COINIT_APARTMENTTHREADED  = 0x2, /* Apartment model */
    COINIT_MULTITHREADED      = 0x0, /* OLE calls objects on any thread */
    COINIT_DISABLE_OLE1DDE    = 0x4, /* Don't use DDE for Ole1 support */
    COINIT_SPEED_OVER_MEMORY  = 0x8  /* Trade memory for speed */
} COINIT;

HRESULT WINAPI CoInitialize(LPVOID lpReserved);
HRESULT WINAPI CoInitializeEx(LPVOID lpReserved, DWORD dwCoInit);
void WINAPI CoUninitialize(void);
DWORD WINAPI CoGetCurrentProcess(void);

HINSTANCE WINAPI CoLoadLibrary(LPOLESTR lpszLibName, BOOL bAutoFree);
void WINAPI CoFreeAllLibraries(void);
void WINAPI CoFreeLibrary(HINSTANCE hLibrary);
void WINAPI CoFreeUnusedLibraries(void);

HRESULT WINAPI CoCreateInstance(REFCLSID rclsid, LPUNKNOWN pUnkOuter, DWORD dwClsContext, REFIID iid, LPVOID *ppv);
HRESULT WINAPI CoCreateInstanceEx(REFCLSID      rclsid,
				  LPUNKNOWN     pUnkOuter,
				  DWORD         dwClsContext,
				  COSERVERINFO* pServerInfo,
				  ULONG         cmq,
				  MULTI_QI*     pResults);

HRESULT WINAPI CoGetInstanceFromFile(COSERVERINFO* pServerInfo, CLSID* pClsid, IUnknown* punkOuter, DWORD dwClsCtx, DWORD grfMode, OLECHAR* pwszName, DWORD dwCount, MULTI_QI* pResults);
HRESULT WINAPI CoGetInstanceFromIStorage(COSERVERINFO* pServerInfo, CLSID* pClsid, IUnknown* punkOuter, DWORD dwClsCtx, IStorage* pstg, DWORD dwCount, MULTI_QI* pResults);

HRESULT WINAPI CoGetMalloc(DWORD dwMemContext, LPMALLOC* lpMalloc);
LPVOID WINAPI CoTaskMemAlloc(ULONG size);
void WINAPI CoTaskMemFree(LPVOID ptr);
LPVOID WINAPI CoTaskMemRealloc(LPVOID ptr, ULONG size);

HRESULT WINAPI CoRegisterMallocSpy(LPMALLOCSPY pMallocSpy);
HRESULT WINAPI CoRevokeMallocSpy(void);

/* class registration flags; passed to CoRegisterClassObject */
typedef enum tagREGCLS
{
    REGCLS_SINGLEUSE = 0,
    REGCLS_MULTIPLEUSE = 1,
    REGCLS_MULTI_SEPARATE = 2,
    REGCLS_SUSPENDED = 4
} REGCLS;

HRESULT WINAPI CoGetClassObject(REFCLSID rclsid, DWORD dwClsContext, COSERVERINFO *pServerInfo, REFIID iid, LPVOID *ppv);
HRESULT WINAPI CoRegisterClassObject(REFCLSID rclsid,LPUNKNOWN pUnk,DWORD dwClsContext,DWORD flags,LPDWORD lpdwRegister);
HRESULT WINAPI CoRevokeClassObject(DWORD dwRegister);
HRESULT WINAPI CoGetPSClsid(REFIID riid,CLSID *pclsid);
HRESULT WINAPI CoRegisterPSClsid(REFIID riid, REFCLSID rclsid);
HRESULT WINAPI CoSuspendClassObjects(void);
HRESULT WINAPI CoResumeClassObjects(void);
ULONG WINAPI CoAddRefServerProcess(void);
HRESULT WINAPI CoReleaseServerProcess(void);

/* marshalling */
HRESULT WINAPI CoCreateFreeThreadedMarshaler(LPUNKNOWN punkOuter, LPUNKNOWN* ppunkMarshal);
HRESULT WINAPI CoGetInterfaceAndReleaseStream(LPSTREAM pStm, REFIID iid, LPVOID* ppv);
HRESULT WINAPI CoGetMarshalSizeMax(ULONG* pulSize, REFIID riid, LPUNKNOWN pUnk, DWORD dwDestContext, LPVOID pvDestContext, DWORD mshlflags);
HRESULT WINAPI CoGetStandardMarshal(REFIID riid, LPUNKNOWN pUnk, DWORD dwDestContext, LPVOID pvDestContext, DWORD mshlflags, LPMARSHAL* ppMarshal);
HRESULT WINAPI CoMarshalHresult(LPSTREAM pstm, HRESULT hresult);
HRESULT WINAPI CoMarshalInterface(LPSTREAM pStm, REFIID riid, LPUNKNOWN pUnk, DWORD dwDestContext, LPVOID pvDestContext, DWORD mshlflags);
HRESULT WINAPI CoMarshalInterThreadInterfaceInStream(REFIID riid, LPUNKNOWN pUnk, LPSTREAM* ppStm);
HRESULT WINAPI CoReleaseMarshalData(LPSTREAM pStm);
HRESULT WINAPI CoUnmarshalHresult(LPSTREAM pstm, HRESULT* phresult);
HRESULT WINAPI CoUnmarshalInterface(LPSTREAM pStm, REFIID riid, LPVOID* ppv);
HRESULT WINAPI CoLockObjectExternal(LPUNKNOWN pUnk, BOOL fLock, BOOL fLastUnlockReleases);
BOOL WINAPI CoIsHandlerConnected(LPUNKNOWN pUnk);

/* security */
HRESULT WINAPI CoInitializeSecurity(PSECURITY_DESCRIPTOR pSecDesc, LONG cAuthSvc, SOLE_AUTHENTICATION_SERVICE* asAuthSvc, void* pReserved1, DWORD dwAuthnLevel, DWORD dwImpLevel, void* pReserved2, DWORD dwCapabilities, void* pReserved3);
HRESULT WINAPI CoGetCallContext(REFIID riid, void** ppInterface);
HRESULT WINAPI CoQueryAuthenticationServices(DWORD* pcAuthSvc, SOLE_AUTHENTICATION_SERVICE** asAuthSvc);

HRESULT WINAPI CoQueryProxyBlanket(IUnknown* pProxy, DWORD* pwAuthnSvc, DWORD* pAuthzSvc, OLECHAR** pServerPrincName, DWORD* pAuthnLevel, DWORD* pImpLevel, RPC_AUTH_IDENTITY_HANDLE* pAuthInfo, DWORD* pCapabilites);
HRESULT WINAPI CoSetProxyBlanket(IUnknown* pProxy, DWORD dwAuthnSvc, DWORD dwAuthzSvc, OLECHAR* pServerPrincName, DWORD dwAuthnLevel, DWORD dwImpLevel, RPC_AUTH_IDENTITY_HANDLE pAuthInfo, DWORD dwCapabilities);
HRESULT WINAPI CoCopyProxy(IUnknown* pProxy, IUnknown** ppCopy);

HRESULT WINAPI CoImpersonateClient(void);
HRESULT WINAPI CoQueryClientBlanket(DWORD* pAuthnSvc, DWORD* pAuthzSvc, OLECHAR16** pServerPrincName, DWORD* pAuthnLevel, DWORD* pImpLevel, RPC_AUTHZ_HANDLE* pPrivs, DWORD* pCapabilities);
HRESULT WINAPI CoRevertToSelf(void);

/* misc */
HRESULT WINAPI CoGetTreatAsClass(REFCLSID clsidOld, LPCLSID pClsidNew);
HRESULT WINAPI CoTreatAsClass(REFCLSID clsidOld, REFCLSID clsidNew);

HRESULT WINAPI CoCreateGuid(GUID* pguid);
BOOL WINAPI CoIsOle1Class(REFCLSID rclsid);

BOOL WINAPI CoDosDateTimeToFileTime(WORD nDosDate, WORD nDosTime, FILETIME* lpFileTime);
BOOL WINAPI CoFileTimeToDosDateTime(FILETIME* lpFileTime, WORD* lpDosDate, WORD* lpDosTime);
HRESULT WINAPI CoFileTimeNow(FILETIME* lpFileTime);

/*****************************************************************************
 *	GUID API
 */
HRESULT WINAPI StringFromCLSID16(REFCLSID id, LPOLESTR16*);
HRESULT WINAPI StringFromCLSID(REFCLSID id, LPOLESTR*);

HRESULT WINAPI CLSIDFromString16(LPCOLESTR16, CLSID *);
HRESULT WINAPI CLSIDFromString(LPCOLESTR, CLSID *);

HRESULT WINAPI CLSIDFromProgID16(LPCOLESTR16 progid, LPCLSID riid);
HRESULT WINAPI CLSIDFromProgID(LPCOLESTR progid, LPCLSID riid);

HRESULT WINAPI ProgIDFromCLSID(REFCLSID clsid, LPOLESTR *lplpszProgID);

INT WINAPI StringFromGUID2(REFGUID id, LPOLESTR str, INT cmax);

/*****************************************************************************
 *	COM Server dll - exports
 */
HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID * ppv);
HRESULT WINAPI DllCanUnloadNow(void);

/*****************************************************************************
 *	Data Object
 */
HRESULT WINAPI CreateDataAdviseHolder(LPDATAADVISEHOLDER* ppDAHolder);
HRESULT WINAPI CreateDataCache(LPUNKNOWN pUnkOuter, REFCLSID rclsid, REFIID iid, LPVOID* ppv);

/*****************************************************************************
 *	Moniker API
 */
HRESULT WINAPI GetClassFile(LPCOLESTR filePathName,CLSID *pclsid);

HRESULT WINAPI CreateBindCtx16(DWORD reserved, LPBC* ppbc);
HRESULT WINAPI CreateBindCtx(DWORD reserved, LPBC* ppbc);

HRESULT WINAPI CreateFileMoniker16(LPCOLESTR16 lpszPathName, LPMONIKER* ppmk);
HRESULT WINAPI CreateFileMoniker(LPCOLESTR lpszPathName, LPMONIKER* ppmk);

HRESULT WINAPI CreateItemMoniker16(LPCOLESTR16 lpszDelim, LPCOLESTR  lpszItem, LPMONIKER* ppmk);
HRESULT WINAPI CreateItemMoniker(LPCOLESTR lpszDelim, LPCOLESTR  lpszItem, LPMONIKER* ppmk);

HRESULT WINAPI CreateAntiMoniker(LPMONIKER * ppmk);

HRESULT WINAPI CreateGenericComposite(LPMONIKER pmkFirst, LPMONIKER pmkRest, LPMONIKER* ppmkComposite);

HRESULT WINAPI BindMoniker(LPMONIKER pmk, DWORD grfOpt, REFIID iidResult, LPVOID* ppvResult);

HRESULT WINAPI CreateClassMoniker(REFCLSID rclsid, LPMONIKER* ppmk);

HRESULT WINAPI CreatePointerMoniker(LPUNKNOWN punk, LPMONIKER* ppmk);

HRESULT WINAPI MonikerCommonPrefixWith(IMoniker* pmkThis,IMoniker* pmkOther,IMoniker** ppmkCommon);

HRESULT WINAPI GetRunningObjectTable(DWORD reserved, LPRUNNINGOBJECTTABLE *pprot);
HRESULT WINAPI GetRunningObjectTable16(DWORD reserved, LPRUNNINGOBJECTTABLE *pprot);

/*****************************************************************************
 *	Storage API
 */
#define STGM_DIRECT		0x00000000
#define STGM_TRANSACTED		0x00010000
#define STGM_SIMPLE		0x08000000
#define STGM_READ		0x00000000
#define STGM_WRITE		0x00000001
#define STGM_READWRITE		0x00000002
#define STGM_SHARE_DENY_NONE	0x00000040
#define STGM_SHARE_DENY_READ	0x00000030
#define STGM_SHARE_DENY_WRITE	0x00000020
#define STGM_SHARE_EXCLUSIVE	0x00000010
#define STGM_PRIORITY		0x00040000
#define STGM_DELETEONRELEASE	0x04000000
#define STGM_CREATE		0x00001000
#define STGM_CONVERT		0x00020000
#define STGM_FAILIFTHERE	0x00000000
#define STGM_NOSCRATCH		0x00100000
#define STGM_NOSNAPSHOT		0x00200000

HRESULT WINAPI StgCreateDocFile16(LPCOLESTR16 pwcsName,DWORD grfMode,DWORD reserved,IStorage16 **ppstgOpen);
HRESULT WINAPI StgCreateDocfile(LPCOLESTR pwcsName,DWORD grfMode,DWORD reserved,IStorage **ppstgOpen);

HRESULT WINAPI StgIsStorageFile16(LPCOLESTR16 fn);
HRESULT WINAPI StgIsStorageFile(LPCOLESTR fn);
HRESULT WINAPI StgIsStorageILockBytes(ILockBytes *plkbyt);

HRESULT WINAPI StgOpenStorage16(const OLECHAR16* pwcsName,IStorage16* pstgPriority,DWORD grfMode,SNB16 snbExclude,DWORD reserved,IStorage16**ppstgOpen);
HRESULT WINAPI StgOpenStorage(const OLECHAR* pwcsName,IStorage* pstgPriority,DWORD grfMode,SNB snbExclude,DWORD reserved,IStorage**ppstgOpen);

HRESULT WINAPI WriteClassStg(IStorage* pStg, REFCLSID rclsid);
HRESULT WINAPI ReadClassStg(IStorage *pstg,CLSID *pclsid);

HRESULT WINAPI StgCreateDocfileOnILockBytes(ILockBytes *plkbyt,DWORD grfMode, DWORD reserved, IStorage** ppstgOpen);
HRESULT WINAPI StgOpenStorageOnILockBytes(ILockBytes *plkbyt, IStorage *pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage **ppstgOpen);

#ifdef __cplusplus
}
#endif

/*****************************************************************************
 * Macros to define a COM interface
 */
/*
 * The goal of the following set of definitions is to provide a way to use the same
 * header file definitions to provide both a C interface and a C++ object oriented
 * interface to COM interfaces. The type of interface is selected automatically
 * depending on the language but it is always possible to get the C interface in C++
 * by defining CINTERFACE.
 *
 * It is based on the following assumptions:
 *  - all COM interfaces derive from IUnknown, this should not be a problem.
 *  - the header file only defines the interface, the actual fields are defined
 *    separately in the C file implementing the interface.
 *
 * The natural approach to this problem would be to make sure we get a C++ class and
 * virtual methods in C++ and a structure with a table of pointer to functions in C.
 * Unfortunately the layout of the virtual table is compiler specific, the layout of
 * g++ virtual tables is not the same as that of an egcs virtual table which is not the
 * same as that generated by Visual C+. There are workarounds to make the virtual tables
 * compatible via padding but unfortunately the one which is imposed to the WINE emulator
 * by the Windows binaries, i.e. the Visual C++ one, is the most compact of all.
 *
 * So the solution I finally adopted does not use virtual tables. Instead I use inline
 * non virtual methods that dereference the method pointer themselves and perform the call.
 *
 * Let's take Direct3D as an example:
 *
 *    #define ICOM_INTERFACE IDirect3D
 *    #define IDirect3D_METHODS \
 *        ICOM_METHOD1(HRESULT,Initialize,    REFIID,) \
 *        ICOM_METHOD2(HRESULT,EnumDevices,   LPD3DENUMDEVICESCALLBACK,, LPVOID,) \
 *        ICOM_METHOD2(HRESULT,CreateLight,   LPDIRECT3DLIGHT*,, IUnknown*,) \
 *        ICOM_METHOD2(HRESULT,CreateMaterial,LPDIRECT3DMATERIAL*,, IUnknown*,) \
 *        ICOM_METHOD2(HRESULT,CreateViewport,LPDIRECT3DVIEWPORT*,, IUnknown*,) \
 *        ICOM_METHOD2(HRESULT,FindDevice,    LPD3DFINDDEVICESEARCH,, LPD3DFINDDEVICERESULT,)
 *    #define IDirect3D_IMETHODS \
 *        IUnknown_IMETHODS \
 *        IDirect3D_METHODS
 *    ICOM_DEFINE(IDirect3D,IUnknown)
 *    #undef ICOM_INTERFACE
 *
 *    #ifdef ICOM_CINTERFACE
 *    // *** IUnknown methods *** //
 *    #define IDirect3D_QueryInterface(p,a,b) ICOM_CALL2(QueryInterface,p,a,b)
 *    #define IDirect3D_AddRef(p)             ICOM_CALL (AddRef,p)
 *    #define IDirect3D_Release(p)            ICOM_CALL (Release,p)
 *    // *** IDirect3D methods *** //
 *    #define IDirect3D_Initialize(p,a)       ICOM_CALL1(Initialize,p,a)
 *    #define IDirect3D_EnumDevices(p,a,b)    ICOM_CALL2(EnumDevice,p,a,b)
 *    #define IDirect3D_CreateLight(p,a,b)    ICOM_CALL2(CreateLight,p,a,b)
 *    #define IDirect3D_CreateMaterial(p,a,b) ICOM_CALL2(CreateMaterial,p,a,b)
 *    #define IDirect3D_CreateViewport(p,a,b) ICOM_CALL2(CreateViewport,p,a,b)
 *    #define IDirect3D_FindDevice(p,a,b)     ICOM_CALL2(FindDevice,p,a,b)
 *    #endif
 *
 * Comments:
 *  - The ICOM_INTERFACE macro is used in the ICOM_METHOD macros to define the type of the 'this'
 *    pointer. Defining this macro here saves us the trouble of having to repeat the interface
 *    name everywhere. Note however that because of the way macros work, a macro like ICOM_METHOD1
 *    cannot use 'ICOM_INTERFACE##_VTABLE' because this would give 'ICOM_INTERFACE_VTABLE' and not
 *    'IDirect3D_VTABLE'.
 *  - ICOM_METHODS defines the methods specific to this interface. It is then aggregated with the
 *    inherited methods to form ICOM_IMETHODS.
 *  - ICOM_IMETHODS defines the list of methods that are inheritable from this interface. It must
 *    be written manually (rather than using a macro to generate the equivalent code) to avoid
 *    macro recursion (which compilers don't like).
 *  - The ICOM_DEFINE finally declares all the structures necessary for the interface. We have to
 *    explicitly use the interface name for macro expansion reasons again.
 *    Inherited methods are inherited in C by using the IDirect3D_METHODS macro and the parent's
 *    Xxx_IMETHODS macro. In C++ we need only use the IDirect3D_METHODS since method inheritance
 *    is taken care of by the language.
 *  - In C++ the ICOM_METHOD macros generate a function prototype and a call to a function pointer
 *    method. This means using once 't1 p1, t2 p2, ...' and once 'p1, p2' without the types. The
 *    only way I found to handle this is to have one ICOM_METHOD macro per number of parameters and
 *    to have it take only the type information (with const if necessary) as parameters.
 *    The 'undef ICOM_INTERFACE' is here to remind you that using ICOM_INTERFACE in the following
 *    macros will not work. This time it's because the ICOM_CALL macro expansion is done only once
 *    the 'IDirect3D_Xxx' macro is expanded. And by that time ICOM_INTERFACE will be long gone
 *    anyway.
 *  - You may have noticed the double commas after each parameter type. This allows you to put the
 *    name of that parameter which I think is good for documentation. It is not required and since
 *    I did not know what to put there for this example (I could only find doc about IDirect3D2),
 *    I left them blank.
 *  - Finally the set of 'IDirect3D_Xxx' macros is a standard set of macros defined to ease access
 *    to the interface methods in C. Unfortunately I don't see any way to avoid having to duplicate
 *    the inherited method definitions there. This time I could have used a trick to use only one
 *    macro whatever the number of parameters but I prefered to have it work the same way as above.
 *  - You probably have noticed that we don't define the fields we need to actually implement this
 *    interface: reference count, pointer to other resources and miscellaneous fields. That's
 *    because these interfaces are just that: interfaces. They may be implemented more than once, in
 *    different contexts and sometimes not even in Wine. Thus it would not make sense to impose
 *    that the interface contains some specific fields.
 *
 *
 * In C this gives:
 *    typedef struct IDirect3DVtbl IDirect3DVtbl;
 *    struct IDirect3D {
 *        IDirect3DVtbl* lpVtbl;
 *    };
 *    struct IDirect3DVtbl {
 *        HRESULT (*QueryInterface)(IDirect3D* me, REFIID riid, LPVOID* ppvObj);
 *        ULONG (*QueryInterface)(IDirect3D* me);
 *        ULONG (*QueryInterface)(IDirect3D* me);
 *        HRESULT (*Initialize)(IDirect3D* me, REFIID a);
 *        HRESULT (*EnumDevices)(IDirect3D* me, LPD3DENUMDEVICESCALLBACK a, LPVOID b);
 *        HRESULT (*CreateLight)(IDirect3D* me, LPDIRECT3DLIGHT* a, IUnknown* b);
 *        HRESULT (*CreateMaterial)(IDirect3D* me, LPDIRECT3DMATERIAL* a, IUnknown* b);
 *        HRESULT (*CreateViewport)(IDirect3D* me, LPDIRECT3DVIEWPORT* a, IUnknown* b);
 *        HRESULT (*FindDevice)(IDirect3D* me, LPD3DFINDDEVICESEARCH a, LPD3DFINDDEVICERESULT b);
 *    };
 *
 *    #ifdef ICOM_CINTERFACE
 *    // *** IUnknown methods *** //
 *    #define IDirect3D_QueryInterface(p,a,b) (p)->lpVtbl->QueryInterface(p,a,b)
 *    #define IDirect3D_AddRef(p)             (p)->lpVtbl->AddRef(p)
 *    #define IDirect3D_Release(p)            (p)->lpVtbl->Release(p)
 *    // *** IDirect3D methods *** //
 *    #define IDirect3D_Initialize(p,a)       (p)->lpVtbl->Initialize(p,a)
 *    #define IDirect3D_EnumDevices(p,a,b)    (p)->lpVtbl->EnumDevice(p,a,b)
 *    #define IDirect3D_CreateLight(p,a,b)    (p)->lpVtbl->CreateLight(p,a,b)
 *    #define IDirect3D_CreateMaterial(p,a,b) (p)->lpVtbl->CreateMaterial(p,a,b)
 *    #define IDirect3D_CreateViewport(p,a,b) (p)->lpVtbl->CreateViewport(p,a,b)
 *    #define IDirect3D_FindDevice(p,a,b)     (p)->lpVtbl->FindDevice(p,a,b)
 *    #endif
 *
 * Comments:
 *  - IDirect3D only contains a pointer to the IDirect3D virtual/jump table. This is the only thing
 *    the user needs to know to use the interface. Of course the structure we will define to
 *    implement this interface will have more fields but the first one will match this pointer.
 *  - The code generated by ICOM_DEFINE defines both the structure representing the interface and
 *    the structure for the jump table. ICOM_DEFINE uses the parent's Xxx_IMETHODS macro to
 *    automatically repeat the prototypes of all the inherited methods and then uses IDirect3D_METHODS
 *    to define the IDirect3D methods.
 *  - Each method is declared as a pointer to function field in the jump table. The implementation
 *    will fill this jump table with appropriate values, probably using a static variable, and
 *    initialize the lpVtbl field to point to this variable.
 *  - The IDirect3D_Xxx macros then just derefence the lpVtbl pointer and use the function pointer
 *    corresponding to the macro name. This emulates the behavior of a virtual table and should be
 *    just as fast.
 *  - This C code should be quite compatible with the Windows headers both for code that uses COM
 *    interfaces and for code implementing a COM interface.
 *
 *
 * And in C++ (with gcc's g++):
 *
 *    typedef struct IDirect3D: public IUnknown {
 *        private: HRESULT (*Initialize)(IDirect3D* me, REFIID a);
 *        public: inline HRESULT Initialize(REFIID a) { return ((IDirect3D*)t.lpVtbl)->Initialize(this,a); };
 *        private: HRESULT (*EnumDevices)(IDirect3D* me, LPD3DENUMDEVICESCALLBACK a, LPVOID b);
 *        public: inline HRESULT EnumDevices(LPD3DENUMDEVICESCALLBACK a, LPVOID b)
 *            { return ((IDirect3D*)t.lpVtbl)->EnumDevices(this,a,b); };
 *        private: HRESULT (*freateLight)(IDirect3D* me, LPDIRECT3DLIGHT* a, IUnknown* b);
 *        public: inline HRESULT CreateLight(LPDIRECT3DLIGHT* a, IUnknown* b)
 *            { return ((IDirect3D*)t.lpVtbl)->CreateLight(this,a,b); };
 *        private: HRESULT (*CreateMaterial)(IDirect3D* me, LPDIRECT3DMATERIAL* a, IUnknown* b);
 *        public: inline HRESULT CreateMaterial(LPDIRECT3DMATERIAL* a, IUnknown* b)
 *            { return ((IDirect3D*)t.lpVtbl)->CreateMaterial(this,a,b); };
 *        private: HRESULT (*CreateViewport)(IDirect3D* me, LPDIRECT3DVIEWPORT* a, IUnknown* b);
 *        public: inline HRESULT CreateViewport(LPDIRECT3DVIEWPORT* a, IUnknown* b)
 *            { return ((IDirect3D*)t.lpVtbl)->CreateViewport(this,a,b); };
 *        private:  HRESULT (*FindDevice)(IDirect3D* me, LPD3DFINDDEVICESEARCH a, LPD3DFINDDEVICERESULT b);
 *        public: inline HRESULT FindDevice(LPD3DFINDDEVICESEARCH a, LPD3DFINDDEVICERESULT b)
 *            { return ((IDirect3D*)t.lpVtbl)->FindDevice(this,a,b); };
 *    };
 *
 * Comments:
 *  - In C++ IDirect3D does double duty as both the virtual/jump table and as the interface
 *    definition. The reason for this is to avoid having to duplicate the mehod definitions: once
 *    to have the function pointers in the jump table and once to have the methods in the interface
 *    class. Here one macro can generate both. This means though that the first pointer, t.lpVtbl
 *    defined in IUnknown,  must be interpreted as the jump table pointer if we interpret the
 *    structure as the the interface class, and as the function pointer to the QueryInterface
 *    method, t.QueryInterface, if we interpret the structure as the jump table. Fortunately this
 *    gymnastic is entirely taken care of in the header of IUnknown.
 *  - Of course in C++ we use inheritance so that we don't have to duplicate the method definitions.
 *  - Since IDirect3D does double duty, each ICOM_METHOD macro defines both a function pointer and
 *    a non-vritual inline method which dereferences it and calls it. This way this method behaves
 *    just like a virtual method but does not create a true C++ virtual table which would break the
 *    structure layout. If you look at the implementation of these methods you'll notice that they
 *    would not work for void functions. We have to return something and fortunately this seems to
 *    be what all the COM methods do (otherwise we would need another set of macros).
 *  - Note how the ICOM_METHOD generates both function prototypes mixing types and formal parameter
 *    names and the method invocation using only the formal parameter name. This is the reason why
 *    we need different macros to handle different numbers of parameters.
 *  - Finally there is no IDirect3D_Xxx macro. These are not needed in C++ unless the CINTERFACE
 *    macro is defined in which case we would not be here.
 *  - This C++ code works well for code that just uses COM interfaces. But it will not work with
 *    C++ code implement a COM interface. That's because such code assumes the interface methods
 *    are declared as virtual C++ methods which is not the case here.
 *
 *
 * Implementing a COM interface.
 *
 * This continues the above example. This example assumes that the implementation is in C.
 *
 *    typedef struct _IDirect3D {
 *        void* lpVtbl;
 *        // ...
 *
 *    } _IDirect3D;
 *
 *    static ICOM_VTABLE(IDirect3D) d3dvt;
 *
 *    // implement the IDirect3D methods here
 *
 *    int IDirect3D_QueryInterface(IDirect3D* me)
 *    {
 *        ICOM_THIS(IDirect3D,me);
 *        // ...
 *    }
 *
 *    // ...
 *
 *    static ICOM_VTABLE(IDirect3D) d3dvt = {
 *        ICOM_MSVTABLE_COMPAT_DummyRTTIVALUE
 *        IDirect3D_QueryInterface,
 *        IDirect3D_Add,
 *        IDirect3D_Add2,
 *        IDirect3D_Initialize,
 *        IDirect3D_SetWidth
 *    };
 *
 * Comments:
 *  - We first define what the interface really contains. This is th e_IDirect3D structure. The
 *    first field must of course be the virtual table pointer. Everything else is free.
 *  - Then we predeclare our static virtual table variable, we will need its address in some
 *    methods to initialize the virtual table pointer of the returned interface objects.
 *  - Then we implement the interface methods. To match what has been declared in the header file
 *    they must take a pointer to a IDirect3D structure and we must cast it to an _IDirect3D so that
 *    we can manipulate the fields. This is performed by the ICOM_THIS macro.
 *  - Finally we initialize the virtual table.
 */



#if !defined(__cplusplus) || defined(CINTERFACE)
#define ICOM_CINTERFACE 1
#endif

#ifndef ICOM_CINTERFACE
/* C++ interface */

#define ICOM_METHOD(ret,xfn) \
     public: virtual ret CALLBACK (xfn)(void) = 0;
#define ICOM_METHOD1(ret,xfn,ta,na) \
     public: virtual ret CALLBACK (xfn)(ta a) = 0;
#define ICOM_METHOD2(ret,xfn,ta,na,tb,nb) \
     public: virtual ret CALLBACK (xfn)(ta a,tb b) = 0;
#define ICOM_METHOD3(ret,xfn,ta,na,tb,nb,tc,nc) \
     public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c) = 0;
#define ICOM_METHOD4(ret,xfn,ta,na,tb,nb,tc,nc,td,nd) \
     public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d) = 0;
#define ICOM_METHOD5(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne) \
     public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e) = 0;
#define ICOM_METHOD6(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf) \
     public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f) = 0;
#define ICOM_METHOD7(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng) \
     public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g) = 0;
#define ICOM_METHOD8(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh) \
     public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h) = 0;
#define ICOM_METHOD9(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni) \
     public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i) = 0;
#define ICOM_METHOD10(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj) \
     public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j) = 0;
#define ICOM_METHOD11(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk) \
     public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k) = 0;
#define ICOM_METHOD12(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l) = 0;
#define ICOM_METHOD13(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m) = 0;
#define ICOM_METHOD14(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n) = 0;
#define ICOM_METHOD15(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o) = 0;
#define ICOM_METHOD16(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p) = 0;
#define ICOM_METHOD17(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q) = 0;
#define ICOM_METHOD18(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r) = 0;
#define ICOM_METHOD19(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s) = 0;
#define ICOM_METHOD20(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t) = 0;
#define ICOM_METHOD21(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u) = 0;
#define ICOM_METHOD22(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v) = 0;
#define ICOM_METHOD23(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w) = 0;
#define ICOM_METHOD24(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw,tx,nx) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w,tx x) = 0;
#define ICOM_METHOD25(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw,tx,nx,ty,ny) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w,tx x,ty y) = 0;
#define ICOM_METHOD26(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw,tx,nx,ty,ny,tz,nz) \
    public: virtual ret CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w,tx x,ty y,tz z) = 0;


#define ICOM_VMETHOD(xfn) \
     public: virtual void CALLBACK (xfn)(void) = 0;
#define ICOM_VMETHOD1(xfn,ta,na) \
     public: virtual void CALLBACK (xfn)(ta a) = 0;
#define ICOM_VMETHOD2(xfn,ta,na,tb,nb) \
     public: virtual void CALLBACK (xfn)(ta a,tb b) = 0;
#define ICOM_VMETHOD3(xfn,ta,na,tb,nb,tc,nc) \
     public: virtual void CALLBACK (xfn)(ta a,tb b,tc c) = 0;
#define ICOM_VMETHOD4(xfn,ta,na,tb,nb,tc,nc,td,nd) \
     public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d) = 0;
#define ICOM_VMETHOD5(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne) \
     public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e) = 0;
#define ICOM_VMETHOD6(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf) \
     public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f) = 0;
#define ICOM_VMETHOD7(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng) \
     public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g) = 0;
#define ICOM_VMETHOD8(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh) \
     public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h) = 0;
#define ICOM_VMETHOD9(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni) \
     public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i) = 0;
#define ICOM_VMETHOD10(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj) \
     public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i, tj j) = 0;
#define ICOM_VMETHOD11(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk) \
     public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i, tj j, tk k) = 0;
#define ICOM_VMETHOD12(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l) = 0;
#define ICOM_VMETHOD13(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m) = 0;
#define ICOM_VMETHOD14(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n) = 0;
#define ICOM_VMETHOD15(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o) = 0;
#define ICOM_VMETHOD16(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p) = 0;
#define ICOM_VMETHOD17(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q) = 0;
#define ICOM_VMETHOD18(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r) = 0;
#define ICOM_VMETHOD19(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s) = 0;
#define ICOM_VMETHOD20(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t) = 0;
#define ICOM_VMETHOD21(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u) = 0;
#define ICOM_VMETHOD22(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v) = 0;
#define ICOM_VMETHOD23(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w) = 0;
#define ICOM_VMETHOD24(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw,tx,nx) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w,tx x) = 0;
#define ICOM_VMETHOD25(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw,tx,nx,ty,ny) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w,tx x,ty y) = 0;
#define ICOM_VMETHOD26(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw,tx,nx,ty,ny,tz,nz) \
    public: virtual void CALLBACK (xfn)(ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w,tx x,ty y,tz z) = 0;


#define ICOM_DEFINE(iface,ibase) \
    struct iface: public ibase { \
        iface##_METHODS \
    } ICOM_COM_INTERFACE_ATTRIBUTE;
#define ICOM_DEFINE1(iface) \
    struct iface { \
        iface##_METHODS \
    } ICOM_COM_INTERFACE_ATTRIBUTE;

#define ICOM_VTBL(iface)         (iface)

#else
/* C interface */

#define ICOM_METHOD(ret,xfn) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me);
#define ICOM_METHOD1(ret,xfn,ta,na) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a);
#define ICOM_METHOD2(ret,xfn,ta,na,tb,nb) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b);
#define ICOM_METHOD3(ret,xfn,ta,na,tb,nb,tc,nc) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c);
#define ICOM_METHOD4(ret,xfn,ta,na,tb,nb,tc,nc,td,nd) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d);
#define ICOM_METHOD5(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e);
#define ICOM_METHOD6(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f);
#define ICOM_METHOD7(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g);
#define ICOM_METHOD8(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h);
#define ICOM_METHOD9(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i);
#define ICOM_METHOD10(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j);
#define ICOM_METHOD11(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k);
#define ICOM_METHOD12(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l);
#define ICOM_METHOD13(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m);
#define ICOM_METHOD14(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n);
#define ICOM_METHOD15(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o);
#define ICOM_METHOD16(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p);
#define ICOM_METHOD17(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q);
#define ICOM_METHOD18(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r);
#define ICOM_METHOD19(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s);
#define ICOM_METHOD20(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t);
#define ICOM_METHOD21(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u);
#define ICOM_METHOD22(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v);
#define ICOM_METHOD23(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w);
#define ICOM_METHOD24(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw,tx,nx) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w,tx x);
#define ICOM_METHOD25(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw,tx,nx,ty,ny) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w,tx x,ty y);
#define ICOM_METHOD26(ret,xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw,tx,nx,ty,ny,tz,nz) \
    ret (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w,tx x,ty y,tz z);

#define ICOM_VMETHOD(xfn) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me);
#define ICOM_VMETHOD1(xfn,ta,na) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a);
#define ICOM_VMETHOD2(xfn,ta,na,tb,nb) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b);
#define ICOM_VMETHOD3(xfn,ta,na,tb,nb,tc,nc) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c);
#define ICOM_VMETHOD4(xfn,ta,na,tb,nb,tc,nc,td,nd) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d);
#define ICOM_VMETHOD5(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e);
#define ICOM_VMETHOD6(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f);
#define ICOM_VMETHOD7(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g);
#define ICOM_VMETHOD8(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,nh) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h);
#define ICOM_VMETHOD9(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ni) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i);
#define ICOM_VMETHOD10(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,nj) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j);
#define ICOM_VMETHOD11(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,nk) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k);
#define ICOM_VMETHOD12(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l);
#define ICOM_VMETHOD13(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m);
#define ICOM_VMETHOD14(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n);
#define ICOM_VMETHOD15(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o);
#define ICOM_VMETHOD16(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p);
#define ICOM_VMETHOD17(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q);
#define ICOM_VMETHOD18(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r);
#define ICOM_VMETHOD19(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s);
#define ICOM_VMETHOD20(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t);
#define ICOM_VMETHOD21(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u);
#define ICOM_VMETHOD22(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v);
#define ICOM_VMETHOD23(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w);
#define ICOM_VMETHOD24(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw,tx,nx) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w,tx x);
#define ICOM_VMETHOD25(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw,tx,nx,ty,ny) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w,tx x,ty y);
#define ICOM_VMETHOD26(xfn,ta,na,tb,nb,tc,nc,td,nd,te,ne,tf,nf,tg,ng,th,nh,ti,ni,tj,nj,tk,nk,tl,nl,tm,nm,tn,nn,to,no,tp,np,tq,nq,tr,nr,ts,ns,tt,nt,tu,nu,tv,nv,tw,nw,tx,nx,ty,ny,tz,nz) \
    void (CALLBACK *xfn)(ICOM_INTERFACE* me,ta a,tb b,tc c,td d,te e,tf f,tg g,th h,ti i,tj j,tk k,tl l,tm m,tn n,to o,tp p,tq q,tr r,ts s,tt t,tu u,tv v,tw w,tx x,ty y,tz z);


#define ICOM_VTABLE(iface)       iface##Vtbl
#define ICOM_VFIELD(iface)       ICOM_VTABLE(iface)* lpVtbl
#define ICOM_VTBL(iface)         (iface)->lpVtbl

#define ICOM_DEFINE(iface,ibase) \
    typedef struct ICOM_VTABLE(iface) ICOM_VTABLE(iface); \
    struct iface { \
        const ICOM_VFIELD(iface); \
    }; \
    struct ICOM_VTABLE(iface) { \
        ICOM_MSVTABLE_COMPAT_FIELDS \
        ibase##_IMETHODS \
        iface##_METHODS \
    };
#define ICOM_DEFINE1(iface) \
    typedef struct ICOM_VTABLE(iface) ICOM_VTABLE(iface); \
    struct iface { \
        const ICOM_VFIELD(iface); \
    }; \
    struct ICOM_VTABLE(iface) { \
        ICOM_MSVTABLE_COMPAT_FIELDS \
        iface##_METHODS \
    };

#define ICOM_THIS(impl,iface)          impl* const This=(impl*)(iface)
#define ICOM_CTHIS(impl,iface)         const impl* const This=(const impl*)(iface)

#define ICOM_THIS_MULTI(impl,field,iface)  impl* const This=(impl*)((char*)(iface) - offsetof(impl,field))
#define ICOM_CTHIS_MULTI(impl,field,iface) const impl* const This=(const impl*)((char*)(iface) - offsetof(impl,field))

#endif /*ICOM_CINTERFACE  */

#define ICOM_CALL(xfn, ptr)                         ICOM_VTBL(ptr)->xfn(ptr)
#define ICOM_CALL1(xfn, ptr,a)                      ICOM_VTBL(ptr)->xfn(ptr,a)
#define ICOM_CALL2(xfn, ptr,a,b)                    ICOM_VTBL(ptr)->xfn(ptr,a,b)
#define ICOM_CALL3(xfn, ptr,a,b,c)                  ICOM_VTBL(ptr)->xfn(ptr,a,b,c)
#define ICOM_CALL4(xfn, ptr,a,b,c,d)                ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d)
#define ICOM_CALL5(xfn, ptr,a,b,c,d,e)              ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e)
#define ICOM_CALL6(xfn, ptr,a,b,c,d,e,f)            ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f)
#define ICOM_CALL7(xfn, ptr,a,b,c,d,e,f,g)          ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g)
#define ICOM_CALL8(xfn, ptr,a,b,c,d,e,f,g,h)        ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h)
#define ICOM_CALL9(xfn, ptr,a,b,c,d,e,f,g,h,i)      ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i)
#define ICOM_CALL10(xfn, ptr,a,b,c,d,e,f,g,h,i,j)   ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j)
#define ICOM_CALL11(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k)
#define ICOM_CALL12(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l)
#define ICOM_CALL13(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m)
#define ICOM_CALL14(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n)
#define ICOM_CALL15(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)
#define ICOM_CALL16(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p)
#define ICOM_CALL17(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q)
#define ICOM_CALL18(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r)
#define ICOM_CALL19(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)
#define ICOM_CALL20(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t)
#define ICOM_CALL21(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u)
#define ICOM_CALL22(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v)
#define ICOM_CALL23(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w)
#define ICOM_CALL24(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x)
#define ICOM_CALL25(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y)
#define ICOM_CALL26(xfn, ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z) ICOM_VTBL(ptr)->xfn(ptr,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z)


#ifndef __WINE__
/* These macros are msdev's way of defining COM objects.
 * They are provided here for use by Winelib developpers.
 */
#define FARSTRUCT
#define HUGEP

#define WINOLEAPI        STDAPI
#define WINOLEAPI_(type) STDAPI_(type)

#if defined(__cplusplus) && !defined(CINTERFACE)
#define interface struct
#define STDMETHOD(method)       virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method) virtual type STDMETHODCALLTYPE method
#define PURE                    = 0
#define THIS_
#define THIS                    void
#define DECLARE_INTERFACE(iface)    interface iface
#define DECLARE_INTERFACE_(iface, baseiface)    interface iface : public baseiface

#define BEGIN_INTERFACE
#define END_INTERFACE

#else

#define interface               struct
#define STDMETHOD(method)       HRESULT STDMETHODCALLTYPE (*method)
#define STDMETHOD_(type,method) type STDMETHODCALLTYPE (*method)
#define PURE
#define THIS_                   INTERFACE FAR* This,
#define THIS                    INTERFACE FAR* This

#ifdef CONST_VTABLE
#undef CONST_VTBL
#define CONST_VTBL const
#define DECLARE_INTERFACE(iface) \
         typedef interface iface { const struct iface##Vtbl FAR* lpVtbl; } iface; \
         typedef const struct iface##Vtbl iface##Vtbl; \
         const struct iface##Vtbl
#else
#undef CONST_VTBL
#define CONST_VTBL
#define DECLARE_INTERFACE(iface) \
         typedef interface iface { struct iface##Vtbl FAR* lpVtbl; } iface; \
         typedef struct iface##Vtbl iface##Vtbl; \
         struct iface##Vtbl
#endif
#define DECLARE_INTERFACE_(iface, baseiface)    DECLARE_INTERFACE(iface)

#define BEGIN_INTERFACE
#define END_INTERFACE

#endif /* __cplusplus && !CINTERFACE */

#endif /* __WINE__ */

#endif /* _OBJBASE_H_ */
