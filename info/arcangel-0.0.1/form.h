/** Header file generated with fdesign on Sat Feb 25 01:43:15 2006.**/

#ifndef FD_arcangel_h_
#define FD_arcangel_h_

/** Callbacks, globals and object handlers **/
extern void form_quit(FL_OBJECT *, long);
extern void form_dial(FL_OBJECT *, long);


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *arcangel;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *quit;
	FL_OBJECT *dial;
} FD_arcangel;

extern FD_arcangel * create_form_arcangel(void);

#endif /* FD_arcangel_h_ */
