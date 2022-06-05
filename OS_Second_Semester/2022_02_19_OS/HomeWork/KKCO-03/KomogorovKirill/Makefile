help:
	@echo "----------[Описание целей Makefile]----------"
	@echo "build - Скомпилировать змейку"
	@echo "run   - Запустить змейку"
	@echo "clean - Вернуть содержимое репозитория в исходное состояние"
	@echo "help  - Вывод справки о целях Makefile"

build: snace.c
	@gcc snace.c -o snace -lncurses

run: snace
	@./snace

clean: snace
	@rm -rf snace *.swp *~
