all: main.c write.c
	gcc -o control main.c
	gcc -o write write.c

clean:
	rm -rf control write *.o
