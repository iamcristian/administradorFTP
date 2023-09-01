#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stddef.h>

int main(){
  // Cambiando nuevo ID y GID al superuser
  setuid(0);
  setgid(0);

  char *cadena = getenv("QUERY_STRING"); 
  char buffer[256];
  char accion[80];
  
  strncpy(buffer, cadena, 255);
  sscanf(buffer, "accion=%s", accion);
  
  if (strcmp(accion,"iniciar")==0){
      system("service vsftpd start");
  }else if (strcmp(accion,"parar")==0){
      system("service vsftpd stop");
  }else{
      system("service vsftpd restart");
  } 

  printf(
	    "Content-type:text/html \n\n" 
	    "<html>\n"
	    "<head>\n"
	    "<META HTTP-EQUIV='REFRESH' CONTENT='5;URL=/cgi-bin/ftp'></META>\n"
	    "<body>\n"
	    "<p>%s el servicio vsftpd exitoso</p>\n", accion
      );
  printf(
      "</body>\n"
      "</html>\n"
      );

  return 0;
}

