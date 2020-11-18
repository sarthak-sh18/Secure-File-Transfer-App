#include <stdio.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <string.h> 
#include <stdint.h>
#include <openssl/aes.h>
#define CBC 1
#define PORT 8080 

 
/* AES key for Encryption and Decryption */
const static unsigned char aes_key[]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};


long long int power(int a,int b,int mod)
{
 long long int t;
 if(b==1)
  return a;
 t=power(a,b/2,mod);
 if(b%2==0)
  return (t*t)%mod;
 else
  return (((t*t)%mod)*a)%mod;
}


void print_data(const char *tittle, const void* data, int len)
{
	printf("%s : ",tittle);
	const unsigned char * p = (const unsigned char*)data;
	int i = 0;
	printf("Decrypted Data : \n");
	for (; i<len; ++i)
		printf("%02X ", *p++);
	printf("\n");
}

int main() 
{ 
    int g=5,n=23;
    struct sockaddr_in address; 
    int option = 1,server_fd, sock;
    int address_length = sizeof(address); 
    char msg[1024] = {0}; 
    char *msg_from_server = "Hello from server"; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        printf("Server Creation Failed\n");
	return 0;
    } 
       
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
    { 
        printf("Binding Failed\n");
		return 0;
    } 
    
    while(1)
    {
    		int in;
		printf("Press 1 for Exit 2 for Sending File : ");
		scanf("%d",&in);
		if(in==1)
			break;
		printf("Going in listening mode...\n");
		
            //Server now in listening with maximum backlog requests upto 5
	    if (listen(server_fd, 3) < 0) 
	    { 
		printf("Listening Failed\n");
		return 0; 
	    } 
    
    	    
	    if ((sock = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&address_length))<0) 
	    { 
	       printf("Connection can't be accepted\n");
	       return 0;
	    } 
	    int value = read( sock , msg, 1024); 
	    
	    printf("Recieving File %s\n",msg);
	    char qw[5];
	    	if(recv(sock,qw,5,0)<=0)
	    	return 0;
	    //	g=5;
	    	//n=23;
	    	int t2=power(g,6,n);
	    	//printf("")
	    	//printf("\nt2=%d\n",t2);
		    char b1[10];
		    sprintf(b1,"%d",t2);
		    if(send(sock,b1,sizeof(b1),0)<0)
		    {
		    	printf("Error in sending...\n");
		    	return 0;
		    }
	    	
	    	int c;
	    	c=atoi(qw);
	    	//printf("Recived=%d",c);
	    	int qq=power(c,6,n);
	    
	    //int n;
	    FILE *fp;
	    unsigned char enc_out[100];
	    
	    fp=fopen("recv.txt","w");
	    while(1)
	    {
	    
	    	unsigned char iv[AES_BLOCK_SIZE];
		memset(iv, 0x00, AES_BLOCK_SIZE);
		AES_KEY dec_key;
		/* Buffers for Encryption and Decryption */
	
	    	
	    	char a[4];
	    	if(recv(sock,a,4,0)<=0)
	    	break;	
	    	int b;
	    	b=atoi(a);
	    	
	    	
	    	
	    	
	    	unsigned char enc_out[b];
	unsigned char dec_out[b];
	
	    	if(recv(sock,enc_out,100,0)<=0)
	    		break;
	    	memset(iv, 0x00, AES_BLOCK_SIZE); // don't forget to set iv vector again, else you can't decrypt data properly
		AES_set_decrypt_key(aes_key, sizeof(aes_key)*8, &dec_key); // Size of key is in bits
		AES_cbc_encrypt(enc_out, dec_out, b, &dec_key, iv, AES_DECRYPT);
		int i=0;
		char temp[b];
		for(i=0;i<b;i++)
		{
			if(((char)dec_out[i]>='a'&&(char)dec_out[i]<='z')||((char)dec_out[i]>='A'&&(char)dec_out[i]<='Z')||((char)dec_out[i]>='0'||(char)dec_out[i]<='9'))
			temp[i]=(char)dec_out[i];
			else
			break;
		}
	    	for(i=0;i<b;i++)
	    	{
	    		printf("%c",temp[i]);
	    	}
	    	fprintf(fp,"%s",temp);
	    	bzero(enc_out,b);
	    }
	    fclose(fp);
	    
    }
    return 0; 
}


