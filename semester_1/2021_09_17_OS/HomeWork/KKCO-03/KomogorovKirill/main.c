#define _XOPEN_SOURCE 500

#include <ftw.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

int work(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
	char *(*pushkin)(char *);
	if (strstr(fpath + ftwbuf -> base, ".so") != NULL)
	{
		void *handle = dlopen(fpath, RTLD_LAZY);
		if (!handle)
		{
			perror("Handle");
			exit(1);
		}
	
		pushkin = (char *(*)(char *))dlsym(handle, "pushkin");

		if (pushkin != NULL)
		{
			static char *str = NULL;
			str = pushkin(str);
			puts("\n-----------------------------------");
			printf("%s", str);
			puts("-----------------------------------\n");
		}
		else
			puts("Function 'pushkin' not found");
		dlclose(handle);
	}
	return 0;
}

int main()
{
	if (nftw(".", work, 20, 0) == -1)
	{
        	perror("nftw");
        	exit(1);
    	}
	return 0;
}
