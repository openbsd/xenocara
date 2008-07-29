/* **********************************************************
 * Copyright 2007 VMware, Inc.  All rights reserved.
 * **********************************************************/

/*
 * svga_escape.h --
 *
 *    Definitions for our own (vendor-specific) SVGA Escape commands.
 */

#ifndef _SVGA_ESCAPE_H_
#define _SVGA_ESCAPE_H_

/*
 * Namespace IDs for the escape command
 */

#define SVGA_ESCAPE_NSID_VMWARE 0x00000000
#define SVGA_ESCAPE_NSID_DEVEL  0xFFFFFFFF

/*
 * Within SVGA_ESCAPE_NSID_VMWARE, we multiplex commands according to
 * the first DWORD of escape data (after the nsID and size). As a
 * guideline we're using the high word and low word as a major and
 * minor command number, respectively.
 */

#define SVGA_ESCAPE_VMWARE_MAJOR_MASK  0xFFFF0000

#endif /* _SVGA_ESCAPE_H_ */
