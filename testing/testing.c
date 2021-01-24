#include "testing.h"

#define BUFFER_SIZE (1024)
#define CLIENTS_COUNT (1024)
#define FORKS_COUNT (512)
#define CHK2(res, eval) if((res = eval) < 0){perror("eval"); exit(-1);}
#define CHK(eval) if(eval < 0){perror("eval"); exit(-1);}


static void load ();
static pid_t init_forks ();

int main() {
    unsigned int finished_count = 0;
    pid_t pid = init_forks();
    load();
    if (pid != 0) {
        pid_t child_pid;
        int status = 0;
        while ((child_pid = wait(&status)) > 0) {
            finished_count++;
            printf("Sent = %d requests\n", CLIENTS_COUNT * finished_count);
        }
    }
    exit(0);
}

static pid_t init_forks () {
    pid_t pid;
    for (int c = 0; c < FORKS_COUNT - 1; ++c) {
        pid = fork();
        if (pid == 0) break;
    }
    return pid;
}

static void load () {
    char message[BUFFER_SIZE];
    int sock;
    struct sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(SERVER_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    for(int c = 0 ; c < CLIENTS_COUNT; c++) {
        CHK2(sock, socket(PF_INET, SOCK_STREAM, 0));
        CHK(connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0);
        send(sock, " ", 1, 0);
        recv(sock, message, BUFFER_SIZE, 0);
        close(sock);
    }
}