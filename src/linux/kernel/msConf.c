/*

  Copyright © Grame 2000

  This library is free software; you can redistribute it and modify it under 
  the terms of the GNU Library General Public License as published by the 
  Free Software Foundation version 2 of the License, or any later version.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public License 
  for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@rd.grame.fr
  
  
  [04-04-05] YO - Removed modversions dependencies 

*/

#ifdef MODULE
#define malloc(size)	kmalloc(size, GFP_KERNEL)
#define free			kfree
#endif

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/current.h>
#include <linux/sched.h>
#include <asm/string.h>

#include "msConf.h"

typedef short Boolean;
typedef struct file * fileptr;

//_______________________________________________________________________
// global variables
char * confName = "/etc/MidiShare.conf";

/* keywords list should be null terminated */
char * keywords[] = {
	"memory",
	"driver",
	0
};
/* enums should be kept in the same order than keywords */
enum { kIgnore, kMem, kDriver };
enum { false, true };

#define rem '#'
#define equal '='
#define defaultMem 30000
#define kMaxLine 255
#define EOF 	-1

//_______________________________________________________________________
static void skip_line (fileptr fd);
static char read_char (fileptr fd);
static Boolean read_line (fileptr fd, char *buff, short len);
static short split (char * line, char *value);
static void get_value (char * line, char *value);
static void parse (char * line, msConf *conf);
static dmodule * newmodule (char * modname);
static Boolean string2num (char * string, long * num);

//_______________________________________________________________________
void freemodules (dmodule * m)
{
	dmodule *next;
	while (m) {
		next = m->next;
		free (m);
		m = next;
	}
}

//_______________________________________________________________________
void read_conf (char * name, msConf *conf)
{
	Boolean eof;
	char line[kMaxLine + 1];
	fileptr f;

	f = filp_open (name, 0, 1);
	conf->kmem = defaultMem;
	conf->drivers = 0;
	if (!IS_ERR(f)) {
		do {
			eof = read_line (f, line, kMaxLine);
			if (strlen(line)) parse (line, conf);
		} while (!eof);
		filp_close (f, current->files);
	}
	else printk("error opening %s (%ld)\n", name, PTR_ERR(f));
}


//_______________________________________________________________________
// parsing
//_______________________________________________________________________
static dmodule * newmodule (char * modname)
{
	dmodule * m = (dmodule *)malloc (sizeof(dmodule));
	if (m) {
		strcpy (m->module_name, modname);
		m->next = 0;
	}
	return m;
}

//_______________________________________________________________________
static Boolean string2num (char * string, long * num)
{
	long n = 0, len = strlen (string), mul = 1;
	while (len--) {
		char c = string[len];
		if ((c >= '0') && (c <= '9')) {
			n += (c - '0') * mul;
			mul *= 10;
		}
		else return false;
	}
	*num = n;
	return true;
}

//_______________________________________________________________________
static void parse (char * line, msConf *conf)
{
	long mem; dmodule * m;
	char value[kMaxLine+1];
	short id = split (line, value);
	switch (id) {
		case kMem:
			if (string2num (value, &mem))
				conf->kmem = mem;
			break;
		case kDriver:
			m = newmodule (value);
			if (m) {
				m->next = conf->drivers;
				conf->drivers = m;
			}
			break;
	}
}

//_______________________________________________________________________
// utilities
//_______________________________________________________________________
static void get_value (char * line, char *value)
{
	while (*line != equal) line++;
	line++;
	while ((*line != ' ') && (*line != '\t') && *line) {
		*value++ = *line++;
	}
	*value = 0;
}

//_______________________________________________________________________
static short split (char * line, char *value)
{
	short i, n;
	for (*value = 0, i=0;  ; i++) {
		if (!keywords[i]) break;
		n = strlen (keywords[i]);
		if (strncmp (keywords[i], line, n) == 0) {
			get_value (line, value);
			break;
		} 
	}
	return i + 1;
}

//_______________________________________________________________________
static void skip_line (fileptr fd)
{
	char c; 
	do {
		c = read_char (fd);
		if (c == EOF) break;
	} while (c != '\n');
}

//_______________________________________________________________________
static char read_char (fileptr fd)
{
	long l; ssize_t n; char *c = (char *)&l;
	long savedLimit = current_thread_info()->addr_limit.seg;
	current_thread_info()->addr_limit.seg = 0xffffffff;

	do {
		n = generic_file_read (fd, c, 1, &(fd->f_pos));
		if (n <= 0) {
			current_thread_info()->addr_limit.seg = savedLimit;
			return EOF;
		}
	} while ((*c == ' ') || (*c == '\t'));
	current_thread_info()->addr_limit.seg = savedLimit;
	return *c;
}

//_______________________________________________________________________
static Boolean read_line (fileptr fd, char *buff, short len)
{
	int c, n = 0;
	do {
		c = read_char (fd);
		if ((c == rem) || (n > len)) {
			skip_line (fd);
			break;
		}
		else if ((c == '\n') || (c == EOF))
			break;
		*buff++ = c;
		n++;
	} while  (1);
	*buff = 0;
	return c == EOF;
}
