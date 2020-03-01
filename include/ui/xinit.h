#ifndef __XINIT_H
#define __XINIT_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>


#include <strings.h>

Display* xinit(void);

#define WIDTH  440
#define HEIGHT 428

Display* display;
int screen;
GC gc;
Colormap colormap;
XColor xcolor;
Window parent_window;
Window calc_window;
Atom wm_delete;

#endif