#include "log.h"


int writeLog(double instant, pid_t pid, action_type action, struct info info){
    int fd;
    char* directory;
    char string_to_write[100];

    directory = getDirectoryEnv();

    if( (fd = open(directory, O_WRONLY | O_CREAT | O_APPEND, 0666)) == -1){ // Mudar o .txt para o sítio apropriado
        perror("Failed to open the directory\n");
        return -1;
    }

    // Construction of the string to write

    char* action_string = action_type_string[action];

    switch (action) {
        case CREATE: {
            char string_arg[100];
            string_arg[0]='\0';

            for (size_t i = 0; i < info.argv_size; i++) {
                strcat(string_arg, info.argv[i]);
                strcat(string_arg, " ");
            }

            snprintf(string_to_write, sizeof(string_to_write), "%.2f - %d - %s - %s\n", instant, (int)pid, action_string, string_arg);
            break;
        }
        case EXIT: {
            snprintf(string_to_write, sizeof(string_to_write), "%.2f - %d - %s - %d", instant, (int)pid, action_string, info.exit_code);
            break;
        }
        case RECV_SIGNAL: {
            snprintf(string_to_write, sizeof(string_to_write), "%.2f - %d - %s - %s", instant, (int)pid, action_string, info.received_signal);
            break;
        }
        case SEND_SIGNAL: {
            snprintf(string_to_write, sizeof(string_to_write), "%.2f - %d - %s - %s", instant, (int)pid, action_string, info.sent_signal);
            break;
        }
        case RECV_PIPE: {
            snprintf(string_to_write, sizeof(string_to_write), "%.2f - %d - %s - %s", instant, (int)pid, action_string, info.received_from_pipe);
            break;
        }
        case SEND_PIPE: {
            snprintf(string_to_write, sizeof(string_to_write), "%.2f - %d - %s - %s", instant, (int)pid, action_string, info.sent_from_pipe);
            break;
        }
        case ENTRY: {
            snprintf(string_to_write, sizeof(string_to_write), "%.2f - %d - %s - %s", instant, (int)pid, action_string, info.entry);
            break;
        }
        default:
            break;
    }

    if(write(fd, string_to_write, strlen(string_to_write)) == -1){
        perror("Failed to write\n");
        return -1;
    }

    return 0;

}

void resetLog(){
    char* directory = getDirectoryEnv();
    FILE * temp;
    temp = fopen(directory, "w");
    fclose(temp);
}

char* getDirectoryEnv(){
    char* directory = getenv("LOG_FILENAME");
    if(directory == NULL)
        directory = "log.txt";

    return directory;
}

void loadArgv(struct info *info, char* argv[], int argc){
    for (size_t i = 0; i < argc; i++) {
        info->argv[i] = argv[i];
    }

    info->argv_size = argc;
}
