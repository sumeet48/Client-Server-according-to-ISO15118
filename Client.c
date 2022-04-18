#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


struct header{

        uint8_t protocol_version; //client protocol version
        uint8_t inverse_protocol_version; //client inverse protocol version
        uint16_t payload_type;   //payload type
        uint32_t payload_length;  //payload length

};
struct payload{

        uint8_t security; //security
        uint8_t transport_protocol;  //trasport protocol
};
struct payload_sec
{
        uint16_t se_ip_address;  //secc ipadress
        uint16_t se_port;     //secc port
        uint8_t se_security;    //secc security
        uint8_t se_transport_protocol;  //secc transport protocol
};
struct se_header
{
        uint8_t se_protocol_version;    //secc protocol version
        uint8_t se_inverse_protocol_version;   //secc inverse protocol version
        uint16_t se_payload_type;  //secc payload
        uint32_t se_payload_length; //secc payload length
};
int main(int argc, char **argv){

        if (argc != 2) {
                printf("Usage: %s <port>\n", argv[0]);
                exit(0);
        }

        char *ip = "127.0.0.1";
        int port = atoi(argv[1]);

        struct se_header *te = malloc(sizeof(struct se_header));
        struct payload_sec *st = malloc(sizeof(struct payload_sec));

        struct header stu;
        struct payload st1;

        stu.protocol_version = 0x01;  // V2GTP version 1
        stu.inverse_protocol_version = 0xFE; // V2GTP version 1
        stu.payload_type = 0x9000; //SDP request message
        stu.payload_length = 0x02; // payload length is two bytes

        st1.security = 0x00; //secured with TLS
        st1.transport_protocol = 0x00; //tcp

        int sockfd;
        struct sockaddr_in addr;
        socklen_t addr_size;

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        memset(&addr, '\0', sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip);

        int x = sendto(sockfd, (struct header*)&stu,sizeof(stu), 0, (struct sockaddr*)&addr, sizeof(addr));
        if(x < 0)
        {
             perror("Sendto 1 error");
        }

        int y = sendto(sockfd, (struct payload*)&st1,sizeof(st1), 0, (struct sockaddr*)&addr, sizeof(addr));
        if(y < 0)
        {
			  perror("Sendto 2 error");
        }
        addr_size = sizeof(addr);
        int z = recvfrom(sockfd, te, sizeof(te), 0, (struct sockaddr*)&addr, &addr_size);
        if(z <0)
        {
                perror("Receive from 1");
        }
        printf("Server protocol version is  %x\n", te->se_protocol_version);
        printf("Server inversion protocol version  %x\n", te->se_inverse_protocol_version);
        printf("Server payload type  %x\n", te->se_payload_type);
        printf("Server payload length  %x\n", te->se_payload_length);
        int v = recvfrom(sockfd, st, sizeof(st), 0, (struct sockaddr*)&addr, &addr_size);
        if(v <0)
        {
                perror("Receive from 2");
        }
        printf("Server ip address is  %d\n", st->se_ip_address);
        printf("Server port  is  %d\n", st->se_port);
        printf("Server %d\n",st->se_security);
        printf("Server %d\n", st->se_transport_protocol);



        return 0;
}