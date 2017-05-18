#define _BSD_SOURCE
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>
using namespace std;

void *rcv(void *);
void *snd(void *);

static int client_sock;
static int server_sock;
static int read_size;
static string clsource;

int main(int argc, char* argv[])
{
    int pid, port;
    socklen_t len;
    struct sockaddr_in server_address, client_address;
    
    pthread_t proc_thread[3];
    
    if (argc < 2)
    {
        cerr << "Syntax : ./chatserver <port>" << endl;
        return 0;
    }
    
    port = atoi(argv[1]);
    
    if((port > 65535) || (port < 1024))
    {
        cerr << "Please enter a port number between 1024 - 65535" << endl;
        return 0;
    }
    
    //create socket
    server_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(server_sock < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    
    bzero((char*) &server_address, sizeof(server_address));
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);
    
    //bind socket
    if(bind(server_sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        cerr << "Cannot bind to port " << port << endl;
        return 0;
    }
   
    //listen on binded socket 
    listen(server_sock, 5);
    
    
    int thread_num = 0;

    while (thread_num <3 )
    {
 
    socklen_t len = sizeof(client_address);

	
        cout << "Listening On Port " << port << endl;

	// Accept Incoming Connections
        client_sock = accept(server_sock, (struct sockaddr *)&client_address, &len);

        if (client_sock < 0)
        {
            cerr << "Cannot accept connection" << endl;
            return 0;
        }
        else
        {
 	clsource=inet_ntoa(client_address.sin_addr);
        cout << "Accepted Connection From:" << clsource << endl;
        }
        
        pthread_create(&proc_thread[thread_num], NULL, rcv, NULL); 
        thread_num++;
        pthread_create(&proc_thread[thread_num], NULL, snd, NULL); 
        thread_num++;
    }
    
    for(int i = 0; i < 3; i++)
    {
        pthread_join(proc_thread[i], NULL);
    }
    
}//main

void *rcv (void *dummyPt)
{
    cout << "Thread No: " << pthread_self() << endl;
    char msg[300];
    bzero(msg, 301);
    while(1)
    {    
        bzero(msg, 301);
        
        
        read_size=read(client_sock, msg, 300);
        
        string rcvmsg (msg);
        cout <<  "\rOther:"  << rcvmsg << endl;
        cout << "Me:";
	fflush(stdout);

        
	if ( rcvmsg =="quit") { break; }
	else if ( read_size ==  0 ) { cout << "\rClient Diconnected" << endl; break; }
	else if ( read_size == -1 ) { cout << "\rRecieve Failed" << endl; break; }

}//while
        close(client_sock);
        close(server_sock);
        cout << "\rClosing thread and connection" << endl;
        cout << "Ended." << endl;
        exit(0);
}//rcv

void *snd (void *dummyPt)
{
    cout << "Thread No: " << pthread_self() << endl;     

	while(1) {

        char message[300];
	cout << "Me:";
        bzero(message, 301);
        cin.getline(message, 300);
      
        send(client_sock, message, strlen(message),0);

	string sndmsg(message);
        
        if ( sndmsg =="quit") {

	break;
     
        }//if
	
	}//while

        cout << "\rClosing thread and connection" << endl;
        cout << "Ended." << endl;
        close(client_sock);
        close(server_sock);
        exit(0);    
}//snd
