/*
 * dump.c
 *
 * PURPOSE
 *	A simple utility to dump disk sectors.
 *
 * DESCRIPTION
 *	I got tired of using dd and hexdump :-)
 *
 *	Usage: dump device sector | less
 *	The sector can be specified in decimal, octal, or hex.
 *
 * CONTACTS
 *	E-mail regarding any portion of the Linux UDF file system should be
 *	directed to the development team mailing list (run by majordomo):
 *		linux_udf@hootie.lvld.hp.com
 *
 * COPYRIGHT
 *	This file is distributed under the terms of the GNU General Public
 *	License (GPL). Copies of the GPL can be obtained from:
 *		ftp://prep.ai.mit.edu/pub/gnu/GPL
 *	Each contributing author retains all rights to their own work.
 *
 * HISTORY
 *	July 1, 1997 - Andrew E. Mileski
 *	Written, tested, and released.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>

typedef unsigned char Uint8;

Uint8 sector[2048];

int main(int argc, char **argv)
{
	int fd;
	long retval;
	unsigned long fp;
	unsigned long sec = 0;

	if (argc < 2) {
		printf("usage: dump <device> <sector>\n");
		return -1;
	}
	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror(argv[1]);
		return -1;
	}

	if (argc == 3)
		sec = strtoul(argv[2], NULL, 0);

	fp= sec << 11L;
	retval = lseek(fd, fp, SEEK_SET);
	if (retval < 0) {
		perror(argv[1]);
		return -1;
	} else
		printf("%ld: seek to %ld ok, retval %lu\n", 
			sec, fp, retval);

	retval = read(fd, sector, 2048);
	if ( retval > 0 ) {
		int i, j;
		printf("retval= %ld\n", retval);
		for (i = 0; i < retval; i += 16) {
			printf("%08lx-%03x: ", sec, i);
			for (j = 0; j < 16; j++)
				printf((j == 7) ? "%02x-": "%02x ",
					sector[i + j]);
			for (j = 0; j < 16; j++) {
				if (sector[i + j] < 31 || sector[i+j] > 126)
					printf(".");
				else
					printf("%c", sector[i+j]);
			}
			printf("\n");
		}
		/*retval = read(fd, sector, 2048);*/
		sec++;
	} else {
		printf("**** errno=%d ****", errno);
	}
	
	close(fd);
	return 0;
}