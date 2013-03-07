#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "iniobj.h"


int main(){
    key_t clef;
    int id_msg;
    int numero_client;
    msg message;
    int choix;

    clef = ftok("./sr03p008.txt", 0);
    id_msg = msgget(clef, 0); // on partage la meme file entre le client et le serveur
    if (id_msg == -1){
        perror("File inexistante");
        return (EXIT_SUCCESS);
    }
    else{
        
        //initialisation du message
        message.type = 1; // 1 sera une question au serveur
        message.operation = 0; // 0 sera la demande de numero client
        printf("Demande d'identifiant en cours...\n");

        msgsnd(id_msg, (void *) &message, sizeof(message.operation), 0); // long_message => longueur du message sans le type
        msgrcv(id_msg, (void *) &message, sizeof(message.operation), 2, 0); //2 represente la reponse au client

        if (message.operation == 400){
            printf("Sorry, max client number reached! Exiting...\n");
            return (EXIT_SUCCESS);
        }
        else if (message.operation == 500){
            printf("Sorry, maximum simultaneous client number reached!Try later...\n");
            return (EXIT_SUCCESS);
        }
        else
            printf("Numero de client : %d\n", message.operation);
        while (choix != 3){
            printf("Client-Serveur\n 1. Demande de la liste des produits\n 2. Demande du stock\n 3. Quitter\nQue souhaitez vous faire ?");
            scanf("%d", &choix);
            switch(choix){
                case(1): 
                    message.type = 1; //question serveur
                    message.operation = 1; //liste des produits

                    msgsnd(id_msg, (void *) &message, sizeof(message.operation), 0); // long_message => longueur du message sans le type
                    msgrcv(id_msg, (void *) &message, 100, 2, 0);
                    printf("toto");

                case(2):
                    message.type = 1; //question serveur
                    message.operation = 2; //Stock

                    msgsnd(id_msg, (void *) &message, sizeof(message.operation), 0); // long_message => longueur du message sans le type
                    msgrcv(id_msg, (void *) &message, 100, 2, 0);

                case(3):
                    message.type = 1; //question suivante
                    message.operation = 3; //liste des produits

                    msgsnd(id_msg, (void *) &message, sizeof(message.operation), 0); // long_message => longueur du message sans le type
                    printf("Exiting. Bye !\n");
                    return (EXIT_SUCCESS); 
            };
        };
    }
    return 0;
}
