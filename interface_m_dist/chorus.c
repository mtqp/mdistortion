

m_delay* delay_new (int size){
	m_delay* d = (m_delay*) malloc(sizeof(m_delay));
	if(d == NULL){
		printf("Couldn't malloc Delay Structure\n");
		return NULL;
	}
	else {
		d->delay_size = size;
		d->delay_buf 		 = (float*) malloc(d->delay_size*FLOAT_SIZE);
		d->delay_buf_2_orden = (float*) malloc(d->delay_size*FLOAT_SIZE);
		d->delay_buf_3_orden = (float*) malloc(d->delay_size*FLOAT_SIZE);
		d->delay_buf_4_orden = (float*) malloc(d->delay_size*FLOAT_SIZE);

