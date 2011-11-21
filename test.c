#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "abstract-buffer.h"

int main(int argc, char *argv[])
{
	int size, count=0;
	char data[60];
	struct buffer *buffer = new_buffer(80);
	int in = open("lorem_ipsum.txt", O_RDONLY);
	int out = open("result.txt", O_WRONLY|O_CREAT|O_TRUNC|S_IRUSR);

	if (!( in > 0  ) || !( out > 0 ))
		return -1;

	do {
		size = read(in, data, 60);

		if (size == -1)
			return -errno;
		else if (size == 0)
			break;

		buffer_append(buffer, data, 0, size);
		count += size;
	} while (size != 0);


	do {
		size = buffer_read(buffer, data, 0, 60);

		if (size == 0)
			break;

		write(out, data, size);
	} while (size != 0);

	return 0;
}
