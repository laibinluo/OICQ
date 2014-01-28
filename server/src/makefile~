OBJECT :=server_main.o server_check.o linkinfo.o linkaddr.o 
INC :=../include/
server:$(OBJECT)
	gcc  $(OBJECT) -o server
server_main.o:server_main.c
	gcc   -I$(INC) -c server_main.c -o server_main.o
server_check.o:server_check.c
	gcc -I$(INC) -c server_check.c -o server_check.o
linkinfo.o:linkinfo.c
	gcc -I$(INC) -c linkinfo.c -o linkinfo.o
linkaddr.o:linkaddr.c
	gcc -I$(INC) -c linkaddr.c -o linkaddr.o

	
clear:
	rm -f *.o

