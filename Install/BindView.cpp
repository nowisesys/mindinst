//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 2001.
//
//  File:       B I N D V I E W . C P P
//
//  Contents:
//
//  Notes:
//
//  Author:     Alok Sinha    15-Amy-01
//
//----------------------------------------------------------------------------

// 
//----------------------------------------------------------------------------
// NOTICE: This is a modified copy of the original bindview.cpp, see README.txt
//----------------------------------------------------------------------------
// 

#include "bindview.h"
#include "stdafx.h"

//
// GUIDs of network components.
//

const GUID     *pguidNetClass [] = {
        &GUID_DEVCLASS_NETCLIENT,
        &GUID_DEVCLASS_NETSERVICE,
        &GUID_DEVCLASS_NETTRANS,
        &GUID_DEVCLASS_NET
};

//
// Function:  InstallSelectedComponentType
//
// Purpose:   Install a network component.
//
// Arguments:
//    uiDeviceType [in]  Type of device to install.
//    lpszInfFile  [in]  Inf file of the network component.
//
// Returns:   None
//
// Notes:
//       If lpszInfFile is NULL, network components are installed from the
//       system directory.
//

HRESULT
InstallSelectedComponentType (
                              UINT uiDeviceType,
                              __in_opt LPWSTR lpszInfFile
                              )
{
        LPARAM    lParam;
        DWORD     dwType;
        BOOL      fEnable;
        HRESULT   hr;

        if ( lpszInfFile ) {

                LPWSTR  lpszPnpID;

                // 
                // The original code called GetItemInfo() to find out if a client, service or 
                // protocol should be installed.
                // 
                lParam = uiDeviceType;

                //
                // Inf file name specified, install the network component
                // from this file.
                //

                hr = GetPnpID( lpszInfFile, &lpszPnpID );

                if ( hr == S_OK ) {

                        hr = InstallSpecifiedComponent( lpszInfFile,
                                lpszPnpID,
                                pguidNetClass[(UINT)lParam] );

                        CoTaskMemFree( lpszPnpID );
                }
                else {
                        ErrMsg( hr,
                                L"Error reading the INF file %s.",
                                lpszInfFile );
                }
        }

        switch( hr ) {

                case S_OK:
                        wprintf(L"Component installed successfully.\n");
                        break;

                case NETCFG_S_REBOOT:
                        wprintf(L"Component installed successfully: Reboot required.\n");
                        break;
        }

        return hr;
}

//
// Function:  InstallSpecifiedComponent
//
// Purpose:   Install a network component from an INF file.
//
// Arguments:
//    lpszInfFile [in]  INF file.
//    lpszPnpID   [in]  PnpID of the network component to install.
//    pguidClass  [in]  Class GUID of the network component.
//
// Returns:   None.
//
// Notes:
//

HRESULT InstallSpecifiedComponent ( __in LPWSTR lpszInfFile,
                                   __in LPWSTR lpszPnpID,
                                   const GUID *pguidClass)
{
        INetCfg    *pnc;
        LPWSTR     lpszApp;
        HRESULT    hr;

        hr = HrGetINetCfg( TRUE,
                APP_NAME,
                &pnc,
                &lpszApp );

        if ( hr == S_OK ) {

                //
                // Install the network component.
                //

                hr = HrInstallNetComponent( pnc,
                        lpszPnpID,
                        pguidClass,
                        lpszInfFile );
                if ( (hr == S_OK) || (hr == NETCFG_S_REBOOT) ) {

                        hr = pnc->Apply();
                }
                else {
                        if ( hr != HRESULT_FROM_WIN32(ERROR_CANCELLED) ) {
                                ErrMsg( hr,
                                        L"Couldn't install the network component." );
                        }
                }

                HrReleaseINetCfg( pnc,
                        TRUE );
        }
        else {
                if ( (hr == NETCFG_E_NO_WRITE_LOCK) && lpszApp ) {
                        ErrMsg( hr,
                                L"%s currently holds the lock, try later.",
                                lpszApp );

                        CoTaskMemFree( lpszApp );
                }
                else {
                        ErrMsg( hr,
                                L"Couldn't the get notify object interface." );
                }
        }

        return hr;
}

//
// Function:  UninstallComponent
//
// Purpose:   Uninstall a network component.
//
// Arguments:
//    lpszInfId  [in]  PnpID of the network component to uninstall.
//
// Returns:   S_OK on success, otherwise an error code.
//
// Notes:
//

HRESULT UninstallComponent (__in LPWSTR lpszInfId)
{
        INetCfg              *pnc;
        INetCfgComponent     *pncc;
        INetCfgClass         *pncClass;
        INetCfgClassSetup    *pncClassSetup;
        LPWSTR               lpszApp;
        GUID                 guidClass;
        OBO_TOKEN            obo;
        HRESULT              hr;

        hr = HrGetINetCfg( TRUE,
                APP_NAME,
                &pnc,
                &lpszApp );

        if ( hr == S_OK ) {

                //
                // Get a reference to the network component to uninstall.
                //

                hr = pnc->FindComponent( lpszInfId,
                        &pncc );

                if ( hr == S_OK ) {

                        //
                        // Get the class GUID.
                        //

                        hr = pncc->GetClassGuid( &guidClass );

                        if ( hr == S_OK ) {

                                //
                                // Get a reference to component's class.
                                //

                                hr = pnc->QueryNetCfgClass( &guidClass,
                                        IID_INetCfgClass,
                                        (PVOID *)&pncClass );
                                if ( hr == S_OK ) {

                                        //
                                        // Get the setup interface.
                                        //

                                        hr = pncClass->QueryInterface( IID_INetCfgClassSetup,
                                                (LPVOID *)&pncClassSetup );

                                        if ( hr == S_OK ) {

                                                //
                                                // Uninstall the component.
                                                //

                                                ZeroMemory( &obo,
                                                        sizeof(OBO_TOKEN) );

                                                obo.Type = OBO_USER;

                                                hr = pncClassSetup->DeInstall( pncc,
                                                        &obo,
                                                        NULL );
                                                if ( (hr == S_OK) || (hr == NETCFG_S_REBOOT) ) {

                                                        hr = pnc->Apply();

                                                        if ( (hr != S_OK) && (hr != NETCFG_S_REBOOT) ) {
                                                                ErrMsg( hr,
                                                                        L"Couldn't apply the changes after"
                                                                        L" uninstalling %s.",
                                                                        lpszInfId );
                                                        }
                                                }
                                                else {
                                                        ErrMsg( hr,
                                                                L"Failed to uninstall %s.",
                                                                lpszInfId );
                                                }

                                                ReleaseRef( pncClassSetup );
                                        }
                                        else {
                                                ErrMsg( hr,
                                                        L"Couldn't get an interface to setup class." );
                                        }

                                        ReleaseRef( pncClass );
                                }
                                else {
                                        ErrMsg( hr,
                                                L"Couldn't get a pointer to class interface "
                                                L"of %s.",
                                                lpszInfId );
                                }
                        }
                        else {
                                ErrMsg( hr,
                                        L"Couldn't get the class guid of %s.",
                                        lpszInfId );
                        }

                        ReleaseRef( pncc );
                }
                else {
                        ErrMsg( hr,
                                L"Couldn't get an interface pointer to %s.",
                                lpszInfId );
                }

                HrReleaseINetCfg( pnc,
                        TRUE );
        }
        else {
                if ( (hr == NETCFG_E_NO_WRITE_LOCK) && lpszApp ) {
                        ErrMsg( hr,
                                L"%s currently holds the lock, try later.",
                                lpszApp );

                        CoTaskMemFree( lpszApp );
                }
                else {
                        ErrMsg( hr,
                                L"Couldn't get the notify object interface." );
                }
        }

        return hr;
}

//
// Function:  ErrMsg
//
// Purpose:   Insert text for each network component type.
//
// Arguments:
//    hr  [in]  Error code.
//
// Returns:   None.
//
// Notes:
//

VOID ErrMsg (
             HRESULT hr, 
             LPCWSTR  lpFmt, 
             ...
             )
{

        LPWSTR   lpSysMsg;
        WCHAR    buf[400];
        size_t   offset;
        va_list  vArgList;


        if ( hr != 0 ) {
                StringCchPrintfW ( buf,
                        celems(buf),
                        L"Error %#lx: ",
                        hr );
        }
        else {
                buf[0] = 0;
        }

        offset = wcslen( buf );

        va_start( vArgList,
                lpFmt );
        StringCchVPrintfW ( buf+offset,
                celems(buf) - offset,
                lpFmt,
                vArgList );

        va_end( vArgList );

        if ( hr != 0 ) {
                FormatMessageW( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                        FORMAT_MESSAGE_FROM_SYSTEM |
                        FORMAT_MESSAGE_IGNORE_INSERTS,
                        NULL,
                        hr,
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                        (LPWSTR)&lpSysMsg,
                        0,
                        NULL );
                if ( lpSysMsg ) {

                        offset = wcslen( buf );

                        StringCchPrintfW ( buf+offset,
                                celems(buf) - offset,
                                L"\n\nPossible cause:\n\n" );

                        offset = wcslen( buf );

                        StringCchCatW ( buf+offset,
                                celems(buf) - offset,
                                lpSysMsg );

                        LocalFree( (HLOCAL)lpSysMsg );
                }

                fwprintf(stderr, L"%s\n", buf);
        }
        else {
                wprintf(L"%s\n", buf);
        }

        return;
}

//
// Function:  GetPnpID
//
// Purpose:   Retrieve PnpID from an inf file.
//
// Arguments:
//    lpszInfFile [in]  Inf file to search.
//    lppszPnpID  [out] PnpID found.
//
// Returns:   TRUE on success.
//
// Notes:
//

HRESULT
GetPnpID (
          __in LPWSTR lpszInfFile,
          __deref_out_opt LPWSTR *lppszPnpID)
{
        HINF    hInf;
        LPWSTR  lpszModelSection;
        HRESULT hr;

        *lppszPnpID = NULL;

        hInf = SetupOpenInfFileW( lpszInfFile,
                NULL,
                INF_STYLE_WIN4,
                NULL );

        if ( hInf == INVALID_HANDLE_VALUE )
        {

                return HRESULT_FROM_WIN32(GetLastError());
        }

        //
        // Read the Model section name from Manufacturer section.
        //

        hr = GetKeyValue( hInf,
                L"Manufacturer",
                NULL,
                1,
                &lpszModelSection );

        if ( hr == S_OK )
        {

                //
                // Read PnpID from the Model section.
                //

                hr = GetKeyValue( hInf,
                        lpszModelSection,
                        NULL,
                        2,
                        lppszPnpID );

                CoTaskMemFree( lpszModelSection );
        }

        SetupCloseInfFile( hInf );

        return hr;
}

//
// Function:  GetKeyValue
//
// Purpose:   Retrieve the value of a key from the inf file.
//
// Arguments:
//    hInf        [in]  Inf file handle.
//    lpszSection [in]  Section name.
//    lpszKey     [in]  Key name.
//    dwIndex     [in]  Key index.
//    lppszValue  [out] Key value.
//
// Returns:   S_OK on success, otherwise and error code.
//
// Notes:
//

HRESULT
GetKeyValue (
             HINF hInf,
             __in LPCWSTR lpszSection,
             __in_opt LPCWSTR lpszKey,
             DWORD  dwIndex,
             __deref_out_opt LPWSTR *lppszValue)
{
        INFCONTEXT  infCtx;
        __range(0, 512) DWORD       dwSizeNeeded;
        HRESULT     hr;

        *lppszValue = NULL;

        if ( SetupFindFirstLineW(hInf,
                lpszSection,
                lpszKey,
                &infCtx) == FALSE )
        {
                return HRESULT_FROM_WIN32(GetLastError());
        }

        if ( SetupGetStringFieldW(&infCtx,
                dwIndex,
                NULL,
                0,
                &dwSizeNeeded) )
        {
                *lppszValue = (LPWSTR)CoTaskMemAlloc( sizeof(WCHAR) * dwSizeNeeded );

                if ( !*lppszValue  )
                {
                        return HRESULT_FROM_WIN32(ERROR_NOT_ENOUGH_MEMORY);
                }

                if ( SetupGetStringFieldW(&infCtx,
                        dwIndex,
                        *lppszValue,
                        dwSizeNeeded,
                        NULL) == FALSE )
                {

                        hr = HRESULT_FROM_WIN32(GetLastError());

                        CoTaskMemFree( *lppszValue );
                        *lppszValue = NULL;
                }
                else
                {
                        hr = S_OK;
                }
        }
        else
        {
                DWORD dwErr = GetLastError();
                hr = HRESULT_FROM_WIN32(dwErr);
        }

        return hr;
}
