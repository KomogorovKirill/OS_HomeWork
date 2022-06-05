help:
	@echo "----------[Описание целей Makefile]----------"
	@echo "build - компиляция домашней работы"
	@echo "clean - возврат репозитория в изначальный вид"

build: snace.c
	@gcc snace.c -o snace -lpthread -lncurses

clean: snace
	@rm -rf snace *.o *.swp *~

