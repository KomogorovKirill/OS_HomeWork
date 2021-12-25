GCC=$(shell which gcc)

help:
	@echo "-------------[Описание целей]-------------"
	@echo "build - компиляция динамической библиотеки"
	@echo "run   - запуск реализации"
	@echo "clean - очистка репозитория от временных файлов"
	@echo "help  - вывод справки о целях в данном Makefile"

build:
	@$(GCC) -c libnew.c
	@ar rc libnew.a libnew.o
	@$(GCC) main.c libnew.a -o main

run:
	@MALLOC_TRACE=main.log ./main
	@mtrace ./main main.log

clean:
	@unset MALLOC_TRACE
	@rm -rf main *.a *.o *.log
