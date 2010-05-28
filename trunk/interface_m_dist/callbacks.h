#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include <gtk/gtk.h>
#include <jack/jack.h>

#ifndef bool		//nadie usa el bool, ni es necesario
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

G_MODULE_EXPORT void on_eq_onoff_toggled (gpointer eqs, GtkToggleButton *b);
G_MODULE_EXPORT void on_bass_scale_value_changed(GtkAdjustment *b_adjs, GtkRange *range );
G_MODULE_EXPORT void on_mid_scale_value_changed(GtkAdjustment *m_adjs, GtkRange *range );
G_MODULE_EXPORT void on_treb_scale_value_changed(GtkAdjustment *t_adjs, GtkRange *range );
G_MODULE_EXPORT void on_reset_bass_clicked(gpointer r, GtkRadioButton *button);
G_MODULE_EXPORT void on_reset_mid_clicked(gpointer r, GtkRadioButton *button);
G_MODULE_EXPORT void on_reset_treb_clicked(gpointer r, GtkRadioButton *button);

G_MODULE_EXPORT void on_volume_value_changed(GtkAdjustment *vol_adjs, GtkRange *range);


///////CALLBACKS-JACK///////

int process (jack_nframes_t nframes, void *arg);
void jack_shutdown (void *arg);

#endif
