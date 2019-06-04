/*************************************************************************
	> File Name: tcp_client.c
	> Author: 
	> Mail: 
	> Created Time: Sat 23 Feb 2019 14:26:47 CST
 ************************************************************************/

#include "common.h"

char *config="./chat_c.conf";
char name[20] = {0};
int client_port, server_port;
char server_ip[20] = {0};
char buff[1024] = {0};
char log[100] = {0};

struct Message {
    char from[20];
    int flag;
    char message[1024];
};

void logout(int sig) {
    freopen(log, "a+", stdout);
    printf("\033[43m您已退出登录, 我们下次见！\033[0m \n");
    fclose(stdout);
    _exit(0);
}


int socket_connect(int port, char *host) {
	int sockfd;
	struct sockaddr_in dest_addr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error");
		return -1;
	}

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(host);

	if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
		perror("connect() error");
		return -1;
	}
	return sockfd;

}

int main(int argc, char *argv[]) {
    
    
    int sock_fd, new_sock;
    char temp_client[5] = {0};
    char temp_server[5] = {0};
    if (argc != 1) {
        printf("Usage: ./client\n");
        exit(1);
    }
   
    //get_conf_value(config, "Server_Port", temp_server);
    //get_conf_value(config, "My_Name", name);
    //get_conf_value(config, "Log_File", log);
    //get_conf_value(config, "Server_Ip", server_ip);
    client_port = atoi(temp_client);
    //server_port = atoi(temp_server);
    struct passwd *pwd;
    pwd = getpwuid(getuid());
    server_port = 8731;
    strcpy(temp_server,"8731");
    strcpy(name, pwd->pw_name);
    strcpy(log, "Log_File");
    strcpy(server_ip, "192.168.43.117");

    
    int pid = 0;

    if ((pid = fork())< 0) {
        perror("fork");
        exit(1);
    }

    if (pid != 0) {
        signal(SIGINT, logout);
        struct Message recv_message;
        int port = 8731;
        struct sockaddr_in peer_addr;
        peer_addr.sin_family = AF_INET;
        peer_addr.sin_port = htons(port);
        peer_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        int len = sizeof(struct sockaddr);
        if((new_sock= accept(sock_fd, (struct sockaddr *)&peer_addr,&len)) < 0) {
            
        } //<<
        while (recv(sock_fd, (char *)&recv_message, sizeof(recv_message), 0) > 0) {
            freopen(log, "a+", stdout);
            if (recv_message.flag == 0) {
                printf("\033[34m%s\033[0m: %s\n", recv_message.from, recv_message.message);
            } else if (recv_message.flag == 2) {
                printf("\033[43m服务器通知消息: \033[0m %s\n", recv_message.message);
            } else {
                printf("\033[34m%s\033[0m: %s   \033[5m \033[32m * \033[0m\n", recv_message.from, recv_message.message);
            }

            fclose(stdout);

        }
        close(sock_fd);

    } else {
        
        // DBG("After connect!\n");
        system("clear");
        while(1) {
            printf("请在以下的对话框输入信息，按回车发送\n\n");
            scanf("%[^\n]s", buff);
            getchar();
            int send_num = send(sock_fd, buff, strlen(buff), 0);
            if (send_num < 0) {
                perror("send");
            }
            memset(buff, 0, sizeof(buff));
            system("clear");
        }
        close(sock_fd);
    }
    return 0;
}

