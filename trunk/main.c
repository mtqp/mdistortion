//http://www.opensound.com/pguide/audio.html
//http://www.opensound.com/pguide/audio2.html

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>

//#include <sys/soundcard.h>


#define B_SIZE    4096

/*
typedef struct _distor {
	char* _d_
}distor;
*/

int main(){
	int audio_fd;
	unsigned char _buffer[B_SIZE];
//	char* open_mode = "O_RDONLY";
	int open_flag = O_RDWR;//O_RDONLY;
	
	if ((audio_fd = open("/dev/dsp", open_flag, 0)) == -1)  
		{ /* no pudo abrir la placa */  
		printf("no se puedo abrir la placa de sonido!!\n");
		}
	else {
		printf("abrio bien la placa\n");
	}
	
	int cant_b;
while(1){
read(audio_fd, _buffer, B_SIZE);
write(audio_fd, _buffer, B_SIZE);
	/*if ((cant_b = read(audio_fd, _buffer, B_SIZE)) == -1)  
	{  
		perror("audio read");  
		//exit(Error code);  
	}
	printf ("se leyo del buffer de sonido: %d\n",cant_b);

	if ((cant_b = write(audio_fd, _buffer, B_SIZE)) == -1)  
	{  
		perror("audio write");  
		//exit(Error code);  
	}*/
	printf("se deberia haber esuchado algun sonido\n");
}
	return 0;
}
