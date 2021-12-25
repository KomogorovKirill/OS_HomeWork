PYTHON3=$(shell which python3)

all: run

help:
	@echo "----------[Описание целей Makefile]----------"
	@echo "run  - запустить парсер NTFS образа"
	@echo "help - вывод справки о целях Makefile"
	@echo "----------[Иноформация о глобальных переменных Makefile]----------"
	@echo "PATH - путь до NTFS образа"

run:
	@$(PYTHON3) ntfs.py $(PATH)
