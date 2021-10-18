GCC=$(shell which gcc)

help:
	@echo "-------------[Описание целей]-------------"
	@echo "build - компиляция динамической библиотеки"
	@echo "run   - запуск реализации"
	@echo "clean - очистка репозитория от временных файлов"
	@echo "help  - вывод справки о целях в данном Makefile"

run: main.c
	@./main

build: libpushkin.so
	@$(GCC) main.c -o main -ldl

libpushkin.so: libpushkin.o
	@$(GCC) -shared -fPIC -o libpushkin.so libpushkin.o

libpushkin.o: libpushkin.c
	@$(GCC) -c libpushkin.c -o libpushkin.o

clean:
	@rm -rf  *.o *.so main *~
