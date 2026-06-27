#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <pthread.h>

#include "ialearner.h"
#include "classifier.h"
#define PORT 5000
#define BUFFER_SIZE 1024

int documentosCorreo=0;
int documentosArticulo=0;
int documentosReporte=0;

int clientesConectados=0;

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;

void *atenderCliente(void *arg)
{
    int cliente = *(int *)arg;
    free(arg);

    char c;
    char oracion[BUFFER_SIZE];
    int indice = 0;

    printf("Nuevo hilo creado para cliente.\n");

    while (recv(cliente, &c, 1, 0) > 0)
    {
        if (c == '\n')
        {
            oracion[indice] = '\0';

            printf("\n=============================\n");
            printf("Documento recibido:\n\n");
            printf("%s\n\n", oracion);

            int tipo = clasificarDocumento(oracion);
	    actualizarResumen(tipo);
            printf("Clasificacion: %s\n",
                   nombreClase(tipo));

            printf("=============================\n\n");

            indice = 0;
        }
        else
        {
            if (indice < BUFFER_SIZE - 1)
            {
                oracion[indice++] = c;
            }
        }
    }

    printf("Cliente desconectado.\n");
    pthread_mutex_lock(&mutex);
    clientesConectados--;

    if(clientesConectados==0)
	{
	    mostrarResumenFinal();
	}

    pthread_mutex_unlock(&mutex);
    close(cliente);

    return NULL;
}

void actualizarResumen(int tipo)
{
    pthread_mutex_lock(&mutex);

    if(tipo==EMAIL)
        documentosCorreo++;

    else if(tipo==ARTICULO)
        documentosArticulo++;

    else if(tipo==REPORTE)
        documentosReporte++;

    pthread_mutex_unlock(&mutex);
}

void mostrarResumenFinal()
{
    printf("\n=============================\n");

    printf("RESUMEN FINAL\n\n");

    printf("Correo electronico : %d\n",
           documentosCorreo);

    printf("Articulo cientifico: %d\n",
           documentosArticulo);

    printf("Reporte            : %d\n\n",
           documentosReporte);

    if(documentosCorreo>0 &&
       documentosArticulo==0 &&
       documentosReporte==0)
    {
        printf("Tipo de usuario: Personal administrativo\n");
    }

    else if(documentosCorreo>0 &&
            documentosReporte>0 &&
            documentosArticulo==0)
    {
        printf("Tipo de usuario: Personal tecnico\n");
    }

    else if(documentosCorreo>0 &&
            documentosArticulo>0 &&
            documentosReporte==0)
    {
        printf("Tipo de usuario: Profesor\n");
    }

    else if(documentosArticulo>0 &&
            documentosReporte>0 &&
            documentosCorreo==0)
    {
        printf("Tipo de usuario: Estudiante\n");
    }

    else
    {
        printf("Tipo de usuario: No determinado\n");
    }

    printf("=============================\n");
}

int main()
{
    int servidor;

    struct sockaddr_in direccion;

    servidor=socket(AF_INET,
                    SOCK_STREAM,
                    0);

    direccion.sin_family=AF_INET;
    direccion.sin_port=htons(PORT);
    direccion.sin_addr.s_addr=INADDR_ANY;

    bind(servidor,
         (struct sockaddr*)&direccion,
         sizeof(direccion));

    listen(servidor,5);

    printf("IA Learner iniciado.\n");

    while(1)
    {
        int *cliente=malloc(sizeof(int));

        *cliente=accept(servidor,
                        NULL,
                        NULL);

        if(*cliente<0)
        {
            free(cliente);
            continue;
        }

        printf("Nueva conexion.\n");
	pthread_mutex_lock(&mutex);

	clientesConectados++;

	pthread_mutex_unlock(&mutex);

        pthread_t hilo;

        pthread_create(&hilo,
                       NULL,
                       atenderCliente,
                       cliente);

        pthread_detach(hilo);
    }

    close(servidor);

    return 0;
}
