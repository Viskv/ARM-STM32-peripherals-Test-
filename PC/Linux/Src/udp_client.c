/*
 * @file    udp_client.c
 * @brif    UDP socket wrapper implementation.
 *
 * @pre     Both the PC the STM32 are little-endian so the
 *          uint32_t test_id field in the structs requires no byte-order
 *          conversion for the application payload.
 */

// #include "udp_client.h"
// #include <stdio.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <sys/time.h>

// static int    s_sock      = -1;
// static struct sockaddr_in s_dest_addr = {0};

// int udp_init(const char *ip, uint16_t port)
// {
//     s_sock = socket(AF_INET, SOCK_DGRAM, 0);
//     if(s_sock < 0)
//     {
//         perror("[UDP] socket");
//         return -1;
//     }

//     memset(&s_dest_addr, 0, sizeof(s_dest_addr));
//     s_dest_addr.sin_family = AF_INET;
//     s_dest_addr.sin_port   = htons(port);

//     if(inet_pton(AF_INET, ip, &s_dest_addr.sin_addr) <= 0)
//     {
//         fprintf(stderr, "[UDP] Invalid IP address: %s\n", ip);
//         close(s_sock);
//         s_sock = -1;
//         return -1;
//     }

//     return 0;
// }