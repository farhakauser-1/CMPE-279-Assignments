// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
int main(int argc, char const *argv[])
{

    // int server_fd, new_socket, valread;
    // struct sockaddr_in address;
    // int opt = 1;
    // int addrlen = sizeof(address);
    // char buffer[1024] = {0};
    // char *hello = "Hello from server";
    if (argc == 1)
    {
        int server_fd, new_socket;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // Forcefully attaching socket to the port 8080
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to the port 8080
        if (bind(server_fd, (struct sockaddr *)&address,
                 sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                                 (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("Parent process uid - ");
        printf("%d\n", getuid());

        if (fork() == 0)
        {

            //only child executes in this block

            int res = setuid(65534);
            printf("Child uid  ");
            printf("%d\n", getuid());
            if (res < 0)
            {
                perror("Dropping privileges- Failed");
                exit(EXIT_FAILURE);
            }
            else
            {
                char socket_fd[12];
                snprintf(socket_fd, 2, "%d", new_socket);
                char *av[3];
                av[0] = "create child";
                av[1] = socket_fd;
                av[2] = NULL;
                printf("till here");
                int exec = execvp(argv[0], av);
                //handle execvp return value
                if (exec < 0)
                {
                    perror("execvp failed");
                    exit(EXIT_FAILURE);
                }
            }

            return 0;
        }
        else
        {
            printf("Parent process waiting for child process to complete\n");
            wait(NULL);
            printf("Parent process exits\n");
            return 0;
        }
    }

    else
    {
        int valread;
        char buffer[1024] = {0};
        char *hello = "Hello from server";
        printf("This is Child process created using execvp\n");
        int i;
        sscanf(argv[1], "%d", &i);
        valread = read(i, buffer, 1024);
        printf("Received : %s\n", buffer);
        send(i, hello, strlen(hello), 0);
        printf("Sent :Hello message sent\n");
        exit(1);

        return 0;
    }
}
