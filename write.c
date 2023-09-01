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
    char *write_enable;
    char *dirmessage_enable;
    char *local_enable;

    strncpy(inputBuffer, cadena, 255);
    write_enable = strtok(inputBuffer,"&");
    dirmessage_enable = strtok(NULL,"&");
    local_enable = strtok(NULL,"&");
  
    //Variables para modificar archivo .conf
    int maxima=1000; // tamaño de cada fila y columna
    char matriz[maxima][maxima]; // Matriz para guardar(cargar) el archivo vsftpd.conf
    int indice=0;
    char buffer[maxima];   // recupera cada fila del archivo .conf para guardar en matriz
    
    //Obtenemos el estado actual write_enable
    FILE *writeGrep = popen("grep -E ^'write_enable=*' /etc/vsftpd.conf","r");
    char writeActual[80];
    fgets(writeActual,80,writeGrep);
    fclose(writeGrep);

    //Obtenemos el estado actual de dirmessage_enable
    FILE *dirmessageGrep = popen("grep -E ^'dirmessage_enable=' /etc/vsftpd.conf","r");
    char dirmessageActual[80];
    fgets(dirmessageActual,80,dirmessageGrep);
    fclose(dirmessageGrep);

    //Obtenemos el estado actual de local_enable
    FILE *localGrep = popen("grep -E ^'local_enable=' /etc/vsftpd.conf","r");
    char localActual[80];
    fgets(localActual,80,localGrep);
    fclose(localGrep);

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
            if((strcmp(matriz[i],writeActual) != 0) && (strcmp(matriz[i],dirmessageActual) != 0) && (strcmp(matriz[i],localActual) != 0)){ //Menos la fila que sea igual a la cadena que se reemplazara
                fprintf(fp2,matriz[i]); //Copiamos menos la variable que cambia
            }else if(strcmp(matriz[i],writeActual) == 0){
                fprintf(fp2,write_enable);//Reemplazamos la variable cambiada
		fprintf(fp2,"\n");
            }else if(strcmp(matriz[i],dirmessageActual) == 0){
	    	fprintf(fp2,dirmessage_enable);
		fprintf(fp2,"\n");
	    }else{
	        fprintf(fp2,local_enable);
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
	    "<br><p>Cambio realizado: %s</p>\n", write_enable, dirmessage_enable, local_enable
    );
    printf(
      "</body>\n"
      "</html>\n"
    );
    system("service vsftpd restart");
    return 0;
}
