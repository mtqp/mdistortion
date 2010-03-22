/* Form definition file generated with fdesign. */

#include <X11/forms.h>
#include <stdlib.h>
#include "form.h"

FD_arcangel *create_form_arcangel(void)
{
  FL_OBJECT *obj;
  FD_arcangel *fdui = (FD_arcangel *) fl_calloc(1, sizeof(*fdui));

  fdui->arcangel = fl_bgn_form(FL_NO_BOX, 210, 100);
  obj = fl_add_box(FL_FRAME_BOX,0,0,210,100,"");
    fl_set_object_color(obj,FL_WHITE,FL_COL1);
  fdui->quit = obj = fl_add_button(FL_NORMAL_BUTTON,120,10,80,80,"Quit");
    fl_set_button_shortcut(obj,"q",1);
    fl_set_object_boxtype(obj,FL_BORDER_BOX);
    fl_set_object_color(obj,FL_WHITE,FL_COL1);
    fl_set_object_gravity(obj, FL_SouthEast, FL_SouthEast);
    fl_set_object_callback(obj,form_quit,0);
  fdui->dial = obj = fl_add_dial(FL_FILL_DIAL,10,10,80,70,"drive");
    fl_set_object_color(obj,FL_WHITE,FL_DODGERBLUE);
    fl_set_object_gravity(obj, FL_SouthEast, FL_SouthEast);
    fl_set_object_callback(obj,form_dial,0);
    fl_set_dial_bounds(obj, 0.1, 400);
    fl_set_dial_value(obj, 1);
    fl_set_dial_step(obj, 0.1);
    fl_set_dial_return(obj, FL_RETURN_ALWAYS);
  fl_end_form();

  fdui->arcangel->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

