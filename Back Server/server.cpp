#include <sys/select.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <string>

#define PORT 5555
#define IP '0.0.0.0'
using namespace std;


void* handle_client(void* conn_fd)
{
    int client_fd = *((int*)(conn_fd));

    //recv name_length
    int name_length;
    recv(client_fd, &name_length, 4, 0);
    name_length = ntohl(name_length);
    cout << "name length = " << name_length << endl;
    // recv name
    char* file_name = (char*)malloc(name_length+1);
 	cout << "ok so far" << endl;
   recv(client_fd, file_name, name_length, 0);
	file_name[name_length] = '\0';
	cout << "file name is " << file_name << endl;

    // recv size_of file
    int size_of_file;
    recv(client_fd, &size_of_file, 4, 0);
    size_of_file = ntohl(size_of_file);

    cout << "size of file = " << size_of_file << endl;

    void* file_buff = malloc(size_of_file);
    // recv file
    recv(client_fd, file_buff, size_of_file, 0);
	cout << "got here" << endl;
    // create file and write to it
	cout << (char*)file_buff<< endl;
    ofstream office_file(file_name);
	cout<<"1"<<endl;
office_file << (char*)file_buff;
	cout <<"2"<<endl;
    office_file.close();
    cout << "3" << endl;
	return NULL;
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);


    if (bind(sockfd, (struct sockaddr*)&address, addrlen)< 0) {
        perror("bind failed");
        exit(-1);
    }

    if (listen(sockfd, 5) < 0) {
        perror("listen");
        exit(-1);
    }

    int conn_fd;
    while(true)
    {
        if ((conn_fd= accept(sockfd, (struct sockaddr*)&address,(socklen_t*)&addrlen)) < 0) {
                perror("accept");
                exit(-1);
            }
        cout << "accepted new client" << endl;
        pthread_t thread_id;
        pthread_create(&thread_id, 0, &handle_client, (void *)&conn_fd);
    }
     return 0;
}
