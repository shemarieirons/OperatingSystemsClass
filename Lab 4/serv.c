#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <limits.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "list.h"
#include <unistd.h>

#define PORT 9001
#define ACK "ACK"

static int server_fd = -1;
static int client_fd = -1;
static list_t *active_list = NULL;

void close_all(int code) {
    if (client_fd != -1) close(client_fd);
    if (server_fd != -1) close(server_fd);
    if (active_list) { list_free(active_list); active_list = NULL; }
    exit(code);
}

void handle_sigint(int sig) {
    (void)sig;
    printf("\nServer shutting down...\n");
    close_all(0);
}

int main(void) {
    signal(SIGINT, handle_sigint);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("bind");
        close_all(1);
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        close_all(1);
    }

    printf("Listening on port %d...\n", PORT);

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("accept");
        close_all(1);
    }

    printf("Client connected.\n");

    active_list = list_alloc();
    char buf[1024];
    char sbuf[1024];

    while (1) {
        memset(buf, 0, sizeof(buf));
        int n = recv(client_fd, buf, sizeof(buf) - 1, 0);
        if (n <= 0) {
            if (n < 0) perror("recv");
            break;
        }

        buf[n] = '\0';
        char *cmd = strtok(buf, " ");
        if (!cmd) continue;

        sbuf[0] = '\0';

        if (strcmp(cmd, "exit") == 0) {
            send(client_fd, "Goodbye\n", 8, 0);
            close_all(0);
        }
        else if (strcmp(cmd, "print") == 0) {
            snprintf(sbuf, sizeof(sbuf), "%s", listToString(active_list));
        }
        else if (strcmp(cmd, "get_length") == 0) {
            int len = list_length(active_list);
            snprintf(sbuf, sizeof(sbuf), "Length = %d", len);
        }
        else if (strcmp(cmd, "add_back") == 0) {
            char *v = strtok(NULL, " ");
            if (!v) snprintf(sbuf, sizeof(sbuf), "ERR missing value");
            else {
                int val = atoi(v);
                list_add_to_back(active_list, val);
                snprintf(sbuf, sizeof(sbuf), "%s %d", ACK, val);
            }
        }
        else if (strcmp(cmd, "add_front") == 0) {
            char *v = strtok(NULL, " ");
            if (!v) snprintf(sbuf, sizeof(sbuf), "ERR missing value");
            else {
                int val = atoi(v);
                list_add_to_front(active_list, val);
                snprintf(sbuf, sizeof(sbuf), "%s %d", ACK, val);
            }
        }
        else if (strcmp(cmd, "add_position") == 0) {
            char *i = strtok(NULL, " ");
            char *v = strtok(NULL, " ");
            if (!i || !v) snprintf(sbuf, sizeof(sbuf), "ERR missing args");
            else {
                int idx = atoi(i), val = atoi(v);
                list_add_at_index(active_list, val, idx);
                snprintf(sbuf, sizeof(sbuf), "%s%d@%d", ACK, val, idx);
            }
        }
        else if (strcmp(cmd, "remove_back") == 0) {
            int r = list_remove_from_back(active_list);
            if (r == INT_MIN) snprintf(sbuf, sizeof(sbuf), "ERR empty list");
            else snprintf(sbuf, sizeof(sbuf), "%d", r);
        }
        else if (strcmp(cmd, "remove_front") == 0) {
            int r = list_remove_from_front(active_list);
            if (r == INT_MIN) snprintf(sbuf, sizeof(sbuf), "ERR empty list");
            else snprintf(sbuf, sizeof(sbuf), "%d", r);
        }
        else if (strcmp(cmd, "remove_position") == 0) {
            char *i = strtok(NULL, " ");
            if (!i) snprintf(sbuf, sizeof(sbuf), "ERR missing index");
            else {
                int idx = atoi(i);
                int r = list_remove_at_index(active_list, idx);
                if (r == INT_MIN) snprintf(sbuf, sizeof(sbuf), "ERR invalid index");
                else snprintf(sbuf, sizeof(sbuf), "%d", r);
            }
        }
        else if (strcmp(cmd, "get") == 0) {
            char *i = strtok(NULL, " ");
            if (!i) snprintf(sbuf, sizeof(sbuf), "ERR missing index");
            else {
                int idx = atoi(i);
                int r = list_get_elem_at(active_list, idx);
                if (r == INT_MIN) snprintf(sbuf, sizeof(sbuf), "ERR invalid index");
                else snprintf(sbuf, sizeof(sbuf), "%d", r);
            }
        }
        else {
            snprintf(sbuf, sizeof(sbuf), "ERR unknown command");
        }

        send(client_fd, sbuf, strlen(sbuf), 0);
    }

    close_all(0);
    return 0;
}
