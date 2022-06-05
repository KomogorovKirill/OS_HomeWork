help:
	@echo "----------[Описание целей Makefile]----------"
	@echo "build - компиляция домашней работы"
	@echo "clean - возврат репозитория в изначальный вид"

build: minitron.c
	@gcc -g minitron.c -o minitron -lpthread -lncurses -lm

clean: minitron
	@rm -rf minitron *.o *.swp *~

