#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "launcher.h"
#include <unistd.h>

void ejecutarWindow()
{
    /* Para ejecutar en la raíz del proyecto */
    execl("./bin/window", "window", NULL);

    /* Para ejecutar dentro de bin */
    execl("./window", "window", NULL);

    perror("Error ejecutando window");
    exit(EXIT_FAILURE);
}

void crearVentanas(int cantidad)
{
    int i;

    for(i=0;i<cantidad;i++)
    {
        pid_t pid=fork();

        if(pid<0)
        {
            printf("Error al crear proceso.\n");
        }

        else if(pid==0)
        {
            ejecutarWindow();
        }

        else
        {
            printf("Launcher: ventana creada PID=%d\n",pid);
        }
    }

    for(i=0;i<cantidad;i++)
    {
        int status;
	pid_t pid;

	pid = wait(&status);

	printf("Launcher: proceso %d finalizado.\n", pid);
    }

    printf("\nTodas las ventanas finalizaron.\n");
}

void mostrarMenu()
{
    int opcion;
    int cantidad;

    do
    {
        printf("\n========== LAUNCHER ==========\n");
        printf("1. Crear ventanas\n");
        printf("0. Salir\n");
        printf("==============================\n");

        printf("Seleccione: ");
        scanf("%d",&opcion);

        switch(opcion)
        {
            case 1:

                printf("Cantidad de ventanas: ");
                scanf("%d",&cantidad);

                if(cantidad>0)
                    crearVentanas(cantidad);

                break;

            case 0:

                printf("Adios.\n");

                break;

            default:

                printf("Opcion invalida.\n");

        }

    }while(opcion!=0);
}

int main()
{
    mostrarMenu();

    return 0;
}
