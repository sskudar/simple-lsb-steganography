#ifndef ERROR_HEADER
#define ERROR_HEADER

//isto tako se cesto koriste
#define ERR_READ "Insufficient reading permissions for given file."
#define ERR_MISS "No such file."
#define ERR_GNRL "Oops. Something went wrong!"

#define EXIT_F EXIT_FAILURE

/* 
 * Designated debugging print function
 */
void debug_print(char* tekst);

/*
 * Error function, terminates the program
 */
void wrap_errx(char* caller, int errind);


/* =====================
 * Error functions for getopt
 ===================== */

/*
 * Checks if the given port is valid.
 * Must be between 1024 and 65535, both included.
 */
void check_port_range(char* port);

/*
 * Checks if required arguments exist
 */
void check_empty_params(int optind, char** argv);

/*
 * Checks if there are too many parameters
 */
void check_too_many_params(int optind, char** argv);

#endif