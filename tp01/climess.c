#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "iniobj.h"
#include "client_serveur.h"


int main(){
    key_t clef;
    int id_msg;
    int client_id;
    msg message;
    char choix;
    int choix_produit;
    int i;

    clef = ftok("./sr03p008.txt", 0);
    id_msg = msgget(clef, 0); // on partage la meme file entre le client et le serveur
    if (id_msg == -1){
        perror("File inexistante");
        return (EXIT_SUCCESS);
    }
    else{
        //initialisation du message
        message.type = 187; // 187 sera une question au serveur
        message.operation = '0'; // 0 sera la demande de numero client
        printf("Demande d'identifiant en cours...\n");

        msgsnd(id_msg, (void *) &message, sizeof(msg) - sizeof(long), 0); // long_message => longueur du message sans le type
        msgrcv(id_msg, (void *) &message, sizeof(msg) - sizeof(long), 200, 0); //200 represente la reponse au client

        if (message.numero_client == 400){
            printf("Sorry, max client number reached! Exiting...\n");
            return (EXIT_SUCCESS);
        }
        else if (message.numero_client == 500){
            printf("Sorry, maximum simultaneous client number reached!Try later...\n");
            return (EXIT_SUCCESS);
        }
        else{
            printf("Numero de client : %d\n", message.numero_client);
            client_id = message.numero_client;
        }
        while (choix != 'q'){
            printf("Client-Serveur\n a. Demande de la liste des produits\n z. Demande du stock\n q. Quitter\nQue souhaitez vous faire ?\n");
            scanf("%s", &choix);
            switch(choix){
                case('a'):
                    printf("\n\n\nDemande de la liste des produits en cours...\n");
                    message.type = 187; //question serveur
                    message.operation = 'a'; //liste des produits
                    message.numero_client = client_id;

                    msgsnd(id_msg, (void *) &message, sizeof(msg) - sizeof(long), 0);
                    msgrcv(id_msg, (void *) &message, sizeof(msg) - sizeof(long), client_id, 0);
                    printf("\n\nListe des produits:\n");
                    for (i=0; i< NB_MAX_TYPE_OBJ; i++){
                        printf("%d - %s\n", i+1, message.objet[i]); //affichage de 1 a 3 (more human readable)
                    }
                    printf("\n\n\n");
                    break;
                case('z'):
                    printf("\n\n\nDemande du stock et du prix\n");
                    printf("Pour quel produit souhaitez vous avoir le stock et le prix? \n\n\n");
                    scanf("%d", &choix_produit);
                    message.type = 187; //question serveur
                    message.operation = 'z'; //Stock
                    message.numero_client = client_id;
                    message.id_produit = choix_produit - 1; //Liste de 0 a 2, affichage de 1 a 3 donc on retranche 1

                    msgsnd(id_msg, (void *) &message, sizeof(msg) - sizeof(long), 0);
                    msgrcv(id_msg, (void *) &message, sizeof(msg) - sizeof(long), message.numero_client, 0);
                    printf("Stock et Prix pour le produit: %s\n", message.objet[0]);
                    printf("- Prix: %f euros\n- Stock: %d piece(s) disponible(s)\n", message.prix, message.stock);
                    break;
                case('q'):
                    message.type = 187; //question suivante
                    message.operation = 'q'; //Quitter

                    msgsnd(id_msg, (void *) &message, sizeof(msg) - sizeof(long), 0);
                    printf("Exiting. Bye !\n");
                    return (EXIT_SUCCESS);
            };
        };
    }
    return 0;
}
