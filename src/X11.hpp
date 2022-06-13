#pragma once
#include <stdio.h>
#include <stdlib.h>

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<X11/Xatom.h>
#include<X11/cursorfont.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

// https://stackoverflow.com/questions/25191715/unwanted-flickering-rubberband-while-dragging-mouse-on-screen-with-xdrawrectangl
// https://stackoverflow.com/questions/57078155/draw-border-frame-using-xlib

#define SET_RECT(event)            \
    x = event->x_root;             \
    y = event->y_root;             \
    sel.w = x - rootx;             \
    if (sel.w < 0) sel.w = -sel.w; \
    sel.h = y - rooty;             \
    if (sel.h < 0) sel.h = -sel.h; \
    sel.x = x < rootx ? x : rootx; \
    sel.y = y < rooty ? y : rooty

namespace HamburgerOCR
{

Display* dpy;
XVisualInfo vi;
Window root = 0, win = 0;
GC gc;
Cursor cursor1, cursor2;
XEvent event;
XButtonEvent* button = &(event.xbutton);
XMotionEvent* motion = &(event.xmotion);

struct Box
{
        int x = 0, y = 0, w = 0, h = 0;
	
	void capture(const char* path)
	{
		XImage* image = XGetImage(dpy, root, x, y, w, h, AllPlanes, ZPixmap);
		int index = 0;
		char channels = 3;
		int n = w * h * channels;
		unsigned long r_mask = image->red_mask;
		unsigned long g_mask = image->green_mask;
		unsigned long b_mask = image->blue_mask;
		unsigned char* buffer = (unsigned char*)malloc(n * sizeof(unsigned char));
	
		int r, g, b;	
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				unsigned long pix = XGetPixel(image, i, j);
				b = pix & b_mask;
				g = (pix & g_mask) >> 8;
				r = (pix & r_mask) >> 16;

				buffer[index++] = r;
				buffer[index++] = g;
				buffer[index++] = b;
			}
		}

		stbi_write_png(path, w, h, channels, buffer, w * channels);
		free(buffer);
	}	
};

void Xinit()
{
	dpy = XOpenDisplay(NULL);
	if(dpy == NULL) 
	{
		fprintf(stderr, "Failed to connect to X server \n");
		exit(-1);
	}

	int n = DefaultScreen(dpy);
	root = DefaultRootWindow(dpy);
	XMatchVisualInfo(dpy, n, 32, TrueColor, &vi);

	XSetWindowAttributes swa = {0};
	swa.colormap = XCreateColormap(dpy, root, vi.visual, AllocNone);
	XColor color;
    	char magenta[] = "#FF00FF";
    	XParseColor(dpy, swa.colormap, magenta, &color);
    	XAllocColor(dpy, swa.colormap, &color);
	
	swa.border_pixel = 0;
    	swa.background_pixel = 0;    	
	swa.override_redirect = 1;
	int w = DisplayWidth(dpy, n);
  	int h = DisplayHeight(dpy, n);
	int mask = CWColormap | CWBorderPixel | CWBackPixel | CWOverrideRedirect;
	win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi.depth, InputOutput, 
			vi.visual, mask, &swa);

	Atom state = XInternAtom(dpy, "_NET_WM_STATE", True);
        Atom skip  = XInternAtom(dpy, "_NET_WM_STATE_SKIP_TASKBAR", True);
	Atom del   = XInternAtom(dpy, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(dpy, win, &del, 1);
	XChangeProperty(dpy, win, state, XA_ATOM, 32,
			PropModeReplace, (unsigned char*)&skip, 1);

	XStoreName(dpy, win, "transparent");
	XSelectInput(dpy, win, StructureNotifyMask| ButtonPressMask| 
			PointerMotionMask| LeaveWindowMask| 
			EnterWindowMask| ButtonReleaseMask);

	XGCValues gcv = {0};
	gcv.foreground = color.pixel;
	gcv.function = GXxor;
	gcv.subwindow_mode = IncludeInferiors;
	gc = XCreateGC(dpy, win, GCFunction | GCForeground | GCSubwindowMode, &gcv);

	// cursors
	cursor1 = XCreateFontCursor(dpy, XC_crosshair);
	cursor2 = XCreateFontCursor(dpy, XC_hand1);
}

void Xdinit()
{
	XFreeCursor(dpy, cursor1);
    	XFreeCursor(dpy, cursor2);
	XFreeGC(dpy, gc);
	XCloseDisplay(dpy);
}

Box makeSelection()
{
	XMapWindow(dpy, win);

	int success = XGrabPointer(dpy, root, False, ButtonPressMask, 
			GrabModeAsync, GrabModeAsync, root, cursor1, CurrentTime);

	if(success != GrabSuccess) 
		fprintf(stderr, "Failed to grab the mouse \n");
	
	HamburgerOCR::Box sel;
	int x = 0, y = 0, rootx = 0, rooty = 0;

	// button press event
	XMaskEvent(dpy, ButtonPressMask, &event);
	rootx = sel.x = button->x_root;
    	rooty = sel.y = button->y_root;
	
	// motion and release events
	XChangeActivePointerGrab(dpy, ButtonMotionMask | ButtonReleaseMask, cursor2, CurrentTime);

	// rubberband on the transparent window	
	bool done = false;	
	while(!done)
	{
		XNextEvent(dpy, &event);
		
		switch(event.type)
		{
			case MotionNotify:

				if (sel.w || sel.h) 
					 XDrawRectangle(dpy, win, gc, sel.x, sel.y, sel.w, sel.h);
				
				SET_RECT(motion);
				XDrawRectangle(dpy, win, gc, sel.x, sel.y, sel.w, sel.h);
				XFlush(dpy);
				break;

			case ButtonRelease:
				if (sel.w || sel.h) 
					 XDrawRectangle(dpy, win, gc, sel.x, sel.y, sel.w, sel.h);

				SET_RECT(button);
				XFlush(dpy);
				done = true;
				break;
			
			default:  break;
		}
	}

	XUngrabPointer(dpy, CurrentTime);
	XUnmapWindow(dpy, win);
	return sel;	
}

}
