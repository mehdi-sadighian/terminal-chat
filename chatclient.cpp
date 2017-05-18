#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>
using namespace std;

void *rcv(void *);
void *snd(void *);

static int client_sock;
static int read_size;

int main (int argc, char* argv[])
{
    int port;
    struct sockaddr_in server_address;
    struct hostent *server;

    pthread_t proc_thread[3];
    
    if(argc < 3)
    {
        cerr<<"Syntax : ./chatclient <host name> <port>"<<endl;
        return 0;
    }
    
    port = atoi(argv[2]);
    
    if((port > 65535) || (port < 1024))
    {
        cerr<<"Please enter port number between 1024 - 65535"<<endl;
        return 0;
    }       
    
    client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if(client_sock < 0)
    {
        cerr << "Cannot open socket" << endl;
        return 0;
    }
    
    server = gethostbyname(argv[1]);
    
    if(server == NULL)
    {
        cerr << "Host does not exist" << endl;
        return 0;
    }
    
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    
    bcopy((char *) server -> h_addr, (char *) &server_address.sin_addr.s_addr, server -> h_length);
    
    server_address.sin_port = htons(port);
    
    int connection = connect(client_sock,(struct sockaddr *) &server_address, sizeof(server_address));
    
    if (connection < 0)
    {
        cerr << "Cannot Connect!" << endl;
        return 0;
    }
  
int thread_num = 0;  

    while (thread_num <3 )
    {

        pthread_create(&proc_thread[thread_num], NULL, snd, NULL);
        thread_num++;
	pthread_create(&proc_thread[thread_num], NULL, rcv, NULL);
        thread_num++;
    }

    for(int i = 0; i < 3; i++)
    {
        pthread_join(proc_thread[i], NULL);    
    }
        

}//main



void *rcv (void *dummyPt)
{
    //cout << "Thread No: " << pthread_self() << endl;
    char msg[300]; 
    while(1) 
    {
        bzero(msg, 301);
    

        read_size=read(client_sock, msg, 300);
    
        string rcvmsg (msg);       
        cout <<  "\rOther:"  << rcvmsg << endl;
        cout << "Me:";
        fflush(stdout);

        if ( rcvmsg  =="quit") { break; }
        else if ( read_size ==  0 ) { cout << "\rServer Closed Connection." << endl; break; }
        else if ( read_size == -1 ) { cout << "\rRecieve Failed" << endl; break; }	

}//while
        cout << "\nClosing thread and connection" << endl;
        cout << "Ended." << endl;
        close(client_sock);
        exit(0);
}//main

void *snd (void *dummyPt)
{
    //cout << "Thread No: " << pthread_self() << endl;

        while(1) {

	char message[300];
        cout << "\rMe:";
        bzero(message, 301);
        cin.getline(message, 300);
     
        send(client_sock, message, strlen(message),0);
	string sndmsg(message);

	if ( sndmsg =="quit") {
	
	break;
	
	}//if
       
        }//while
    	cout << "\nClosing thread and connection" << endl;
    	cout << "Ended." << endl;
    	close(client_sock);
 	exit(0);	
}//snd
