#include "webserv.hpp"

bool	is_directory(const char* path)
{
	struct stat	statbuf;
	if (stat(path, &statbuf) < 0)
	{
		perror("stat()");
		exit(EXIT_FAILURE);
	}
	return S_ISDIR(statbuf.st_mode);
}