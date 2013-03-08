#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "iniobj.h"
#include "client_serveur.h"

#define NB_MAX_CLT 4
#define NB_MAX_CLT_SIM 2


int main(){
    key_t clef;
    int id_msg;
    msg message;
    int i;
    int num_client = 0;
    int tot_clt = 0;
    int sim_clt = 0;

    produit* liste = iniobj();
    int id_produit;

    clef = ftok("./sr03p008.txt", 0);
    id_msg = msgget(clef, IPC_CREAT|IPC_EXCL|0666); // on cree la file pour éviter d'avoir déjà quelque chose dedans
    if (id_msg == -1)
        perror("File deja existante");
    else{
        printf("Serveur actif. En attente de requetes...\n");
        while(1){
        //lecture de la file tant que le 4eme et dernier client demande des reponse
        msgrcv(id_msg, (void *) &message, sizeof(msg) - sizeof(long), 187, 0); 
        // long_message => longueur du message sans le type, 187 vient du client, cest une question au serveur
                if(message.operation == '0'){

                    printf("Demande d'identifiant recue, traitement en cours...\n");
                    // unique client number si plus de NB_MAX_CLT client renvoyer un code pour refuser lacces
                    message.type = 2;
                    if (num_client < NB_MAX_CLT && sim_clt < NB_MAX_CLT_SIM){
                        num_client++;
                        tot_clt++;
                        sim_clt++;
                        message.numero_client = num_client ; //identifiant du client 
                        printf("Ajout client # %d: \nsimultane: %d \ntotal: %d\n",message.numero_client, sim_clt, tot_clt);
                    }
                    else if (tot_clt >= NB_MAX_CLT){
                        printf ("ID attribution denied: Maximum client number reached\n");
                        message.numero_client = 400;
                    }
                    else if (sim_clt >= NB_MAX_CLT_SIM){
                        printf ("ID attribution denied: Maximum simultaneous client number reached\n");
                        message.numero_client = 500;
                    }
                    msgsnd(id_msg, (void*) &message, sizeof(msg) - sizeof(long), 0);

                }

                else if (message.operation == 'a'){
                    printf("Demande de la liste des produits recue, envoi en cours...\n");
                    message.type = message.numero_client;

                    for (i=0; i < NB_MAX_TYPE_OBJ; i++){
                        strcpy(message.objet[i], liste[i].nom);
                    }

                    msgsnd(id_msg, (void *) &message, sizeof(msg) - sizeof(long), 0);
                }

                else if (message.operation == 'z'){
                    message.type = message.numero_client;
                    id_produit = message.id_produit;

                    printf("Demande du stock et du prix pour l'article \"%s\" recue, envoi en cours...\n", liste[id_produit].nom);

                    strcpy(message.objet[0], liste[id_produit].nom);
                    message.prix = liste[id_produit].prix;
                    message.stock = liste[id_produit].stock;

                    msgsnd(id_msg, (void *) &message, sizeof(msg) - sizeof(long), 0);
                }

                else if(message.operation == 'q'){
                    sim_clt--;
                    printf("Suppression client :\n simultane: %d \ntotal: %d\n", sim_clt, tot_clt);
                } 

                if (tot_clt == NB_MAX_CLT && sim_clt == 0){
                    printf("Server handled all allowed clients\n");
                    printf("Server shutting down...\n");
                    msgctl(id_msg, IPC_RMID, NULL); // removing queue
                    return (EXIT_SUCCESS);
                }


        };
    }

    return 0;
}
