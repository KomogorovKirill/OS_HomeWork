PYTHON3=$(shell which python3)
PIP3=$(shell which pip3)

help:
	@echo "---------------[Цели Makefile]---------------"	
	@echo "mount  - Подключить файловую систему облака"
	@echo "umount - Отключить файловую систему облака"
	@echo "help   - Вывод справки о целях Makefile"
mount:
	@$(PIP3) install -r requirement.txt
	@mkdir -p library
	@$(PYTHON3) driverfs.py library
umount:
	@fusermount -u library
	@rm -rf library
	@rm -rf *.json
