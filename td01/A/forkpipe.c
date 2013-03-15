#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

// Constants

const char* FILE_NAME = "input.txt";


// Functions declarations

// Create child process from parent and execute
// corresponding function.
int createChildren();

// Child 1 function, send data to pipe
void fileToPipe(int pipeWriteDesc);

// Child 2 function, get data from pipe.
void pipeToStdout(int pipeReadDesc);


// Main function

int main()
{
    return createChildren();
}

// Functions definition

int createChildren()
{
    pid_t pidChild1, pidChild2;
    int pipeText[2];

    /* Create the pipe. */
    if (pipe (pipeText)) {
        perror("Pipe failed.");
        return -1;
    }

    /* Create the child process. */
    pidChild1 = fork();
    if (pidChild1 < 0) {
        // Child 1 failed.
        perror("Fork child 1 failed.");
        return -2;
    } else if (pidChild1 == 0) {

        // Child 1 process
        close (pipeText[0]);
        fileToPipe(pipeText[1]);

    } else {
        pidChild2 = fork();
        if (pidChild2 < 0) {
            // Child 2 failed.
            kill(pidChild1, SIGKILL);
            perror("Fork child 2 failed.");
            return -3;

        } else if (pidChild2 == 0) {

            // Child 2 process
            close (pipeText[1]);
            pipeToStdout(pipeText[0]);

        } else {
            // Parent process

            close(pipeText[0]);
            close(pipeText[1]);

            if (waitpid(pidChild1, 0, 0) == -1) {
                kill(pidChild2, SIGKILL);
                perror("Wait child 1 failed");
                return -4;
            }

            if (waitpid(pidChild2, 0, 0) == -1) {
                perror("Wait child 2 failed");
                return -4;
            }

        }
    }

    return 0;
}

void fileToPipe(int pipeWriteDesc)
{
    FILE *fileP;
    FILE *pipeWriteP;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;


    fileP = fopen(FILE_NAME, "r");
    if (fileP == NULL) {
        perror("Failed to open file");
        exit(-5);
    }


    pipeWriteP = fdopen(pipeWriteDesc, "w");

    // Read each line from file and put into the pipe
    while ((read = getline(&line, &len, fileP)) != -1) {
        if (line[read - 1] == '\n') {
            read--;
        }
        fprintf(pipeWriteP, "[%03d][", (int)read);
        fwrite(line, 1, read, pipeWriteP);
        fprintf(pipeWriteP, "]");
    }
    if (line) {
        free(line);
    }

    fclose(fileP);
    fclose(pipeWriteP);
}

void pipeToStdout(int pipeReadDesc)
{
    FILE *pipeReadP;
    char buff[20];
    size_t buffLen = sizeof(buff) / sizeof(char);

    pipeReadP = fdopen(pipeReadDesc, "r");

    // Preious data length stored in buffer
    size_t prevBuffDataLen = 0;

    // Read until reach eof
    while (!feof(pipeReadP)) {

        // Data that can be read.
        size_t availableBuffLen = buffLen - prevBuffDataLen;
        char *buffNewData = buff + prevBuffDataLen;

        // Line length readed from pipe
        int lineLen = 0;

        // Total length of data in the buffer
        size_t currentDataLen = prevBuffDataLen;

        int nbSscanf;

        // Line start position
        size_t lineStartPos;
        // Line end position
        size_t lineEndPos;

        // Read new line len
        if (availableBuffLen > 0) {
            size_t readDataLen = fread(buffNewData,
                sizeof(char), availableBuffLen, pipeReadP);

            if (readDataLen == 0) {
                continue;
            }
            currentDataLen += readDataLen;
        }

        nbSscanf = sscanf(buff, "[%03d]", &lineLen);

        if (nbSscanf < 1) {
            perror("Error during reading file");
            exit(-5);
        }

        // Increment to consider final ] of line
        lineLen++;

        lineStartPos = 6;
        lineEndPos = lineStartPos;


        // Print data
        printf("reçu>>>");

        // While there is something to print
        while (lineLen > 0) {

            size_t dataLen = lineLen;
            size_t printedLen;
            lineEndPos = lineStartPos + lineLen;

            // If line length > current data length in buffer
            // then print all data in buffer
            if (lineEndPos > currentDataLen) {
                dataLen = currentDataLen - lineStartPos;
                printedLen = dataLen;
            // Otherwise, print the data from line except final ]
            } else {
                printedLen = dataLen - 1;
            }
            fwrite(buff+lineStartPos, sizeof(char), printedLen, stdout);

            // Decrement remaining line length to print.
            prevBuffDataLen = currentDataLen - dataLen - lineStartPos;
            lineLen -= dataLen;
            // If there is still something to print, 
            // read from pipe
            if (lineLen > 0) {
                currentDataLen = fread(buff, sizeof(char), buffLen, pipeReadP);
                lineStartPos = 0;
            }
        }
        printf("<<<\n");

        // move buffer to remove read line.
        memmove(buff, buff+lineEndPos, prevBuffDataLen);
    }

    fclose(pipeReadP);
}
