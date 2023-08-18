README
Author: Ytzhak Ariel Armoni 

HTTP CLIENT SIDE

===Description ===

Program files:
client.c - file that implementaion a  request server and its actions.


Funcation:

int check_exist_P_OR_R(char const * arr[],int arrSize)//this method chek if into argv exist -p or -r and return -1 if the them doesnt exist 1 if just -p exist, 2 if jus-r exist and 0 if both exist.

void printINPUTERROR(char const *arr[],int arrSize)//print if happends a input error

char* serverARRAY(int size)// this function make an array

int pathISValid(char const *arr[],int from,int to)//this method checks if the path its valid, return 0 if its valid otherwise return -1 

int checkingN(char const *number)//this method checks if the number of variables its valid (n)

char* pathOF_R(char const *arr[],int from,int to,int sizePath)//this method creates the path complete

char* requestR(char const *arr[],int arrSize)//this method make the string of the path after to check all the cases not valids

int itsOK(char* str)//this function check if the text of the request Post its permmited or not return 0 if the text its valid otherwise return -1

char* requestPost(char const *arr[],int arrSize){//this method calls to the method itsOK() for to check if the text its permmited, if its permmited return the text otherise print a message error

int port_NEW(char* url)//this method get the port from the urlin the case 

int sizeToken(char *comm) //this function returns the size of the URL array

char **convert_to_Comm(char  *arrCom) //this function converts the URL to array duo dimensional

char **requestHOST(char const *arr[],int arrSize)//return an array duo dimensional with the data of url

char *my_itoa(int num, char *str)//this method converts a int to string

char* quit_port_from_host(char* host)//this function quit the port from the host string

void freeing(char *arr[],char* reqP,char* reqR,char* servARR,char* number,char* request)//free the memory allocated





