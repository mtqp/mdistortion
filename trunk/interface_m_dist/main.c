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

#include "globals.h"
#include "m_distortion.h"
#include "callbacks.h"


#include <jack/jack.h>
#include <gtk/gtk.h>

#define sensitivo true;
#define no_sensitivo false;


/////////////////////////////////////////////
///////////CODIGO-MAIN//////////////////////
////////////////////////////////////////////
 
	int main (int argc, char *argv[]) {
		printf("exito al entrar al main\n");
		jack_client_t *client;
		const char **ports;
		unsigned long buf_size = 4096;
		
	
		if (argc < 2) {
			 fprintf (stderr, "usage: jack_simple_client <name>\n");
			 return 1;
		}

		/* try to become a client of the JACK server */

		if ((client = jack_client_new (argv[1])) == 0) {
			 fprintf (stderr, "jack server not running?\n");
			 return 1;
		}

		/* tell the JACK server to call `process()' whenever
		there is work to be done.
		*/

		jack_set_process_callback (client, process, 0);

		/* tell the JACK server to call `jack_shutdown()' if
		it ever shuts down, either entirely, or if it
		just decides to stop calling us.
		*/

		jack_on_shutdown (client, jack_shutdown, 0);

		/* display the current sample rate. 
		*/

		printf ("engine sample rate: %" PRIu32 "\n",
			 jack_get_sample_rate (client));

		/* create three ports, two for stereo speakers and the other for the input*/

		input_port = jack_port_register (client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, buf_size);
		output_left = jack_port_register (client, "outputLEFT", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, buf_size);
		output_right = jack_port_register (client, "outputRIGHT",JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, buf_size);

		/* tell the JACK server that we are ready to roll */

		if (jack_activate (client)) {
			 fprintf (stderr, "cannot activate client");
			 return 1;
		}

		/* connect the ports. Note: you can't do this before
		the client is activated, because we can't allow
		connections to be made to clients that aren't
		running.
		*/

		if ((ports = jack_get_ports (client, NULL, NULL, JackPortIsPhysical|JackPortIsOutput)) == NULL) {
			 fprintf(stderr, "Cannot find any physical capture ports\n");
			 exit(1);
		}

		if (jack_connect (client, ports[0], jack_port_name (input_port))) {
			 fprintf (stderr, "cannot connect input ports\n");
		}

		free (ports);

		if ((ports = jack_get_ports (client, NULL, NULL, JackPortIsPhysical|JackPortIsInput)) == NULL) {
			 fprintf(stderr, "Cannot find any physical playback ports\n");
			 exit(1);
		}

		if (jack_connect (client, jack_port_name (output_left), ports[0])) {
			fprintf (stderr, "PARLANTE IZQ?cannot connect output ports\n");
		}

		if (jack_connect (client, jack_port_name (output_right), ports[1])){
			fprintf(stderr, "negativo parlante derecho\n");
		}
		free (ports);

		///////////////////////////////////////
		/////////inicializar m dist////////////
		///////////////////////////////////////
		
		m_dist = (m_distortion *) malloc(sizeof(m_distortion));
		init_m_distortion(m_dist);

		///////////////////////////////////////
		//////inicializar interfaz grafica/////
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

		/* Connect signals */
		gtk_builder_connect_signals( builder, NULL );

		/* Destroy builder, since we don't need it anymore */
		g_object_unref( G_OBJECT( builder ) );

		/* Show window. All other widgets are automatically shown by GtkBuilder */
		gtk_widget_show( window );

		/* Start main loop */
		gtk_main();

	}

