
#include "m_hall.h"

m_hall* hall_new(int size){
	m_hall* h = (m_hall*) malloc(sizeof(m_hall));
	if(h == NULL){
		printf("Couldn't malloc Hall Structure\n");
		return NULL;
	}
	else {
		h->hll_size  = size;
		h->hll_coef1 = 1.0;
		h->hll_coef2 = 1.0;
		h->hll_coef3 = 1.0;
		h->hll_coef4 = 1.0;
		printf("por lo pronto los coefs no estan ajustados al valor inicial DEBIDO\n");
		h->hll_buf1 = (float*) malloc(h->hll_size*sizeof(float));
		h->hll_buf2 = (float*) malloc(h->hll_size*sizeof(float));
		h->hll_buf3 = (float*) malloc(h->hll_size*sizeof(float));
		h->hll_buf4 = (float*) malloc(h->hll_size*sizeof(float));
		
		if((h->hll_buf1 == NULL) || (h->hll_buf2 == NULL) || (h->hll_buf3 == NULL) || (h->hll_buf4 == NULL)){
			printf("Couldn't malloc Buffer Hall Structure\n");
			return NULL;
		} else {
			printf("	Hall Effect Set\n");
			int i;
			for(i=0;i<h->hll_size;i++){
				h->hll_buf1[i] = 0.0;
				h->hll_buf2[i] = 0.0;
				h->hll_buf3[i] = 0.0;
				h->hll_buf4[i] = 0.0;
			}
			return h;
		}
	}
}

