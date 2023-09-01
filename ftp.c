#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

void head();
void body(); 
void end();
void estados();
char llenarEstados[20][100];

int main(){
setuid(0);
setgid(0);

 char *estado;
 FILE *vsftpd;
 vsftpd = fopen("/etc/vsftpd.conf","r");
 //verifica si esta instalado el vsftpd
  if(vsftpd == NULL){
    estado = "Estado vsftp: no instalado vsftpd, se procedera a instalar...";
  }else{
    estado = "Estado vsftp: instalado";
    fclose(vsftpd);
  }

  estados();
  head();
  body(estado);
  end();
 return 0;
}

void head(){
  printf(
	  "Content-type:text/html\n\n"
	  "<html>\n"
	  "<head>\n"
	    "<meta charset='UTF-8'>\n"
	    "<title>Administrador ftp</title>\n"
	    "<meta name='viewport' content='width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0'>\n"
	    "<link type='text/css' rel='stylesheet' href='/estilos.css'>\n"
	    "<link href='http://fonts.googleapis.com/css?family=Oswald:400,700' rel='stylesheet' type='text/css'>\n"
	  "</head>\n"
  );
}

void body(char *estadoftp){
  char selectedYES[]="<option selected>YES</option><option>NO</option>";
  char selectedNO[]="<option>YES</option><option selected>NO</option>";
  if(strcmp(estadoftp,"Estado vsftp: instalado")==0){
      printf(
          "<body>\n"
	  "<div id='particles-js'></div>\n"
	  "<header>"
		"<div class='menu'>"
			"<div class='contenedor'>"
				"<div class='logo'>Administrador FTP</div>"
				"<nav>"
					"<ul>"
						"<li><a href='/cgi-bin/ftp'>Inicio</a></li>"
					"</ul>"
				"</nav>"
			"</div>"
		"</div>"
	  "</header>"
	  "<div class='texto'>"
			"<h2>Configuracion del servicio FTP</h2>"
			"<br>"
			"<p>(File Transfer Protocol)</p>"
		"</div>"
      "<div class='principal contenedor'>"
          //verificacion de instalacion vsftpd
          "<br><br><h2>Verificacion de instalacion vsftpd</h2>"
          "<p>%s</p><br>\n", estadoftp
  );
  }else{
      printf(
	  "<body>\n"
	   "<div id='particles-js'></div>\n"
	  "<header>"
		"<div class='menu'>"
			"<div class='contenedor'>"
				"<div class='logo'>Administrador FTP</div>"
				"<nav>"
					"<ul>"
						"<li><a href='/cgi-bin/ftp'>Inicio</a></li>"
					"</ul>"
				"</nav>"
			"</div>"
		"</div>"
	  "</header>"
	  "<div class='texto'>"
			"<h2>Configuracion del servicio FTP</h2>"
			"<br>"
			"<p>(File Transfer Protocol)</p>"
		"</div>"
      "<div class='principal contenedor'>"
	  //verificacion de instalacion vsftpd
	  "<br<br><h2>Verificacion de instalacion vsftpd</h2>"
	  "<form action='/cgi-bin/instalar.cgi' method='get'>\n"
	  "<p>%s</p><br><input type='submit' value='instalar'></form><br>\n", estadoftp
      ); 
  }
  printf(
	  //configuracion de arranque
	  "<h2>Configuracion de arranque</h2>\n"
	  "<form action='/cgi-bin/arranque.cgi' method='get'>\n"
	  "<p>Seleccione la opcion a modificar con el arranque\n"
	  "<select name='arranque'>\n"
	  "<option>enable</option>\n"
	  "<option>disable</option>\n"
	  "</select>\n"
	  "<input type='submit' value='Modificar'></p></form><br>\n"
      );
  char status[10];
  FILE *fp = popen("service vsftpd status | awk '{print $2}' | grep active*","r");
  fgets(status,10,fp);
  fclose(fp);
  printf(
	  //modificar estado vsftpd
	  "<h2>Modificar estado vsftpd</h2>\n"
	  "<form action='/cgi-bin/servicio.cgi' method='get'>\n"
	  "<p>Estado actual del servicio: %s\n"
	  "<br>Seleccione la accion a realizar con el servicio<select name='accion'>\n"
	  "<option>iniciar</option>\n"
	  "<option>parar</option>\n"
	  "<option>reiniciar</option>\n"
	  "</select>\n"
	  "<input type = 'submit' value='Modificar'></p></form><br>\n", status
      );
  printf(
	  //configuraciones generales   matriz[0,1,2]
	  "<h2>Configuraciones Generales</h2>\n"
          "<form action='/cgi-bin/write.cgi' method='get'>\n"
          "<p>write_enable=\n"
	  "<select name='write_enable'>\n");
          if(strcmp(llenarEstados[0],"write_enable=YES\n") == 0){printf(selectedYES);}
	  else{printf(selectedNO);}
  printf( "</select><br>\n"
	  "dirmessage_enable=\n"
	  "<select name='dirmessage_enable'>\n");
	  if(strcmp(llenarEstados[1],"dirmessage_enable=YES\n") == 0){printf(selectedYES);}
          else{printf(selectedNO);}
  printf( "</select><br>\n"
	  "local_enable=\n"
	  "<select name='local_enable'>");
	  if(strcmp(llenarEstados[2],"local_enable=YES\n") == 0){printf(selectedYES);}
          else{printf(selectedNO);}
  printf( "</select><br>\n"
          "<input type = 'submit' value='Modificar'></p></input>\n"
          "</form>\n<br>"
      );
  printf(
	  //enjaular usuario   matriz[3,4,5]
	  "<h2>Enjaular usuarios</h2>\n"
	    "<form action='/cgi-bin/enjaular.cgi' method='get'>\n"
	  "<p>\n"
	    "chroot_local_user=\n"
	    "<select name='chroot_local_user'>\n");
	    if(strcmp(llenarEstados[3],"chroot_local_user=YES\n") == 0 || strcmp(llenarEstados[3],"#chroot_local_user=YES") == 0){
		printf(selectedYES);
	    }else{printf(selectedNO);}
  printf(   "</select>\n"
	    "<br>chroot_list_enable=\n"
	    "<select name='chroot_list_enable'>\n");
	    if(strcmp(llenarEstados[4],"chroot_list_enable=YES\n") == 0 || strcmp(llenarEstados[4],"#chroot_list_enable=YES") == 0){
                printf(selectedYES);
            }else{printf(selectedNO);}
  printf("</select>\n"
	    "<br>allow_writeable_chroot=\n"
	    "<select name='allow_writeable_chroot'>\n");
	    if(strcmp(llenarEstados[5],"allow_writeable_chroot=YES\n") == 0 || strcmp(llenarEstados[5],"#allow_writeable_chroot=YES") == 0){
                printf(selectedYES);
            }else{printf(selectedNO);}
  printf("</select>\n"
	    "<br>chroot_list_file=/etc/vsftpd.chroot_list"
	"<br><input type='submit' value='Modificar'>\n"
	"<a href='https://ibb.co/pxfpk79' target='_blank'>help</a>\n"
	"</p></form><br>\n"
      );
    //Cargar todos los usuarios disponibles
    int maxima=100;
    char buffer[maxima];

    int indiceUsers=0;
    char Users[maxima][maxima];
    FILE *Usuarios=popen(" awk  -F: '$NF!~/false$/ && $NF!~/nologin$/{print $1}' /etc/passwd","r");
    while (fgets(buffer, maxima, Usuarios)){
        memcpy(Users[indiceUsers],buffer,maxima);
        indiceUsers++;
    }
    fclose(Usuarios);

    char Lista[maxima][maxima];
    int indiceList=0;
    FILE *VerifList = popen("awk '{print $1}' /etc/vsftpd.chroot_list","r");
    while(fgets(buffer, maxima, VerifList)){
    	memcpy(Lista[indiceList],buffer,maxima);
	indiceList++;
    }
    fclose(VerifList);

    char UsersDisp[maxima][maxima];
    int indiceUsersDisp=0;
    int verif=1;	//1 si no esta en la lista, añadir| 0 si ya esta en la lista, no añadir   
    for(int i=0; i<indiceUsers; i++){
        for(int j=0; j<indiceList && verif; j++){
	    if(strcmp(Users[i],Lista[j])==0){
	        verif=0;
	    }
	}
	if(verif){
	    memcpy(UsersDisp[indiceUsersDisp],Users[i],maxima);
            indiceUsersDisp++;
	}
	verif=1;
    }

    //Cargar todos los usuarios del .chroot_list
    char UserDele[maxima][maxima];
    int indiceUserD=0;
    FILE *UserList=popen("awk '{print $1}' /etc/vsftpd.chroot_list","r");
    while (fgets(buffer, maxima, UserList)){
        memcpy(UserDele[indiceUserD],buffer,maxima);
        indiceUserD++;
    }
    fclose(UserList);
  printf(
	//modificar usuarios de archivo vsftpd.chroot_list
	"<h2>Modificar usuarios de archivo vsftpd.chroot_list</h2>\n"
	"<form action='/cgi-bin/usuarios.cgi' method='get'>\n"
	"<p>\n"
	  "Agregar usuario en chroot_list\n"
	  "<select name='add_usuario'>\n"
		"<option selected>vacio</option>\n");
  	for(int i=0; i<indiceUsersDisp; i++){
		printf("<option>");
		printf(UsersDisp[i]);
		printf("</option>\n");
	}
  printf(
	  "</select>\n"
	  "<br>Quitar usuario de chroot_list\n"
	  "<select name='rm_usuario'>\n"
		"<option selected>vacio</option>\n");
  	for(int i=0; i<indiceUserD; i++){
		printf("<option>");
		printf(UserDele[i]);
		printf("</option>\n");
	}
  printf( "</select>\n"
	  "<br><input type='submit' value='Modificar usuarios'>\n"
	"</p>\n"
	"</form><br>\n"
    );
  printf(
	//anonimos   matriz[6,7]
	"<h2>Configuracion de usuario anonimo</h2>\n"
	"<form action='/cgi-bin/anonymous.cgi' method='get'>\n"
	"<p>\n"
	"anonymous_enable=\n"
	"<select name='anonymous_enable'>\n");
	if(strcmp(llenarEstados[6],"anonymous_enable=YES\n") == 0){printf(selectedYES);}
	else{printf(selectedNO);}
  printf("</select><br>\n"
	"anon_world_readable_only=\n"
	"<select name='anon_world_readable_only'>\n");
	if(strcmp(llenarEstados[7],"anon_world_readable_only=YES\n") == 0){printf(selectedYES);}
        else{printf(selectedNO);}
  printf("</select><br>\n"
	"<input type='submit' value='Modificar usuarios'>\n"
	"</p></form><br>\n"
      );
  printf(
	//configuracion log  matriz[8,9,10], [18] para log_file
	"<h2>Configuracion log</h2>\n"
	"<form action='/cgi-bin/log.cgi'>\n"
		"<p>\n"
			"syslog_enable=\n"
			"<select name='syslog_enable'>\n");
			if(strcmp(llenarEstados[8],"syslog_enable=YES\n") == 0){printf(selectedYES);}
            		else{printf(selectedNO);}
  printf(		"</select><br>\n"
			"log_ftp_protocol=\n"
			"<select name='log_ftp_protocol'>\n");
				if(strcmp(llenarEstados[9],"log_ftp_protocol=YES\n") == 0 || strcmp(llenarEstados[9],"#log_ftp_protocol=YES") == 0){
				    printf(selectedYES);
				}else{printf(selectedNO);}
  printf(		"</select><br>\n"
			"xferlog_enable=\n"
			"<select name='xferlog_enable'>\n");
				if(strcmp(llenarEstados[10],"xferlog_enable=YES\n") == 0 || strcmp(llenarEstados[10],"#xferlog_enable=YES") == 0){
                                    printf(selectedYES);
                                }else{printf(selectedNO);}
  printf(			"</select><br>\n"
			"vsftpd_log_file=var/log/vsftpd.log "
			"<select name='vsftpd_log_file'>\n");
				if(strcmp(llenarEstados[18],"vsftpd_log_file=var/log/vsftpd.log\n")==0){
				    printf("<option selected>enable</option><option>disable</option>");
				}else{printf("<option>enable</option><option selected>disable</option>");}
  printf(		"</select>\n"
		  	"<br><input type='submit' value='Modificar'>\n"
		"</p>\n"
	"</form><br>\n"
      );
  printf(
	//configuracion de transferencia  matriz[11-17]
	"<h2>Configuracion de transferencia</h2>\n"
	"<form action='/cgi-bin/transfer.cgi'>\n"
		"<p>\n"
			"connect_from_port_20=\n"
			"<select name='connect_from_port_20'>\n");
				if(strcmp(llenarEstados[11],"connect_from_port_20=YES\n") == 0){printf(selectedYES);}
                        	else{printf(selectedNO);}
  printf(			"</select><br>\n"
			"ascii_upload_enable=\n"
			"<select name='ascii_upload_enable'>\n");
				if(strcmp(llenarEstados[12],"ascii_upload_enable=YES\n") == 0){printf(selectedYES);}
	                        else{printf(selectedNO);}
  printf(		"</select><br>\n"
			"listen=\n"
			"<select name='listen'>\n");
				if(strcmp(llenarEstados[13],"listen=YES\n") == 0){printf(selectedYES);}
	                        else{printf(selectedNO);}
  printf(		"</select><br>\n"
			"listen_ipv6=\n"
			"<select name='listen_ipv6'>\n");
				if(strcmp(llenarEstados[14],"listen_ipv6=YES\n") == 0){printf(selectedYES);}
                        	else{printf(selectedNO);}
  printf(		"</select><br>\n"
			"ssl_enable=\n"
			"<select name='ssl_enable'>\n");
				if(strcmp(llenarEstados[15],"ssl_enable=YES\n") == 0){printf(selectedYES);}
	                        else{printf(selectedNO);}
  printf(		"</select><br>\n"
			"pasv_min_port=\n"
			"<input name='pasv_min_port' value='%s'>\n"
			"<br>\n"
			"pasv_max_port=\n"
			"<input name='pasv_max_port' value='%s'>\n"
			"<br><input type='submit' value='Modificar'>\n"
		"</p>"
	"</form>"
	"</div>"
	"<script src='http://code.jquery.com/jquery-latest.js'></script>"
	"<script src='/particles.min.js'></script>"
	"<script src='/particulas.js'></script>",llenarEstados[16], llenarEstados[17]
      );
}

void end(){
  printf(
	  "</body>\n"
	  "</html>\n"
  );
}

void estados(){
  FILE *fp; 
  fp = popen("grep -E ^'write_enable=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[0],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'dirmessage_enable=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[1],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'local_enable=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[2],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'chroot_local_user=*|#chroot_local_user=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[3],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'chroot_list_enable=*|#chroot_list_enable=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[4],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'allow_writeable_chroot=*|#allow_writeable_chroot=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[5],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'anonymous_enable=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[6],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'anon_world_readable_only=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[7],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'syslog_enable=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[8],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'log_ftp_protocol=*|#log_ftp_protocol=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[9],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'xferlog_enable=*|#xferlog_enable=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[10],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'connect_from_port_20=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[11],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'ascii_upload_enable=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[12],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'listen=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[13],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'listen_ipv6=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[14],100,fp);
  fclose(fp);
  fp = popen("grep -E ^'ssl_enable=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[15],100,fp);
  fclose(fp);
  fp = popen("grep pasv_min_port=^'*' /etc/vsftpd.conf | awk -v FS='(pasv_min_port=)' '{print $2}'","r");
  fgets(llenarEstados[16],100,fp);
  fclose(fp);
  fp = popen("grep pasv_max_port=^'*' /etc/vsftpd.conf | awk -v FS='(pasv_max_port=)' '{print $2}'","r");
  fgets(llenarEstados[17],100,fp);
  fclose(fp);
  //Para el log
  fp = popen("grep -E ^'vsftpd_log_file=*|#vsftpd_log_file=*' /etc/vsftpd.conf","r");
  fgets(llenarEstados[18],100,fp);
  fclose(fp);
}

