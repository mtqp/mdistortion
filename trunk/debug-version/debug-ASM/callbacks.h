#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "globals.h"
#include "enum_dist.h"
#include "enum_effects.h"
#include "m_window.h"

#define false 0
#define true  1

///////CALLBACKS-GTK///////

G_MODULE_EXPORT int on_quit_clicked( GtkButton *button, gpointer   data );
G_MODULE_EXPORT int on_info_clicked(GtkButton *button, gpointer data);
G_MODULE_EXPORT int on_save_clicked(GtkButton *button, gpointer data);
/*G_MODULE_EXPORT void on_info_m_distortion_hide(GtkObject* o, gpointer data);
G_MODULE_EXPORT void on_save_m_distortion_hide(GtkObject* o, gpointer data);*/
		/*MODES*/
G_MODULE_EXPORT void on_rock_mode_clicked (gpointer distors, GtkRadioButton *button);
G_MODULE_EXPORT void on_random_mode_clicked ( gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_mute_mode_clicked ( gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_by_pass_mode_clicked ( gpointer distors, GtkRadioButton *button);

		/*DISTORS*/
G_MODULE_EXPORT void on_log_rock_clicked(gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_log_rock_II_clicked(gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_hell_sqrt_clicked(gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_psychedelic_if_clicked(gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_by_60s_clicked(gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_fuzzy_dark_pow_IV_clicked(gpointer distors,GtkRadioButton *button);
G_MODULE_EXPORT void on_rare_cuadratic_clicked(gpointer distors,GtkRadioButton *button);

		  /*EQ*/
G_MODULE_EXPORT void on_eq_onoff_toggled (gpointer eqs, GtkToggleButton *b);
G_MODULE_EXPORT void on_bass_scale_value_changed(GtkAdjustment *b_adjs, GtkRange *range );
G_MODULE_EXPORT void on_mid_scale_value_changed(GtkAdjustment *m_adjs, GtkRange *range );
G_MODULE_EXPORT void on_treb_scale_value_changed(GtkAdjustment *t_adjs, GtkRange *range );
G_MODULE_EXPORT void on_reset_bass_clicked(gpointer r, GtkRadioButton *button);
G_MODULE_EXPORT void on_reset_mid_clicked(gpointer r, GtkRadioButton *button);
G_MODULE_EXPORT void on_reset_treb_clicked(gpointer r, GtkRadioButton *button);

		/*VOLUME*/
G_MODULE_EXPORT void on_volume_value_changed(GtkAdjustment *vol_adjs, GtkRange *range);

   		/*DELAY*/
G_MODULE_EXPORT void on_delay_toggled(GtkToggleButton *eq, GtkToggleButton *button);
G_MODULE_EXPORT void on_dl_onoff_toggled(GtkContainer* sets, GtkToggleButton *on_offb);
G_MODULE_EXPORT void on_q_one_clicked(gpointer n, GtkRadioButton* b);
G_MODULE_EXPORT void on_q_two_clicked(gpointer n, GtkRadioButton* b);
G_MODULE_EXPORT void on_q_three_clicked(gpointer n, GtkRadioButton* b);
G_MODULE_EXPORT void on_q_four_clicked(gpointer n, GtkRadioButton* b);
G_MODULE_EXPORT void on_q_extreme_clicked(gpointer n, GtkRadioButton* b);
G_MODULE_EXPORT void on_dl_speed_scale_value_changed(GtkAdjustment* dl_speed_adjs, GtkRange* range);

		/*HALL*/
G_MODULE_EXPORT void on_hall_toggled(GtkToggleButton *eq, GtkToggleButton *button);
G_MODULE_EXPORT void on_hl_onoff_toggled(GtkContainer* intensity, GtkToggleButton *on_offb);
G_MODULE_EXPORT void on_hl_intensity_scale_value_changed(GtkAdjustment* hl_intensity_adjs, GtkRange* range);

///////////AUXs//////////////
void set_effect_window_position(GtkWindow* w, int effect);
void reset_effects_buttons();

/////////DUM-AUDIO//////////
void on_dum_callback_clicked();
#endif
