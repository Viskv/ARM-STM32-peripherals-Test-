#ifndef WIN32_LEAN_AND_MEAN // Exclude rarely-used services from Windows headers
#define WIN32_LEAN_AND_MEAN
#endif

#include "udp_client.h"
#include <windows.h>
#include <winsock2.h> 
#include <ws2tcpip.h> 
#include <string.h>

// Instructs GCC to link against Windows Sockets kernel dynamic link library
#pragma comment(lib, "Ws2_32.lib")

static SOCKET client_socket = INVALID_SOCKET; // Global socket handle for UDP communication with the UUT
static struct sockaddr_in target_addr; // Global structure to hold the target UUT's IP address and port for outgoing UDP packets
static int winsock_initialized = 0; // Flag to track whether Winsock has been initialized, ensuring proper cleanup and preventing redundant initialization calls

int udp_init(const char *ip, uint16_t port) 
{
    WSADATA wsaData;
    
    if (!winsock_initialized)  // Initialize Winsock library for use in this process if it hasn't been done already
    {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
        {
            return -1;
        }
        winsock_initialized = 1;
    }

    if (client_socket != INVALID_SOCKET)  // If a socket is already open, close it before creating a new one to prevent resource leaks and ensure a clean state for the new connection
    {
        closesocket(client_socket);
        client_socket = INVALID_SOCKET;
    }

    client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Create a new UDP socket for communication with the UUT.
    if (client_socket == INVALID_SOCKET)
    {
        return -1;
    }

    memset(&target_addr, 0, sizeof(target_addr)); // Zero out the target address structure to ensure all fields are initialized to a known state before setting specific values for the IP address and port of the UUT.
    target_addr.sin_family = AF_INET; // Set the address family to AF_INET for IPv4 communication.
    target_addr.sin_port = htons(port); // Convert the provided port number from host byte order to network byte order using htons() to ensure correct interpretation by the network stack when sending UDP packets to the UUT.
    
    // Convert the provided IP address string to binary form and store it in the target address structure.
    if (inet_pton(AF_INET, ip, &target_addr.sin_addr) != 1) 
    {
        closesocket(client_socket);
        client_socket = INVALID_SOCKET;
        return -1;
    }

    return 0;
}

int udp_sendCmd(const TestCommand_t *cmd) 
{
    if (client_socket == INVALID_SOCKET || cmd == NULL) // Vlidate that socket was created or if thair is a cmd at all
    {
        return -1;
    }

    // Dynamic stream calculation based on raw network data layout:
    // PROTOCOL_CMD_LEN (7 Bytes fixed header block) + length of pattern buffer data
    int packet_size = PROTOCOL_CMD_LEN + cmd->pattern_len;

    int bytes_sent = sendto(client_socket, (const char*)cmd, packet_size, 0, (struct sockaddr*)&target_addr, sizeof(target_addr));
                            
    if (bytes_sent == SOCKET_ERROR)  // if send is failed
    {
        return -1;
    }
    return 0;
}

int udp_recvRes(TestResult_t *res, int timeout_sec) 
{
    if (client_socket == INVALID_SOCKET || res == NULL)  // Vlidate that socket was created or if thair is a res at all
    {
        return -1;
    }

    int timeout_ms = timeout_sec * 1000;
    // Set the receive timeout for the socket to prevent blocking indefinitely while waiting for a response from the UUT
    if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout_ms, sizeof(timeout_ms)) == SOCKET_ERROR) 
    {
        return -1;
    }

    struct sockaddr_in from_addr; // Structure to hold the address of the sender (UUT) when receiving a response
    int from_len = sizeof(from_addr); // Variable to hold the address of the sender (UUT) and its length

    // Expecting incoming buffer payload equivalent to PROTOCOL_RESULT_LEN (5 Bytes)
    int bytes_rx = recvfrom(client_socket, (char*)res, sizeof(TestResult_t), 0,(struct sockaddr*)&from_addr, &from_len);
    if (bytes_rx == SOCKET_ERROR) 
    {
        return -1; 
    }

    return 0;
}

int udp_close(void) // Closes the UDP socket and cleans up Winsock resources
{
    int error_occurred = 0;

    if (client_socket != INVALID_SOCKET) 
    {
        if (closesocket(client_socket) == SOCKET_ERROR) 
        {
            error_occurred = -1;
        }
        client_socket = INVALID_SOCKET;
    }

    if (winsock_initialized) 
    {
        WSACleanup();
        winsock_initialized = 0;
    }

    return error_occurred;
}