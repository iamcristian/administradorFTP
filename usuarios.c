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
    char *token;
    char *aux;
    char add_user[80];
    char rm_user[80];

    strncpy(inputBuffer, cadena, 255);
    token = strtok(inputBuffer,"&");

    sscanf(token,"add_usuario=%s",add_user);
    strcat(add_user,"\n");
    
    token = strtok(NULL,"&");
    sscanf(token,"rm_usuario=%s",rm_user);
    strcat(rm_user,"\n");

    //Variables para modificar archivo .chroot_list
    int maxima=1000; // tamaño de cada fila y columna
    char matriz[maxima][maxima]; // Matriz para cargar el archivo vsftpd.chroot_list
    int indice=0;
    char buffer[maxima];   // recupera cada fila del archivo .chroot_list para guardar en matriz

    //Cargar todo el documento de usuarios en la variable matriz
    FILE *fp1 = fopen("/etc/vsftpd.chroot_list","r"); 
    if(fp1 == NULL){
       // printf("<p>No esta habilitado el campo: chroot_list_file=/etc/vsftpd.chroot_list</p><br>\n"); 
    }else{
        while (fgets(buffer, maxima, fp1 )){      //Mientras haya lineas por leer almacenandolas en el buffer
            memcpy(matriz[indice],buffer,maxima); //Copiar todos los usuarios a la matriz
            indice++;
        }
    }
    fclose(fp1);
    
    //Quitar un usuario de la lista
    if(strcmp(rm_user,"vacioi\n") != 0){
        FILE *fp2 = fopen("/etc/vsftpd.chroot_list","w");
        if(fp2 != NULL){
            for (int i = 0; i < indice; i++){ 
                if(strcmp(matriz[i],rm_user) != 0){
                    fprintf(fp2,matriz[i]);
		}
	    }
        }
	fclose(fp2);
    }

    //Añadir un usuario de la lista
    if(strcmp(add_user,"vacio\n")!=0){
        FILE *fp3 = fopen("/etc/vsftpd.chroot_list","a");
        if(fp3 != NULL){
	     fprintf(fp3,add_user);
        }
        fclose(fp3);
    }
    int controlador=1;
    printf(
        "Content-type:text/html\n\n"
        "<head><title>usuarios</title>\n"
        "<body><p>\n"
        "<META HTTP-EQUIV='REFRESH' CONTENT='5;URL=/cgi-bin/ftp'></META>\n");
	if(strcmp(add_user,"vacio\n")!=0){
	    printf("Se agrego al usuario: %s",add_user);
	    if(strcmp(rm_user,"vacio\n")!=0){
	    	printf("<br>Se quito al usuario: %s",rm_user);
		controlador=0;
	    }
	}
	if(strcmp(rm_user,"vacio\n")!=0 && controlador){
	    printf("Se quito al usuario: %s",rm_user);
	}
 printf( "</p></body>\n"
        );
    system("service vsftpd restart");
    return 0;
}
