/*
	* 
	*BIENVENIDOS AL PROGRAMA M_DISTORTION!
	* 
*/
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "m_distortion.h"
#include "globals.h"
#include "m_user_interface.h"
#include "callbacks.h"

//#include <jack/jack.h>
#include <gtk/gtk.h>


/////////////////////////////////////////////
//-------------CODIGO-MAIN-----------------//
/////////////////////////////////////////////
 
	int main (int argc, char *argv[]) {

		///////////////////////////////////////
		//-------inicializar m dist----------//
		///////////////////////////////////////
		
		m_dist = (m_distortion *) malloc(sizeof(m_distortion));
		init_m_distortion(m_dist);
		
		///////////////////////////////////////
		//----inicializar interfaz grafica---//
		///////////////////////////////////////

		GtkBuilder *builder;
		GtkWidget  *window;
		GError     *error = NULL;

		/* Init GTK+ */
		gtk_init( &argc, &argv );
		/* Create new GtkBuilder object */
		builder = gtk_builder_new();

		// Load UI from file. If error occurs, report it and quit application.
		if( ! gtk_builder_add_from_file( builder, "interface_m_dist.glade", &error ) ){
			g_warning( "%s", error->message );
			g_free( error );
			return( 1 );
		}

		/* Get main window pointer from UI */
		window = GTK_WIDGET( gtk_builder_get_object( builder, "m_distortion" ) );
		
		//*Set Up m_user_interface Structure*//
		m_ui = (m_user_interface*) malloc(sizeof(m_user_interface));
		if(m_ui == NULL)	printf("Couldn't malloc UI Structure\n");
		else {
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
		}
		
		/* Connect signals */
		gtk_builder_connect_signals( builder, NULL );
		/* Destroy builder, since we don't need it anymore */
		g_object_unref( G_OBJECT( builder ) );
		/* Show window. All other widgets are automatically shown by GtkBuilder */
		gtk_widget_show( window );
		/* Start main loop */
		gtk_main();
	}

