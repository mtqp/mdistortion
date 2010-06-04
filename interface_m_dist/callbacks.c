#include <stdlib.h>
#include <string.h>
#include <math.h>

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
	if(global_ptr->_eq_sensitive)	global_ptr->_eq_sensitive = 0;
	else							global_ptr->_eq_sensitive = 1;
	gtk_widget_set_sensitive((GtkWidget*) eqs, global_ptr->_eq_sensitive);
//	g_print("eq sensitive == %d\n", global_eq_sensitive);
}

G_MODULE_EXPORT
void on_bass_scale_value_changed(GtkAdjustment *b_adjs, GtkRange *range ){
//	g_print("bass eq is not working right now == %f\n", b_adjs->value);
	float rate = (m_dist->m_eq->_srate * b_adjs->value*10000.0) +4096.0;
	g_print("srate = %f\n", rate);
	lsh_reset_eq_params(m_dist->m_eq, m_dist->m_eq->_dbgain, m_dist->m_eq->_freq, rate);
/*	dt = (t_adjs->value);
	RC = 1.0;
	alpha = RC / (RC+dt);
	global_ptr->alpha = b_adjs->value;*/
	//m_dist->m_eq->lg = b_adjs->value+1.0;
	//set_gain(m_dist->m_eq->lg,b_adjs->value+1.0);
}

G_MODULE_EXPORT
void on_mid_scale_value_changed(GtkAdjustment *m_adjs, GtkRange *range ){
	float freq = 800.0 + (8000.0*m_adjs->value);
	g_print("freq = %f\n", freq);
	lsh_reset_eq_params(m_dist->m_eq, m_dist->m_eq->_dbgain, freq, m_dist->m_eq->_srate);

//	g_print("mid eq is not working right now == %f\n", m_adjs->value);
//	m_dist->m_eq->mg = m_adjs->value+1.0;
	//set_gain(m_dist->m_eq->mg,m_adjs->value+1.0);
}

G_MODULE_EXPORT
void on_treb_scale_value_changed(GtkAdjustment *t_adjs, GtkRange *range ){
	float gain = (100000.0*t_adjs->value)+1.0;
	g_print("gain = %f\n", gain);	
	lsh_reset_eq_params(m_dist->m_eq, gain, m_dist->m_eq->_freq, m_dist->m_eq->_srate);
//	g_print("treb eq is not working right now == %f\n", t_adjs->value);
//	m_dist->m_eq->hg = t_adjs->value+1.0;
	//set_gain(m_dist->m_eq->hg,t_adjs->value+1.0);
}

G_MODULE_EXPORT
void on_volume_value_changed(GtkAdjustment *vol_adjs, GtkRange *range){
	m_dist->_dvol = vol_adjs->value;
}

G_MODULE_EXPORT
void on_reset_bass_clicked(gpointer reset, GtkRadioButton *button){
	gtk_adjustment_set_value(reset,0.0);
} 

G_MODULE_EXPORT
void on_reset_mid_clicked(gpointer reset, GtkRadioButton *button){
	gtk_adjustment_set_value(reset,0.0);
} 

G_MODULE_EXPORT
void on_reset_treb_clicked(gpointer reset, GtkRadioButton *button){
	gtk_adjustment_set_value(reset,0.0);
} 

////////////NOISE-RED///////////////////////

G_MODULE_EXPORT
void on_noise_toggled(gpointer p, GtkToggleButton *button){
	if(global_ptr->_noise_toggled) 	global_ptr->_noise_toggled = 0;
	else					 		global_ptr->_noise_toggled = 1;						
	g_print("noise reduction still not working, toggled button value == %d\n", global_ptr->_noise_toggled);
}


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


