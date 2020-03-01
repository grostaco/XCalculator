#include "ui/xinit.h"
#include <stdio.h>
#include <sys/select.h>

int main () {
    int x11_fd;
    fd_set in_fd;
    struct timeval tv;
    int r= 0;
    xinit();
    XEvent event;
    x11_fd = ConnectionNumber(display);
    while (1) {
        tv.tv_usec = 0;
        tv.tv_sec = 1;
        FD_ZERO(&in_fd);
        FD_SET(x11_fd, &in_fd);
        if (select(x11_fd + 1, &in_fd, NULL, NULL, &tv) == 0) 
            goto nonevent;
        XNextEvent(display, &event);
        if (event.type == Expose) {
            XClearWindow(display, calc_window);
            
            XSetForeground(display, gc, BlackPixel(display, screen));
            XSetBackground(display, gc, BlackPixel(display, screen));

            XFillArc(display, calc_window, gc, 10, 10, 40, 40, 0, 90*64);  
            XFillArc(display, calc_window, gc, 10, 10, 40, 40, -180*64, 90*64);  

            XSetForeground(display, gc, WhitePixel(display, screen));
            XDrawArc(display, calc_window, gc, 10, 10, 40, 40, 0, 360*64);
            

            
        }
nonevent:
// x + width/2, y + height/2
        printf("%d\n", r);
        if (r < 40)++r;
        else r^=r;
        XFillArc(display, calc_window, gc, 10, 10, r, r, 0, 360*64);
        if (event.type == ClientMessage) 
            if ((long unsigned)event.xclient.data.l[0] == wm_delete)
                break;
    }
    /*
    if (!xinit())
        printf("Something went wrong\n");
    XSetLineAttributes(display, gc, 3, LineSolid, CapButt, JoinRound);
    XEvent event;
    //XTextItem textitems[] = {{"%", 1, 0, XLoadFont(display, "-misc-hack-medium-i-normal--0-0-0-0-m-0-ibm-cp850")}};
    XColor light_gray;
    XParseColor(display, colormap, "#C0C0C0", &light_gray);
    XAllocColor(display, colormap, &light_gray);

    XSetWindowAttributes attrs;
    attrs.event_mask = SubstructureRedirectMask 
                     | SubstructureNotifyMask
                     | StructureNotifyMask
                     | ExposureMask;
    attrs.background_pixel = xcolor.pixel;
    attrs.border_pixel = light_gray.pixel;
    attrs.do_not_propagate_mask = 0;
    unsigned long attrs_mask = CWEventMask 
                             | NoEventMask
                             | CWBackPixel
                             | CWBorderPixel;
    

    Window subwindow = XCreateWindow(display, calc_window,
                                     10, 70,
                                     45, 25,
                                     1, 
                                     CopyFromParent,
                                     InputOutput,
                                     CopyFromParent, attrs_mask, &attrs);
    XMapWindow(display, subwindow);
    //char** fonts = XListFonts(display, "*", 255, &n);
    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose || event.type == KeyPress) {
            XClearWindow(display, calc_window);
            XSetForeground(display, gc, light_gray.pixel);
            // Expression and sum display
            XSetLineAttributes(display, gc, 1, LineSolid, CapButt, JoinRound);
            XDrawRectangle(display, calc_window, gc, 10, 10, WIDTH-20, 50);
        }
        if (event.type == ClientMessage) {
            if ((long unsigned)event.xclient.data.l[0] == wm_delete) {
                break;
            }
        }
    }

    XCloseDisplay(display);
    */
    return 0;
}
