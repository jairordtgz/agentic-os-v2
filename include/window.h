#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>

Display *iniciarDisplay(void);
Window crearVentana(Display *display, Atom *wmDelete);
void procesarEventos(Display *display, Atom wmDelete, int socketServidor);
void cerrarVentana(Display *display, Window window);

#endif
