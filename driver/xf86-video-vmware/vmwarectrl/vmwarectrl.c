#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/Xproto.h>
#include <X11/extensions/panoramiXproto.h>
#include <string.h>
#include <stdlib.h>

#include "libvmwarectrl.h"

int
main (int argc, char **argv)
{
   Display *dpy;
   char *displayName = NULL;
   int screen;
   int major, minor;

   dpy = XOpenDisplay(displayName);
   if (!dpy) {
      printf("Could not open default X Display\n");
      exit(EXIT_FAILURE);
   }
   screen = DefaultScreen(dpy);

   if (VMwareCtrl_QueryVersion(dpy, &major, &minor)) {
      printf("Got Extension version %d.%d\n", major, minor);
   } else {
      printf("VMWARE_CTRL Extension not found.\n");
      exit(EXIT_FAILURE);
   }

   if (argc >= 2) {
      if (strcmp(argv[1], "setres") == 0) {
         int x, y;
         if (argc < 4) {
            printf("Setres needs x and y too\n");
            exit(EXIT_FAILURE);
         }

         x = atoi(argv[2]);
         y = atoi(argv[3]);

         if (VMwareCtrl_SetRes(dpy, screen, x, y)) {
            printf("Set Res was successful\n");
         } else {
            printf("Set Res failed\n");
         }
      } else if (strcmp(argv[1], "settopology") == 0) {
         xXineramaScreenInfo extents[2];

         if (major == 0 && minor < 2) {
            printf("VMWARE_CTRL version >= 0.2 is required\n");
            exit(EXIT_FAILURE);
         }

         printf("Requesting hard-coded topology\n");

         extents[0].x_org = 0;
         extents[0].y_org = 0;
         extents[0].width = 800;
         extents[0].height = 600;
         extents[1].x_org = 800;
         extents[1].y_org = 0;
         extents[1].width = 800;
         extents[1].height = 600;
         if (VMwareCtrl_SetTopology(dpy, screen, extents, 2)) {
            printf("SetTopology was successful\n");
         } else {
            printf("SetTopology failed\n");
         }
      }
   }

   return EXIT_SUCCESS;
}
