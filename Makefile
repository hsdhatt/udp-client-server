all : udpclient udpserver

udpclient: 
	$(CC) udpclient.c -o $@
udpserver: 
	$(CC) udpserver.c -o $@

clean:
	rm -f *.o udpclient udpserver 
