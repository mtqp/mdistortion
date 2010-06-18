
#include "callbacks.h"

////////////////////////////////////////////
//---------CALLBACKS-INTERFAZ-------------//
////////////////////////////////////////////

G_MODULE_EXPORT int on_m_distortion_destroy (GtkObject *object, gpointer user_data){
    gtk_main_quit();
	printf("ON M DISTORTION DESTROY SIN LIBERAR NADA PAPA\n");
	return false;	
}
//ES LA MISMA FUNCION, NO TIENE SENTIDO Q ESTE DOS VECES, MANDARLAS JUNTAS
G_MODULE_EXPORT int on_quit_clicked( GtkButton *button, gpointer   data ){
    gtk_main_quit ();
    printf("ON QUIT CLICKED SIN LIBERAR NADA\n");
    return false;
}

G_MODULE_EXPORT int on_info_clicked(GtkButton *button, gpointer data){
	/*global_ptr->_info_w = */open_sub_window("info_m_distortion");
	//gtk_widget_show((GtkWidget*) m_ui->info_m_distortion);
	//printf("las recupera y nunca mas las levanta...");
}

G_MODULE_EXPORT int on_save_clicked(GtkButton *button, gpointer data){
	/*global_ptr->_save_w = */open_sub_window("save_m_distortion");
	//gtk_widget_show((GtkWidget*) m_ui->save_m_distortion);
	//printf("una vez cerrada nunca las recupera");
}

//////////MODES////////////
G_MODULE_EXPORT void on_rock_mode_clicked (gpointer distors, GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, sensitivo);
	if(GTK_TOGGLE_BUTTON(button)->active){
		printf("set_m_distortion back to rock mode\n");
		set_m_distortion(back_to_rock_mode);
	}
}

G_MODULE_EXPORT void on_random_mode_clicked ( gpointer distors,GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, no_sensitivo);
	printf("set_m_distortion random day\n");
	set_m_distortion(e_random_day);
}

G_MODULE_EXPORT void on_mute_mode_clicked ( gpointer distors,GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, no_sensitivo);
	printf("set_m_distortion mute\n");
	set_m_distortion(e_mute);
}

G_MODULE_EXPORT void on_by_pass_mode_clicked ( gpointer distors, GtkRadioButton *button){
	gtk_widget_set_sensitive((GtkWidget*) distors, no_sensitivo);
	printf("set_m_distortion by pass\n");
	set_m_distortion(e_by_pass);
}

//////////////DISTORSIONES/////////////////
G_MODULE_EXPORT void on_hell_sqrt_clicked(gpointer distors,GtkRadioButton *button){
	printf("set_m_distortion hell sqrt\n");
	set_m_distortion(e_hell_sqrt);
}
G_MODULE_EXPORT void on_log_rock_clicked(gpointer distors,GtkRadioButton *button){
	printf("set_m_distortion log rock\n");
	set_m_distortion(e_log_rock);
}
G_MODULE_EXPORT void on_log_rock_II_clicked(gpointer distors,GtkRadioButton *button){
	printf("set_m_distortion log rock II\n");
	set_m_distortion(e_log_rock_II);
}
G_MODULE_EXPORT void on_psychedelic_if_clicked(gpointer distors,GtkRadioButton *button){
	printf("set_m_distortion psyche if\n");
	set_m_distortion(e_psychedelic_if);
}
G_MODULE_EXPORT void on_by_60s_clicked(gpointer distors,GtkRadioButton *button){
	printf("set_m_distortion by 60s\n");
	set_m_distortion(e_by_60s);
}
G_MODULE_EXPORT void on_fuzzy_dark_pow_IV_clicked(gpointer distors,GtkRadioButton *button){
	printf("set_m_distortion fuzzy dark pow iv\n");
	set_m_distortion(e_fuzzy_dark_pow_IV);
}
G_MODULE_EXPORT void on_rare_cuadratic_clicked(gpointer distors,GtkRadioButton *button){
	printf("set_m_distortion rare cuadratic\n");
	set_m_distortion(e_rare_cuadratic);
}

///////////////////EQ///////////////////////

G_MODULE_EXPORT 
void on_eq_onoff_toggled (gpointer eqs, GtkToggleButton *b){
	if(b->active) {
		printf("b->active\n");
		gtk_widget_set_sensitive((GtkWidget*) m_ui->eq_bars, sensitivo);
		gtk_widget_set_sensitive((GtkWidget*) m_ui->hell_sqrt,no_sensitivo);
		gtk_toggle_button_set_active((GtkToggleButton*) m_ui->dl_onoff,false); //apago el delay
		gtk_toggle_button_set_active((GtkToggleButton*) m_ui->hl_onoff,false); //apago el hall
		gtk_widget_set_sensitive((GtkWidget*)m_ui->dl_onoff, no_sensitivo);  //cancelo los botones para activarlo
		gtk_widget_set_sensitive((GtkWidget*)m_ui->hl_onoff, no_sensitivo); //PARECE NO ANDAR
		printf("last dist active = %d\n", last_dist_active);
		if(last_dist_active == e_hell_sqrt) {
			gtk_toggle_button_set_active((GtkToggleButton*)m_ui->by_60s,true);	//ESTO ES TRUE, X FAVOR HACER UN DEFINE!
			on_by_60s_clicked(NULL, m_ui->by_60s);
		}
	}
	else {
		gtk_widget_set_sensitive((GtkWidget*)m_ui->eq_bars,  no_sensitivo);
		gtk_widget_set_sensitive((GtkWidget*)m_ui->dl_onoff, sensitivo);  //cancelo los botones para activarlo
		gtk_widget_set_sensitive((GtkWidget*)m_ui->hl_onoff, sensitivo); //PARECE NO ANDAR
//		if((!m_ui->dl_onoff->active) && (!m_ui->hl_onoff->active))
			gtk_widget_set_sensitive((GtkWidget*) m_ui->hell_sqrt,sensitivo);
		
//VER XQ TODAVIA ANDA RE MAL!
	}
}

G_MODULE_EXPORT
void on_bass_scale_value_changed(GtkAdjustment *b_adjs, GtkRange *range ){
	printf("bass scale value changed\n");
}

G_MODULE_EXPORT
void on_mid_scale_value_changed(GtkAdjustment *m_adjs, GtkRange *range ){
	printf("mid scale value changed\n");
}

G_MODULE_EXPORT
void on_treb_scale_value_changed(GtkAdjustment *t_adjs, GtkRange *range ){
	printf("treb scale value changed\n");
}

G_MODULE_EXPORT
void on_volume_value_changed(GtkAdjustment *vol_adjs, GtkRange *range){
	printf("volume value changed\n");
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

////////////DELAY & HALL///////////////////////

G_MODULE_EXPORT
void on_delay_toggled(GtkToggleButton *eq, GtkToggleButton *button){
	static GtkWindow* dl_w;
	static int prev_eq_active = 0;//x las dudas lo seteo, pensar si no se jode...
	
	if(button->active){
		//gtk_window_get_position de m_d
		//gtk_window_move a m_delay ,x,y
		//m_distortion tiene 545x317 pxs!
		set_effect_window_position(m_ui->m_delay,e_delay);
		gtk_widget_show((GtkWidget*)m_ui->m_delay);
		prev_eq_active = eq->active;
		gtk_toggle_button_set_active(eq,false);	//eso es falso!
		gtk_widget_set_sensitive((GtkWidget*) m_ui->hell_sqrt,no_sensitivo);
		if(last_dist_active == e_hell_sqrt) {
			gtk_toggle_button_set_active((GtkToggleButton*)m_ui->by_60s,true);	//ESTO ES TRUE, X FAVOR HACER UN DEFINE!
			on_by_60s_clicked(NULL, m_ui->by_60s);
		}
	} else {
		gtk_widget_hide((GtkWidget*)m_ui->m_delay);
		gtk_toggle_button_set_active(eq,prev_eq_active);
	}
}

void set_effect_window_position(GtkWindow* window, int effect){
	int x,y;//por lo pronto anda solo para delay!
	int w,h;
	int w_d,h_d;
	gtk_window_get_position(m_ui->m_distortion, &x, &y);
//	printf("m dist x=%d , y=%d\n",x,y);
	gtk_window_get_size(m_ui->m_distortion, &w, &h);
	gtk_window_get_size(window,&w_d,&h_d);
	if(effect == e_delay) 		gtk_window_move(window,x,y-h_d);
	if(effect == e_hall)		gtk_window_move(window,x+w,y);
	/*	if(x-h_d>=0){	//tengo arriba espacio para ponerlo.
		gtk_window_move(window,x,y-h_d);
	} else {
		gtk_window_move(m_ui->m_delay,x,y+w);
	}*/
}

G_MODULE_EXPORT 
void on_dl_onoff_toggled(GtkContainer* sets, GtkToggleButton *on_offb){
	if(on_offb->active)
		gtk_widget_set_sensitive((GtkWidget*) sets, sensitivo);		
	else
		gtk_widget_set_sensitive((GtkWidget*) sets, no_sensitivo);
}


G_MODULE_EXPORT 
void on_hall_toggled(GtkToggleButton *eq, GtkToggleButton *button){
	static GtkWindow* ch_w;
	static int prev_eq_active = 0;//x las dudas lo seteo, pensar si no se jode...
	if(button->active) {
		set_effect_window_position(m_ui->m_hall,e_hall);
		gtk_widget_show((GtkWidget*)m_ui->m_hall);
		prev_eq_active = eq->active;
		gtk_toggle_button_set_active(eq,false);	//eso es falso!
		gtk_widget_set_sensitive((GtkWidget*) m_ui->hell_sqrt,no_sensitivo);
		if(last_dist_active == e_hell_sqrt) {
			gtk_toggle_button_set_active((GtkToggleButton*)m_ui->by_60s,true);	//ESTO ES TRUE, X FAVOR HACER UN DEFINE!
			on_by_60s_clicked(NULL, m_ui->by_60s);
		}
	}
	else {
		//set_m_distortion(m_dist,e_delay);  no existe mas delay papa
		gtk_widget_hide((GtkWidget*)m_ui->m_hall);
		gtk_toggle_button_set_active(eq,prev_eq_active);
	}
}




G_MODULE_EXPORT 
void on_hl_onoff_toggled(GtkContainer* intensity, GtkToggleButton *on_offb){
	if(on_offb->active)
		gtk_widget_set_sensitive((GtkWidget*) intensity, sensitivo);		
	else
		gtk_widget_set_sensitive((GtkWidget*) intensity, no_sensitivo);
}


