/*
 * Código ejemplo del servidor tcp
 * Obtenido de http://codebase.eu/tutorial/linux-socket-programming-c/
 */
#include <iostream>
#include <iomanip>
#include <cstring>      // Needed for memset
#include <unistd.h>
#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions

#include <vector>
#include "ModbusServer.hpp"
//#include "ModbusServer.cpp"
typedef unsigned char byte;

int main()
{

    int status;
    struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
    struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.
    ModbusServer mbs(0x06);

    // The MAN page of getaddrinfo() states "All  the other fields in the structure pointed
    // to by hints must contain either 0 or a null pointer, as appropriate." When a struct
    // is created in c++, it will be given a block of memory. This memory is not nessesary
    // empty. Therefor we use the memset function to make sure all fields are NULL.
    memset(&host_info, 0, sizeof host_info);

    std::cout << "Setting up the structs..."  << std::endl;

    host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
    host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
    host_info.ai_flags = AI_PASSIVE;     // IP Wildcard

    //El Modbus/TCP es el 502, pero para no tener que ejecutar como root, usamos el 1502
    status = getaddrinfo(NULL, "1502", &host_info, &host_info_list);
    // getaddrinfo returns 0 on succes, or some other value when an error occured.
    // (translated into human readable text by the gai_gai_strerror function).
    if (status != 0)  std::cout << "getaddrinfo error" << gai_strerror(status) ;


    std::cout << "Creating a socket..."  << std::endl;
    int socketfd ; // The socket descripter
    socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
                      host_info_list->ai_protocol);
    if (socketfd == -1)  std::cout << "socket error " ;

    std::cout << "Binding socket..."  << std::endl;
    // we use to make the setsockopt() function to make sure the port is not in use
    // by a previous execution of our code. (see man page for more information)
    int yes = 1;
    status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    std::cout << "Usando puerto:" << ntohs(((struct sockaddr_in *)(host_info_list->ai_addr))->sin_port) << std::endl ;

    status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)  std::cout << "bind error" << std::endl ;

    std::cout << "Listening for connections..."  << std::endl;
    status =  listen(socketfd, 5);
    if (status == -1)  std::cout << "listen error" << std::endl ;


    int new_sd;
    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    new_sd = accept(socketfd, (struct sockaddr *)&their_addr, &addr_size);
    if (new_sd == -1)
    {
        std::cout << "listen error" << std::endl ;
    }
    else
    {
        std::cout << "Connection accepted. Using new socketfd : "  <<  new_sd << std::endl;
    }


    std::cout << "Waiting to recieve data..."  << std::endl;
    ssize_t bytes_recieved;
    char incomming_data_buffer[1000];
    while(1) {
      std::vector<byte> input;
      std::vector<byte> output;
      char msg[1000];
      bytes_recieved = recv(new_sd, incomming_data_buffer,1000, 0);
      // If no data arrives, the program will just wait here until some data arrives.
      if (bytes_recieved == 0) {
        std::cout << "host shut down." << std::endl ;
        break;
      }
      if (bytes_recieved == -1)std::cout << "recieve error!" << std::endl ;
      //std::cout << bytes_recieved << " bytes recieved :" << std::endl ;
      incomming_data_buffer[bytes_recieved] = '\0';
      std::cout << incomming_data_buffer << std::endl;

      std::cout << "[("  << bytes_recieved << ") " << std::setfill('0') << std::hex ;
      for ( unsigned int ba = 0; ba < bytes_recieved; ba++){
        std::cout << std::setw(2) << (int)incomming_data_buffer[ba] << ' ';
        if (ba < 5)
          msg[ba] = incomming_data_buffer[ba];
        if (ba > 5)
          input.push_back((byte)incomming_data_buffer[ba]);
        }
      std::cout << "] " << std::dec;
      std::cout << std::endl;

      /* AQUÍ DEBERÍAMOS INVOCAR A peticion( ) y devolver el valor vector generado
       En primer lugar hay que convertir del array de char a vector de bytes,
       para ello se puede usar: vector<byte>( v, v + sizeof(v)/sizeof(byte) )

       Lo devuelto por peticion( ) hay que convertirlo a array,
       para ello se puede usar: std::copy(v.begin(), v.end(), arr);

       */
      mbs.AddVector(&input, mbs.CRC16(input));
      output = mbs.peticion(input);

      if (output.size() != 0)
      {
        output.pop_back();
        output.pop_back();

        msg[5] = (char) output.size();
        unsigned int i;
        for (i = 0; i < output.size(); i++)
          msg[6+i] = output[i];

        std::cout << "send()ing back a message..."  << std::endl;
        int len = 6 + output.size();
        ssize_t bytes_sent;
        bytes_sent = send(new_sd, msg, len, 0);
      }

    }
    std::cout << "Stopping server..." << std::endl;
    freeaddrinfo(host_info_list);
    close(new_sd);
    close(socketfd);

return 0 ;


}


