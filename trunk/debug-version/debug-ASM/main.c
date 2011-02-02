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
#include "m_effects.h"
#include "globals.h"
#include "m_user_interface.h"
#include "m_window.h"
#include "callbacks.h"

//#include <jack/jack.h>
#include <gtk/gtk.h>
#include "dummy_audio.h"

/////////////////////////////////////////////
//-------------CODIGO-MAIN-----------------//
/////////////////////////////////////////////
 
int main (int argc, char *argv[]) {
	srand(time(NULL));
	
	int buf_size = 4096;
	
	///////////////////////////////////////
	//-------inicializar m dist----------//
	///////////////////////////////////////
	
	m_dist = (m_distortion *) malloc(sizeof(m_distortion));
	if(m_dist == NULL){
		printf("Couldn't malloc M_DISTORTION structure\n");
	} else {
		init_m_distortion(m_dist);
		init_m_effects(m_dist);
	}		
	
	printf("Initialising DUM AUDIO STRUCTURE\n");
	dum_audio = init_dummy_audio(buf_size);
	printf("DUM AUDIO INITIALIZED\n");
	
	//free_m_distortion_and_effects(m_dist);
	///////////////////////////////////////
	//----inicializar interfaz grafica---//
	///////////////////////////////////////

	GtkBuilder *builder;
	GtkWidget  *window;
	GError     *error = NULL;

	gtk_init( &argc, &argv );
	builder = gtk_builder_new();

	// Load UI from file. If error occurs, report it and quit application.
	if( ! gtk_builder_add_from_file( builder, "interface_m_dist.glade", &error ) ){
		g_warning( "%s", error->message );
		g_free( error );
		return( 1 );
	}

	window = GTK_WIDGET( gtk_builder_get_object( builder, "m_distortion" ) );
	
	m_ui = (m_user_interface*) malloc(sizeof(m_user_interface));
	if(m_ui == NULL)	printf("Couldn't malloc UI Structure\n");
	else {
		init_m_ui(m_ui, builder, (GtkWindow*) window);
	}
	
	gtk_builder_connect_signals( builder, NULL );
	g_object_unref( G_OBJECT( builder ) );
	gtk_widget_show( window );

	////////////////////////
	open_sub_window("d_audio");
	open_sub_window("stream_to_file");
	////////////////////////

	gtk_main();
}

