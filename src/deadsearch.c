#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <X11/Xlib.h>
#ifndef XINERAMA
#include <X11/extensions/Xinerama.h>
#endif

#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#define INTERSECT(x,y,w,h,r)  (MAX(0, MIN((x)+(w),(r).x_org+(r).width)  - MAX((x),(r).x_org)) \
                             * MAX(0, MIN((y)+(h),(r).y_org+(r).height) - MAX((y),(r).y_org)))

// User defined variables
int bar_location = 0; // 0=top;1=bottom;2=center
int bar_output_numeric = 0;  // 0=false;1=true
int bar_width = 100; // By default, it uses 100% of the width of your monitor
int bar_height = 25; // By default, it is set to 20 pixels
int bar_x = 0;
int bar_y = 0;
int bar_background_color = 0x4a4a4a;
int bar_border_width = 0;
int bar_border_color = 0x000000;

// X11 sensitive variables
Display *display;
int default_screen;
Window bar_window;
Window root_window;
XSetWindowAttributes window_attributes;
XEvent event;

void show_help(void) {
    printf("\
ds - deadsearch\
\n\
\nUsage: ds [arguments] -       read text from stdin (splitted by \\n characters)\
    \n");
}

void debug(const char *msg) {
    printf("0xdeadsearch debug: %s\n", msg);
}

// Get the top-level window containing current input focus.
int get_focused_window(XWindowAttributes *focused_attributes) {
    Window focused_window;
    int revert_to_return;

    XGetInputFocus(display, &focused_window, &revert_to_return);

    Window root;
    Window parent_window;
    Window *window_children;
    unsigned int children_number;

    do {
        if (XQueryTree(display, (parent_window = focused_window), &root, &focused_window, &window_children, &children_number) && window_children) {
            XFree(window_children);
        }
    } while (focused_window != root && focused_window != parent_window);

    if (!XGetWindowAttributes(display, parent_window, focused_attributes)) {
        return 1;
    }

    return 0;
}

int setup_menu(void) {
    display = XOpenDisplay(0);
    if (display == NULL) {
        debug("Unable to open display");
        return 1;
    }

    default_screen = DefaultScreen(display);
    root_window = RootWindow(display, default_screen);

    if (bar_width < 0 || bar_height < 0) {
        debug("Bar width or height are under 0");
        return 1;
    }

    XWindowAttributes root_attributes;
    if (!XGetWindowAttributes(display, root_window, &root_attributes)) {
        debug("Can't get window attributes of root window");
        return 1;
    }

#ifndef XINERAMA
    int screen_number;
    int chosen_screen_area;
    int chosen_screen;
    XineramaScreenInfo *screen_info;

    XWindowAttributes focused_window_attributes;
    if (get_focused_window(&focused_window_attributes) != 0) {
        debug("Can't get window attributes of focused top-level window");
        return 1;
    }

    if ((screen_info = XineramaQueryScreens(display, &screen_number))) {
        int screen;
        for (screen = 0; screen < screen_number; screen++) {
            int current_screen_area;
            if ((current_screen_area = INTERSECT(focused_window_attributes.x, focused_window_attributes.y, focused_window_attributes.width, focused_window_attributes.height, screen_info[screen])) > chosen_screen_area) {
                chosen_screen_area = current_screen_area;
                chosen_screen = screen;
            }
        }

        bar_x = screen_info[chosen_screen].x_org;
        bar_y = screen_info[chosen_screen].y_org;
        bar_width = screen_info[chosen_screen].width;
    } else {
#endif
        bar_x = root_attributes.x;
        bar_y = root_attributes.y;
        bar_width = root_attributes.width;
    }

    window_attributes.override_redirect = True;
    window_attributes.background_pixel = bar_background_color;
    window_attributes.border_pixel = bar_border_color;

    bar_window = XCreateWindow(display, root_window, bar_x, bar_y, bar_width, bar_height, bar_border_width,
                                      CopyFromParent, CopyFromParent, CopyFromParent, CWOverrideRedirect|CWBackPixel|CWBorderPixel, &window_attributes);


    return 0;
}

int main(int argc, char **argv) {
    int option;
    int ret;

    while ((option = getopt(argc, argv, "hnl::")) != -1) {
        switch (option) {
            case 'n':
                bar_output_numeric = 1;
                break;
            case 'l':{
                int location = atoi(optarg);
                if (location >= 0 && location <= 2)
                    bar_location = location;
                break;
            }

            case 'h':
            default:
                show_help();
                return 1;
                break;
        }
    }

    ret = setup_menu();
    if (ret != 0) {
        printf("Menu setup failed!");
        return 1;
    }

    XMapWindow(display, bar_window);

    for (;;) {
        XNextEvent(display, &event);

    }

    XDestroyWindow(display, bar_window);
    XCloseDisplay(display);

    return 0;
}
