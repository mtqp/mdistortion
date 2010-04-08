#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <jack/jack.h>

#include "globals.h"
#include "callbacks.h"

////////////////////////////////////////////
//---------CALLBACKS-INTERFAZ-------------//
////////////////////////////////////////////

G_MODULE_EXPORT bool on_m_distortion_destroy (GtkObject *object, gpointer user_data)
{
    gtk_main_quit();
	return false;	
}

G_MODULE_EXPORT bool  on_quit_clicked( GtkButton *button, gpointer   data ) {
    gtk_main_quit ();
    free_m_distortion(m_dist);
    return false;
}

//////////MODES////////////
G_MODULE_EXPORT void on_rock_mode_clicked (gpointer distors, GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, true);//sensitivo);
	set_m_distortion(m_dist, -1);
}

G_MODULE_EXPORT void on_random_mode_clicked ( gpointer distors,GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, false);//no_sensitivo);
	set_m_distortion(m_dist,7);//e_random_day);		//quizas es mejor hacer un enum de los numeros para q se entienda mas!
}

G_MODULE_EXPORT void on_mute_mode_clicked ( gpointer distors,GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, false);//no_sensitivo);
	set_m_distortion(m_dist, 8);//e_mute);
}

G_MODULE_EXPORT void on_by_pass_mode_clicked ( gpointer distors, GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, false);//no_sensitivo);
	set_m_distortion(m_dist, 9);//e_by_pass);
}

//////////////DISTORSIONES/////////////////
G_MODULE_EXPORT void on_log_rock_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 0);//e_log_rock);
}
G_MODULE_EXPORT void on_log_rock_II_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 1);//e_log_rock_II);
}
G_MODULE_EXPORT void on_hell_sqrt_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 2);//e_hell_sqrt);
}
G_MODULE_EXPORT void on_psychedelic_if_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 3);//e_psychedelic_if);
}
G_MODULE_EXPORT void on_by_60s_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 4);//e_by_60s);
}
G_MODULE_EXPORT void on_fuzzy_dark_pow_IV_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 5);//e_fuzzy_dark_pow_IV);
}
G_MODULE_EXPORT void on_rare_cuadratic_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 6);//e_rare_cuadratic);
}

///////////////////EQ///////////////////////

G_MODULE_EXPORT 
void on_eq_onoff_toggled (gpointer eqs, GtkToggleButton *b){
	if(global_eq_sensitive)	global_eq_sensitive = 0;
	else					global_eq_sensitive = 1;
	gtk_widget_set_sensitive((GtkWidget*) eqs, global_eq_sensitive);
	g_print("eq sensitive == %d\n", global_eq_sensitive);
}

G_MODULE_EXPORT
static gchar* on_bass_scale_format_value (GtkScale *scale, gdouble   value) {
	return g_strdup_printf ("-->%0.*g<--", gtk_scale_get_digits (scale), value);
}

/*G_MODULE_EXPORT void on_bass_scale_format_value (GtkScale *bass, gpointer eqs){
	g_print("cambiando valor de bass, se mueve la giladA?\n");
}*/
////////////////////////////////////////////
//-----------CALLBACKS-JACK---------------//
////////////////////////////////////////////

int process (jack_nframes_t nframes, void *arg)
{
	jack_default_audio_sample_t *outL = (jack_default_audio_sample_t *) jack_port_get_buffer (output_left, nframes);
  	jack_default_audio_sample_t *outR = (jack_default_audio_sample_t *) jack_port_get_buffer (output_right, nframes);
	jack_default_audio_sample_t *in = (jack_default_audio_sample_t *) jack_port_get_buffer (input_port, nframes);

	memcpy (outL, in, sizeof (jack_default_audio_sample_t) * nframes);
	memcpy (outR, in, sizeof (jack_default_audio_sample_t) * nframes);

	distortionize(m_dist, outL, sizeof (jack_default_audio_sample_t) * nframes); //la gran magia

	return 0;      
}

void jack_shutdown (void *arg) {
	exit (1);
}


