
#include "m_eq.h"

/* Aplica ecualización sobre el sample */
float equalize_sample(float sample, m_equalizer * eq){
	float eq_sample;

	/* Obtener sample ecualizada */
	eq_sample = eq->a0[0] * sample + eq->a1 * eq->x1 + eq->a2 * eq->x2 - eq->a3 * eq->y1 - eq->a4 * eq->y2;

	/* Recalcular history buffers */
	eq->x2 = eq->x1;
	eq->x1 = sample;
	eq->y2 = eq->y1;
	eq->y1 = eq_sample;

	return eq_sample;
}

/* Inicializa filter ecualizador */
m_equalizer *band_EQ_new(int type, float dbGain, float freq, float srate, float bandwidth){
	m_equalizer *eq;
	float A, omega, sn, cs, alpha, beta;
	float a0, a1, a2, b0, b1, b2;

	eq = malloc(sizeof(m_equalizer));
	if (eq == NULL)
	   return NULL;

	eq->_dbgain = dbGain;
	eq->_freq = freq;
	eq->_srate = srate;
	eq->_bandwidth = bandwidth;
	
	/* Inicializacion variables */
	A = pow(10, dbGain /40);
	omega = 2 * M_PI * freq /srate;
	sn = sin(omega);
	cs = cos(omega);
	alpha = sn * sinh(M_LN2 /2 * bandwidth * omega /sn);
	beta = sqrt(A + A);

	switch (type) {
	case LPF:
	   b0 = (1 - cs) /2;
	   b1 = 1 - cs;
	   b2 = (1 - cs) /2;
	   a0 = 1 + alpha;
	   a1 = -2 * cs;
	   a2 = 1 - alpha;
	   break;
	case HPF:
	   b0 = (1 + cs) /2;
	   b1 = -(1 + cs);
	   b2 = (1 + cs) /2;
	   a0 = 1 + alpha;
	   a1 = -2 * cs;
	   a2 = 1 - alpha;
	   break;
	case BPF:
	   b0 = alpha;
	   b1 = 0;
	   b2 = -alpha;
	   a0 = 1 + alpha;
	   a1 = -2 * cs;
	   a2 = 1 - alpha;
	   break;
	case NOTCH:
	   b0 = 1;
	   b1 = -2 * cs;
	   b2 = 1;
	   a0 = 1 + alpha;
	   a1 = -2 * cs;
	   a2 = 1 - alpha;
	   break;
	case PEQ:
	   b0 = 1 + (alpha * A);
	   b1 = -2 * cs;
	   b2 = 1 - (alpha * A);
	   a0 = 1 + (alpha /A);
	   a1 = -2 * cs;
	   a2 = 1 - (alpha /A);
	   break;
	case LSH:
	   b0 = A * ((A + 1) - (A - 1) * cs + beta * sn);
	   b1 = 2 * A * ((A - 1) - (A + 1) * cs);
	   b2 = A * ((A + 1) - (A - 1) * cs - beta * sn);
	   a0 = (A + 1) + (A - 1) * cs + beta * sn;
	   a1 = -2 * ((A - 1) + (A + 1) * cs);
	   a2 = (A + 1) + (A - 1) * cs - beta * sn;
	   break;
	case HSH:
	   b0 = A * ((A + 1) + (A - 1) * cs + beta * sn);
	   b1 = -2 * A * ((A - 1) + (A + 1) * cs);
	   b2 = A * ((A + 1) + (A - 1) * cs - beta * sn);
	   a0 = (A + 1) - (A - 1) * cs + beta * sn;
	   a1 = 2 * ((A - 1) - (A + 1) * cs);
	   a2 = (A + 1) - (A - 1) * cs - beta * sn;
	   break;
	default:
	   free(eq);
	   return NULL;
	}

	/* Coeficientes EQ */
	eq->a0[0] = b0 /a0;
	eq->a0[1] = b0 /a0;
	eq->a0[2] = b0 /a0;
	eq->a0[3] = b0 /a0;
	eq->a1 = b1 /a0;
	eq->a2 = b2 /a0;
	eq->a3 = a1 /a0;
	eq->a4 = a2 /a0;

	/* History buffer inicial */
	eq->x1 = eq->x2 = 0;
	eq->y1 = eq->y2 = 0;

	return eq;
}

/*Dada una banda, recalcula el filtro dependiendo del parámetro bandwith*/
void reset_eq_params(m_equalizer *eq, float bandwidth, int filter_type){
	float omega, sn, cs, alpha;
	float a0, a1, a2, b0, b1, b2;

	/* Inicializa variables */
	omega = 2 * M_PI * eq->_freq / eq->_srate;
	sn = sin(omega);
	cs = cos(omega);
	alpha = sn * sinh(M_LN2 /2 * bandwidth * omega /sn);

	switch(filter_type){
		case LPF:
			b0 = (1 - cs) /2;
			b1 = 1 - cs;
			b2 = (1 - cs) /2;
			a0 = 1 + alpha;
			a1 = -2 * cs;
			a2 = 1 - alpha;
			break;
		case HPF:
			b0 = (1 + cs) /2;
			b1 = -(1 + cs);
			b2 = (1 + cs) /2;
			a0 = 1 + alpha;
			a1 = -2 * cs;
			a2 = 1 - alpha;
			break;
		case BPF:
			b0 = alpha;
			b1 = 0;
			b2 = -alpha;
			a0 = 1 + alpha;
			a1 = -2 * cs;
			a2 = 1 - alpha;
			break;
	}

	/* Coeficientes EQ */
	eq->a0[0] = b0 /a0;
	eq->a0[1] = b0 /a0;
	eq->a0[2] = b0 /a0;
	eq->a0[3] = b0 /a0;
	eq->a1 = b1 /a0;
	eq->a2 = b2 /a0;
	eq->a3 = a1 /a0;
	eq->a4 = a2 /a0;

	eq->_bandwidth = bandwidth;
}

