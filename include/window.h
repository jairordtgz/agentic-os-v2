#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>

Display *iniciarDisplay();
Window crearVentana(Display *display);
void procesarEventos(Display *display, Window window);
void cerrarVentana(Display *display, Window window);

#endif
