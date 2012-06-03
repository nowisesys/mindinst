//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 2001.
//
//  File:       N E T C F G A P I . H
//
//  Contents:   Functions Prototypes
//
//  Notes:      
//
//  Author:     Alok Sinha    15-May-01
//
//----------------------------------------------------------------------------

// 
//----------------------------------------------------------------------------
// NOTICE: This is a modified copy of the original netcfgapi.h, see README.txt
//----------------------------------------------------------------------------
// 

#ifndef __NETCFGAPI_H__
#define __NETCFGAPI_H__

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wchar.h>
#include <netcfgx.h>
#include <netcfgn.h>
#include <setupapi.h>
#include <devguid.h>
#include <objbase.h>
#include <strsafe.h>

#define LOCK_TIME_OUT     5000

HRESULT HrGetINetCfg (IN BOOL fGetWriteLock,
                      IN LPCWSTR lpszAppName,
                      OUT INetCfg** ppnc,
                      __deref_opt_out_opt LPWSTR *lpszLockedBy);

HRESULT HrInstallNetComponent (IN INetCfg *pnc,
                               IN LPCWSTR szComponentId,
                               IN const GUID    *pguildClass,
                               IN LPCWSTR lpszInfFullPath);

HRESULT HrInstallComponent(IN INetCfg* pnc,
                           IN LPCWSTR szComponentId,
                           IN const GUID* pguidClass);

HRESULT HrReleaseINetCfg (INetCfg* pnc,
                          BOOL fHasWriteLock);

VOID ReleaseRef (IUnknown* punk);

#endif  // __NETCFG_API_H__
