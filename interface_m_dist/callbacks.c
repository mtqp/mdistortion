#include <gtk/gtk.h>

#include "callbacks.h"
/*
////////////////////////////////////////////
///////////CALLBACKS-INTERFAZ///////////////
////////////////////////////////////////////

//cierra con la X
G_MODULE_EXPORT bool on_m_distortion_destroy (GtkObject *object, gpointer user_data)
{
    gtk_main_quit();
	return false;	
}

//cierra con el boton QUIT
G_MODULE_EXPORT bool  on_quit_clicked( GtkButton *button, gpointer   data ) {
    gtk_main_quit ();
    return false;
}
///////////////////////////////////////////
G_MODULE_EXPORT void on_rock_mode_clicked (gpointer distors, GtkRadioButton *button){
	g_print("CLICKED ROCK AND ROOL BABY\n");
	gtk_widget_set_sensitive((GtkWidget*) distors, true);//sensitivo);
}

G_MODULE_EXPORT void on_random_mode_clicked ( gpointer distors,GtkRadioButton *button){
	g_print("CLICKED random mode\n");
	gtk_widget_set_sensitive((GtkWidget*) distors, false);//no_sensitivo);
}

G_MODULE_EXPORT void on_by_pass_mode_clicked ( gpointer distors, GtkRadioButton *button){
	g_print("CLICKED by_pass\n");
	gtk_widget_set_sensitive((GtkWidget*) distors, false);//no_sensitivo);
}

G_MODULE_EXPORT void on_mute_mode_clicked ( gpointer distors,GtkRadioButton *button){
	g_print("CLICKED muteeeee\n");
	gtk_widget_set_sensitive((GtkWidget*) distors, false);//no_sensitivo);
}
*/

