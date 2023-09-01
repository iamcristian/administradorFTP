===================================================================
Proyecto ASO - Página Web
Administrador FTP
Instrucciones de uso
===================================================================
Ejecución del programa en la web
===================================================================

1.- Verificar la instalación del paquete apache2 reiniciando el
    servicio con el comando:
     	service apache2 restart
    En caso de no tenerlo instalado ejecutar:
	zypper install apache2

2.- Mover los programas con extensión .c .sh y el backup al
    siguiente directorio con el comando:
	mv *.c *.sh -r install/ /srv/www/cgi-bin

3.- Mover los programas con estensión .js .css al siguiente directorio:
	/srv/www/htdocs
	mv *.js *.css /srv/www/htdocs

4.- Se debe conceder los permisos al script para su ejecucion sin 
    errores con el comando:
	chmod 777 script.sh

5.- A continuación se debe ejecutar el script:
	./script.sh
    Éste compilará todos los programas con extensión .c y también
    brindara modo de ejecucion de superusuario para el correcto 
    funcionamiento de la página.

6.- Posterior a eso, se debe abrir un navegador para ejecutar los 
    programas y visualizar la página, usando la siguiente dirección
    web:
    	localhost/cgi-bin/ftp

7.- En la página, se debe seleccionar los valores de los parámetros 
    a cambiar. Los valores mostrados inicialmente, son los actuales
    del archivo de configuración.

===================================================================
