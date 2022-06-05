help:
	@echo "----------[Описание целей Makefile]----------"
	@echo "build - компиляция домашней работы"
	@echo "run   - пробный запуск"
	@echo "clean - возврат репозитория в изначальный вид"

build: matrix.c
	@gcc matrix.c -o matrix -lpthread

run: matrix m1 m2
	@./matrix m1 m2 res

clean: matrix
	@rm -rf matrix res *.o *.swp *~

