help:
	@echo "-------------[Описание целей Makefile]-------------"
	@echo "build_boot      - собрать загрузчик"
	@echo "build_decryptor - собрать декриптор загрузчика"
	@echo "build           - собрать и загрузчик, и декриптор"
	@echo "run             - запустить загрузчик в эмуляторе"
	@echo "clean           - очистить за собой папку"

build_boot:
	@nasm -f bin boot.asm -o boot.bin

build_decryptor:
	@gcc decryptor.c -o decryptor

build: build_boot build_decryptor

run:
	@qemu-system-x86_64 -fda boot.bin

clean:
	@rm -rf boot.bin decryptor
