/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 12:54:55 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/17 18:43:59 by smia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/server.hpp"
// #include <typeinfo>

#include <typeinfo>
#include <arpa/inet.h>


// const int BUF_SIZE = 1024;

int main(int ac, char **av) {
    
    if (ac < 3)
        std::cerr << "invalid arguments" << std::endl;
    else{
        
        // char syn_ack[] = "SYN-ACK";
        
        Server server(av[1], av[2]);
        // Create socket
        server._socket();
        // Set server address information
        server.setaddrinfo();
        server.lencli = sizeof(server.client_address);
       

        // Bind socket to address and port
        // Start listening for incoming connections
        if (server._bind() == EXIT_FAILURE || server._listen() == EXIT_FAILURE)
            return (EXIT_FAILURE);
        memset(server.buffer, 0, BUF_SIZE);
        while (true) {
            // Clear the file descriptor set
            FD_ZERO(&server.readfds);
            // Add the socket and connected server.clients to the file descriptor set
            FD_SET(server.serverfd, &server.readfds);
            for (iterator it = server.map_clients.begin(); it != server.map_clients.end(); ++it)
            {
                FD_SET(it->first, &server.readfds);
            }
            // Set up timeout for select()
            server.setTime();
            // Wait for activity on any file descriptor
            if (server._select() == EXIT_FAILURE)
                return (EXIT_FAILURE);
            // Check if there is a new incoming connection
            if (FD_ISSET(server.serverfd, &server.readfds)) 
            {
                int clientfd = accept(server.serverfd, (struct sockaddr *)&server.client_address, &server.lencli);
                if (clientfd < 0) 
                {
                    std::cerr << "Error accepting connection" << std::endl;
                    continue;
                }
                // Add new client to connected server.clients map
                std::cout << "accept fd: " << clientfd << std::endl;
                server.map_clients[clientfd];
            }

            // Check if any connected server.clients have sent data
            for (iterator it = server.map_clients.begin(); it != server.map_clients.end(); ++it) 
            {
                if (FD_ISSET(it->first, &server.readfds)) 
                {
                    int x = recv(it->first, server.buffer, BUF_SIZE, 0);
                    if (x <= 0)
                    {
                        x = it->first;
                        ++it;
                        close(x);
                        server.map_clients.erase(x);
                        std::cout << "Client disconnected" << std::endl;
                    } 
                    else 
                    {
                        if (server.map_clients[it->first].verified == false)
                        {
                            connect(&server, server.buffer, it->first);
                        }
                        else
                        {
                            std::cout << "hi" << std::endl;
                        }
                        // std::cout << server.buffer << std::endl;
                        // send(server.clients[i],":localhost CAP * LS :multi-prefix sasl", sizeof(":localhost CAP * LS :multi-prefix sasl"), 0);  
                        // std::cout << "--" << server.buffer<< "--" << std::endl;
                        
                        // if (strcmp(server.buffer,syn_ack) == 0)
                        //     send(server.clients[i], syn_ack, sizeof(syn_ack), 0);

                        // Add code here to handle the received message and send it to other server.clients as necessary
                        // for (int j = 0; j < server.clients.size(); j++) {
                        //     if (j != i) {
                        //         send(server.clients[j], buf, bytes_received, 0);
                        //     }
                        // }
                    }
                }
            }
        }
    }
    return 0;
}
