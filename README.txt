========================================================================
    NDIS MINIPORT DRIVER INSTALLER (MINDINST)
========================================================================

This is an command line installer application capable of installing NDIS Miniport 
drivers (like the BSD IP Firewall [IPFW32]) on Windows XP or later.

** HISTORY:

   The initial project name was simply "install" with description "BDS IP 
   Firewall (IPFW32) Installer" because its primary goal was to create an command 
   line tool to install the ipfw.sys NDIS miniport driver.
   
   As the project evolved to become a generic installer for network client, service 
   and protocol drivers, the project name was changed to mindinst (miniport driver 
   installer).
   
** SOURCE CODE:

   The installer is using the InetCfg API.

   The code used in this application is borrowed from the BindView application 
   written by Alok Sinha, Copyright (C) Microsoft Corporation, 2001. This source 
   code for that application is bundled with the Windows Driver Kit (WDK).

   The original code by Alok Sinha is keept in bindview.* and netcfgapi.*, and 
   is the unmodified except for removal of GUI calls. These files are _not_ the 
   original, only the minimal set of required function have been used.

** EXIT CODES:

   The following exit code are used:

     0 - Success
     1 - Failed
     2 - Reboot required

** RELEASE NOTES:

   This code is not permitted to be released as source code, only in object code 
   form (binary). See 2a (i) in license.rtf distributed with WDK.
   
   A binary release can be downloaded from: 
   http://it.bmc.uu.se/andlov/win32/mindinst/


// Anders Lövgren, 2011-10-28
