
/******************************************************************************
 PORTABLE ROUTINES FOR WRITING PRIVATE PROFILE STRINGS --  by Joseph J. Graf
 Header file containing prototypes and compile-time configuration.
******************************************************************************/

#define MAX_LINE_LENGTH    80

int get_private_profile_int(char *, char *, int, char *);
int get_private_profile_string(char *, char *, char *, char *, int, char *);
int write_private_profile_string(char *, char *, char *, char *);
