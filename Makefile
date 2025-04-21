client: src/tftp_client.c
	gcc -ansi src/tftp_client.c -o out/tftp-client

server: src/tftp_server.c
	gcc -ansi src/tftp_server.c -o out/tftp-server

all:
	gcc -ansi src/tftp_server.c -o out/tftp-server
	gcc -ansi src/tftp_client.c -o out/tftp-client

clean:
	rm -f out/*
