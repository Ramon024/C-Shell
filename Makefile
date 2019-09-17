
default: main.c
	gcc -o nsh main.c

clean:
	rm -f nsh
