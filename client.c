// socket client multi clients
// File: client.c
// compile: gcc -Wall client.c -o client
// run: ./client localhost
#include <stdio.h>
#include <stdlib.h>
#include <string.h>        
#include <unistd.h>        	          // for read/write/close
#include <sys/types.h>     	          /* standard system types*/
#include <netinet/in.h>    	          /* Internet address structures */
#include <sys/socket.h>   	          /* socket interface functions  */
#include <netdb.h>         	          /* host to IP resolution*/
#define h_addr h_addr_list[0] /* for backward compatibility */
#define  BUFLEN  40          /* maximum response size     */
#define SIZEREQUEST 500 

//////////////////////////////////////////////SOS METHODS///////////////////////////////////////////////////////

int check_exist_P_OR_R(char const * arr[],int arrSize){//this method chek if into argv exist -p or -r and return -1 if the them doesnt exist
                                                //1 if just -p exist, 2 if just -r exist and 0 if both exist.
int flagP=0,flagR=0;
int i;
    for(i=1;i<arrSize;i++){
        
        if(strcmp("-p",arr[i])==0){
            flagP=1;
        }
        if(strcmp("-r",arr[i])==0){
            flagR=1;
        }
    }
    if(flagR==0 && flagP==0)
        return -1;
    if(flagR==1 && flagP==1)
        return 0;
    if(flagR==0 && flagP==1)
        return 1;
    if(flagR==1 && flagP==0)
        return 2;

} 
/////////////////////////////////////////////////print error///////////////////////////////////////////
void printINPUTERROR(char const *arr[],int arrSize){//print if happends a input error
    fprintf(stderr,"Usage: ");
    for(int i =0;i< arrSize;i++){
        fprintf(stderr,"%s ",arr[i]);
    }
    fprintf(stderr,"invalid input\n");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
int sizeRequestFunc(char const *arr[],int indStart,int filedN){//this method get what is the input size string 
    int size=0;
    int spaces=1;
    int i;
    for(i=0;i<filedN;i++){
        size+=strlen(arr[indStart]);
        spaces++;
        indStart++;
    }
    size+=spaces;
    return size;
}
char* serverARRAY(int size){// this function make an array
    char *temp=(char*)malloc(size*sizeof(char));
    if (temp==NULL){
        perror("ERROR:aloocation failed\n");
        exit(1);
    }
    return temp;
}
///////////////////////////////////////////////////GETING REQUEST WITH R/////////////////////////////////////////////////////
int pathISValid(char const *arr[],int from,int to){//this method checks if the path its valid, return 0 if its valid otherwise return -1 
    char const *temp=arr[from];
    int tempnum;
    int i;
    for(i=0;i<to;i++){
        if(strchr(temp,'=')==NULL){
            printf("NOT ENOUGHT PARAMETERS!!!\n");
            return -1;
        }
            
        if(temp[0]=='=')
            return -1;
        if(temp[strlen(temp)-1]=='=')
            return -1;
        from++;
        temp=arr[from];
    }
    if(strchr(temp,'=')!=NULL){
        printf("TOO MANY PARAMETERS!!!\n");
        return -1;
    }
    return 0;
}
int checkingN(char const *number){//this method checks if the number of variables its valid (n)
    int OK;
    char temmp;
    int sizeTempp=strlen(number)+1;
    char tem1[sizeTempp];
    for(int i=0;i<strlen(number);i++){
        temmp=number[i];
        if((temmp>=48 && temmp<=57))
            continue;
        else{
            printf("JUST INSERT NUMBER OF A PARAMETER NO LETTERS OR SYMBOLS!!!\n");
            return -1;
        }
    }
    strcpy(tem1,number);
    OK=atoi(tem1);
    return OK;
}
char* pathOF_R(char const *arr[],int from,int to,int sizePath){//this method creates the path complete
    
    char *path=(char*)malloc(sizePath*sizeof(char));
    int tempSIZE;
    char* temp;
    int i;
    if(to==0){
        strcat(path,"");
        return path;
    }
    else{
    strcat(path,"?");
    for(i=0;i<to;i++){
        strcat(path,arr[from]);
        if(i!=to-1)
            strcat(path,"&");
        from++;
    }
    }
    return path;
}
char* requestR(char const *arr[],int arrSize){//this method make the string of the path after to check all the cases not valids
    int val_n;
    int sizePath;
    char* textPath=NULL;
    int pathValid;
    int i;
    int k;
    for(i=1;i<arrSize;i++){
        if(strcmp("-r",arr[i])==0){
            val_n=checkingN(arr[i+1]);
            if (val_n==-1){
                return NULL;
            }
            else{
                k=i+2;
                sizePath=sizeRequestFunc(arr,k,val_n);//this is the new path size
                pathValid=pathISValid(arr,k,val_n);//0 if the path is valid, otherise -1
                if(pathValid==-1)
                    return NULL;
                if(pathISValid==0){
                    textPath=" ";
                    return textPath;
                }
                textPath=pathOF_R(arr,k,val_n,sizePath);
                return textPath;
            }
        }
    }
}
///////////////////////////////////////////////////////GETING REUEST WITH P///////////////////////////////////////////////////////////////////////////
int itsOK(char* str){//this function check if the text of the request Post its permmited or not return 0 if the text its valid otherwise return -1
    int OK=0;
    char temmp;
    for(int i=0;i<strlen(str);i++){
        temmp=str[i];
        if((temmp>=65 && temmp<=90)||(temmp>=97 && temmp<=122))
            continue;
        else{
            OK=-1;
            break;
        }
    }return OK;
}

char* requestPost(char const *arr[],int arrSize){//this method calls to the method itsOK() for to check if the text its permmited, if its permmited
                                                // return the text otherise print a message error
    char* textPost;
    int sizeTemp;
    int i;
    for(i=0;i<arrSize;i++){
        if(strcmp("-p",arr[i])==0){
            if(i==arrSize-1)
                return NULL;
            if(strstr(arr[i+1],"-r")!=NULL || strstr(arr[i+1],"http://")!=NULL)
                return NULL;
            
            sizeTemp=strlen(arr[i+1])+1;
            textPost=(char*)malloc(sizeTemp*sizeof(char));
            if (textPost==NULL){
                perror("Error: memory allocation failed!!!\n");
                return NULL;
            }
            strcpy(textPost,arr[i+1]);
            if(itsOK(textPost)==-1){
                fprintf(stderr,"The text contains chars not allwoed, please insert text without symbols and spaces\n");
                return NULL;
            }
            else return textPost;
        }
    }
}
//////////////////////////////////////  URL  ////////////////////////////////////////////////////////////////
int port_NEW(char* url){//this method get the port from the urlin the case 
    if(strchr(url,':')==NULL)
        return 80;
    int port;
    int counter=0;
    char *token = strtok(url, ":");

    while (token != NULL)
    {
        if(counter==1){
            if(checkingN(token)==-1){
                printf("THE PORT IS NOT VALID,PLEASE INSERT JUST NUMBERS!!!\n");

            }

            port=atoi(token);
            break;
        }

        token = strtok(NULL, ":");
        counter++;
    }
    return port;
}
int sizeToken(char *comm){ //this function returns the size of the URL array
    int count = 1;
    char *token = strtok(comm, "/");
    while (token != NULL)
    {
        count++;
        token = strtok(NULL, "/");
    }
    return count;
}
char **convert_to_Comm(char  *arrCom){ //this function converts the URL to array duo dimensional
    int arrcomSIZE=strlen(arrCom)+1;
    char temp[arrcomSIZE];
    strcpy(temp,arrCom);
    int sizeURL=sizeToken(arrCom);

    char **realCommand = (char **)malloc(sizeURL * sizeof(char *));
    if(realCommand==NULL){
        perror("ERROR memory allocation\n");
        return NULL;
    }
    char *token = strtok(temp, "/");
    int ind = 0;
    while (token != NULL)
    {
        realCommand[ind] = token;
        ind++;
        token = strtok(NULL, "/");
    }
    return realCommand;
}

char **requestHOST(char const *arr[],int arrSize){//return an array duo dimensional with the data of url
    char  **url;
    int tempSIZE;
    int i;
    for(i=1;i<arrSize;i++){
        if(strstr(arr[i],"http://")!=NULL){
            if(strchr(arr[i],'.')==NULL)
                return NULL;
            tempSIZE=strlen(arr[i])+1;
            char temp[tempSIZE];
            strcpy(temp,arr[i]);
            url=convert_to_Comm(temp);
            return url;
        }
    }
}

char *my_itoa(int num, char *str){//this method converts a int to string
        if(str == NULL)
        {
                return NULL;
        }
        sprintf(str, "%d", num);
        return str;
}
char* quit_port_from_host(char* host){//this function quit the port from the host string
    char *token = strtok(host, ":");
    return token;
}
void error(char* str){
    perror(str);
    exit(EXIT_FAILURE) ;
}

void freeing(char *arr[],char* reqP,char* reqR,char* servARR,char* number,char* request){//free the memory allocated
    if(reqP!=NULL){
        free(reqP);
        reqP=NULL;
    }
    if(reqR!=NULL){
        free(reqR);
        reqR=NULL;
    }
    if(arr!=NULL){
        int i=0;
        while (arr[i]!=NULL){
            arr[i]=NULL;
            i++;
        }
        free(arr);
        arr=NULL;
    }
    free(servARR);
    servARR=NULL;
    free(number);
    number=NULL;
    free(request);
    request=NULL;
   
}
//////////////////////////////////////////////////////MAINMAINMAINMAINMAINMAIN///////////////////////////////////////////////////
int main(int argc, char const *argv[]){
    char* request=serverARRAY(SIZEREQUEST);
    int rc;            /* system calls return value storage */
    int sockfd;
    char rbuf[BUFLEN];
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char* number=(char*)malloc(20*sizeof(char));
    char* stringP=NULL;
    char* stringR=NULL;
    char **url=NULL;
    int port=80;
    char* serverARR=NULL;
    int strLENHOst;
    ///////////////***************************************************///////////////////////
    if(check_exist_P_OR_R(argv,argc)==0){//thats means that -p and -r exist into the input

        stringP=requestPost(argv,argc);
        if(stringP==NULL){
            printINPUTERROR(argv,argc);
            freeing(url,stringP,stringR,serverARR,number,request);
            exit(1);
        }
        stringR=requestR(argv,argc);
        if(stringR==NULL){
            printINPUTERROR(argv,argc);
            freeing(url,stringP,stringR,serverARR,number,request);
            exit(1);
        }
        url=requestHOST(argv,argc);
        if(url==NULL){
            printINPUTERROR(argv,argc);
            freeing(url,stringP,stringR,serverARR,number,request);
            exit(1);
        }    
        strLENHOst=strlen(url[1])+1;////////////////////////////
        serverARR=serverARRAY(strLENHOst);
        strcpy(serverARR,url[1]);      

        if(strchr(url[1],':')!=NULL){
            port=port_NEW(url[1]);
            url[1]=quit_port_from_host(url[1]);
        }

        else{
            int i=0;
            int start=2;
            while(url[i]!=NULL)
                i++;
                               //in this level we make the reuest
            strcat(request,"POST ");
            int number1=strlen(stringP);
            if(i>2){
                while(start<i){
                    strcat(request,"/");
                    strcat(request,url[start]);
                    start++;
                }
                strcat(request,stringR);
                strcat(request," HTTP/1.0\r\nHOST:");
                strcat(request,url[1]);
                strcat(request,"\r\nContent-length:");
                my_itoa(number1,number);
                strcat(request,number);
                strcat(request,"\r\n\r\n");
                strcat(request,stringP);
                strcat(request,"\r\n\r\n");
            }
            else{
                strcat(request,"/");
                strcat(request,stringR);
                strcat(request," HTTP/1.0\r\nHOST:");
                strcat(request,url[1]);
                strcat(request,"\r\nContent-length:");
                my_itoa(number1,number);
                strcat(request,number);
                strcat(request,"\r\n\r\n");
                strcat(request,stringP);
                strcat(request,"\r\n\r\n");
            }
            printf("HTTP request =\n%s\nLEN = %ld\n", request, strlen(request));
        }
    }//////////////////////////////////////*******************************/////////////////////////////////////////
    if(check_exist_P_OR_R(argv,argc)==1){//this means that just -p exist 
        stringP=requestPost(argv,argc);
        if(stringP==NULL){
            printINPUTERROR(argv,argc);
            freeing(url,stringP,stringR,serverARR,number,request);
            exit(1);
        }
        url=requestHOST(argv,argc);
        if(url==NULL){
            printINPUTERROR(argv,argc);
            freeing(url,stringP,stringR,serverARR,number,request);
            exit(1);
        }
        strLENHOst=strlen(url[1])+1;////////////////////////////
        serverARR=serverARRAY(strLENHOst);
        strcpy(serverARR,url[1]);      

        if(strchr(url[1],':')!=NULL){
            port=port_NEW(url[1]);
            url[1]=quit_port_from_host(url[1]);
        }
        else{
            int i=0;
            int start=2;
            while(url[i]!=NULL)
                i++;
                               //in this level we make the reuest
            strcat(request,"POST ");
            int number1=strlen(stringP);
            if(i>2){
                while(start<i){
                    strcat(request,"/");
                    strcat(request,url[start]);
                    start++;
                }
                strcat(request," HTTP/1.0\r\nHOST:");
                strcat(request,url[1]);
                strcat(request,"\r\nContent-length:");
                my_itoa(number1,number);//getting number
                strcat(request,number);// put in at request the number
                strcat(request,"\r\n\r\n");
                strcat(request,stringP);
                strcat(request,"\r\n\r\n");
            }
            else{
                strcat(request,"/");
                strcat(request,stringR);
                strcat(request," HTTP/1.0\r\nHOST:");
                strcat(request,url[1]);
                strcat(request,"\r\nContent-length:");
                my_itoa(number1,number);
                strcat(request,number);
                strcat(request,"\r\n\r\n");
                strcat(request,stringP);
                strcat(request,"\r\n\r\n");
            }
            printf("HTTP request =\n%s\nLEN = %ld\n", request, strlen(request));
        }
    }////////////////////////////////////////////////////**************************************////////////////////////////////////////
    if(check_exist_P_OR_R(argv,argc)==2){//this means that just -r exist 
        stringR=requestR(argv,argc);
        if(stringR==NULL){
            printINPUTERROR(argv,argc);
            freeing(url,stringP,stringR,serverARR,number,request);
            exit(1);
        }
        url=requestHOST(argv,argc);
        if(url==NULL){
            printINPUTERROR(argv,argc);
            freeing(url,stringP,stringR,serverARR,number,request);
            exit(1);
        }        
        strLENHOst=strlen(url[1])+1;////////////////////////////
        serverARR=serverARRAY(strLENHOst);
        strcpy(serverARR,url[1]); 

        if(strchr(url[1],':')!=NULL){
            port=port_NEW(url[1]);
            url[1]=quit_port_from_host(url[1]);
        }               
        
        else{
            int i=0;
            int start=2;
            while(url[i]!=NULL)
                i++;
                               //in this level we make the reuest
            strcat(request,"GET ");
            if(i>2){
                while(start<i){
                    strcat(request,"/");
                    strcat(request,url[start]);
                    start++;
                }
                strcat(request,stringR);
                strcat(request," HTTP/1.0\r\nHOST:");
                strcat(request,url[1]);
                strcat(request,"\r\n\r\n");
              
            }
            else{
                strcat(request,"/");
                strcat(request,stringR);
                strcat(request," HTTP/1.0\r\nHOST:");
                strcat(request,url[1]);
                strcat(request,"\r\n\r\n");
                
            }
            printf("HTTP request =\n%s\nLEN = %ld\n", request, strlen(request));
        }
    }////////////////////////////////////////////////////****************************************//////////////////////////////////////////////
    if(check_exist_P_OR_R(argv,argc)==-1){//this means that -p and -r doesnt exist 
       
        url=requestHOST(argv,argc);
        if(url==NULL){
            printINPUTERROR(argv,argc);
            freeing(url,stringP,stringR,serverARR,number,request);
            exit(1);
        }
        strLENHOst=strlen(url[1])+1;////////////////////////////
        serverARR=serverARRAY(strLENHOst);
        strcpy(serverARR,url[1]);      

        if(strchr(url[1],':')!=NULL){
            port=port_NEW(url[1]);
            url[1]=quit_port_from_host(url[1]);
        }
        else{
            int i=0;
            int start=2;
            while(url[i]!=NULL)
                i++;
                               //in this level we make the reuest
            strcat(request,"GET ");
            if(i>2){
                while(start<i){
                    strcat(request,"/");
                    strcat(request,url[start]);
                    start++;
                }
                strcat(request," HTTP/1.0\r\nHOST:");
                strcat(request,url[1]);
                strcat(request,"\r\n\r\n");
              
            }
            else{
                strcat(request,"/");
                strcat(request," HTTP/1.0\r\nHOST:");
                strcat(request,url[1]);
                strcat(request,"\r\n\r\n");
                
            }
            printf("HTTP request =\n%s\nLEN = %ld\n", request, strlen(request));        }
    }
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        freeing(url,stringP,stringR,serverARR,number,request);
        error("socket failed");
    } 

    // connect to server
    server = gethostbyname(serverARR);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n"); 
        freeing(url,stringP,stringR,serverARR,number,request);
        exit(EXIT_FAILURE);
    }
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    rc = connect(sockfd,(const struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (rc < 0){
        error("connect failed:");
        freeing(url,stringP,stringR,serverARR,number,request);
    } 

	write(sockfd, request, strlen(request)+1) ;
    int bytesReaded=0;
    	rc = read(sockfd, rbuf,BUFLEN) ;
    while(rc > 0){
        bytesReaded+=rc;
        if(rc > 0)
	    	  printf("%s\n", rbuf) ;
	    else 
              error("read() failed") ;
        rc = read(sockfd, rbuf,BUFLEN) ;
    }
    close(sockfd);
    printf("\n Total received response bytes: %d\n",bytesReaded);
    freeing(url,stringP,stringR,serverARR,number,request);
    return EXIT_SUCCESS;
}
//request[0]='\0';
    //int argc=6;    
    //char const *argv[]={"./client", "-r" ,"3" ,"addr=jecrusalem" ,"tel=02-6655443" ,"age=23" ,"http://www.ptsv2.com/t/puebla/post","-p" ,"hksdk"};
    //char const *argv11[]={"./client","-r","3","addr=jecrusalem","tel=02-6655443","age=23","http://www.ptsv2.com/t/puebla/post"};
    //char const *argv[]={"./client","-p","text","http://www.ptsv2.com/t/puebla/post"};
    //char const *argv[]={"./client","-p","text","http://www.ptsv2.com","-r","2","pr2=value2","pr3=value3"};
    //char const *argv[]={"./client","-p","text","http://www.ptsv2.com/t/ex2"};
    //char const *argv[]={"./client","http://www.ptsv2.com"};
    //char const *argv[]={"./client","-r","2","pr2=value2","pr3=value3","http://wwwptsv2com/t/puebla/post"};
