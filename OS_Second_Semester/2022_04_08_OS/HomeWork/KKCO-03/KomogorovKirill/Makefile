CC=gcc
CFLAGS=-Wall

LD=$(CC)
LDFLAGS_EXE=-ldl -lncurses -ltinfo
LDFLAGS_LIB=-shared -fPIC

all: small large libspf.so
	@echo "Build done"
large: main.c
	@echo "(BUILD) $@"
	@$(CC) $(CFLAGS) -DDYNAMIC_PROCESS_COUNT $< -o $@ $(LDFLAGS_EXE)
small: main.c
	@echo "(BUILD) $@"
	@$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS_EXE)
libspf.so: spf.c
	@echo "(BUILD) $@"
	@$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS_LIB)
test: small libspf.so
	@echo "Test lib"
	@./small libspf.so
clean:
	@echo "Clean project"
	@rm -rf large small *.so
