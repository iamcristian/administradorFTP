#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stddef.h>

int main(){
    setuid(0);
    setgid(0);

    //Recuperando URL
    char *cadena = getenv("QUERY_STRING");
    char inputBuffer[256];
    char *syslog_enable;
    char *log_ftp_protocol; //Comentada inicialmente
    char *xferlog_enable;   //Comentada inicialmente
    char *estado_vsftpd_log;  //Comentada inicialmente
    char valor_vsftpd_log_file[80];
    char *vsftpd_log_file;

    strncpy(inputBuffer, cadena, 255);
    syslog_enable = strtok(inputBuffer,"&");
    log_ftp_protocol = strtok(NULL,"&");
    xferlog_enable = strtok(NULL,"&");
    estado_vsftpd_log = strtok(NULL,"&");
    sscanf(estado_vsftpd_log,"vsftpd_log_file=%s",valor_vsftpd_log_file);

    if(strcmp(valor_vsftpd_log_file,"enable") == 0){
        vsftpd_log_file="vsftpd_log_file=var/log/vsftpd.log";
    }else{
        vsftpd_log_file="#vsftpd_log_file=var/log/vsftpd.log";
    }

    //Variables para modificar archivo .conf
    int maxima=1000; // tamaño de cada fila y columna
    char matriz[maxima][maxima]; // Matriz para guardar(cargar) el archivo vsftpd.conf
    int indice=0;
    char buffer[maxima];   // recupera cada fila del archivo .conf para guardar en matriz

    //Obtenemos el estado actual de syslog_enable
    FILE *syslogGrep = popen("grep -E ^'syslog_enable=*' /etc/vsftpd.conf","r");
    char syslogActual[80];
    fgets(syslogActual,80,syslogGrep);
    fclose(syslogGrep);

    //Obtenemos el estado actual de log_ftp_protocol (COMENTADO)
    FILE *logGrep = popen("grep -E ^'log_ftp_protocol=*|#log_ftp_protocol=*' /etc/vsftpd.conf","r");
    char logActual[80];
    fgets(logActual,80,logGrep);
    fclose(logGrep);

    //Obtenemos el estado actual de xferlog_enable
    FILE *xferlogGrep = popen("grep -E ^'xferlog_enable=*|#xferlog_enable=*' /etc/vsftpd.conf","r");
    char xferlogActual[80];
    fgets(xferlogActual,80,xferlogGrep);
    fclose(xferlogGrep);
    
    //Obtenemos el estado actual de vsftpd_log_file
    FILE *vsftpdGrep = popen("grep -E ^'vsftpd_log_file=*|#vsftpd_log_file=*' /etc/vsftpd.conf","r");
    char vsftpdActual[80];
    fgets(vsftpdActual,80,vsftpdGrep);
    fclose(vsftpdGrep);

    //Cargar todo el documento de configuracion en la variable matriz
    FILE *fp1 = fopen("/etc/vsftpd.conf","r"); //Abrir el archivo .conf (Necesario modo superuser)
    if(fp1 == NULL){
        printf("<p>Error al abrir</p><br>\n");  //Instalar****Codigo Cris
    }else{
        while (fgets(buffer, maxima, fp1 )){      //Mientras haya lineas por leer almacenandolas en el buffer
            memcpy(matriz[indice],buffer,maxima); //Copiar todo el archivo al array(matriz)
            indice++;
        }
    }
    fclose(fp1);

    FILE *fp2 = fopen("/etc/vsftpd.conf","w"); // para escribir borrando todo lo que ya habia (Necesario modo superuser)
    if(fp2 == NULL){
        printf("<p>Error al abrir</p>\n");
    }else{
        for (int i = 0; i < indice; i++){ //Sobre escribir el archivo de configuracion cambiando el valor
            if((strcmp(matriz[i],syslogActual) != 0) && (strcmp(matriz[i],logActual) != 0) && (strcmp(matriz[i],xferlogActual) != 0) 
	    && (strcmp(matriz[i],vsftpdActual)!=0)){
	        fprintf(fp2,matriz[i]); //Copiamos menos la variable que cambia
            }else if(strcmp(matriz[i],syslogActual) == 0){
                fprintf(fp2,syslog_enable);//Reemplazamos la variable cambiada
                fprintf(fp2,"\n");
            }else if(strcmp(matriz[i],logActual) == 0){
                fprintf(fp2,log_ftp_protocol);
                fprintf(fp2,"\n");
            }else if(strcmp(matriz[i],xferlogActual) == 0){
                fprintf(fp2,xferlog_enable);
                fprintf(fp2,"\n");
            }else{
	    	fprintf(fp2,vsftpd_log_file);
                fprintf(fp2,"\n");
	    }
        }
    }
    fclose(fp2);
    printf(
            "Content-type:text/html \n\n"
            "<html>\n"
            "<head>\n"
            "<META HTTP-EQUIV='REFRESH' CONTENT='5;URL=/cgi-bin/ftp'></META>\n"
            "<body>\n"
            "<p>Cambio realizado: %s</p>\n"
            "<br><p>Cambio realizado: %s</p>\n"
	    "<br><p>Cambio realizado: %s</p>\n"
            "<br><p>Cambio realizado vsftpd_log_file=var/log/vsftpd.log : %s </p>\n", syslog_enable, log_ftp_protocol, xferlog_enable, valor_vsftpd_log_file
    );
    printf(
      "</body>\n"
      "</html>\n"
    );
    system("service vsftpd restart");
    return 0;
}

