
#include "callbacks.h"

////////////////////////////////////////////
//---------CALLBACKS-INTERFAZ-------------//
////////////////////////////////////////////

G_MODULE_EXPORT int on_m_distortion_destroy (GtkObject *object, gpointer user_data){
    gtk_main_quit();
	return false;	
}

G_MODULE_EXPORT int on_quit_clicked( GtkButton *button, gpointer   data ){
    gtk_main_quit ();
    /*free_m_distortion(m_dist);
    jack_shutdown(input_port);
    jack_shutdown(output_left);
    jack_shutdown(output_right);*/
    printf("SIN LIBERAR NADA\n");
    return false;
}

G_MODULE_EXPORT int on_info_clicked(GtkButton *button, gpointer data){
	open_window(window_info);
}

G_MODULE_EXPORT int on_save_clicked(GtkButton *button, gpointer data){
	open_window(window_save);
}

//////////MODES////////////
G_MODULE_EXPORT void on_rock_mode_clicked (gpointer distors, GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, sensitivo);
	if(GTK_TOGGLE_BUTTON(button)->active){
		set_m_distortion(m_dist, back_to_rock_mode);
	}
}

G_MODULE_EXPORT void on_random_mode_clicked ( gpointer distors,GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, no_sensitivo);
	set_m_distortion(m_dist,e_random_day);
}

G_MODULE_EXPORT void on_mute_mode_clicked ( gpointer distors,GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, no_sensitivo);
	set_m_distortion(m_dist, e_mute);
}

G_MODULE_EXPORT void on_by_pass_mode_clicked ( gpointer distors, GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, no_sensitivo);
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
//	g_print("eq_sensitive = %d\n",global_ptr->_eq_sensitive);
}

G_MODULE_EXPORT
void on_bass_scale_value_changed(GtkAdjustment *b_adjs, GtkRange *range ){
	float band = (5.0*b_adjs->value)+6.5;
//	g_print("lpf band = %f\n", band);	
	reset_eq_params(m_dist->m_bass, band, LPF);
}

G_MODULE_EXPORT
void on_mid_scale_value_changed(GtkAdjustment *m_adjs, GtkRange *range ){
	float band = (4.0*m_adjs->value)+10.5;
//	g_print("bpf band = %f\n", band);	
	reset_eq_params(m_dist->m_mid, band, BPF);
}

G_MODULE_EXPORT
void on_treb_scale_value_changed(GtkAdjustment *t_adjs, GtkRange *range ){
	float band = (4.0*t_adjs->value)+5.5;
//	g_print("hpf band = %f\n", band);	
	reset_eq_params(m_dist->m_treb, band, HPF);
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
void on_delay_toggled(gpointer p, GtkToggleButton *button){
	if(global_ptr->_noise_toggled) {
		global_ptr->_noise_toggled = 0;	//esto no puede ser un nuemero HACER UN DEFINE LOCOOOO}
		set_m_distortion(m_dist,back_to_rock_mode);
	}
	else {
		set_m_distortion(m_dist,e_delay);
		global_ptr->_noise_toggled = 1;
	}
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


