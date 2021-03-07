#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

regex_t floatRegex;
regex_t intRegex;
int regexStatus;
char msgbuf[100];

/*int isShape(const char* word) {
	return (word == "SPHERE" ||
		word == "TORUS" ||
		word == "CCONE");
}

void readScene() {
	const char* filename = "test.scene";

	regexStatus = regcomp(&floatRegex, "^-?[0-9]+.[0-9]+$", REG_EXTENDED);
	if (regexStatus) {
		fprintf(stderr, "Could not compile regex\n");
		exit(1);
	}

	regexStatus = regcomp(&intRegex, "^-?[0-9]+$", REG_EXTENDED);
	if (regexStatus) {
		fprintf(stderr, "Could not compile regex\n");
		exit(1);
	}

	FILE* fp = fopen(filename, "r");

	if (fp == NULL) {
		printf("Error opening file \"%s\"\n", filename);
		exit(1);
	}

	const unsigned int wmaxlen = 30;
	char word[wmaxlen];
	char c;
	unsigned int wlen = 0;
	char activeKey = ' ';

	while ((c = fgetc(fp)) != EOF) { // read file byte by byte until EOF
		if (wlen >= wmaxlen) { printf("error parsing file \"%s\"\n the word \"%s...\" is too long\n", filename, word); exit(1); }
		if (c == ' ' || c == '\n') {
			if (wlen != 0) {
				if (word[0] == '/' && word[1] == '/') { // if word == "//"
					while ((c = fgetc(fp)) != '\n') { } // skip until end of line
				} else {
					int floatR = regexec(&floatRegex, word, 0, NULL, 0);
					int intR = regexec(&intRegex, word, 0, NULL, 0);

					if (!floatR) { // its float
						printf("- %0.2f\n", atof(word));
					} else if (!intR) { // its int
						printf("- %d\n", atoi(word));
					} else if (floatR == REG_NOMATCH || intR == REG_NOMATCH) { // or keyword
						printf("%s\n", word);
						if (word == "#PRIMITIVES") { activeKey = 'P'; }
						else if (isShape(word))    { activeKey = 'S'; }
					} else {
						regerror(regexStatus, &floatRegex, msgbuf, sizeof(msgbuf));
						fprintf(stderr, "Regex match failed: %s\n", msgbuf);
						exit(1);
					}
				}
				wlen = 0;
				memset(word, 0, wmaxlen);
			}
			continue;
		}

		word[wlen++] = c;
	}

	regfree(&floatRegex);

	fclose(fp);
}

//void main() { readScene(); } // */
