PYTHON3=$(shell which python3)
PIP3=$(shell which pip3)

help:
	@echo "-------------[Описание целей]-------------"
	@echo "run   - запуск реализации"
	@echo "help  - вывод справки о целях в данном Makefile"

run:
	@$(PIP3) install -r requirement.txt
	@$(PYTHON3) library.py
