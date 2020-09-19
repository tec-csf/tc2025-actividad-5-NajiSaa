//
//  main.c
//  Actividad 5 TokenRing
//
//  Created by MacBook Pro on 16/09/20.
//  Copyright © 2020 Naji M A Saadat. All rights reserved.
//  Basado en factorial_pipe

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *
         const * argv) {
    int option = 0 , ChildrenProNum = 0, i = 1;
    int pipes[2*ChildrenProNum];
    double size= sizeof(char);
    char * NChildrenOpt; char Witness = 'T' ; char WitnessChar = '\0';
    pid_t pid = 0;
    pid_t ppid = 0;
    
    while ((option = getopt(argc, argv, "n:")) != -1)
        switch (option) {
            case 'n':
                NChildrenOpt = optarg;
                if (atoi(NChildrenOpt) <= 1) {
                    printf("\n \t \t  **Invalid Input**\n");
                    printf("**Try Again! Must be a number higher than 1 ** \n\n");
                    return 0;
                } else {
                    printf("\n \t \t  **Valid Input**\n");
                    ChildrenProNum = atoi(NChildrenOpt);
                }
                break;
        }
    for (int i = 0; i < ChildrenProNum; i++) {
        // two pipes spaces per Child
        pipe(&pipes[2*i]);
    }
    ppid=getpid();
    while (i <= ChildrenProNum)
    {
        pid = fork();
        pid_t ppidParent = getppid();
        if (pid == 0)
        {
            close(pipes[i*2 - 1]);
            read(pipes[i*2 - 2], &WitnessChar, size);
            printf("\n\t********Recibiendo Mensaje del Proceso con PPID: %d ********\n", ppidParent);
            printf("—->Soy el proceso con PID * %d * y recibí el testigo * '%c' * , el cual tendré por 5 segundos \t**Recibido**\n", getpid(), WitnessChar);
            sleep(1);
            printf("\n\t\t********Enviando******** \n");
            sleep(4);
            close(pipes[i*2]);
            write(pipes[i*2], &WitnessChar, size);
            printf("<—-Soy el proceso con PID * %d * y acabo de enviar el testigo * '%c'  * \t**Enviado**\n\n",  getpid(), WitnessChar);
            ppidParent = getppid();
            if ( i == ChildrenProNum)
            {
                close(pipes[i*2 - 1]);
                read(pipes[i*2 - 2], &WitnessChar, size);
                printf("\n\t\t****Proceso padre*****\n" );
                printf("\n\t********Recibiendo Mensaje del Proceso con PPID: %d ********\n", getpid());
                printf("\n------>****Proceso Padre con PPID: %d, Recibi: '%c' ****** \t**Recibido** \n\n",  ppid,  Witness);
                break;
            }
        }
        else if (pid == -1)
        {
            printf("Failed Child Creation Process \n" );
            exit(-1);
        }
        else
        {   // Father Process
            if (i == 1)
            {
                // Estoy en el padre
                close(pipes[0]);
                write(pipes[1], &Witness, size);
                printf("\n\t\t****Proceso padre*****\n" );
                printf("\n****Proceso Padre con PPID: %d, Enviando: '%c' a Proceso Hijo ******\n\n", getpid(), Witness);
            }
            break;
        }
        i++;
    }
    wait(0);
    return 0;
}
