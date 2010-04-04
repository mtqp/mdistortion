//http://tadeboro.blogspot.com/2009/09/glade3-tutorial-2-constructing.html


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
//#include "callbacks.h"


#include <jack/jack.h>
#include <gtk/gtk.h>

#define sensitivo true;
#define no_sensitivo false;

jack_port_t *input_port;
jack_port_t *output_left;
jack_port_t *output_right;

m_distortion *m_dist; 

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

//////////MODOS//////////////////
G_MODULE_EXPORT void on_rock_mode_clicked (gpointer distors, GtkRadioButton *button){
	g_print("CLICKED ROCK AND ROOL BABY\n");
	gtk_widget_set_sensitive((GtkWidget*) distors, true);//sensitivo);
	set_m_distortion(m_dist, -1);
}

G_MODULE_EXPORT void on_random_mode_clicked ( gpointer distors,GtkRadioButton *button){
	g_print("CLICKED random mode\n");
	gtk_widget_set_sensitive((GtkWidget*) distors, false);//no_sensitivo);
	set_m_distortion(m_dist,7);//e_random_day);		//quizas es mejor hacer un enum de los numeros para q se entienda mas!
}

G_MODULE_EXPORT void on_mute_mode_clicked ( gpointer distors,GtkRadioButton *button){
	g_print("CLICKED muteeeee\n");
	gtk_widget_set_sensitive((GtkWidget*) distors, false);//no_sensitivo);
	set_m_distortion(m_dist, 8);//e_mute);
}

G_MODULE_EXPORT void on_by_pass_mode_clicked ( gpointer distors, GtkRadioButton *button){
	g_print("CLICKED by_pass\n");
	gtk_widget_set_sensitive((GtkWidget*) distors, false);//no_sensitivo);
	set_m_distortion(m_dist, 9);//e_by_pass);
}


//////////////DISTORSIONES/////////////////
G_MODULE_EXPORT void on_log_rock_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 0);//e_log_rock);
}
G_MODULE_EXPORT void on_log_rock_II_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 1);//e_log_rock_II);
}
G_MODULE_EXPORT void on_hell_sqrt_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 2);//e_hell_sqrt);
}
G_MODULE_EXPORT void on_psychedelic_if_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 3);//e_psychedelic_if);
}
G_MODULE_EXPORT void on_by_60s_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 4);//e_by_60s);
}
G_MODULE_EXPORT void on_fuzzy_dark_pow_IV_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 5);//e_fuzzy_dark_pow_IV);
}
G_MODULE_EXPORT void on_rare_cuadratic_clicked(gpointer distors,GtkRadioButton *button){
	set_m_distortion(m_dist, 6);//e_rare_cuadratic);
}

////////////////////////////////////////////
///////////CALLBACKS-JACK///////////////////
////////////////////////////////////////////

int process (jack_nframes_t nframes, void *arg)
{
	///puedo usar x⁴ como una buena aproximacion para el volumen.
	jack_default_audio_sample_t *outL = (jack_default_audio_sample_t *) jack_port_get_buffer (output_left, nframes);
  	jack_default_audio_sample_t *outR = (jack_default_audio_sample_t *) jack_port_get_buffer (output_right, nframes);
	jack_default_audio_sample_t *in = (jack_default_audio_sample_t *) jack_port_get_buffer (input_port, nframes);

	memcpy (outL, in, sizeof (jack_default_audio_sample_t) * nframes);
	memcpy (outR, in, sizeof (jack_default_audio_sample_t) * nframes);

	distortionize(m_dist, outL, outR, sizeof (jack_default_audio_sample_t) * nframes); //la gran magia

	return 0;      
}
 /**
  * This is the shutdown callback for this JACK application.
  * It is called by JACK if the server ever shuts down or
  * decides to disconnect the client.
  */
void jack_shutdown (void *arg) {
	exit (1);
}


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
		init_m_distortion(m_dist,master_on);

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

		//////////////////////////////////////////////
		///////manejo en terminal m_distortion////////
		//////////////////////////////////////////////
/*
		char option;
		unsigned char l = 0;
		unsigned char r = 0;

		while(1){
			switch(option = (char) getchar()){
				case 'q':
					printf("closing m_distortion\n");
					free_m_distortion(m_dist);
					exit(0);
					break;
				case 'm':
					change_master(m_dist);
					break;
				case 's':
					printf("set distortion NOT WORKING YET\n\n\n");
					printf("ingrese el valor para izq\n");		
					//l = (unsigned char) getchar();
					printf("ingrese el valor para der\n");							
					//r = (unsigned char) getchar();
					//set_m_distortion(m_dist,l,r);
					break;
				case '1':
					printf("changing sig dist left\n");		
					l++;
					l = l%8;
					set_m_distortion(m_dist,l,r);
					break;
				case '2':
					printf("changing pre dist left\n");
					if(l == 0){
						l = 7;
						set_m_distortion(m_dist,l,r);
					} else {
						l--;
						set_m_distortion(m_dist,l,r);
					}
					break;
				case '4':
					printf("changing sig dist right\n");		
					r++;
					r = r%8;
					set_m_distortion(m_dist,l,r);
					break;
				case '5':
					printf("changing pre dist right\n");
					if(r == 0){
						r = 7;
						set_m_distortion(m_dist,l,r);
					} else {
						r--;
						set_m_distortion(m_dist,l,r);
					}
					break;
				case 'b':
					printf("volume up\n");
					vol_up_md(m_dist, speaker_izq);
					break;
				case 'v':
					printf("volume down\n");
					vol_down_md(m_dist, speaker_izq);
					break;
				case 'x':
					printf("gain up\n");
					gain_up_md(m_dist, speaker_izq);
					break;
				case 'z':
					printf("gain down\n");
					gain_down_md(m_dist, speaker_izq);
					break;		
			}
		}
	*/
