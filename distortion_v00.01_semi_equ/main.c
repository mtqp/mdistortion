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

//____________________________ insertado x mi
#include "m_distortion.h"
#include "m_delay.h"
//----------------------------

#include <jack/jack.h>
 
jack_port_t *input_port;
jack_port_t *output_left;
jack_port_t *output_right;

m_distortion *m_dist;
m_equalizer *m_eq; 
//m_delay *delay;
bool eqbool;

int process (jack_nframes_t nframes, void *arg)
{
	
	jack_default_audio_sample_t *outL = (jack_default_audio_sample_t *) jack_port_get_buffer (output_left, nframes);
 	jack_default_audio_sample_t *outR = (jack_default_audio_sample_t *) jack_port_get_buffer (output_right, nframes);
	jack_default_audio_sample_t *in = (jack_default_audio_sample_t *) jack_port_get_buffer (input_port, nframes);

	memcpy (outL, in, sizeof (jack_default_audio_sample_t) * nframes);
	memcpy (outR, in, sizeof (jack_default_audio_sample_t) * nframes);
	
	distortionize(m_dist, outL, outR, sizeof (jack_default_audio_sample_t) * nframes,m_eq, eqbool); //la gran magia
//	delaying(m_dist, delay, outL, outR, sizeof (jack_default_audio_sample_t) * nframes);
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

		input_port = jack_port_register (client, "input", JACK_DEFAULT_AUDIO_TYPE, /*JackPortIsPhysical|*/JackPortIsInput, buf_size);
		output_left = jack_port_register (client, "outputLEFT", JACK_DEFAULT_AUDIO_TYPE, /*JackPortIsPhysical|*/JackPortIsOutput, buf_size);
		output_right = jack_port_register (client, "outputRIGHT",JACK_DEFAULT_AUDIO_TYPE, /*JackPortIsPhysical|*/JackPortIsOutput, buf_size);

		/* tell the JACK server that we are ready to roll */

		/////////////////////////////////////
		//PEDACITO DE CODIGO ESCRITO POR MI//
		/////////////////////////////////////
		
		////////VOY A NECESITAR UNA CALLBACK X SI ME CAMBIAN EL BUF SIZE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		
		/*jack_nframes_t bufsize = jack_get_buffer_size(client);
		printf("\n\nTHE BUF SIZE IS == %d\n\n", (int) bufsize);
		*/
		
		/////////////////////////////////////
		/////////////////////////////////////
		////////////////////////////////////

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
		
		m_eq = (m_equalizer *) malloc(sizeof(m_equalizer));
		init_equalizer (m_eq,880,5000,48000);
		
		
		//delay = (m_delay *) malloc(sizeof(m_delay));
		//init_m_delay(delay,bufsize);		//TENGO Q SABER ESTOS PARAMETROS DE ENTRADA SI O SI
		
		////////////////////////////////////////
		////////////////TERMINAL////////////////
		////////////////////////////////////////
		
		char option;
		unsigned char l = 0;
		unsigned char r = 0;
		bool up = 1;
		bool down = 0;

		while(1){
			switch(option = (char) getchar()){
				case 'q':
					printf("closing m_distortion\n");
					//free_m_distortion(m_dist);
					//free_m_delay(delay);
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
				case '.':
					eqbool = !eqbool;
					printf("eq bool = %d\n",eqbool);
					break;
				case '1':
					printf("changing sig dist left\n");		
					l++;
					l = l%9;
					set_m_distortion(m_dist,l,r);
					break;
				case '2':
					printf("changing pre dist left\n");
					if(l == 0){
						l == 7;
						set_m_distortion(m_dist,l,r);
					} else {
						l--;
						set_m_distortion(m_dist,l,r);
					}
					break;
				case '4':
					printf("changing sig dist right\n");		
					r++;
					r = r%9;
					set_m_distortion(m_dist,l,r);
					break;
				case '5':
					printf("changing pre dist right\n");
					if(r == 0){
						r == 7;
						set_m_distortion(m_dist,l,r);
					} else {
						r--;
						set_m_distortion(m_dist,l,r);
					}
					break;
				case 't':
					printf("treble up\n");
					set_treble(m_eq,up);
					break;
				case 'g':
					printf("treble down\n");
					set_treble(m_eq,down);
					break;
				case 'y':
					printf("middle up\n");
					set_mid(m_eq,up);
					break;
				case 'h':
					printf("middle down\n");
					set_mid(m_eq,down);
					break;		
				case 'u':
					printf("bass up\n");
					set_bass(m_eq,up);
					break;
				case 'j':
					printf("bass down\n");
					set_bass(m_eq,down);
					break;
				case '6':
					printf("reset eq\n");
					reset_equalizer(m_eq);
					break;
			}
		}
	}
