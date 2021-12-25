GCC=$(shell which gcc)

help:
	@echo "-------------[Описание целей]-------------"
	@echo "build - компиляция клиента и сервера"
	@echo "clean - очистка репозитория от временных файлов"
	@echo "help  - вывод справки о целях в данном Makefile"

build:
	@$(GCC) -o server server.c
	@$(GCC) -o test/client test/client.c

clean:
	@rm -rf test/client server *.o test/*.o testt
