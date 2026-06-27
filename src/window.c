#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include "socket_utils.h"
#include <string.h>
#include "window.h"
#include <sys/socket.h>

Display *iniciarDisplay()
{
    Display *display = XOpenDisplay(NULL);

    if (display == NULL)
    {
        fprintf(stderr, "No se pudo abrir el servidor X11.\n");
        exit(EXIT_FAILURE);
    }

    return display;
}

Window crearVentana(Display *display)
{
    int screen = DefaultScreen(display);

    Window window = XCreateSimpleWindow(
        display,
        RootWindow(display, screen),
        10,
        10,
        400,
        200,
        1,
        BlackPixel(display, screen),
        WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask | KeyPressMask);

    XMapWindow(display, window);

    return window;
}

void procesarEventos(Display *display, Window window)
{
    XEvent event;
    int socketServidor;
    socketServidor = conectarServidor();

    while (1)
    {
        XNextEvent(display, &event);

        if(event.type == KeyPress)
	{
	    char texto[10];

	    KeySym keysym;

	    int longitud;

	    longitud = XLookupString(&event.xkey,
	                             texto,
	                             sizeof(texto),
	                             &keysym,
	                             NULL);

	    if(longitud > 0)
	    {
	        texto[longitud]='\0';

	        printf("%s",texto);

	        send(socketServidor,
	             texto,
	             strlen(texto),
	             0);
	    }

	    if(keysym==XK_Return)
	    {
	        send(socketServidor,"\n",1,0);
	    }

	    if(keysym==XK_Escape)
	    {
	        break;
	    }
	}
    }
}

void cerrarVentana(Display *display, Window window)
{
    XDestroyWindow(display, window);

    XCloseDisplay(display);
}

int main()
{
    Display *display = iniciarDisplay();

    Window window = crearVentana(display);

    procesarEventos(display, window);

    cerrarVentana(display, window);

    return 0;
}
