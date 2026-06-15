/*
 * @file    udp_client.c
 * @brif    UDP socket wrapper implementation.
 *
 * @pre     Both the PC the STM32 are little-endian so the
 *          uint32_t test_id field in the structs requires no byte-order
 *          conversion for the application payload.
 */


#include "udp_client.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <string.h>

static int client_socket = -1; // Global socket handle for UDP communication with the UUT
static struct sockaddr_in target_addr; // Global structure to hold the target UUT's IP address and port for outgoing UDP packets

int udp_init(const char *ip, uint16_t port) 
{
    if (client_socket >= 0)  // If a socket is already open, close it before creating a new one to prevent resource leaks and ensure a clean state for the new connection
    {
        close(client_socket);
        client_socket = -1;
    }

    client_socket = socket(AF_INET, SOCK_DGRAM, 0); // Create a new UDP socket for communication with the UUT.
    if (client_socket < 0)
    {
        return -1;
    }

    memset(&target_addr, 0, sizeof(target_addr)); // Zero out the target address structure to ensure all fields are initialized to a known state before setting specific values for the IP address and port of the UUT.
    target_addr.sin_family = AF_INET; // Set the address family to AF_INET for IPv4 communication.
    target_addr.sin_port = htons(port); // Convert the provided port number from host byte order to network byte order using htons() to ensure correct interpretation by the network stack when sending UDP packets to the UUT.
    
    // Convert the provided IP address string to binary form and store it in the target address structure.
    if (inet_pton(AF_INET, ip, &target_addr.sin_addr) != 1) 
    {
        close(client_socket);
        client_socket = -1;
        return -1;
    }

    return 0;
}

int udp_sendCmd(const TestCommand_t *cmd) 
{
    if (client_socket < 0 || cmd == NULL) // Vlidate that socket was created or if thair is a cmd at all
    {
        return -1;
    }

    // Dynamic stream calculation based on raw network data layout:
    // PROTOCOL_CMD_LEN (7 Bytes fixed header block) + length of pattern buffer data
    int packet_size = PROTOCOL_CMD_LEN + cmd->pattern_len;

    int bytes_sent = sendto(client_socket, (const char*)cmd, packet_size, 0, (struct sockaddr*)&target_addr, sizeof(target_addr));                     
    if (bytes_sent < 0)  // if send is failed
    {
        return -1;
    }
    return 0;
}

int udp_recvRes(TestResult_t *res, int timeout_sec) 
{
    if (client_socket < 0 || res == NULL)  // Vlidate that socket was created or if thair is a res at all
    {
        return -1;
    }

    int timeout_ms = timeout_sec * 1000;
    // Set the receive timeout for the socket to prevent blocking indefinitely while waiting for a response from the UUT
    if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout_ms, sizeof(timeout_ms)) < 0) 
    {
        return -1;
    }

    struct sockaddr_in from_addr; // Structure to hold the address of the sender (UUT) when receiving a response
    socklen_t from_len = sizeof(from_addr); // Variable to hold the address of the sender (UUT) and its length

    // Expecting incoming buffer payload equivalent to PROTOCOL_RESULT_LEN (5 Bytes)
    ssize_t bytes_rx = recvfrom(client_socket, (char*)res, sizeof(TestResult_t), 0,(struct sockaddr*)&from_addr, &from_len);
    if (bytes_rx < 0) 
    {
        return -1; 
    }

    return 0;
}

int udp_close(void) // Closes the UDP socket and cleans up Winsock resources
{
    int error_occurred = 0;

    if (client_socket < 0)
    {
        if (close(client_socket) < 0) 
        {
            error_occurred = -1;
        }
        client_socket = -1;
    }

    return error_occurred;
}