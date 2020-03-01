#include "ui/xinit.h"


Display* xinit(void) {
    display = XOpenDisplay(NULL);
    

    if (display == NULL)
        return NULL;
    screen = DefaultScreen(display);
    colormap = XDefaultColormap(display, screen);
    gc = DefaultGC(display, screen);

    parent_window = DefaultRootWindow(display);
    XParseColor(display, colormap, "#232629", &xcolor);
    XAllocColor(display, colormap, &xcolor);
    xcolor.flags = DoRed | DoBlue | DoGreen;
    
    calc_window = XCreateSimpleWindow(display, parent_window,
                                      0, 0,
                                      WIDTH, HEIGHT,
                                      1, BlackPixel(display, screen), xcolor.pixel);
    XSelectInput(display, calc_window, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | FocusChangeMask);
    XInstallColormap(display, colormap);
    XMapWindow(display, calc_window);
    
    XStoreName(display, calc_window, "XCalculator");

    wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", True);

    XSetWMProtocols(display, calc_window, &wm_delete, 1);

    return display;
};