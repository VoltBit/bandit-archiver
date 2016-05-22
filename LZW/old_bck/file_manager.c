#include "file_manager.h"

#ifdef __linux__

handle_t open_file(const char *path, ftype mode)
{
	int fd;
	if (mode == SOURCE)
		fd = open(path, O_RDONLY);
	if (mode == DESTINATION)
		fd = open(path, O_RDWR | O_CREAT, 0666);
		// fd = open(path, O_RDWR | O_APPEND | O_CREAT, 0666);
	return fd;
}

boolean close_file(handle_t file)
{
	int ret;
	ret = close(file);
	if (ret < 0) {
		perror("Close failed");
		return false;
	}
	return true;
}

os_size read_file(handle_t handle, void *buffer, os_size count)
{
	return read(handle, buffer, count);
}

os_size write_file(handle_t handle, void *buffer, os_size count)
{
	return write(handle, buffer, count);
}

os_size sizeof_file(handle_t file)
{
	struct stat st;
	fstat(file, &st);
	return st.st_size;
}

#elif defined(_WIN32)

handle_t open_file(char *path, char *mode)
{
	
}

boolean close_file(handle_t file)
{

}

os_size read_file(handle_t handle, void *buffer, os_size count)
{

}

os_size write_file(handle_t handle, void *buffer, os_size count)
{

}

os_size sizeof_file(handle_t file)
{

}
#endif
