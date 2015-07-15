/*
Copyright (c) 2015 Thomas Hurst <tom@hur.st>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <paths.h>
#include <stdio.h>
#include <strings.h>
#include <sys/disk.h>
#include <sys/ioctl.h>
#include <sysexits.h>

int
main(int argc, char **argv)
{
	if (argc != 2) {
		err(EX_USAGE, "Usage: %s device\n", argv[0]);
	}

	int fd;
	fd = open(argv[1], O_RDWR);

	if (fd < 0 && errno == ENOENT && *argv[1] != '/') {
		char buf[BUFSIZ];
		sprintf(buf, "%s%s", _PATH_DEV, argv[1]);
		fd = open(buf, O_RDWR);
	}

	if (fd < 0) {
		err(EX_NOINPUT, "failed to open device");
	}

	off_t mediasize;
	if (ioctl(fd, DIOCGMEDIASIZE, &mediasize)) {
		err(EX_UNAVAILABLE, "failed to query media size");
	}

	char str[BUFSIZ];
	printf("trim %ld bytes from %s? (yes/no) ", mediasize, argv[1]);
	if (fgets(str, sizeof(str), stdin) != NULL) {
		if (strncasecmp(str, "yes", 3) != 0) {
			printf("operation aborted\n");
			return (EX_OK);
		}
	}

	off_t ioarg[2] = { 0, mediasize };
	if (ioctl(fd, DIOCGDELETE, ioarg)) {
		err(EX_UNAVAILABLE, "failed to trim device");
	}
	return (EX_OK);
}
