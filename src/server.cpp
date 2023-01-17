/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smia <smia@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 11:10:21 by mhaddaou          #+#    #+#             */
/*   Updated: 2023/01/17 18:59:16 by smia             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

Server::Server(char * port, char * passwd){
    this->_port = atoi(port);
    this->_passwd = passwd;
}

void Server::setaddrinfo(){
    
    memset(&this->server_address, 0, sizeof(this->server_address));
    this->server_address.sin_family = AF_INET;
    this->server_address.sin_addr.s_addr = INADDR_ANY;
    this->server_address.sin_port = htons(this->_port);
}
int Server::_socket(){
    this->serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->serverfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
int Server::_bind(){
    if (bind(this->serverfd, (struct sockaddr *)&this->server_address, sizeof(this->server_address)) < 0){
        std::cerr << "bind failed" << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
int Server::_listen(){
    if (listen(this->serverfd, MAX_CLIENT) < 0){
        std::cerr << "listen failed" << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
void Server::listOfSockets(){
    FD_ZERO(&this->sockets_list);
    FD_SET(this->serverfd, &this->sockets_list);
}
void Server::setTime(){
    this->timeout.tv_sec = 1;
    this->timeout.tv_usec = 0;
}
int Server::_select(){
    if (select(FD_SETSIZE, &this->readfds,NULL, NULL, &this->timeout ) < 0){
        std::cerr << "error in select" << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
std::vector<std::string> Server::splitCMD(std::string msg){
    
    std::vector<std::string> cmd;
    std::stringstream ss(msg);
    std::string word;
    while (getline(ss, word, ' ')) { 
    cmd.push_back(word);
    }
    return (cmd);
}
int Server::getPort(){
    return (this->_port);
}

int Server::_accept(){
    int newfd = accept(this->serverfd, NULL, NULL);
    if (newfd < 0){
        std::cerr << "error in accept" << std::endl;
        return (EXIT_FAILURE);
    }
    // send(newfd, "Please enter first name", 23, 0);
    // while(true){
    //     int bytes_received = recv(newfd, this->buffer, BUF_SIZE, 0);
    //     if (bytes_received > 0){
    //         std::vector<std::string> cmd = this->splitCMD(this->buffer);
    //         memset((void *)this->buffer, 0, BUF_SIZE);
    //         std::cout << "`" << cmd[2] << "`" << std::endl;
    //         std::cout << cmd.size() << std::endl;
    //     }
    // }
    this->map_clients[newfd];
    std::cout << "new client connected" << std::endl;
    return (EXIT_SUCCESS);
}

void nick(Server server,std::vector<std::string> cmd, int fd){
    if (cmd[0] == "NICK")
        if (cmd.size() == 2){
            std::cout << fd << " :" <<"NICK OK" << std::endl;
            server.map_clients[fd].setNickName(cmd[1]);
            for(size_t i = 0; i < cmd.size(); i++){
                std::cout << cmd[i];
            }
            std::cout << fd << " : " << cmd[1] << std::endl;
            server.map_clients[fd].verif++;
            // server.infoClients->fd = fd;
           std::cout << server.map_clients[fd].verif << " :" <<  "PASS OK" << std::endl;
        }
}
void user(Server server, std::vector<std::string> cmd, int fd){
    if (cmd[0] == "USER"){
        std::cout << fd << " :" << "USER OK" << std::endl;
        std::cout << fd << " : " << cmd[1] << std::endl;
        server.map_clients[fd].setName(cmd[1]);
        server.map_clients[fd].setRealName(cmd[4]);
        // server.map_clients[fd].fd = fd;
        server.map_clients[fd].verif++;
        server.map_clients[fd].id = atoi(cmd[2].c_str());
        std::cout << server.map_clients[fd].verif << " :" <<  "PASS OK" << std::endl;
    }
}
void passwd(Server server, std::vector<std::string> cmd,  int fd){
    if (cmd[0] == "PASS")
        if (cmd.size() == 2){ 
            server.map_clients[fd].setPassword(cmd[1]);
            //check if password
            // if (server.infoClients->getPassword() == server.) 
            server.map_clients[fd].verif++;
            std::cout << server.map_clients[fd].verif << " :" <<  "PASS OK" << std::endl;
            // return (EXIT_SUCCESS);
        }
    // return (EXIT_FAILURE);
    
}
void connect (Server server,char *buffer, int fd)
{
    std::vector<std::string> cmd = server.splitCMD(buffer);
    nick(server,cmd, fd);
    user(server, cmd, fd);
    passwd(server, cmd, fd);
    // std::cout << server.map_clients[fd].verif <<std::endl;
    if (server.map_clients[fd].is_verified())
        std::cout << "new client is verified" << std::endl;
}