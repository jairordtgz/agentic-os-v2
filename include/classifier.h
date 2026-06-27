#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#define EMAIL 0
#define ARTICULO 1
#define REPORTE 2
#define DESCONOCIDO -1

#define PALABRAS 10

typedef struct
{
    char *nombre;
    char *palabras[PALABRAS];

} Diccionario;

int clasificarDocumento(char texto[]);
const char *nombreClase(int tipo);

#endif
