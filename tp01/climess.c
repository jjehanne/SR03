#include <sys/types.h>
#include <stdio.h>
#include <iniobj.h>


int main(){
    key_t clef;
    int id_msg;
    msg message;

    clef = ftok("./sr03p008.txt", 0);
    id_msg = msgget(clef, 0); // on partage la meme file entre le client et le serveur
    if (id_msg == -1)
        perror("File inexistante");
    else{
        //initialisation du message
        message.type = 1; // 1 sera une question au serveur
        message.operation = 0; // 0 sera la demande de numero client

        msgsnd(id_msg, (void *) &message, long_message, 0); // long_message => longueur du message sans le type
        msgrcv(id_msg, (void *) &message, long_message, 2, 0); //2 represente la reponse au client
        numero_client = message.operation;

        message.type = 1; //question suivante
        message.operation = 1; //liste des produits
        
        message.numero_client = 1;
        msgsnd(id_msg, (void*) &message, long_message, 0);
        msgrcv(id_msg, (void *) &message, long_message, numero_client+100, 0);

        /* printf("%s", message.param); // afficher la liste des produits */

        message.type = 1;
        message.operation = 2; // demande de stock

        msgsnd(id_msg, (void *) &message, long_message, 0);
        msgrcv(id_msg, (void *) &message, long_message, numero_client+100, 0);

        /* printf("%s", message.param); // afficher le stock */

        /* if (numero_client == 0){
            printf("Sorry, maximum existing customer number reached, no more  allowed\n");
            exit(0);
        } 
        */

       	message.type = 1;
		message.operation = 3; // demande de fin de session

		msgsnd(id_msg, (void *) &message, long_message, 0);
		msgrcv(id_msg, (void *) &message, long_message, numero_client+100, 0);

		/* printf("Au revoir !");
		exit(0);*/

    }
    return 0;
}
