#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct client_header{

    uint8_t protocol_version;
    uint8_t inverse_protocol_version;
    uint16_t payload_type;
    uint32_t payload_length;
};
struct client_payload
{
    uint8_t security; //security

    uint8_t transport_protocol;  //trasport protocol
};

struct secc_payload
{
    uint16_t secc_ip_address;
    uint16_t secc_port;
    uint8_t secc_security;
    uint8_t transport_protocol;
};
struct secc_header
{
    uint8_t secc_protocol_version;
    uint8_t secc_inverse_protocol_version;
    uint16_t payload_type;
    uint32_t payload_length;
};
int main(int argc, char **argv){

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(0);
    }
	
	char *ip = "127.0.0.1";
    int port = atoi(argv[1]);
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_size;
    int n;

    struct client_header *temp = malloc(sizeof(struct client_header));
    struct client_payload *temp_1 = malloc(sizeof(struct client_payload));

    struct secc_payload s2;
    struct secc_header s1;

    s1.secc_protocol_version = 0x01; //protocol version
    s1.secc_inverse_protocol_version = 0xFE; // secc inverse protocol version
    s1.payload_type = 0x9001; //SDP response message
    s1.payload_length = 0x14; //secc payload length

    s2.secc_ip_address = inet_addr(ip); //secc ip address
    s2.secc_port = 15118; //port number of secc
    s2.secc_security  = 0x00; //secured with tls
    s2.transport_protocol = 0x00; //tcp protocol
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("[-]socket error");
        exit(1);
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (n < 0){
        perror("[-]bind error");
        exit(1);
    }

    addr_size = sizeof(client_addr);
	
	int p = recvfrom(sockfd, temp, sizeof(temp), 0, (struct sockaddr*)&client_addr, &addr_size);
    if(p < 0)
    {
         perror("Receive from 1");
    }
 
    printf("Client protocol version is  %x\n", temp->protocol_version); 
    printf("client inverse protocol version  %x\n", temp->inverse_protocol_version);	
    printf("Client payload type is  %x\n", temp->payload_type);
    printf("Client payload length is  %x\n", temp->payload_length);

    int q = recvfrom(sockfd, temp_1, sizeof(temp_1), 0, (struct sockaddr*)&client_addr, &addr_size);
    if(q < 0)
    {
            perror("Receive from 2");
    }
    printf("Client security is  %x\n", temp_1->security);
    printf("Client transport  %x\n", temp_1->transport_protocol);

    int r = sendto(sockfd, (struct secc_header*)&s1,sizeof(s1), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
    if(r < 0)
    {
            perror("Sendto 1");
    }
   int t = sendto(sockfd, (struct secc_payload*)&s2,sizeof(s2), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
   if(t <0)
   {
           perror("Sendto 2");
   }
    return 0;
}