OBJECT =client_main.o linkclient.o login_main.o register_main.o 
INC =../include/
client:$(OBJECT)
	gcc  $(OBJECT) -o client -lpthread  
client_main.o:client_main.c
	gcc   -I$(INC) -c client_main.c -o client_main.o 
linkclient.o:linkclient.c
	gcc -I$(INC) -c linkclient.c -o linkclient.o  
login_mian.o:login_main.c
	/usr/bin/gcc -I$(INC) -c login_main.c -o login_main.o 
register_main.o:register_main.c
	gcc -I$(INC) -c register_main.c -o register_main.o  

	
clear:
	rm -f *.o

