//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 2001.
//
//  File:       B I N D V I E W . H
//
//  Contents:   Function Prototypes
//
//  Notes:      
//
//  Author:     Alok Sinha    15-May-01
//
//----------------------------------------------------------------------------

// 
//----------------------------------------------------------------------------
// NOTICE: This is a modified copy of the original bindview.h, see README.txt
//----------------------------------------------------------------------------
// 

#ifndef __BINDVIEW_H__
#define __BINDVIEW_H__

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <wchar.h>
#include <setupapi.h>
#include <devguid.h>
#include <netcfgx.h>
#include <netcfgn.h>
#include <strsafe.h>

#include "netcfgapi.h"

#define celems(_x)          (sizeof(_x) / sizeof(_x[0]))

VOID ErrMsg (HRESULT hr,
             LPCWSTR  lpFmt,
             ...);

HRESULT InstallSelectedComponentType (__in UINT uiDeviceType,
                                      __in_opt LPWSTR lpszInfFile);

HRESULT InstallSpecifiedComponent (__in LPWSTR lpszInfFile,
                                   __in LPWSTR lpszPnpID,
                                   const GUID *pguidClass);

HRESULT UninstallComponent (__in LPWSTR lpszInfId);

HRESULT GetPnpID (__in LPWSTR lpszInfFile,
                  __deref_out_opt LPWSTR *lppszPnpID);

HRESULT GetKeyValue (HINF hInf,
                     __in LPCWSTR lpszSection,
                     __in_opt LPCWSTR lpszKey,
                     DWORD  dwIndex,
                     __deref_out_opt LPWSTR *lppszValue);

#endif  // __BINDVIEW_H__
