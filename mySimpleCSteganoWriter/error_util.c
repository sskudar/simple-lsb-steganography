#include "error_util.h"
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void debug_print(char* text) 
{
    printf("%s", text);
}


void wrap_errx(char* caller, int errind)
{
	errx(EXIT_F, "%s: %d - %s", caller, errind, strerror(errind));
}


/* =====================
 * Error functions for getopt
 ===================== */
void check_port_range(char* port) {
	if (atoi(port) < 1024 || atoi(port) > 65535) {
		errx(EXIT_F, "Invalid port. Allowed range: [1024, 65535].");
	}
}


void check_empty_params(int optind, char** argv)
{
	if (argv[optind] == NULL) {
		errx(EXIT_F, "Missing required arguments.");
	}
}


void check_too_many_params(int optind, char** argv)
{
	if (argv[optind] != NULL) {
		errx(EXIT_F, "Too many input arguments.");
	}
}
