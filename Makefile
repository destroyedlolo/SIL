# makefile created automaticaly by LFMakeMaker
# LFMakeMaker 1.4 (Apr  3 2018 23:14:49) (c)LFSoft 1997

gotoall: all


#The compiler (may be customized for compiler's options).
cc=cc
opts=-Wall -O2

sil.o : sil.c config.h storage.h tokenize.h Makefile 
	$(cc) -c -o sil.o sil.c $(opts) 

storage.o : storage.c storage.h Makefile 
	$(cc) -c -o storage.o storage.c $(opts) 

tokenize.o : tokenize.c tokenize.h config.h storage.h Makefile 
	$(cc) -c -o tokenize.o tokenize.c $(opts) 

sil : tokenize.o storage.o sil.o Makefile 
	 $(cc) -o sil tokenize.o storage.o sil.o $(opts) 

all: sil 
