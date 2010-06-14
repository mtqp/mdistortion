
#include "m_window.h"

//ESTA MAS Q CLARO Q PASANDOLE EL STRING Q TIENE Q ABRIR DIRECTAMENTE PODRIA HACERLO CON ALGUN STRCAT O ALGO ASI

GtkWindow* open_sub_window(char* sub_window){
	GtkBuilder *builder;
	GtkWidget  *window;
	GError     *error = NULL;

	/* Create new GtkBuilder object */
	builder = gtk_builder_new();


	// Load UI from file. If error occurs, report it and quit application.
	if( ! gtk_builder_add_from_file( builder, "interface_m_dist.glade", &error ) ){
		g_warning( "%s", error->message );
		g_free( error );
		//g_print("couldnt create windows at all\n");
		exit(-1);
	}

	/* Get main window pointer from UI */
	window = GTK_WIDGET( gtk_builder_get_object( builder, sub_window ) );

	/* Connect signals */
	gtk_builder_connect_signals( builder, NULL );
	/* Destroy builder, since we don't need it anymore */
	g_object_unref( G_OBJECT( builder ) );
	/* Show window. All other widgets are automatically shown by GtkBuilder */
	gtk_widget_show( window );
	return window;
}


