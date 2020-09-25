#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
//#include <linux/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main() {

    int n_read;
    char r_buf[128] = {0};
    char *w_buf = "This is from client.";

    struct sockaddr_in c_addr;
    struct sockaddr_in s_addr;

    memset(&s_addr, 0, sizeof(struct sockaddr_in));
    memset(&c_addr, 0, sizeof(struct sockaddr_in));

    // socket
    //int socket(int domain, int type, int protocol);
    int c_fd = socket(AF_INET, SOCK_STREAM, 0);     // net tcp
    if (c_fd == -1) {
        printf("socket error\n");
        exit(-1);
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(8989);
    inet_aton("localhost", &s_addr.sin_addr);

    c_addr.sin_family = AF_INET;
    c_addr.sin_port = htons(9955);
    inet_aton("localhost", &c_addr.sin_addr);

    // bind 可以不绑定，系统自动分配
    //int bind(int socketfd, const struct sockaddr *addr, socklen_t addrlen);
    bind(c_fd, (struct sockaddr *) &c_addr, sizeof(struct sockaddr_in));

    // connect
    if (connect(c_fd, (struct sockaddr *) &s_addr, sizeof(struct sockaddr_in)) == -1) {
        perror("connect fail:");
        exit(-1);
    }

    // write
    write(c_fd, w_buf, strlen(w_buf));

    // read
    n_read = read(c_fd, &r_buf, 128);
    printf("get msg=%s, %dbytes.\n", r_buf, n_read);

    return 0;
}