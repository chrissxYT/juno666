#ifndef UITYPES_H
#define UITYPES_H

#define UIBUTTON 1
#define UISLIDER 2
#define UIKNOB 3
#define UIKICKBUTTON 4
#define UITHREESWITCH 5
#define UITWOSWITCH 6
#define UIKEY 7

typedef struct
{
	unsigned char controltype;
	unsigned int posx;
	unsigned int posy;
	unsigned int controlidx;
	unsigned int resource_id;
	char *control_name;
} UITYPES;
#endif
