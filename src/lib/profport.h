
/******************************************************************************
 PORTABLE ROUTINES FOR WRITING PRIVATE PROFILE STRINGS --  by Joseph J. Graf
 Header file containing prototypes and compile-time configuration.
******************************************************************************/

#ifndef __profport__
#define __profport__

#define MAX_LINE_LENGTH    1024

#ifdef __cplusplus
extern "C" {
#endif

int get_private_profile_int(char *, char *, int, char *);
int get_private_profile_string(char *, char *, char *, char *, int, char *);
int write_private_profile_string(char *, char *, char *, char *);

#ifdef __cplusplus
}
#endif

#endif