#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

char *sock_path = "./socket";

void sig_handle(int signo) {
    unlink(sock_path);
    _exit(0);
}

int main() {
    struct sockaddr_un srv_addr;
    int srv_sock, clt_sock;
    ssize_t bytes_size;

    signal(SIGINT, sig_handle);

    srv_sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if (srv_sock == -1) {
        perror("Failed to execute socket.");
        return -1;
    }

    memset(&srv_addr, 0, sizeof(srv_addr));
    srv_addr.sun_family = AF_UNIX;
    strncpy(srv_addr.sun_path, sock_path, sizeof(srv_addr.sun_path) - 1);

    if (bind(srv_sock, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) == -1) {
        perror("Failed to execute bind.");
        return -1;
    }

    if (listen(srv_sock, 1) == -1) {
        perror("Failed to listen.");
        unlink(sock_path);
        return -1;
    }

    
    if ((clt_sock = accept(srv_sock, NULL, NULL)) == -1) {
        perror("Failed to accept.");
        unlink(sock_path);
        return -1;
    }

    unlink(sock_path);

    char buffer[BUFSIZ];
    while ((bytes_size = read(clt_sock, buffer, BUFSIZ)) > 0) {
        for (int i = 0; i < bytes_size; i++) {
            putchar(toupper(buffer[i]));
        }
    }

    if (bytes_size == -1) {
        perror("Failed to read.");
        return -1;
    }

    close(clt_sock);
    close(srv_sock);

    return 0;
}
