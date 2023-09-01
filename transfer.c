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
    char *connect_from_port_20;
    char *ascii_upload_enable;
    char *listen;
    char *listen_ipv6;
    char *ssl_enable;
    char *pasv_min_port;
    char *pasv_max_port;

    strncpy(inputBuffer, cadena, 255);
    connect_from_port_20 = strtok(inputBuffer,"&");
    ascii_upload_enable = strtok(NULL,"&");
    listen = strtok(NULL,"&");
    listen_ipv6 = strtok(NULL,"&");
    ssl_enable = strtok(NULL,"&");
    pasv_min_port = strtok(NULL,"&");
    pasv_max_port = strtok(NULL,"&");

    //Variables para modificar archivo .conf
    int maxima=1000; // tamaño de cada fila y columna
    char matriz[maxima][maxima]; // Matriz para guardar(cargar) el archivo vsftpd.conf
    int indice=0;
    char buffer[maxima];   // recupera cada fila del archivo .conf para guardar en matriz

    //Obtenemos el estado actual connect_from_port_20
    FILE *connectGrep = popen("grep -E ^'connect_from_port_20=*' /etc/vsftpd.conf","r");
    char connectActual[80];
    fgets(connectActual,80,connectGrep);
    fclose(connectGrep);

    //Obtenemos el estado actual de ascii_upload_enable
    FILE *asciiGrep = popen("grep -E ^'ascii_upload_enable=*' /etc/vsftpd.conf","r");
    char asciiActual[80];
    fgets(asciiActual,80,asciiGrep);
    fclose(asciiGrep);

    //Obtenemos el estado actual de listen
    FILE *listenGrep = popen("grep -E ^'listen=*' /etc/vsftpd.conf","r");
    char listenActual[80];
    fgets(listenActual,80,listenGrep);
    fclose(listenGrep);

    //Obtenemos el estado actual de listen_ipv6
    FILE *listen_ipv6Grep = popen("grep -E ^'listen_ipv6=*' /etc/vsftpd.conf","r");
    char listen_ipv6Actual[80];
    fgets(listen_ipv6Actual,80,listen_ipv6Grep);
    fclose(listen_ipv6Grep);

    //Obtenemos el estado actual de ssl_enable
    FILE *sslGrep = popen("grep -E ^'ssl_enable=*' /etc/vsftpd.conf","r");
    char sslActual[80];
    fgets(sslActual,80,sslGrep);
    fclose(sslGrep);

    //Obtenemos el estado actual de pasv_min_port
    FILE *minGrep = popen("grep -E ^'pasv_min_port=*' /etc/vsftpd.conf","r");
    char minActual[80];
    fgets(minActual,80,minGrep);
    fclose(minGrep);

    //Obtenemos el estado actual de pasv_max_port
    FILE *maxGrep = popen("grep -E ^'pasv_max_port=*' /etc/vsftpd.conf","r");
    char maxActual[80];
    fgets(maxActual,80,maxGrep);
    fclose(maxGrep);

    //Cargar todo el documento de configuracion en la variable matriz
    FILE *fp1 = fopen("/etc/vsftpd.conf","r"); //Abrir el archivo .conf (Necesario modo superuser)
    if(fp1 == NULL){
        //printf("<p>Error al abrir</p><br>\n");  //Instalar****Codigo Cris
    }else{
        while (fgets(buffer, maxima, fp1 )){      //Mientras haya lineas por leer almacenandolas en el buffer
            memcpy(matriz[indice],buffer,maxima); //Copiar todo el archivo al array(matriz)
            indice++;
        }
    }
    fclose(fp1);

    FILE *fp2 = fopen("/etc/vsftpd.conf","w"); // para escribir borrando todo lo que ya habia (Necesario modo superuser)
    if(fp2 == NULL){
       // printf("<p>Error al abrir</p>\n");
    }else{
        for (int i = 0; i < indice; i++){ //Sobre escribir el archivo de configuracion cambiando el valor
            if((strcmp(matriz[i],connectActual)!=0) && (strcmp(matriz[i],asciiActual)!=0) && (strcmp(matriz[i],listenActual)!=0) && 
	 (strcmp(matriz[i],listen_ipv6Actual)!=0) && (strcmp(matriz[i],sslActual)!=0) && (strcmp(matriz[i],minActual)!=0) && (strcmp(matriz[i],maxActual)!=0)){
	        fprintf(fp2,matriz[i]); //Copiamos menos la variable que cambia
            }else if(strcmp(matriz[i],connectActual) == 0){
                fprintf(fp2,connect_from_port_20);//Reemplazamos la variable cambiada
                fprintf(fp2,"\n");
            }else if(strcmp(matriz[i],asciiActual) == 0){
                fprintf(fp2,ascii_upload_enable);
                fprintf(fp2,"\n");
            }else if(strcmp(matriz[i],listenActual) == 0){
                fprintf(fp2,listen);
                fprintf(fp2,"\n");
            }else if(strcmp(matriz[i],listen_ipv6Actual) == 0){
	    	fprintf(fp2,listen_ipv6);
		fprintf(fp2,"\n");
	    }else if(strcmp(matriz[i],sslActual) == 0){
	    	fprintf(fp2,ssl_enable);
		fprintf(fp2,"\n");
	    }else if(strcmp(matriz[i],minActual) == 0){
	    	fprintf(fp2,pasv_min_port);
                fprintf(fp2,"\n");
	    }else{
	    	fprintf(fp2,pasv_max_port);
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
	    "<br><p>Cambio realizado: %s</p>\n"
	    "<br><p>Cambio realizado: %s</p>\n"
	    "<br><p>Cambio realizado: %s</p>\n"
            "<br><p>Cambio realizado: %s</p>\n", connect_from_port_20, ascii_upload_enable, listen, listen_ipv6, ssl_enable, pasv_min_port, pasv_max_port
    );
    printf(
      "</body>\n"
      "</html>\n"
    );
    system("service vsftpd restart");
    return 0;
}
