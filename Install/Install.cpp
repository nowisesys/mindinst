// -----------------------------------------------------------------------
// Install.cpp : Defines the entry point for the console application.
// 
// Copyright (C) 2011 Anders Lövgren
// Computing Department at BMC, Uppsala University
// -----------------------------------------------------------------------
//

#include "bindview.h"
#include "stdafx.h"

static void Usage() 
{
        wprintf(L"NDIS Miniport Driver Installer\n");
        wprintf(L"\n");
        wprintf(L"Usage: %s <inf-file> [-i|-u] [-c|-s|-p]\n", APP_NAME);
        wprintf(L"Options:\n");
        wprintf(L"  -i:  Install device (default).\n");
        wprintf(L"  -u:  Uninstall device.\n");
        wprintf(L"  -c:  Configure network client.\n");
        wprintf(L"  -s:  Configure network service (default).\n");
        wprintf(L"  -p:  Configure network protocol.\n");
        wprintf(L"  -h:  This casual help.\n");
        wprintf(L"\n");
        wprintf(L"Copyright (C) 2011-2012 Anders Lövgren, Computing Department at BMC, Uppsala University.\n");
}

HRESULT
UninstallComponentType(__in_opt LPWSTR lpszInfFile)
{
        HRESULT   hr;

        if ( lpszInfFile ) {

                LPWSTR  lpszPnpID;

                //
                // Inf file name specified, install the network component
                // from this file.
                //

                hr = GetPnpID( lpszInfFile, &lpszPnpID );

                if ( hr == S_OK ) {

                        hr = UninstallComponent(lpszPnpID);
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
                        wprintf(L"Component uninstalled successfully.\n");
                        break;

                case NETCFG_S_REBOOT:
                        wprintf(L"Component uninstalled successfully: Reboot required.\n");
                        break;
        }

        return hr;
}

int _tmain(int argc, _TCHAR* argv[])
{
        UINT uiDeviceType  = INSTALL_DEVCLASS_NETSERVICE;
        UINT uiInstallMode = MODE_INSTALL;
        HRESULT hr;
        int i;

        // 
        // Required to get output of swedish characters to console:
        // 
        setlocale(LC_CTYPE, "swedish");

        if(argc < 2) {
                Usage();
                return 1;
        }
        if(wcscmp(argv[1], L"-h") == 0 || wcscmp(argv[1], L"--help") == 0 || wcscmp(argv[1], L"/?") == 0) {
                Usage();
                return 0;
        }

        for(i = 1; i < argc; ++i) {
                if(wcscmp(argv[i], L"-c") == 0) {
                        uiDeviceType = INSTALL_DEVCLASS_NETCLIENT;
                } else if(wcscmp(argv[i], L"-s") == 0) {
                        uiDeviceType = INSTALL_DEVCLASS_NETSERVICE;
                } else if(wcscmp(argv[i], L"-s") == 0) {
                        uiDeviceType = INSTALL_DEVCLASS_NETTRANS;
                } else if(wcscmp(argv[i], L"-i") == 0) {
                        uiInstallMode = MODE_INSTALL;
                } else if(wcscmp(argv[i], L"-u") == 0) {
                        uiInstallMode = MODE_REMOVE;
                }
        }

        switch(uiInstallMode) {
                case MODE_INSTALL:
                        hr = InstallSelectedComponentType(uiDeviceType, argv[1]);
                        break;
                case MODE_REMOVE:
                        hr = UninstallComponentType(argv[1]);
                        break;
        }

        switch(hr) {
                case S_OK:
                        return EXIT_SUCCESS;
                case NETCFG_S_REBOOT:
                        return EXIT_REBOOT_REQ;
                default:
                        return EXIT_FAILURE;
        }
}

