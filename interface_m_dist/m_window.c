
#include "m_window.h"

//ESTA MAS Q CLARO Q PASANDOLE EL STRING Q TIENE Q ABRIR DIRECTAMENTE PODRIA HACERLO CON ALGUN STRCAT O ALGO ASI

void open_window(int window_name){
	GtkBuilder *builder;
	GtkWidget  *window;
	GError     *error = NULL;

	/* Create new GtkBuilder object */
	builder = gtk_builder_new();

	if(window_name == window_save){
		// Load UI from file. If error occurs, report it and quit application.
		if( ! gtk_builder_add_from_file( builder, "save_m_distortion.glade", &error ) ){
			g_warning( "%s", error->message );
			g_free( error );
			//g_print("couldnt create windows at all\n");
			exit(-1);
		}

		/* Get main window pointer from UI */
		window = GTK_WIDGET( gtk_builder_get_object( builder, "save_m_distortion" ) );
	} else {
		if(window_name == window_info){
			if( ! gtk_builder_add_from_file( builder, "info_m_distortion.glade", &error ) ){
				g_warning( "%s", error->message );
				g_free( error );
				exit(-1);
			}
			window = GTK_WIDGET( gtk_builder_get_object( builder, "info_m_distortion" ) );
		} else {
			g_print("Couldn't open window, exit status -1\n");
			exit(-1);
		}
	}
	/* Connect signals */
	gtk_builder_connect_signals( builder, NULL );
	/* Destroy builder, since we don't need it anymore */
	g_object_unref( G_OBJECT( builder ) );
	/* Show window. All other widgets are automatically shown by GtkBuilder */
	gtk_widget_show( window );
}


