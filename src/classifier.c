#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "classifier.h"

Diccionario diccionarios[3]=
{
    {
        "Correo electronico",
        {
            "thank","please","regards","meeting","attached",
            "information","update","schedule","team","project"
        }
    },

    {
        "Articulo cientifico",
        {
            "data","analysis","results","method","study",
            "model","research","system","significant","effect"
        }
    },

    {
        "Reporte",
        {
            "system","data","network","security","application",
            "server","user","performance","service","infrastructure"
        }
    }
};

void convertirMinusculas(char texto[])
{
    for(int i=0;texto[i]!='\0';i++)
        texto[i]=tolower(texto[i]);
}

void construirVector(char texto[],
                     Diccionario dic,
                     int vector[])
{
    for(int i=0;i<PALABRAS;i++)
        vector[i]=0;

    char copia[1024];

    strcpy(copia,texto);

    convertirMinusculas(copia);

    char *palabra=strtok(copia," ,.;:\n\t");

    while(palabra!=NULL)
    {
        for(int i=0;i<PALABRAS;i++)
        {
            if(strcmp(palabra,dic.palabras[i])==0)
            {
                vector[i]++;
            }
        }

        palabra=strtok(NULL," ,.;:\n\t");
    }
}

int sumaVector(int vector[])
{
    int suma=0;

    for(int i=0;i<PALABRAS;i++)
        suma+=vector[i];

    return suma;
}

void imprimirVector(Diccionario dic,int vector[])
{
    printf("\nVector de frecuencias (%s)\n",dic.nombre);

    for(int i=0;i<PALABRAS;i++)
    {
        printf("%-15s : %d\n",
               dic.palabras[i],
               vector[i]);
    }
}

int clasificarDocumento(char texto[])
{
    int mejorTipo=DESCONOCIDO;

    int mayorFrecuencia=0;

    for(int i=0;i<3;i++)
    {
        int vector[PALABRAS];

        construirVector(texto,
                        diccionarios[i],
                        vector);

        imprimirVector(diccionarios[i],
                       vector);

        int frecuencia=sumaVector(vector);

        printf("Total=%d\n\n",
               frecuencia);

        if(frecuencia>=3 &&
           frecuencia>mayorFrecuencia)
        {
            mayorFrecuencia=frecuencia;
            mejorTipo=i;
        }
    }

    return mejorTipo;
}

const char *nombreClase(int tipo)
{
    if(tipo==DESCONOCIDO)
        return "Documento desconocido";

    return diccionarios[tipo].nombre;
}
