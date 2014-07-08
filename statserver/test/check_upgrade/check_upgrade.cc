#include <iostream>
#include <vector>
#include <string>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <vector>
#include <map>
#include <sys/time.h>
#include <sys/epoll.h>


using namespace std;

#define CMD_CHECK_USER_CODE 13041702
#define CMD_CHECK_UPGRADE 13041703

#pragma pack(1)
typedef struct _HEAD {
    uint16_t version;
    uint32_t pkglen; // head + body
    uint32_t akg_id;
    char user_code[64];
    char machine_code[64];
} HEAD;
#pragma pack()

int create_socket()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd) {
		printf("create socket failed, errno %d\n", errno);
		exit(1);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(8088);

	if (-1 == connect(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr))) {
		printf("connect failed, errno %d\n", errno);
		exit(1);
	}

	return sockfd;
};

void mysleep(unsigned long millisec)
{ 
	struct timeval tv;
	tv.tv_sec = millisec / 1000;
	tv.tv_usec = millisec - tv.tv_sec*1000;
	select(0, NULL, NULL, NULL, &tv);

};


int send_cmd(int c, int cmd, char *data, size_t data_len) 
{
	HEAD *h = (HEAD *) (data);
	h->pkglen = sizeof(HEAD) + data_len;
	h->version = 1;
	h->akg_id = CMD_CHECK_UPGRADE;
	int rs = send(c, data, h->pkglen, 0);
	return rs == h->pkglen;
};


int main()
{
    int socket = create_socket();
    char buffer[1024] = {0};
    size_t data_size = 0;
    int ret = send_cmd(socket, CMD_CHECK_USER_CODE, buffer, data_size);

    if (!ret) {
        cout << "send failed" << endl;
        return EXIT_FAILURE;
    }

    char recv_buf[1024] = {0};

    int recv_bytes = recv(socket, recv_buf, 1024, 0);
    cout << "recv_bytes:" << recv_bytes << endl;
    if (-1 == recv_bytes) {
        cout << "recv error" << endl;
    }
    else if (0 == recv_bytes) {
        cout << "peer clsoed connection" << endl;
    }
    else {
        size_t data_size = recv_bytes - sizeof(HEAD);
        string data(&recv_buf[138], data_size);
        cout << "recv_data: " << data << endl;
        cout << "data_size: " << data_size << endl;
    }

    close(socket);
    return EXIT_SUCCESS;
}


//vim: ts=2,tw=120,sw=2
