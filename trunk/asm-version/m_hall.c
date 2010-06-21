
#include "m_hall.h"

/*Inicializa la estructura HALL*/
m_hall* hall_new(int size, int quantity){
	m_hall* h = (m_hall*) malloc(sizeof(m_hall));		//HALL*
	if(h == NULL){
		printf("Couldn't malloc Hall Structure\n");
		return NULL;
	}
	else {
		h->hll_size_bufs  	= size;						//largo de los buffers (este programa usa 4096)
		h->hll_buf_quantity = quantity;					//cantidad de buffers (a mayor cantidad, mayor hall)
		h->hll_coef = 1.0;								//coeficiente por el que se multiplica el smp
		
		h->hll_bufs = (float**) malloc(h->hll_buf_quantity*sizeof(float*));		//array de buffers

		if(h->hll_bufs == NULL){
			printf("Couldn't malloc Buffer Hall Structure\n");
			return NULL;
		} else {
			int i;
			for(i=0;i<h->hll_buf_quantity;i++){
				h->hll_bufs[i] = (float*) malloc(h->hll_size_bufs*sizeof(float));//hall buffers
			}

			int alguno_null = 0;
			for(i=0;i<h->hll_buf_quantity;i++)
				alguno_null |= (h->hll_bufs[i] == NULL);
			if(alguno_null){
				printf("Couldn't malloc Buffer Hall Structure\n");
				return NULL;
			} else {
				printf("	Hall Effect Set\n");
				for(i=0;i<h->hll_buf_quantity;i++){			//inicialización en 0 de los arrays
					int j;
					for(j=0;j<h->hll_size_bufs;j++) 
						h->hll_bufs[i][j] = 0.0;
				}
				return h;
			}
		}
	}
}

