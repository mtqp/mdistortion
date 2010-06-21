
#include "callbacks.h"

		////////////////////////////////////////////
		//---------CALLBACKS-INTERFAZ-------------//
		////////////////////////////////////////////

/*Cierra el programa cuando es llamada por la interfaz*/
G_MODULE_EXPORT int on_quit_clicked( GtkButton *button, gpointer   data ){
    gtk_main_quit ();
    free_m_distortion_and_effects(m_dist);
	/*free(input_port);
	free(output_left);
	free(output_right);*/
    printf("LIBERA TODO M_DISTORTION, Y GTK, PENSAR COMO VER SI LIBERA TODO JACK\n");
    return false;
}

/*Abre ventana de información*/
G_MODULE_EXPORT int on_info_clicked(GtkButton *button, gpointer data){
	GtkWindow* w;
	w = open_sub_window("info_m_distortion");
}

/*Abre ventana graciosa*/
G_MODULE_EXPORT int on_save_clicked(GtkButton *button, gpointer data){
	GtkWindow* w;
	w = open_sub_window("save_m_distortion");	//NO ESTOY USANDO EL PARAMETRO Q DEVUELVE, OJO
}

				///////////////////////////
				//////////MODES////////////
				///////////////////////////
/*
	Existen varios modos posibles en M_DISTORTION, 
	dependiendo de cual se seleccione, se cambia a ese modo
	y habilita o deshabilita los efectos o distorsiones que
	NO se pueden utilizar en simultáneo.
*/
G_MODULE_EXPORT void on_rock_mode_clicked (gpointer distors, GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, sensitivo);
	if(GTK_TOGGLE_BUTTON(button)->active){
		set_m_distortion(m_dist, back_to_rock_mode);
		if(m_dist->_last_dist_active == e_hell_sqrt){
			on_hell_sqrt_clicked(NULL, m_ui->hell_sqrt);
		}
	}
}

G_MODULE_EXPORT void on_random_mode_clicked ( gpointer distors,GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, no_sensitivo);
	reset_effects_buttons();
	set_m_distortion(m_dist,e_random_day);
}

G_MODULE_EXPORT void on_mute_mode_clicked ( gpointer distors,GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, no_sensitivo);
	reset_effects_buttons();
	set_m_distortion(m_dist, e_mute);
}

G_MODULE_EXPORT void on_by_pass_mode_clicked ( gpointer distors, GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, no_sensitivo);
	reset_effects_buttons();
	set_m_distortion(m_dist, e_by_pass);
}
		///////////////////////////////////////////
		//////////////DISTORSIONES/////////////////
		///////////////////////////////////////////
/*
	Cada llamada desde la interfaz a una distorsión la setea en el programa, 
	así como además modifica las posibilidades de utilizar distintos efectos.
*/

G_MODULE_EXPORT void on_hell_sqrt_clicked(gpointer distors,GtkRadioButton *button){
	/*hell sqrt puede utilizarse sólo sin efectos*/
	gtk_toggle_button_set_active(m_ui->eq_onoff, false);	//apago el eq
	gtk_toggle_button_set_active(m_ui->dl_onoff, false); 	//apago el delay
	gtk_toggle_button_set_active(m_ui->hl_onoff, false); 	//apago el hall
	gtk_widget_set_sensitive((GtkWidget*)m_ui->eq_onoff, no_sensitivo);//deshabilito los botones para utilzarlos
	gtk_widget_set_sensitive((GtkWidget*)m_ui->dl_onoff, no_sensitivo);
	gtk_widget_set_sensitive((GtkWidget*)m_ui->hl_onoff, no_sensitivo);

	set_m_distortion(m_dist, e_hell_sqrt);					//seteo distorsion hell_sqrt
}

G_MODULE_EXPORT void on_log_rock_clicked(gpointer distors,GtkRadioButton *button){
	/*log rock puede utilizar todos los efectos*/
	reset_effects_buttons();
	set_m_distortion(m_dist, e_log_rock);
}
G_MODULE_EXPORT void on_log_rock_II_clicked(gpointer distors,GtkRadioButton *button){
	/*log rock II puede utilizar todos los efectos*/
	reset_effects_buttons();
	set_m_distortion(m_dist, e_log_rock_II);
}
G_MODULE_EXPORT void on_psychedelic_if_clicked(gpointer distors,GtkRadioButton *button){
	/*psychedelic if puede utilizar todos los efectos*/
	reset_effects_buttons();
	set_m_distortion(m_dist, e_psychedelic_if);
}
G_MODULE_EXPORT void on_by_60s_clicked(gpointer distors,GtkRadioButton *button){
	/*by 60s puede utilizar todos los efectos*/
	reset_effects_buttons();
	set_m_distortion(m_dist, e_by_60s);
}
G_MODULE_EXPORT void on_fuzzy_dark_pow_IV_clicked(gpointer distors,GtkRadioButton *button){
	/*fuzzy dark pow IV puede utilizar todos los efectos*/
	reset_effects_buttons();
	set_m_distortion(m_dist, e_fuzzy_dark_pow_IV);
}
G_MODULE_EXPORT void on_rare_cuadratic_clicked(gpointer distors,GtkRadioButton *button){
	/*rare cuadratic puede utilizar todos los efectos*/
	reset_effects_buttons();
	set_m_distortion(m_dist, e_rare_cuadratic);
}
				///////////////////
				////////EQ/////////
				///////////////////
/*
	Activa o desactiva el ecualizador, además de recalcular parámetros
	que son modificables por el usuario
*/
G_MODULE_EXPORT 
void on_eq_onoff_toggled (gpointer eqs, GtkToggleButton *b){
	/*el ecualizador funciona sin los efectos delay y hall*/
	if(b->active) {
		/*activa eq*/
		equalizer_effect = f_effect[e_equalizer];
		gtk_widget_set_sensitive((GtkWidget*) m_ui->eq_bars, sensitivo);
		gtk_toggle_button_set_active((GtkToggleButton*) m_ui->dl_onoff,false); //apaga el delay
		gtk_toggle_button_set_active((GtkToggleButton*) m_ui->hl_onoff,false); //apaga el hall
	}
	else {
		/*desactiva eq*/
		equalizer_effect = f_effect[e_dummy];
		gtk_widget_set_sensitive((GtkWidget*)m_ui->eq_bars,  no_sensitivo); 
	}
}

/*
	Recalculo de parámetros modificados por el usuario
*/
G_MODULE_EXPORT
void on_bass_scale_value_changed(GtkAdjustment *b_adjs, GtkRange *range ){
	float band = (5.0*b_adjs->value)+6.5;
	reset_eq_params(m_dist->m_bass, band, LPF);
}

G_MODULE_EXPORT
void on_mid_scale_value_changed(GtkAdjustment *m_adjs, GtkRange *range ){
	float band = (4.0*m_adjs->value)+10.5;
	reset_eq_params(m_dist->m_mid, band, BPF);
}

G_MODULE_EXPORT
void on_treb_scale_value_changed(GtkAdjustment *t_adjs, GtkRange *range ){
	float band = (4.0*t_adjs->value)+5.5;
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

				///////////////////////////
				//////////DELAY////////////
				///////////////////////////

/*
	Activa o desactiva el delay, además de recalcular parámetros
	que son modificables por el usuario
*/

G_MODULE_EXPORT
void on_delay_toggled(GtkToggleButton *eq, GtkToggleButton *button){
	if(button->active){
		/*abre ventana del delay*/
		set_effect_window_position(m_ui->m_delay,e_delay);
		gtk_widget_show((GtkWidget*)m_ui->m_delay); 
	} else {
		/*esconde la ventana del delay*/
		gtk_widget_hide((GtkWidget*)m_ui->m_delay); 
	}
}

G_MODULE_EXPORT 
void on_dl_onoff_toggled(GtkContainer* sets, GtkToggleButton *on_offb){
	if(on_offb->active){
		/*activa el delay*/
		gtk_widget_set_sensitive((GtkWidget*) sets, sensitivo);		
		delay_effect = f_effect[e_delay];
		/*desactiva eq*/
		gtk_toggle_button_set_active((GtkToggleButton*) m_ui->eq_onoff,false);
	}
	else {
		/*desactiva delay*/
		gtk_widget_set_sensitive((GtkWidget*) sets, no_sensitivo);
		delay_effect = f_effect[e_dummy];
	}
}

/*setea la cantidad de delay que va a tener el output*/
G_MODULE_EXPORT void on_q_one_clicked(gpointer n, GtkRadioButton* b){
	m_dist->_delay->dl_cant_bufs_active = 1;
}
G_MODULE_EXPORT void on_q_two_clicked(gpointer n, GtkRadioButton* b){
	m_dist->_delay->dl_cant_bufs_active = 2;
}
G_MODULE_EXPORT void on_q_three_clicked(gpointer n, GtkRadioButton* b){
	m_dist->_delay->dl_cant_bufs_active = 3;
}
G_MODULE_EXPORT void on_q_four_clicked(gpointer n, GtkRadioButton* b){
	m_dist->_delay->dl_cant_bufs_active = 4;
}

G_MODULE_EXPORT void on_q_extreme_clicked(gpointer n, GtkRadioButton* b){
	m_dist->_delay->dl_cant_bufs_active = m_dist->_delay->dl_total_bufs;
}

G_MODULE_EXPORT void on_dl_speed_scale_value_changed(GtkAdjustment* dl_speed_adjs, GtkRange* range){
	/*ajusta la velocidad del delay a el valor de la barra*/
	m_dist->_delay->dl_speed = (int)dl_speed_adjs->value;	//todavia hay q ver como arreglar el cambiar la velocidad
}

////////////HALL///////////////////////
G_MODULE_EXPORT 
void on_hall_toggled(GtkToggleButton *eq, GtkToggleButton *button){
	if(button->active) {
		/*abre ventana de hall*/
		set_effect_window_position(m_ui->m_hall,e_hall);
		gtk_widget_show((GtkWidget*)m_ui->m_hall);
	}
	else {
		/*esconde ventana de hall*/
		gtk_widget_hide((GtkWidget*)m_ui->m_hall);
	}
}

G_MODULE_EXPORT 
void on_hl_onoff_toggled(GtkContainer* intensity, GtkToggleButton *on_offb){
	if(on_offb->active){
		/*activa hall*/
		gtk_widget_set_sensitive((GtkWidget*) intensity, sensitivo);
		hall_effect = f_effect[e_hall];
		/*desactiva eq*/
		gtk_toggle_button_set_active((GtkToggleButton*) m_ui->eq_onoff,false);

	}
	else{
		/*desactiva hall*/
		gtk_widget_set_sensitive((GtkWidget*) intensity, no_sensitivo);
		hall_effect = f_effect[e_dummy];
	}
}

G_MODULE_EXPORT void on_hl_intensity_scale_value_changed(GtkAdjustment* hl_intensity_adjs, GtkRange* range){
	/*ajusta el porcentaje de hall en la señal dado x la barra en la interfaz*/
	m_dist->_hall->hll_coef = hl_intensity_adjs->value + 1.0;
}

////////////////////////////////////////////
//-----------CALLBACKS-JACK---------------//
////////////////////////////////////////////

/*
	Callback a la que llama jackd para procesar el buffer.
	'distortionize' es el método de M_DISTORTION que procesa
*/
int process (jack_nframes_t nframes, void *arg){
	jack_default_audio_sample_t *outL = (jack_default_audio_sample_t *) jack_port_get_buffer (output_left, nframes);
  	jack_default_audio_sample_t *outR = (jack_default_audio_sample_t *) jack_port_get_buffer (output_right, nframes);
	jack_default_audio_sample_t *in = (jack_default_audio_sample_t *) jack_port_get_buffer (input_port, nframes);

	memcpy (outL, in, sizeof (jack_default_audio_sample_t) * nframes);
	memcpy (outR, in, sizeof (jack_default_audio_sample_t) * nframes);
	
	distortionize(m_dist, outL, sizeof (jack_default_audio_sample_t) * nframes); //la gran magia

	return 0;      
}

/*
	Cierra jackd
*/
void jack_shutdown (void *arg){
	exit (1);
}

/*
	LAS VENTANAS TENDRIAN Q SEGUIR A MDISTORTION SI SE CORRE ELLA
*/


//////////////AUXs////////////////
void set_effect_window_position(GtkWindow* window, int effect){
	int x,y;
	int w,h;
	int w_d,h_d;
	gtk_window_get_position(m_ui->m_distortion, &x, &y);
	gtk_window_get_size(m_ui->m_distortion, &w, &h);
	gtk_window_get_size(window,&w_d,&h_d);
	/*inicializa la ventana de delay arriba de M_DISTORTION*/
	if(effect == e_delay) 		gtk_window_move(window,x,y-h_d-1);
	/*inicializa la ventana de hall a la derecha de M_DISTORTION*/
	if(effect == e_hall)		gtk_window_move(window,x+w+4,y);
}

/*reseteo de botones de efectos*/
void reset_effects_buttons(){
	gtk_widget_set_sensitive((GtkWidget*)m_ui->eq_onoff, sensitivo);
	if(!m_ui->eq_onoff->active){
		gtk_widget_set_sensitive((GtkWidget*)m_ui->dl_onoff, sensitivo);
		gtk_widget_set_sensitive((GtkWidget*)m_ui->hl_onoff, sensitivo);
	}
}

