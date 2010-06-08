#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <gtk/gtk.h>
#include <jack/jack.h>

#include "callbacks.h"
#include "globals.h"
#include "enum_dist.h"
////////////////////////////////////////////
//---------CALLBACKS-INTERFAZ-------------//
////////////////////////////////////////////

G_MODULE_EXPORT bool on_m_distortion_destroy (GtkObject *object, gpointer user_data){
    gtk_main_quit();
	return false;	
}

G_MODULE_EXPORT bool  on_quit_clicked( GtkButton *button, gpointer   data ){
    gtk_main_quit ();
    free_m_distortion(m_dist);
    return false;
}

G_MODULE_EXPORT bool on_info_clicked(GtkButton *button, gpointer data){
	//llama a un nuevo archivo de glade x lo pronto, dsp lo arreglamos en el mismo archivo
	GtkBuilder *builder;
	GtkWidget  *window;
	GError     *error = NULL;

	/* Create new GtkBuilder object */
	builder = gtk_builder_new();

	// Load UI from file. If error occurs, report it and quit application.
	if( ! gtk_builder_add_from_file( builder, "info_m_distortion.glade", &error ) ){
		g_warning( "%s", error->message );
		g_free( error );
		//g_print("couldnt create windows at all\n");
		return( 0 );
	}

	/* Get main window pointer from UI */
	window = GTK_WIDGET( gtk_builder_get_object( builder, "info_m_distortion" ) );
	/* Connect signals */
	gtk_builder_connect_signals( builder, NULL );
	/* Destroy builder, since we don't need it anymore */
	g_object_unref( G_OBJECT( builder ) );
	/* Show window. All other widgets are automatically shown by GtkBuilder */
	gtk_widget_show( window );
}

//////////MODES////////////
G_MODULE_EXPORT void on_rock_mode_clicked (gpointer distors, GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, true);//sensitivo);
	set_m_distortion(m_dist, -1);
}

G_MODULE_EXPORT void on_random_mode_clicked ( gpointer distors,GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, false);//no_sensitivo);
	set_m_distortion(m_dist,e_random_day);		//quizas es mejor hacer un enum de los numeros para q se entienda mas!
}

G_MODULE_EXPORT void on_mute_mode_clicked ( gpointer distors,GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, false);//no_sensitivo);
	set_m_distortion(m_dist, e_mute);
}

G_MODULE_EXPORT void on_by_pass_mode_clicked ( gpointer distors, GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, false);//no_sensitivo);
	set_m_distortion(m_dist, e_by_pass);
}

//////////////DISTORSIONES/////////////////
G_MODULE_EXPORT void on_log_rock_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, e_log_rock);
}
G_MODULE_EXPORT void on_log_rock_II_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, e_log_rock_II);
}
G_MODULE_EXPORT void on_hell_sqrt_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, e_hell_sqrt);
}
G_MODULE_EXPORT void on_psychedelic_if_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, e_psychedelic_if);
}
G_MODULE_EXPORT void on_by_60s_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, e_by_60s);
}
G_MODULE_EXPORT void on_fuzzy_dark_pow_IV_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, e_fuzzy_dark_pow_IV);
}
G_MODULE_EXPORT void on_rare_cuadratic_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, e_rare_cuadratic);
}

///////////////////EQ///////////////////////

G_MODULE_EXPORT 
void on_eq_onoff_toggled (gpointer eqs, GtkToggleButton *b){
	if(global_ptr->_eq_sensitive)	global_ptr->_eq_sensitive = 0;
	else							global_ptr->_eq_sensitive = 1;
	gtk_widget_set_sensitive((GtkWidget*) eqs, global_ptr->_eq_sensitive);
	g_print("eq_sensitive = %d\n",global_ptr->_eq_sensitive);
}

G_MODULE_EXPORT
void on_bass_scale_value_changed(GtkAdjustment *b_adjs, GtkRange *range ){
	//g_print("bass eq is not working right now == %f\n", b_adjs->value);
	float band = (5.0*b_adjs->value)+6.5;
	g_print("lpf band = %f\n", band);	
	lpf_reset_eq_params(m_dist->m_bass, m_dist->m_bass->_freq, m_dist->m_bass->_srate, band);
}

G_MODULE_EXPORT
void on_mid_scale_value_changed(GtkAdjustment *m_adjs, GtkRange *range ){
	//g_print("mid eq is not working right now == %f\n", m_adjs->value);
	float band = (4.0*m_adjs->value)+10.5;
	g_print("bpf band = %f\n", band);	
	bpf_reset_eq_params(m_dist->m_mid, m_dist->m_mid->_freq, m_dist->m_mid->_srate, band);
}

G_MODULE_EXPORT
void on_treb_scale_value_changed(GtkAdjustment *t_adjs, GtkRange *range ){
	//g_print("treb eq is not working right now == %f\n", t_adjs->value);
	float band = (4.0*t_adjs->value)+5.5;
	g_print("hpf band = %f\n", band);	
	hpf_reset_eq_params(m_dist->m_treb, m_dist->m_treb->_freq, m_dist->m_treb->_srate, band);
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

int process (jack_nframes_t nframes, void *arg){
	jack_default_audio_sample_t *outL = (jack_default_audio_sample_t *) jack_port_get_buffer (output_left, nframes);
  	jack_default_audio_sample_t *outR = (jack_default_audio_sample_t *) jack_port_get_buffer (output_right, nframes);
	jack_default_audio_sample_t *in = (jack_default_audio_sample_t *) jack_port_get_buffer (input_port, nframes);

	memcpy (outL, in, sizeof (jack_default_audio_sample_t) * nframes);
	memcpy (outR, in, sizeof (jack_default_audio_sample_t) * nframes);
	
	distortionize(m_dist, outL, sizeof (jack_default_audio_sample_t) * nframes); //la gran magia
	return 0;      
}

void jack_shutdown (void *arg){
	exit (1);
}


