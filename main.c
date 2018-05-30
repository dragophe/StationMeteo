#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>



int main(int argc, char ** argv) {
  int fd;

  FILE * fdp;

  // Open the Port. We want read/write, no "controlling tty" status, and open it no matter what state DCD is in
  fd = open("/dev/serial0", O_RDWR | O_NOCTTY);
  if (fd == -1) {
    perror("open_port: Unable to open /dev/ttyAMA0 - ");
    return(-1);
  }
  
		struct termios options;
			tcgetattr(fd, &options);
			options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
			options.c_iflag = IGNPAR;
			options.c_oflag = 0;
			options.c_lflag = 0;
			options.c_cc[VMIN]=18;
			tcflush(fd, TCIFLUSH);
			tcsetattr(fd, TCSANOW, &options);

  // Turn off blocking for reads, use (fd, F_SETFL, FNDELAY) if you want that
  fcntl(fd, F_SETFL, 0);

  // Read up to 255 characters from the port if they are there
  char buf[18];
  char tabtp[10];
  char tabhum[10];
  char tabpres[10];
  char tabvent[10];
  char tabeau[10];
  char tab[18];
  int i=0;
  int j=0;
  int n;
  int k=0;
 
  
 while(1)
  {
	  
	
	/*sprintf(buf,"25654785236951426");*/
	n = read(fd, (void*) buf, sizeof(buf));
	if (n < 0) {
		perror("Read failed - \n");
		return -1;
				} 
	else if (n == 0)
		printf("No data on port\n");
		
	else
	{
		buf[n] = '\0';
	}


	
	
	
	
	for(i = 0;i<n;i++)
	{
		if(buf[i]=='#')
		{
			k=i;
			for(j=0;j<n;j++)
			{
				
				if(k==18)
				{k=0;}
				tab[j]=buf[k];
				k++;
			}
		}
	}
	
	printf("%i bytes read : %s\n", n, tab);
	
	sprintf(tabtp,"%c%c,%c ",tab[1],tab[2],tab[3]);
	sprintf(tabhum,"%c%c%c ",tab[4],tab[5],tab[6]);
	sprintf(tabpres,"%c%c%c%c,%c ",tab[7],tab[8],tab[9],tab[10],tab[11]);
	sprintf(tabvent,"%c%c%c ",tab[12],tab[13],tab[14]);
	sprintf(tabeau,"%c%c,%c ",tab[15],tab[16],tab[17]);

	fdp=fopen ("/var/www/html/donnees.txt","w+");
	
	if (fdp != NULL)
	{
		printf("%c",tabvent[0]);
		printf ("ouvre\n");		
		fputs("Température : ",fdp);
		fputs(tabtp,fdp);
		fputs("°C \nHumidité : ",fdp);
		fputs(tabhum,fdp);
		fputs("% \nPression : ",fdp);
		fputs(tabpres,fdp);
		fputs("hPa \nVent : ",fdp);
		fputs(tabvent,fdp);
		fputs("km/h \nPrécipitation : ",fdp);
		fputs(tabeau,fdp);
		fputs("mm \n",fdp);
	}
	
	else 
	{
		printf ("ouvre pas\n");
	}
	/*while (i<n)
	{
		buf[i]="\0";
		i++;
	}*/
	

	
	fclose(fdp);
	sleep(5);
	

}


  // Don't forget to clean up
  close(fd);
  return 0;
}
