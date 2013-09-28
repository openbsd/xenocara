/*

Copyright (c) 1987, 1988  X Consortium

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/

/* Vendor-specific data structures and operations */

#include "globals.h"
#include "vendor.h"

typedef struct sectionName {
    const char *name;
    const char *suffix;
} SectionNameRec;

#if (defined(__osf__) || defined(SVR4)) && !defined(sun)

static SectionNameRec SectionNames[] = {
    {"(1) User Commands",               "1"},
    {"(1m) Sys, Administration",        "1m"},
    {"(2) System Calls",                "2"},
    {"(3) Subroutines",                 "3"},
    {"(4) File Formats",                "4"},
    {"(5) Miscellaneous",               "5"},
    {"(6) Games",                       "6"},
    {"(7) Devices",                     "7"},
    {"(8) Sys. Administration",         "8"},
    {"(l) Local",                       "l"},
    {"(n) New",                         "n"},
    {"(o) Old",                         "o"}
#ifdef __SCO__
    ,
    {"(ADM) System Administration", "ADM"},
    {"(ADMN) Network Administration", "ADMN"},
    {"(ADMP) Protocol Administration", "ADMP"},
    {"(C) Commands", "C"},
    {"(CMD) DOS Commands", "CMD"},
    {"(CP) Programming Commands", "CP"},
    {"(DOS) DOS Subroutines and Libraries", "DOS"},
    {"(F) File Formats", "F"},
    {"(HW) Hardware Dependant", "HW"},
    {"(K) Kernel Subroutines", "K"},
    {"(LOCAL) Local utilities for your system", "LOCAL"},
    {"(M) Miscellaneous", "M"},
    {"(NADM) NFS Administration", "NADM"},
    {"(NC) Network Commands", "NC"},
    {"(NF) Network File Formats", "NF"},
    {"(NS) Network Subroutines", "NS"},
    {"(NSL) Network Services", "NSL"},
    {"(S) Subroutines and Libraries", "S"},
    {"(SCO) Product Engineering Toolkit", "SCO"},
    {"(SFF) Socket File Formats", "SFF"},
    {"(STR) Streams", "STR"},
    {"(TC) Transport Layer Commands", "TC"},
    {"(X) X Man pages", "X"},
    {"(XNX) Xenix Subroutines, Commands and Libs", "XNX"},
    {"(XS) X11 Subroutines and Libraries", "XS"},
    {"(Xm) Motif Commands and Subroutines", "Xm"},
    {"(Xt) X Intrinsics", "Xt"}
#endif
};

#else

#if defined(sun) && defined(SVR4)
/* losing man.cf is useless because it has no section names */

static SectionNameRec SectionNames[] = {
    {"(1)  User Commands",                      "1"},
    {"(1b) SunOS/BSD Compatibility Commands",   "1b"},
    {"(1c) Communication Commands",             "1c"},
    {"(1f) FMLI Commands",                      "1f"},
    {"(1m) Maintenance Commands",               "1m"},
    {"(1s) SunOS Specific Commands",            "1s"},
    {"(2)  System Calls",                       "2"},
    {"(3)  Introduction to Library Functions",  "3"},
    {"(3adm) General Administrative Library Functions", "3adm"},
    {"(3aio) Asynchronous I/O Library Functions",       "3aio"},
    {"(3b) SunOS/BSD Compatibility Functions",  "3b"},
    {"(3bsm) Security and Auditing Library Functions",  "3bsm"},
    {"(3c) C Library Functions",                "3c"},
    {"(3cfgadm) Configuration Administration Library Functions",  "3cfgadm"},
    {"(3crypt) Encryption and Decryption Library",  "3crypt"},
    {"(3curses) Curses Library Functions",      "3curses"},
    {"(3devid) Device ID Library Functions",    "3devid"},
    {"(3devinfo) Device Information Library Functions",  "3devinfo"},
    {"(3dl) Dynamic Linking Library Functions", "3dl"},
    {"(3dmi) DMI Library Functions",            "3dmi"},
    {"(3door) Door Library Functions",          "3door"},
    {"(3e) ELF Library Functions",              "3e"},
    {"(3elf) ELF Library Functions",            "3elf"},
    {"(3ext) Extended Library Functions",       "3ext"},
    {"(3g) C Library Functions",                "3g"},
    {"(3gen) String Pattern-Matching Library Functions", "3gen"},
    {"(3head) Headers",                         "3head"},
    {"(3i) Wide Character Functions",           "3i"},
    {"(3k) Kernel VM Library Functions",        "3k"},
    {"(3krb) Kerberos Library Functions",       "3krb"},
    {"(3kstat) Kernel Statistics Library Functions",    "3kstat"},
    {"(3kvm) Kernel VM Library Functions",      "3kvm"},
    {"(3layout) Layout Services Library Functions",     "3layout"},
    {"(3ldap) LDAP Library Functions",          "3ldap"},
    {"(3lib) Interface Libraries",              "3lib"},
    {"(3libucb) SunOS/BSD Compatibility Interface Libraries",   "3libucb"},
    {"(3m) Mathematical Library Functions",     "3m"},
    {"(3mail) User Mailbox Library Functions",  "3mail"},
    {"(3malloc) Memory Allocation Library Functions",   "3malloc"},
    {"(3mp) Integer Mathematical Library Functions",    "3mp"},
    {"(3n) Network Functions",                  "3n"},
    {"(3nsl) Networking Services Library Functions",    "3nsl"},
    {"(3pam) PAM Library Functions",                    "3pam"},
    {"(3plot) Graphics Interface Library Functions",    "3plot"},
    {"(3proc) Process Control Library Functions",       "3proc"},
    {"(3r) Realtime Library",                   "3r"},
    {"(3rac) Remote Asynchronous Calls Library Functions", "3rac"},
    {"(3resolv) Resolver Library Functions",    "3resolv"},
    {"(3rpc) RPC Library Functions",            "3rpc"},
    {"(3rt) Realtime Library Functions",        "3rt"},
    {"(3s) Standard I/O Functions",             "3s"},
    {"(3sched) LWP Scheduling Library Functions",       "3sched"},
    {"(3sec) File Access Control Library Functions",    "3sec"},
    {"(3secdb) Security Attributes Database Library Functions", "3secdb"},
    {"(3snmp) SNMP Library Functions",          "3snmp"},
    {"(3socket) Sockets Library Functions",     "3socket"},
    {"(3t) Threads Library Functions",          "3t"},
    {"(3thr) Threads Library Functions",        "3thr"},
    {"(3tnf) TNF Library Functions",            "3tnf"},
    {"(3ucb) SunOS/BSD Compatibility Library Functions", "3ucb"},
    {"(3volmgt) Volume Management Library Functions",   "3volmgt"},
    {"(3x) Miscellaneous Library Functions",    "3x"},
    {"(3xc) X/Open Curses Library Functions",   "3xc"},
    {"(3xcurses) X/Open Curses Library Functions",      "3xcurses"},
    {"(3xfn) XFN Interface Library Functions",  "3xfn"},
    {"(3xn) X/Open Networking Services Library Functions",      "3xn"},
    {"(3xnet) X/Open Networking Services Library Functions",    "3xnet"},
    {"(3x11)  X Window System: Xlib Functions", "3x11"},
    {"(3xcb)  X Window System: X C Binding Functions",          "3xcb"},
    {"(3xext) X Window System: Protocol Extension Functions",   "3xext"},
    {"(3xi)   X Window System: Input Extension Functions",      "3xi"},
    {"(3xmu)  X Window System: Miscellaneous Utility Functions","3xmu"},
    {"(3xp)   X Window System: Print Extension Functions",      "3xp"},
    {"(3xt)   X Window System: Toolkit Intrinsics Functions",   "3xt"},
    {"(4)  File Formats",                       "4"},
    {"(4b) Misc. Reference Manual Pages",       "4b"},
    {"(5)  Environments, Tables, and TROFF Macros", "5"},
    {"(6)  Games and Demos",                    "6"},
    {"(7)  Special Files",                      "7"},
    {"(7d)  Devices",                           "7d"},
    {"(7fs) File Systems",                      "7fs"},
    {"(7i)  Ioctl Requests",                    "7i"},
    {"(7m)  STREAMS Modules",                   "7m"},
    {"(7p)  Protocols",                         "7p"},
    {"(9)  Device Driver Information",          "9"},
    {"(9e) DDI and DKI Driver Entry Points",    "9e"},
    {"(9f) DDI and DKI Kernel Functions",       "9f"},
    {"(9s) DDI and DKI Data Structures",        "9s"},
    {"(l)  Local",                              "l"},
    {"(n)  New",                                "n"},
    {"(o)  Old",                                "o"}
};

#else

static SectionNameRec SectionNames[] = {
    {"(1) User Commands",                       "1"},
    {"(2) System Calls",                        "2"},
    {"(3) Subroutines",                         "3"},
    {"(4) Devices",                             "4"},
    {"(5) File Formats",                        "5"},
    {"(6) Games",                               "6"},
    {"(7) Miscellaneous",                       "7"},
    {"(8) Sys. Administration",                 "8"},
#if defined(__OpenBSD__) || defined(__NetBSD__)
    {"(9) Kernel Manual",                       "9"},
#endif
#if defined(__FreeBSD__)
    {"(9) Kernel Interfaces",                   "9"},
#endif
    {"(l) Local",                               "l"},
    {"(n) New",                                 "n"},
    {"(o) Old",                                 "o"},
#ifdef sony
    {"(p) Public Domain",                       "p"},
    {"(s) Sony Specific",                       "s"},
#endif
};

#endif
#endif

/*	Function Name: AddStandardSections
 *	Description: Adds all the standard sections to the list for this path.
 *	Arguments: list - a pointer to the section list.
 *                 path - the path to these standard sections.
 *	Returns: none.
 */

void
AddStandardSections(SectionList ** list, const char *path)
{
    register int i;
    char file[BUFSIZ];
    int numSections = sizeof(SectionNames) / sizeof(SectionNames[0]);

    for (i = 0; i < numSections; i++) {
        snprintf(file, sizeof(file), "%s%s", SEARCHDIR, SectionNames[i].suffix);
        AddNewSection(list, path, file, SectionNames[i].name, TRUE);
#ifdef SEARCHOTHER
        snprintf(file, sizeof(file), "%s%s", SEARCHOTHER,
                 SectionNames[i].suffix);
        AddNewSection(list, path, file, SectionNames[i].name, TRUE);
#endif
    }
}



/*	Function Name: CreateManpageName
 *	Description: Creates the manual page name for a given item.
 *	Arguments: entry - the entry to convert.
 *	Returns: the manual page properly allocated.
 */

/*
 * If the filename is foo.3     - Create an entry of the form:  foo
 * If the filename is foo.3X11  - Create an entry of the form:  foo(X11)
 * IF the filename is a.out.1   - Create an entry of the form:  a.out
 */

char *
CreateManpageName(const char *entry, int section,     /* FIXME: unused */
                  int flags)
{
    char *cp;
    char page[BUFSIZ];
    char sect[BUFSIZ];

    ParseEntry(entry, NULL, sect, page);

    if ((cp = strrchr(page, '.')) != NULL) {
        if ((int) strlen(cp) > 2) {
            *cp++ = '(';
            while ((cp[1] != '\0')) {
                *cp = *(cp + 1);
                cp++;
            }
            *cp++ = ')';
            *cp = '\0';
        }
        else
            *cp = '\0';
    }


    return (XtNewString(page));
}
