/*
 * Copyright (c) 2016, Linaro Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright (c) 2025, NXP
 */

#include <sys/ioctl.h>
#include <linux/rpmsg.h>

#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void usage(void)
{
	extern char *__progname;

	fprintf(stderr, "%s <ctrl> <name> <src> <dst>\n", __progname);
	exit(EXIT_FAILURE);
}

static uint32_t get_value(char *text)
{
	char *endptr;
	uint32_t value;

	if (text[0]=='0' && text[1]=='x') {
		value = strtoul(text+2, &endptr, 16);
	} else {
		value = strtoul(text, &endptr, 10);
	}

	if (*endptr) {
		usage();
	}

	return value;

}

int main(int argc, char **argv)
{
	struct rpmsg_endpoint_info ept;
	int ret;
	int fd;

	if (argc == 5) {
		strncpy(ept.name, argv[2], sizeof(ept.name));
		ept.name[sizeof(ept.name)-1] = '\0';
		ept.src = get_value(argv[3]);
		ept.dst = get_value(argv[4]);
	} else {
		usage();
	}

	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		err(1, "failed to open %s\n", argv[1]);
	}

	ret = ioctl(fd, RPMSG_CREATE_EPT_IOCTL, &ept);
	if (ret < 0) {
		fprintf(stderr, "failed to create endpoint\n");
		close(fd);
		return EXIT_FAILURE;
	}

	close(fd);
	return EXIT_SUCCESS;
}
