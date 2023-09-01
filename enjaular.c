#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>

int main(){
  
  setuid(0);
  setgid(0);

  //obtenemos el estado actual de chroot_local_user
  FILE* chroot_local = popen("grep -E ^'chroot_local_user=*|#chroot_local_user=*' /etc/vsftpd.conf","r");
  char local[80];
  fgets(local,80,chroot_local);
  fclose(chroot_local);
  
  //obtenemos el estado actual de chroot_list_enable
  FILE* chroot_list = popen("grep -E ^'chroot_list_enable=*|#chroot_list_enable=*' /etc/vsftpd.conf","r");
  char list[80];
  fgets(list,80,chroot_list);
  fclose(chroot_list);

  //obtenemos el estado actual de allow_writeable_chroot
  FILE* allow_writeable = popen("grep -E ^'allow_writeable_chroot=*|#allow_writeable_chroot=*' /etc/vsftpd.conf","r");
  char allow[80];
  fgets(allow,80,allow_writeable);
  fclose(allow_writeable);
 
  //obtenemos el estado actual de chroot_list_file = /etc/vsftpd.chroot_list  **************+
  FILE* chroot_fileGrep = popen(" grep -E ^'chroot_list_file=*|#chroot_list_file=*' /etc/vsftpd.conf","r");
  char chroot_fileActual[80];
  fgets(chroot_fileActual,80,chroot_fileGrep);
  fclose(chroot_fileGrep);

  //variables url
  char *qstr;
  qstr = getenv("QUERY_STRING");
  char Buffer[256];
  char *token;
  char url_local[80], url_list[80], url_allow[80];

  strncpy(Buffer,qstr,255);
  token = strtok(Buffer,"&");
  sscanf(token,"%s",url_local);
  strcat(url_local,"\n");

  token = strtok(NULL,"&");
  sscanf(token,"%s",url_list);
  strcat(url_list,"\n");

  token = strtok(NULL,"&");
  sscanf(token,"%s",url_allow);
  strcat(url_allow,"\n");

  //abre el archivo vsftpd.conf
  FILE *fp = fopen("/etc/vsftpd.conf","r");
  int maximo = 1000;
  char buffer[maximo];
  char archivo[maximo][maximo];
  

  //actualizamos valores de vsftpd
  int i = 0;
  if(fp != NULL){
    while(fgets(buffer,maximo,fp)){
      if(strcmp(buffer,local) == 0){
	memcpy(archivo[i],url_local,maximo);
      }else if(strcmp(buffer,list)==0){
	memcpy(archivo[i],url_list,maximo);
      }else if(strcmp(buffer,allow)==0){
	  memcpy(archivo[i],url_allow,maximo);  
      }else if(strcmp(buffer,chroot_fileActual)==0){
	  char var[] = "chroot_list_file=/etc/vsftpd.chroot_list\n";
	  memcpy(archivo[i],var,maximo);
      }else{
	memcpy(archivo[i],buffer,maximo);
      }
      i++;
    }
  }
  fclose(fp);

  //escribe en el archivo
  FILE *fp2 = fopen("/etc/vsftpd.conf","w");
  for(int x=0;x<i;x++){
    fprintf(fp2,archivo[x]);
  }
  fclose(fp2);

  printf(
            "Content-type:text/html \n\n"
            "<html>\n"
            "<head>\n"
            "<META HTTP-EQUIV='REFRESH' CONTENT='5;URL=/cgi-bin/ftp'></META>\n"
            "<body>\n"
            "<p>de la url<br>\n"
	    "%s<br>\n"
	    "%s<br>\n"
	    "%s<br>\n"
	    "de grep<br>\n"
	    "%s<br>%s<br>%s<br>"
	    , url_local, url_list,url_allow, local,list,allow
  );
  printf(
      "</body>\n"
      "</html>\n"
  );
  system("service vsftpd restart");
  return 0;
}
