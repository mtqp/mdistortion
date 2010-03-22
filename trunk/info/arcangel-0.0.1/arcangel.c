/**   arcangel - arctangent saturate/distort
  *   Copyright (C) 2006  James Shuttleworth
  *
  *   Contact: james@dis-dot-dat.net
  *
  *   This program is free software; you can redistribute it and/or modify
  *   it under the terms of the GNU General Public License as published by
  *   the Free Software Foundation; either version 2 of the License, or
  *   (at your option) any later version.
  *
  *   This program is distributed in the hope that it will be useful,
  *   but WITHOUT ANY WARRANTY; without even the implied warranty of
  *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *   GNU General Public License for more details.
  *
  *   You should have received a copy of the GNU General Public License
  *   along with this program; if not, write to the Free Software
  *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  */

#include <math.h>
#include <stdlib.h>
#include "arc.h"
#include <jack/jack.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <X11/forms.h>
#include "form.h"

jack_port_t *input_port;
jack_port_t *output_port;

//Multiplier for arcify
float multiplier=1;

//Switch to FALSE to stop
int run=TRUE;

jack_client_t *client;


int process (jack_nframes_t nframes, void *arg)
{
  jack_default_audio_sample_t *out = (jack_default_audio_sample_t *) jack_port_get_buffer (output_port, nframes);
  jack_default_audio_sample_t *in = (jack_default_audio_sample_t *) jack_port_get_buffer (input_port, nframes);
  
  memcpy (out, in, sizeof (jack_default_audio_sample_t) * nframes);
  arcify(out,nframes,multiplier);
  
  return 0;      
}

void jack_shutdown (void *arg)
{
  printf("Jack has finished\n");
  exit (1);
}

/*** callbacks and freeobj handles for form arcangel ***/
void form_quit(FL_OBJECT *ob, long data)
{
  run=FALSE;
}

void form_dial(FL_OBJECT *ob, long data)
{
  /* fill-in code for callback */

  multiplier=  fl_get_dial_value(ob);
}

void idle_callback(int xev, void* user_data){
  if(!run){
    jack_client_close (client);
    exit(0);
  }
}


/***********************/


int main(int argc, char *argv[]){

  if(argc<2){
    //I think this cocks up xforms dealilngs with argv and argc, but I don't care
    printf("Usage:\n\t%s <client name>\n",argv[0]);
    return 1;
  }

  FD_arcangel *fd_arcangel;
  
  fl_initialize(&argc, argv, 0, 0, 0);
  fd_arcangel = create_form_arcangel();
  fl_show_form(fd_arcangel->arcangel,FL_PLACE_CENTERFREE,FL_FULLBORDER,"arcangel");



  
  if ((client = jack_client_new (argv[1])) == 0) {
    printf ("The jack server seems not to be running.\n");
    return 1;
  }

  jack_set_process_callback (client, process, 0);
  jack_on_shutdown (client, jack_shutdown, 0);
  input_port = jack_port_register (client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
  output_port = jack_port_register (client, "output", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

  if (jack_activate (client)) {
    printf("cannot activate client");
    return 1;
  }
  
  //When form is idle, check to see if we need to quit
  fl_set_idle_callback((void*)&idle_callback,0);

  fl_do_forms();

  //Should only get here if the form is finished, so quit nicely.

  run=FALSE;
  idle_callback(0,0);

  return 0;
}
