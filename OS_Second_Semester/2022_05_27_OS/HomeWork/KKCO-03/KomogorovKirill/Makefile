help:
	@echo "----------[Описание целей Makefile]----------"
	@echo "build - собрать решение"
	@echo "clean - вернуть состояние репозитория к изначальному состоянию"


build: enc.c main.c upx_start.asm upx_end.asm
	@nasm -f elf64 upx_start.asm
	@nasm -f elf64 upx_end.asm
	@gcc -o prog.exe upx_start.o main.c upx_end.o
	@gcc -o enc.exe enc.c
clean:
	@rm -rf *.exe *.o
