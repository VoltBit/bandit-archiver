#ifndef FMAN_H_
#define FMAN_H_

/*
 *	File management module used for easier access to files on both Linux and
 *	Windows.
 */

#define true	1
#define false	0

#ifdef __linux__

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
 
typedef int handle_t;
typedef int os_size;
typedef int boolean;

#elif defined(_WIN32)

#include <windows.h>

typedef HANDLE handle_t;
typedef DWORD os_size;
typedef DWORD boolean;

#else
#error "Unknown OS"
#endif


typedef enum {
	SOURCE,
	DESTINATION
} ftype;

handle_t open_file(const char *path, ftype mode);
boolean close_file(handle_t file);
os_size read_file(handle_t handle, void* buffer, os_size count);
os_size write_file(handle_t handle, void* buffer, os_size count);
os_size sizeof_file(handle_t file);

#endif
