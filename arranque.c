#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stddef.h>

int main(){
  setuid(0);
  setgid(0);

  char *cadena = getenv("QUERY_STRING");
  char buffer[256];
  char arranque[80];

  strncpy(buffer,cadena,255);
  sscanf(buffer,"arranque=%s", arranque);

  if(strcmp(arranque,"enable")==0){
    system("systemctl enable vsftpd");
  }else{
    system("systemctl disable vsftpd");
  }

  printf(
	    "Content-type:text/html \n\n" 
	    "<html>\n"
	    "<head>\n"
	    "<META HTTP-EQUIV='REFRESH' CONTENT='5;URL=/cgi-bin/ftp'></META>\n"
	    "<body>\n"
	    "<p>systemctl %s vsftpd exitoso </p>\n", arranque
      );
  printf(
      "</body>\n"
      "</html>\n"
      );

return 0;
}
