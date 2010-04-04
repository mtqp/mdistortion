#ifndef __CALLBACKS_C__
#define __CALLBACKS_C__

#include <gtk/gtk.h>

#ifndef bool
    #define bool unsigned char
    #define true 1
    #define false 0
#endif

///////CALLBACKS-GTK///////

G_MODULE_EXPORT bool on_quit_clicked( GtkButton *button, gpointer   data );
G_MODULE_EXPORT bool on_m_distortion_destroy (GtkObject *object, gpointer user_data);
G_MODULE_EXPORT void on_rock_mode_clicked (gpointer distors, GtkRadioButton *button);
G_MODULE_EXPORT void on_random_mode_clicked ( gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_mute_mode_clicked ( gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_by_pass_mode_clicked ( gpointer distors, GtkRadioButton *button);

G_MODULE_EXPORT void on_log_rock_clicked(gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_log_rock_II_clicked(gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_hell_sqrt_clicked(gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_psychedelic_if_clicked(gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_by_60s_clicked(gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_fuzzy_dark_pow_IV_clicked(gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_rare_cuadratic_clicked(gpointer distors,GtkRadioButton *button);

///////CALLBACKS-JACK///////

int process (jack_nframes_t nframes, void *arg);
void jack_shutdown (void *arg);

#endif
