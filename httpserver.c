/* 
This code primarily comes from 
http://www.prasannatech.net/2008/07/socket-programming-tutorial.html
and
http://www.binarii.com/files/papers/c_sockets.txt
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "requestparse.h"
#include "readHTML.h"
#include "parse.h"
#include "sort.h"
#include <pthread.h>

void* respond(void* response_data);
int end = 0; // signal to stop the server

void* stop(void* arg){
    char* input = malloc(sizeof(char)*10); 
    while(end != 1){
        printf("Enter 'q' to stop program: "); 
        scanf("%s", input); 
        // printf("%s\n", input);
        if(strcmp("q",input) == 0)
            end = 1;  
    }
    free(input);
    pthread_exit(NULL); 
}


typedef struct threaddata {
    char* header;
    char* footer;
    int fd;
    data_container* data;
} thread_data;


int start_server(int PORT_NUMBER)
{

    // structs to represent the server and client
    struct sockaddr_in server_addr,client_addr;
    
    int sock; // socket descriptor

    // 1. socket: creates a socket descriptor that you later use to make other system calls
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }
    int temp;
    if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&temp,sizeof(int)) == -1) {
        perror("Setsockopt");
        exit(1);
    }

    // configure the server
    server_addr.sin_port = htons(PORT_NUMBER); // specify port number
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero),8);
      
      // 2. bind: use the socket and associate it with the port number
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Unable to bind");
        exit(1);
    }
    
    unlink("data.html");
    data_container* data = parse_data("course_evals.txt");
    
    char* header = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
    char* footer = "</body></html>";
    
    unsigned int request_count = 0;
    size_t num_threads = 100;
    
    pthread_t threads[num_threads];
    
    data_to_HTML(data, "data.html");
    
    while(end != 1) {
        // 3. listen: indicates that we want to listen to the port to which we bound; second arg is number of allowed connections
        // second arg here is the number of possible queued connections
        if (listen(sock, 10) == -1) {
            perror("Listen");
            exit(1);
        }
        
        // once you get here, the server is set up and about to start listening
        printf("\nServer configured to listen on port %d\n", PORT_NUMBER);
        fflush(stdout);
        
        
        // 4. accept: wait here until we get a connection on that port
        int sin_size = sizeof(struct sockaddr_in);
        int fd = accept(sock, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);
        if (fd != -1) {
            printf("Server got a connection from (%s, %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
            printf("fd: %d\n", fd);
            printf("sock: %d\n\n", sock);
            
            thread_data* pass_data = malloc(sizeof(thread_data));
            pass_data->header = header;
            pass_data->footer = footer;
            pass_data->fd = fd;
            pass_data->data = copy_data(data);
            
           // pthread_join(threads[request_count % num_threads], NULL);
            
            printf("creating thread");
            pthread_create(&threads[request_count % num_threads], NULL, respond, pass_data);
        
            request_count++;
            
        }
        // 7. close: close the connection
        printf("Server closed connection\n"); 
    }
    // join
    //for (int i = 0; i < num_threads; i++) pthread_join(threads[i], NULL);
    
    // free the data container
    free_data_container(data);

    // 8. close: close the socket
    close(sock);
    printf("Server shutting down\n");
  
    return 0;
} 


int main(int argc, char *argv[])
{
  // check the number of arguments
  if (argc != 2) {
      printf("\nUsage: %s [port_number]\n", argv[0]);
      exit(-1);
  }

  int port_number = atoi(argv[1]);
  if (port_number <= 1024) {
    printf("\nPlease specify a port number greater than 1024\n");
    exit(-1);
  }
  // create stop thread 
  pthread_t t;
  pthread_create(&t, NULL, &stop, NULL); 
  // start server 
  start_server(port_number);
  pthread_join(t, NULL);

  return 0; 
}

void* respond(void* response_data) {
    thread_data* td = (thread_data*)response_data;

    // buffer to read data into
    char request[1024];
    
    // 5. recv: read incoming message (request) into buffer
    int bytes_received = recv(td->fd,request,1024,0);
    
    // null-terminate the string
    request[bytes_received] = '\0';

    parsed_request* pr = parse_request(request);
    print_request(*pr);
    
    data_container* pd;
    char filename[100];
    
    if (  isPost(pr)  ) {
        post_request* post_req = malloc(sizeof(post_request));
        if(post_req == NULL) return NULL;
        
        parse_post(post_req, pr->rest);
        int (*comparep) (course_data*, course_data*) = compare_course_id;
        
        // print_post_request(post_req);
         
        // printf("Choosing filter...");
        pd = choose_filter(td->data, post_req);
        // printf("done.\n");
    
        // printf("pd length: %zu\n", pd->length);
        
        // printf("Choosing sort...");
        comparep = choose_sort(post_req);
        // printf("done.\n");
        
        // printf("Evaluating sort...");
        if (comparep != NULL) {
            sleep(30);
            printf("sort request detected: sorting...");
            // sleep(40);
            quicksort_data(pd->data, 0, pd->length - 1, comparep);
        }
        // printf("done.\n");
        
        sprintf(filename, "data%d.html", td->fd);
        data_to_HTML(pd, filename);  

        // free temporary post data container 
        free_shallow_data(pd);
        // free post request 
        free(post_req); 
    } else {
        strcpy(filename,"data.html");
        printf("%s\n", filename);
    }
    
    //read in HTML file
    char* resource = readHTML("index.html");

    // parse data into structure and format data into html
    char* data = readHTML(filename);
    
    // 6. send: send the outgoing message (response) over the socket
    send(td->fd, td->header, strlen(td->header), 0);
    send(td->fd, resource, strlen(resource), 0);
    send(td->fd, data, strlen(data), 0);
    send(td->fd, td->footer, strlen(td->footer), 0);
    
    // free parsed request 
    free(pr); 

//    unlink(filename);
    close(td->fd);
    return &td->fd;
}







































