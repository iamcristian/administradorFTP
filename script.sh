#!/bin/bash
# -*- ENCODING: UTF-8 -*-
gcc ftp.c -o ftp
gcc instalar.c -o instalar.cgi
gcc arranque.c -o arranque.cgi
gcc enjaular.c -o enjaular.cgi
gcc servicio.c -o servicio.cgi
gcc anonimo.c -o anonymous.cgi
gcc log.c -o log.cgi
gcc write.c -o write.cgi
gcc transfer.c -o transfer.cgi
gcc usuarios.c -o usuarios.cgi
chmod u+s ftp
chmod u+s *.cgi
