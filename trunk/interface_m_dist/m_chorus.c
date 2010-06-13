
#include "m_chorus.h"

m_chorus* chorus_new(int size){
	m_chorus* c = (m_chorus*) malloc(sizeof(m_chorus));
	if(c == NULL){
		printf("Couldn't malloc Chorus Structure\n");
		return NULL;
	}
	else {
		c->chr_size = size;
		c->chr_buf1 = (float*) malloc(c->chr_size*sizeof(float));
		c->chr_buf2 = (float*) malloc(c->chr_size*sizeof(float));
		c->chr_buf3 = (float*) malloc(c->chr_size*sizeof(float));
		c->chr_buf4 = (float*) malloc(c->chr_size*sizeof(float));
		
		if((c->chr_buf1 == NULL) || (c->chr_buf2 == NULL) || (c->chr_buf3 == NULL) || (c->chr_buf4 == NULL)){
			printf("Couldn't malloc Buffer Chorus Structure\n");
			return NULL;
		} else {
			printf("	Chorus Effect Set\n");
			int i;
			for(i=0;i<c->chr_size;i++){
				c->chr_buf1[i] = 0.0;
				c->chr_buf2[i] = 0.0;
				c->chr_buf3[i] = 0.0;
				c->chr_buf4[i] = 0.0;
			}
			return c;
		}
	}
}

