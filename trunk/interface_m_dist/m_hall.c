
#include "m_hall.h"

m_hall* hall_new(int size, int quantity){
	m_hall* h = (m_hall*) malloc(sizeof(m_hall));
	if(h == NULL){
		printf("Couldn't malloc Hall Structure\n");
		return NULL;
	}
	else {
		h->hll_size_bufs  	= size;
		h->hll_buf_quantity = quantity;
		h->hll_coef = 1.0;
		
		h->hll_bufs = (float**) malloc(h->hll_buf_quantity*sizeof(float*));

		if(h->hll_bufs == NULL){
			printf("Couldn't malloc Buffer Hall Structure\n");
			return NULL;
		} else {
			int i;
			for(i=0;i<h->hll_buf_quantity;i++){
				h->hll_bufs[i] = (float*) malloc(h->hll_size_bufs*sizeof(float));
			}

			int alguno_null = 0;	//ESTO ES FALSE!
			for(i=0;i<h->hll_buf_quantity;i++)
				alguno_null |= (h->hll_bufs[i] == NULL);
			if(alguno_null){
				printf("Couldn't malloc Buffer Hall Structure\n");
				return NULL;
			} else {
				printf("	Hall Effect Set\n");
				for(i=0;i<h->hll_buf_quantity;i++){
					int j;
					for(j=0;j<h->hll_size_bufs;j++) 
						h->hll_bufs[i][j] = 0.0;
				}
				return h;
			}
		}
	}
}

