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
	printf("Ecrypted Data : \n");
	for (; i<len; ++i)
		printf("%02X ", *p++);
	
	printf("\n");
}
char * encrypt(char*line)
{
	//cout<<"hey";
	for(int i=0;line[i]!='\n';i++)
	{
		line[i]=(line[i]+14)%26;
	}
	printf("In e %s\n",line);
	return line;
}
int main() 
{ 
    int sock1 = 0, value,g=5,n=23; 
    struct sockaddr_in serv_addr; 
    char *msg_from_client = "file.txt"; 
    char msg[1024] = {0}; 
    if ((sock1 = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; //ipv4
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
       printf("Invalid Address\n");
		return 0;
    } 
   
    if (connect(sock1, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("Connection Failed\n");
		return 0;
    } 
    
    send(sock1 , msg_from_client , strlen(msg_from_client) ,0); 
    
    int t=power(g,15,n);
    //printf("\nt=%d\n",t);
    char b1[10];
    sprintf(b1,"%d",t);
    if(send(sock1,b1,sizeof(b1),0)<0)
    {
    	printf("Error in sending...\n");
    	return 0;
    }
    char qw[10];
    if(recv(sock1,qw,5,0)<=0)
    return 0;
    int c1;
    c1=atoi(qw);
    //printf("\nRecieved Key=%d\n",c1);
    //int qq=power(c1,15,n);
    
    
    FILE *fp;
    fp=fopen("file.txt","r");
    unsigned char line[100];
    
    while(fgets(line,100,fp)!=NULL)
    {
    char a[2];
    sprintf(a,"%lu",strlen(line));
     if(send(sock1,a,sizeof(a),0)<0)
    	{
    		printf("Error in sending...\n");
    		return 0;
    	}
    unsigned char iv[AES_BLOCK_SIZE];
	memset(iv, 0x00, AES_BLOCK_SIZE);
	
	/* Buffers for Encryption and Decryption */
	unsigned char aes_input[100];
	strcpy(aes_input,line);

	unsigned char enc_out[sizeof(aes_input)];
	unsigned char dec_out[sizeof(aes_input)];
	
	/* AES-128 bit CBC Encryption */
	AES_KEY enc_key, dec_key;
	AES_set_encrypt_key(aes_key, sizeof(aes_key)*8, &enc_key);
	AES_cbc_encrypt(aes_input, enc_out, sizeof(aes_input), &enc_key, iv, AES_ENCRYPT);

	print_data("\n Encrypted",enc_out, sizeof(enc_out));
	
    	if(send(sock1,enc_out,sizeof(enc_out),0)<0)
    	{
    		printf("Error in sending...\n");
    		return 0;
    	}
    	bzero(line,100);
    	
    }
    fclose(fp);
   
    
    return 0; 
}


