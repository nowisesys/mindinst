========================================================================
    BSD IP FIREWALL (IPFW32) MINIPORT DRIVER INSTALLER
========================================================================

This is an installer application for the BSD IP Firewall (IPFW32) for Windows XP
or later. The installer is using the InetCfg API.

The code used in this application is borrowed from the BindView application written 
by Alok Sinha, Copyright (C) Microsoft Corporation, 2001. This source code for that
application is bundled with the Windows Driver Kit (WDK).

The original code by Alok Sinha is keept in bindview.* and netcfgapi.*, and is the
unmodified except for removal of GUI calls. These files are _not_ the original, only
the minimal set of required function have been used.

The following exit code are used:

  0 - Success
  1 - Failed
  2 - Reboot required

This code is not permitted to be released as source code, only in object code 
form (binary). See 2a (i) in license.rtf distributed with WDK.

// Anders Lövgren, 2011-10-28
