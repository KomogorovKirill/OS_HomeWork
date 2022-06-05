all: main.c
		gcc main.c -o normal.exe
		gcc main.c -DUNSAFE_MEMORY_BLOCK -o unsafe.exe
clean:
		rm -f *.exe
