#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * 
 * 【C语言】socket 通信编程
 *  https://mp.weixin.qq.com/s/NCZ2B4_1Jx80YkPq7bYDSg
 *
 * */
int main() {

    int n_read;
    char r_buf[128] = {0};
    char *w_buf = "I get your message.\n";

    struct sockaddr_in s_addr;
    struct sockaddr_in c_addr;

    memset(&s_addr, 0, sizeof(struct sockaddr_in));    // 清空内存
    memset(&c_addr, 0, sizeof(struct sockaddr_in));

    // socket
    //int socket(int domain, int type, int protocol);
    int s_fd = socket(AF_INET, SOCK_STREAM, 0);     // net tcp
    if (s_fd == -1) {
        printf("socket error\n");
        exit(-1);
    }

    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(8989);        // 用来将主机字节顺序转换为网络字节顺序
    inet_aton("localhost", &s_addr.sin_addr);  // 转大端

    // bind
    //int bind(int socketfd, const struct sockaddr *addr, socklen_t addrlen);
    bind(s_fd, (struct sockaddr *) &s_addr, sizeof(struct sockaddr_in));

    // listen
    listen(s_fd, 10);

    // accept
    int clen = sizeof(struct sockaddr_in);
    int c_fd = accept(s_fd, (struct sockaddr *) &c_addr, &clen);
    if (c_fd == -1) {
        perror("accept fail:");
    }
    printf("get connect:%s\n", inet_ntoa(c_addr.sin_addr));

    // read
    n_read = read(c_fd, &r_buf, 128);
    printf("get msg=%s, %dbytes.\n", r_buf, n_read);

    // write
    write(c_fd, w_buf, strlen(w_buf));

    return 0;
}