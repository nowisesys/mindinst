#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#define APP_NAME            L"Install"

#define INSTALL_DEVCLASS_NETCLIENT   0
#define INSTALL_DEVCLASS_NETSERVICE  1
#define INSTALL_DEVCLASS_NETTRANS    2

#define EXIT_SUCCESS    0
#define EXIT_FAILURE    1
#define EXIT_REBOOT_REQ 2

#define MODE_INSTALL    1
#define MODE_REMOVE     2
