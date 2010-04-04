#ifndef __CALLBACKS_C__
#define __CALLBACKS_C__

#include <gtk/gtk.h>

#ifndef bool
    #define bool unsigned char
    #define true 1
    #define false 0
#endif

G_MODULE_EXPORT bool on_quit_clicked( GtkButton *button, gpointer   data );
G_MODULE_EXPORT bool on_m_distortion_destroy (GtkObject *object, gpointer user_data);
G_MODULE_EXPORT void on_rock_mode_clicked (gpointer distors, GtkRadioButton *button);
G_MODULE_EXPORT void on_random_mode_clicked ( gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_by_pass_mode_clicked ( gpointer distors, GtkRadioButton *button);
G_MODULE_EXPORT void on_mute_mode_clicked ( gpointer distors,GtkRadioButton *button);

#endif
