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

#include <jack/jack.h>
 
jack_port_t *input_port;
jack_port_t *output_left;
jack_port_t *output_right;
 
void m_dist_0(float* data,long len,float mult){ //arco tangente
	int i = 0;
	for(i;i<len;i++){
		data[i]=atan(data[i]*mult)/atan(mult);
	}
}

void m_dist_1(float* data,long len,float mult){ //cuadratica RARISIMA
	int i = 0;
	for(i;i<len;i++){
		data[i]= (data[i]*data[i])*mult;
	}
}


void m_dist_2(float* data,long len,float mult){ //sum(d/2^i) bastante normal
	int i = 0;
	float d;
	for(i;i<len;i++){
		d = data[i];
		data[i]= (d + d/2 + d/4 + d/8 + d/16 + d/32 + d/64 + d/128)*mult;
	}
}

void m_dist_3(float* data,long len,float mult){ //fuzzy oscura // siempre negativa.
	int i = 0;
	float d;
	for(i;i<len;i++){
		d = data[i];
		data[i]= (-pow(d,4))*mult;
	}
}

void m_dist_4(float* data,long len,float divider){//logaritmic rock //seno hace mas agudo// cos mas grave
	int i = 0;
	for(i;i<len;i++){
		data[i]=(log(data[i]/divider));//con seno tbm qda RE buena
	}
}

void m_dist_5(float* data,long len,float mult){//
	int i = 0;
	for(i;i<len;i++){
		data[i]= log(sin(data[i]))/(cos(-mult));//tan lo hace mas grave sintbm, pero menos q tan ///-sen mas grave todavia
	}
}

void m_dist_6(float* data,long len,float vol){//raiz nasty
	int i = 0;
	for(i;i<len;i++){
		data[i]= sqrt(data[i]); ///se puede elevar hasta el cuadrado data y suena copado
	}
}


int process (jack_nframes_t nframes, void *arg)
{
	float divider = 1.0;
	float multiplier = 250000.0;
	jack_default_audio_sample_t *outL = (jack_default_audio_sample_t *) jack_port_get_buffer (output_left, nframes);
  	jack_default_audio_sample_t *outR = (jack_default_audio_sample_t *) jack_port_get_buffer (output_right, nframes);
	jack_default_audio_sample_t *in = (jack_default_audio_sample_t *) jack_port_get_buffer (input_port, nframes);

	memcpy (outL, in, sizeof (jack_default_audio_sample_t) * nframes);
	memcpy (outR, in, sizeof (jack_default_audio_sample_t) * nframes);
	m_dist_6(outL,nframes,divider);
	m_dist_6(outR,nframes,divider);
//	m_dist_5(out,nframes,divider);
//	m_dist_4(out,nframes,divider);
//	m_dist_3(out,nframes,multiplier);
//	m_dist_2(out,nframes,multiplier);
//	m_dist_1(out,nframes,multiplier);
//	m_dist_0(out,nframes,multiplier);
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
 
int main (int argc, char *argv[]) {
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

	/* POR AHORA, SERA EN ALGUN TIEMPO PURO ROCK BABY Since this is just a toy, run for a few seconds, then finish */
	for(;;){
		sleep(1);
	}
	exit (0);
}
