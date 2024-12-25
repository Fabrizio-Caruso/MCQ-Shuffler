CC = i686-w64-mingw32-gcc

all: 
	$(CC) shuffle.c -o shuffle.exe

clean:
	rm -rf ./versions/*.txt
	rm -rf ./versions/*.html


