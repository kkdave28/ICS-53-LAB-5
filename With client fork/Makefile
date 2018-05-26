all:
	gcc lab5c.c -o client
	gcc lab5s.c -o server
client:
	gcc lab5c.c -o client
server:
	gcc lab5s.c -o server
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
