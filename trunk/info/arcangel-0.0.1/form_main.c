#include "forms.h"
#include "form.h"

int main(int argc, char *argv[])
{
   FD_arcangel *fd_arcangel;

   fl_initialize(&argc, argv, 0, 0, 0);
   fd_arcangel = create_form_arcangel();

   /* fill-in form initialization code */

   /* show the first form */
   fl_show_form(fd_arcangel->arcangel,FL_PLACE_CENTERFREE,FL_FULLBORDER,"arcangel");
   fl_do_forms();
   return 0;
}
