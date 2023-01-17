/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:57:51 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/17 18:53:00 by smia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>
# include <cstring>
# include <netdb.h>
# include <cstdlib>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fstream>
# include <unistd.h>
# include <vector>
# include <sstream>
# include "../includes/client.hpp"
# define MAX_CLIENT (10)
# define BUF_SIZE (1024)

#include <map>
#include <algorithm>

typedef std::map<int, Client>::iterator iterator;
class Server{
    private:
        int _port;
        std::string _passwd;
    public:
        // Client infoClients[MAX_CLIENT];
        Server(char * port, char * passwd);
        // std::vector<int> clients;
        int serverfd;
        struct sockaddr_in server_address;
        struct sockaddr_in client_address;
        socklen_t lencli;
        fd_set sockets_list;
        fd_set readfds;
        int client_socket;
        char buffer[BUF_SIZE];
        void setaddrinfo();
        void check_error(std::string err);
        int _socket();
        int _bind();
        int getPort();

        int _listen();
        int _select();
        int _accept();
        void listOfSockets();
        std::vector<std::string> splitCMD(std::string msg);
        struct timeval timeout;
        void setTime();

        std::map<int, Client> map_clients;
        
};
void connect(Server server, char *buffer, int fd);
void nick(Server server, std::vector<std::string> cmd, int fd);
void user(Server server, std::vector<std::string> cmd, int fd);
void passwd(Server server, std::vector<std::string> cmd, int fd);