/**
 * mkthomsonpod
 * Copyright (C) 2020 Joseph Lehner <joseph.c.lehner@gmail.com>
 *
 * mkthomsonpod is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * bcm2-utils is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bcm2-utils.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <openssl/md5.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/**
 * PoD (password-of-the day) generator for Thomson/Technicolor modems.
 *
 * Compilation:
 * $ gcc mkthomsonpod.c -o mkthomsonpod -lcrypto
 *
 * First argument is a UNIX timestamp of the current time perceived by
 * the modem's firmware.
 *
 * The second argument is optional and defaults to "thomson".
 *
 * $ ./mkthomsonpod 87318 thomson
 * cadff4300001088
 */

// As usual, many a snakeoil lies ahead.

void hash_thomson_pod(char* data1, uint32_t len1, char* data2, uint16_t len2, uint8_t* md)
{
	char buf1[64];
	char buf2[64];
	char d2hash[16];
	MD5_CTX ctx;

	if (len2 > 64) {
		MD5_Init(&ctx);
		MD5_Update(&ctx, data2, len2);
		MD5_Final((uint8_t*)d2hash, &ctx);
		data2 = d2hash;
		len2 = 16;
	}

	memset(buf1, 0, sizeof(buf1));
	memset(buf2, 0, sizeof(buf2));

	if (len2) {
		memcpy(buf1, data2, len2);
		memcpy(buf2, data2, len2);
	}

	for (int i = 0; i < sizeof(buf1); ++i) {
		buf1[i] ^= 0x36;
		buf2[i] ^= 0x5c;
	}

	MD5_Init(&ctx);
	MD5_Update(&ctx, buf1, sizeof(buf1));
	MD5_Update(&ctx, data1, len1);
	MD5_Final(md, &ctx);

	MD5_Init(&ctx);
	MD5_Update(&ctx, buf2, sizeof(buf2));
	MD5_Update(&ctx, md, 16);
	MD5_Final(md, &ctx);
}

char* generate_thomson_pod(time_t timestamp, const char* salt)
{
	struct tm* tm = localtime(&timestamp);

	char data1[64];
	sprintf(data1, "%d.%02d.%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
	uint32_t len1 = strlen(data1);

	char data2[20];
	memset(data2, 0, 20);
	snprintf(data2, sizeof(data2), "%s525",salt);
	uint32_t len2 = strlen(data2);

	uint8_t md[16];
	hash_thomson_pod(data1, len1, data2, len2 & 0xffff, md);

	// we only want the first 15 bytes of the hash's string representation
	static char pod[16];
	sprintf(pod,"%02x%02x%02x%02x%02x%02x%02x%x", md[0], md[1],
			md[2], md[3], md[4], md[5], md[6], md[7] >> 4);
	return pod;
}

int main(int argc, char** argv)
{
	if (argc == 1) {
		fprintf(stderr, "Usage: mkthomsonpod <timestamp> [<salt>]\n");
		return 1;
	}

	long time;

	if (argc >= 2) {
		char* endptr;
		time = strtol(argv[1], &endptr, 10);
		if (*endptr) {
			fprintf(stderr, "Error: bad number: %s\n", argv[1]);
			return 1;
		}
	}

	const char* salt = (argc >= 3) ? argv[2] : "thomson";
	printf("%s\n", generate_thomson_pod(time, salt));
	return 0;
}
