#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stddef.h>

int main(){
  setuid(0);
  setgid(0);
  
  system("zypper in -y vsftpd");

  printf(
            "Content-type:text/html \n\n"
            "<html>\n"
            "<head>\n"
            "<META HTTP-EQUIV='REFRESH' CONTENT='5;URL=/cgi-bin/ftp'></META>\n"
            "<body>\n"
            "<p>Instalado</p>\n"
      );

  system("cp /srv/www/cgi-bin/install/vsftpd.conf /etc/vsftpd.conf");
  system("touch /etc/vsftpd.chroot_list");
}
