#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>//para superuser
#include<string.h>
#include<stdlib.h>
#include<stddef.h>

int main(){
    setuid(0);
    setgid(0);

    //Recuperando URL
    char *cadena = getenv("QUERY_STRING");
    char inputBuffer[256];
    char *anonymous_enable;
    char *anon_world;

    //Se divide el URL
    strncpy(inputBuffer, cadena, 255);
    anonymous_enable = strtok(inputBuffer,"&");
    anon_world = strtok(NULL,"&");   

    //Variables para modificar archivo .conf
    int maxima=1000; // tamaño de cada fila y columna
    char matriz[maxima][maxima]; // Matriz para guardar(cargar) el archivo vsftpd.conf
    int indice=0;
    char buffer[maxima];   // recupera cada fila del archivo .conf para guardar en matriz
   
    //Obtenemos el estado actual de anonymous_enable
    FILE *anonymousGrep = popen("grep -E ^'anonymous_enable=*' /etc/vsftpd.conf","r");
    char anonymousActual[80];
    fgets(anonymousActual,80,anonymousGrep);
    fclose(anonymousGrep);

    //Obtenemos el estado actual de anon_world_readable_only
    FILE *anonGrep = popen("grep -E ^'anon_world_readable_only=' /etc/vsftpd.conf","r");
    char anonActual[80];
    fgets(anonActual,80,anonGrep);
    fclose(anonGrep);

    //Cargar todo el documento de configuracion en la variable matriz
    FILE *fp1 = fopen("/etc/vsftpd.conf","r"); //Abrir el archivo .conf (Necesario modo superuser)
    if(fp1 == NULL){
        printf("<p>Error al abrir</p><br>\n");  //Instalar****Codigo Cris
    }else{
        while (fgets(buffer, maxima, fp1)){      //Mientras haya lineas por leer almacenandolas en el buffer
            memcpy(matriz[indice],buffer,maxima); //Copiar todo el archivo al array(matriz)
            indice++;
        }
    }
    fclose(fp1);

  FILE *fp2;
    fp2 = fopen("/etc/vsftpd.conf","w"); // para escribir borrando todo lo que ya habia (Necesario modo superuser)
    if(fp2 == NULL){
        printf("<p>Error al abrir</p>\n");
    }else{
        for (int i = 0; i < indice; i++){ //Sobre escribir el archivo de configuracion cambiando los valores
            if((strcmp(matriz[i], anonymousActual) != 0) && (strcmp(matriz[i],anonActual) != 0)){ //Menos las filas que se reemplazaran
                fprintf(fp2,matriz[i]); //Copiamos menos la variables que cambian
	    }else if(strcmp(matriz[i],anonActual) == 0){
		fprintf(fp2,anon_world);
		fprintf(fp2,"\n");
	    }else{
                fprintf(fp2,anonymous_enable);//Reemplazamos la variable cambiada
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
	    "<br>"
	    "<p>Cambio realizado: %s</p>", anonymous_enable, anon_world
    );
    printf(
      "</body>\n"
      "</html>\n"
    );
    system("service vsftpd restart");
    return 0;
}

