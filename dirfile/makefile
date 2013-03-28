#/**
#*
#* @File : makefile
#*
#*
#* @Synopsis : Makefile pour les TD/TP Systemes
#*
#**/
include ../include/INCLUDE_H
#
COMPILER = gcc -c -I$(INCLUDE) -Wall -w -ansi -pedantic -std=c99 -O3 -Werror $*.c

LIBBUILDER = mkdir -p ../lib; ar -cqs ../lib/libSys.a $*.o

#
# Edition de liens
#codeobjsyst 
$(nom).run : $(nom).o 
	gcc -s -o $(nom).run $(nom).o -L../lib -lSys 
#
$(nom).o : $(nom).c
#$(CEXC_H) $(BOR_UTIL_H)
	$(COMPILER)
#
# Code Objet pour la bibliotheque Systemes
# 

#codeobjsyst: bor-util.o
#	touch codeobjsyst
#
#bor-util.o : bor-util.c $(BOR_UTIL_H)
#	$(COMPILER); $(LIBBUILDER) 
#
# Nettoyage du repertoire courant : executables et fichiers .o
#
clean : 
	clear; rm -rf *.o *.run ../lib -v *~ ../include/*~
#codeobjsyst 
