#include <sys/types.h>
#include <stdio.h>
#include <iniobj.h>
#define client_max 2


int main(){
    key_t clef;
    int id_msg;

    msg message;

    clef = ftok("./sr03p008.txt", 0);
    id_msg = msgget(clef, IPC_CREAT|IPC_EXCL|0666); // on cree la file pour éviter qu'il y a déjà quelque chose dedans
    if (id_msg == -1)
        perror("File deja existante");
    else{
        //lecture de la file tant que le 4eme et dernier client demande des reponse
        msgrcv(id_msg, (void *) &message, long_message, 1, 0); 
        // long_message => longueur du message sans le type, truc cf type dans climess.c, 1 vient du client, cest une question au serveur
        switch (message.operation){
            case(0):     //demande d'identifiant
                // unique_client_number(); si plus de 4 client renvoyer 0 pour refuser lacces
                message.type = 2;
                message.operation = 1; //identifiant du client 
                msgsnd(id_msg, (void*) &message, long_message, 0);

            case(1):    //demande liste produits + dispo

                // la reponse est a adressée a un client en particulier donc type depend du num_client
                message.type = message.numero_client + 100 ;
                message.param = liste_produit;
                msgsnd(id_msg, (void*) &message, long_message,0);

            case(2):    // Demande stock et prix
                message.type = message.numero_client + 100 ;
                message.param = stock;
                msgsnd(id_msg, (void*) &message, long_message,0);

            case(3):  // Demande de fin de session

                message.type = message.numero_client + 100 ;
                msgsnd(id_msg, (void*) &message, long_message,0);
                
       
    }

    /*int unique_client_number(){
        si plus de client_max numeros attribués , renvoyer 0
        sinon attribuer un numero unique
        
    }

    int server_shutdown(){
        recuperer nombre de numero attribué et le mettre dans total_count
        pour chaque connexion, ajouté 1 a clients_co
        pour chaque deconnexion enlever 1 a client_co
        if (total_count == client_max && client_co == 0){
            printf("The %d allowed customers have been handled and all of them left !"\n Bye !! \n, client_max);
            exit(0);
        }


        

        
    }
    return 0;
}
