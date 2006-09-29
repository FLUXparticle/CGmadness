#include "files.h"

#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Liest eine komplette Textdatei als String ein 
 */
char* textFileRead(char* fn) {
	FILE* fp;
	char* content = NULL;

	int count = 0;

	if (fn != NULL) {
		fp = fopen(fn, "rt");

		if (fp != NULL) {
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				MALLOC(content, sizeof(char) * (count+1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

/*
 * Schreibt einen String in eine Textdatei
 */
int textFileWrite(char *fn, char *s) {
	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}
