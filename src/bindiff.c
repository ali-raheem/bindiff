#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("Usage\n%s OLDFILE NEWFILE\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	FILE *of;
	FILE *nf;

	of = fopen(argv[1], "r");
	nf = fopen(argv[2], "r");

	if (of == NULL | nf == NULL) {
		puts("error opening files");
		exit(EXIT_FAILURE);
	}

	size_t i = 0;
	uint8_t c,d;

	bool delta = false;

	puts("TEMP1=`mktemp`\nTEMP2=`mktemp`\ncp \"$1\" $TEMP1");
	bool even = true;

	while(1){
		if(feof(of)) break;
		if(feof(nf)) break;
		c = fgetc(of);
		d = fgetc(nf);
		if(c != d) {
			if (delta == false) {
				delta = true;
				printf("binedit %s %zu \"\\x%02X", even ? "$TEMP1" : "$TEMP2", i, d);
			} else {
				printf("\\x%02X", d);
			}
		} else if (delta == true) {
			printf("\" > %s\n", even ? "$TEMP2" : "$TEMP1");
                        even ^= 1;
			delta = false;
		}
		i++;
	}
        printf("rm %s\n", even ? "$TEMP2" : "$TEMP1");
        even ^= 1;
        printf("echo \"your patched file can be found at %s.\"", even ? "$TEMP2" : "$TEMP1");
}
