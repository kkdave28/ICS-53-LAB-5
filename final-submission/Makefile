all:
	gcc Lab5_Client.c -o client
	gcc Lab5_Server.c -o server
client:
	gcc lab5client.c -o client
server:
	gcc lab5server.c -o server
runs:
	server 30006
runc:
	client andromeda-32.ics.uci.edu 30006 
clean:
	clear
	rm -f client
	rm -f server
	rm -f core.*
	rm -f vgcore.*
