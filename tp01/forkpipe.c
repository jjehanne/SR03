#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define INPUT_BUFFER_SIZE 1000
#define OUTPUT_BUFFER_SIZE 20


int main(){

    int pipefd[2];
    pid_t pid1, pid2;
    char inputBuffer[INPUT_BUFFER_SIZE];
    char outputBuffer[OUTPUT_BUFFER_SIZE];

    int lenstr = 0;
    
    pipe(pipefd);

    pid1 = fork();

    switch(pid1){
        case(pid_t) 0 : 
            printf("fils1 \n");
            close(pipefd[0]); // fermeture de la sortiepour fils1
            FILE* input = fopen('./input.txt');
            while(fgets(line, 80, input) != NULL){
                print line;
                lenstr = strlen(line);
                //inputBuffer = sscanf("[%03d] [%s]", lenstr, line);

            }
            fclose(input);
            break;
        case(pid_t) -1 : 
            perror("erreur fork 1 \n");
            break;
        default : printf("pere \n");
            pid2 = fork();
            switch(pid2){
                case(pid_t) 0 : 
                    printf("fils2 \n");
                    close(pipefd[1]); // fermeture de lentree pour fils2
                    break;
                case(pid_t) -1 : 
                    perror("erreur fork 2 \n");
                    break;
                default : 
                    printf("pere \n");
                    close(pipefd[1]); //fermeture entree pour le pere
                    close(pipefd[0]); //fermeture sortie pour le pere

                    //waitpid(pid1, WUNTRACED);
                    //waitpid(pid2, WUNTRACED);

                
               }
    }

    return 0;
}
