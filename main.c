/** @file simple_client.c
  *
  * @brief This is very simple client that demonstrates the basic
  * features of JACK as they would be used by many applications.
  */
 
 #include <stdio.h>
 #include <errno.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <string.h>
 #include <math.h>

 #include <jack/jack.h>
 
 jack_port_t *input_port;
 jack_port_t *output_port;
 
 /**
  * The process callback for this JACK application.
  * It is called by JACK at the appropriate times.
  */

float f_dist(float f1){
	float f2 = (f1*f1*2.0);
	float f3 = (f1*f1*f1)*3.0;
	float c = 0.01;
	float f = f1;
	//sqrt(f1 + f2 + f3)/(100.0)+sqrt(f1 + f2)/(100.0);
	//f1 + f2 + f3 + c;
	return f/10;
}

//void distortion(jack_default_audio_sample_t * buf){
float* distortion(jack_default_audio_sample_t * buf){
	float* prom = (float*) malloc(8);
	int n = sizeof(buf);
	int i = 0;
	float umbral = 10.0/1.0; //pasar algun parametro
	float bufi = buf[0];
	prom[0] = 0.0;
	prom[1] = n;
	while(i<n){
		bufi = buf[i];
		prom[0]+=bufi;
		if (bufi < (-0.0015)) 	buf[i] = 0.0;
		else					buf[i] = f_dist(bufi);
		printf("bufi = %f\n",buf[i]);
		/*
		if(bufi < 0.0)	buf[i] = -1.0;
		else			buf[i] = 1.0;
		if (bufi > umbral)						buf[i] = umbral; 
		if (bufi < -umbral)						buf[i] = -umbral;
		if (bufi < 0.0009)						buf[i] = 0.0;		///noise reduction
		if (bufi > -umbral && bufi < umbral) 	buf[i] = f_dist(bufi);*/
		i+=1;
	}
return prom;
}

void arcify(float* data,long len,float mult){
  int i = 0;
  for(i;i<len;i++)
    data[i]=atan(data[i]*mult)/atan(mult);
}
/*
int process (jack_nframes_t nframes, void *arg) {
	float * prom;
    
    jack_default_audio_sample_t *out = (jack_default_audio_sample_t *) jack_port_get_buffer (output_port, nframes);
    jack_default_audio_sample_t *in = (jack_default_audio_sample_t *) jack_port_get_buffer (input_port, nframes);
	jack_default_audio_sample_t distor_buffer[nframes];///linea agregada
	
	memcpy (distor_buffer,in,sizeof(jack_default_audio_sample_t) * nframes);///linea agregada
	prom = distortion(distor_buffer);///linea agregada
    
    memcpy (out, distor_buffer, sizeof (jack_default_audio_sample_t) * nframes);///linea modif

	

    return 0;      
}*/

int process (jack_nframes_t nframes, void *arg)
{
  float multiplier = 3000.14;
  jack_default_audio_sample_t *out = (jack_default_audio_sample_t *) jack_port_get_buffer (output_port, nframes);
  jack_default_audio_sample_t *in = (jack_default_audio_sample_t *) jack_port_get_buffer (input_port, nframes);
  
  memcpy (out, in, sizeof (jack_default_audio_sample_t) * nframes);
  arcify(out,nframes,multiplier);
  
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

     /* create two ports */

     input_port = jack_port_register (client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, buf_size);
     output_port = jack_port_register (client, "output", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, buf_size);

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

    if (jack_connect (client, jack_port_name (output_port), ports[0])) {
            fprintf (stderr, "cannot connect output ports\n");
     }

     free (ports);

     /* Since this is just a toy, run for a few seconds, then finish */
	for(;;)
		sleep(1);
		
     exit (0);
}
