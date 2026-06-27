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

void *atenderCliente(void *arg)
{
    int cliente = *(int *)arg;

    free(arg);

    char buffer[BUFFER_SIZE];

    char oracion[BUFFER_SIZE];

    int indice = 0;

    printf("Nuevo hilo creado para cliente.\n");

    while(1)
    {
        int n = recv(cliente,
                     buffer,
                     sizeof(buffer)-1,
                     0);

        if(n<=0)
            break;

        buffer[n]='\0';

        for(int i=0;i<n;i++)
        {
            char c=buffer[i];

            if(c=='\n')
            {
                oracion[indice]='\0';

		printf("\n=============================\n");

		printf("Documento recibido:\n\n");

		printf("%s\n\n",oracion);

		int tipo=clasificarDocumento(oracion);

		printf("Clasificacion: %s\n",
		nombreClase(tipo));

		printf("=============================\n\n");

		indice=0;
            }
            else
            {
                if(indice<BUFFER_SIZE-1)
                {
                    oracion[indice++]=c;
                }
            }
        }
    }

    printf("Cliente desconectado.\n");

    close(cliente);

    pthread_exit(NULL);
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
