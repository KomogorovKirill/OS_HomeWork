all:
	@echo -e "Имеющиеся варианты использоваия:\n" \
	"\tbuild - собрать загрузчик\n" \
	"\trun - запусть эмуляцию в qemu (не консоль)\n" \
	"\tconsole - запуск эмуляции в консоли"
build:
	nasm -f bin boot.asm -o floppy.img
run:	build
	qemu-system-x86_64 -fda floppy.img
console: build
	qemu-system-x86_64 -fda floppy.img -curses
