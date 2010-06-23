
#include "m_user_interface.h"

void init_m_ui(m_user_interface* m_ui, GtkBuilder *builder, GtkWindow* window){
	m_ui->m_distortion 	= (GtkWindow*) 		window;
	m_ui->box_eq 	   	= (GtkContainer*) 	gtk_builder_get_object(builder, "box_eq");
	m_ui->eq_onoff 		= (GtkToggleButton*)gtk_builder_get_object(builder,"eq_onoff");
	m_ui->extra_effects = (GtkContainer*) 	gtk_builder_get_object(builder,"extra_effects");
	m_ui->delay 		= (GtkToggleButton*)gtk_builder_get_object(builder,"delay");
	m_ui->hall 			= (GtkToggleButton*)gtk_builder_get_object(builder,"hall");
	m_ui->eq_bars 		= (GtkContainer*) 	gtk_builder_get_object(builder,"eq_bars");
	m_ui->distors 		= (GtkContainer*) 	gtk_builder_get_object(builder,"distors");
	m_ui->hell_sqrt 	= (GtkRadioButton*) gtk_builder_get_object(builder,"hell_sqrt");
	m_ui->by_60s		= (GtkRadioButton*) gtk_builder_get_object(builder,"by_60s");
	m_ui->volume 		= (GtkScaleButton*) gtk_builder_get_object(builder,"volume");
	m_ui->m_delay 		= (GtkWindow*)/*(GtkWidget*)*/ 		gtk_builder_get_object(builder,"m_delay");
	m_ui->dl_onoff 		= (GtkToggleButton*)gtk_builder_get_object(builder,"dl_onoff");
	m_ui->dl_sets 		= (GtkContainer*) 	gtk_builder_get_object(builder,"dl_sets");
	m_ui->m_hall 		= (GtkWindow*)/*(GtkWidget*)*/ 		gtk_builder_get_object(builder,"m_hall");
	m_ui->hl_onoff 		= (GtkToggleButton*)gtk_builder_get_object(builder,"hl_onoff");
	m_ui->chintensity 	= (GtkContainer*) 	gtk_builder_get_object(builder,"chintensity");
	/*m_ui->info_m_distortion = (GtkWindow*)  gtk_builder_get_object(builder,"info_m_distortion");
	m_ui->save_m_distortion = (GtkWindow*)  gtk_builder_get_object(builder,"save_m_distortion");*/
}
