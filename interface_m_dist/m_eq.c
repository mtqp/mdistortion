#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "m_eq.h"

/* Computes a BiQuad filter on a sample */
smp_type BiQuad(smp_type sample, m_equalizer * b)
{
	smp_type result;

	/* compute result */
	result = b->a0 * sample + b->a1 * b->x1 + b->a2 * b->x2 -
	   b->a3 * b->y1 - b->a4 * b->y2;

	/* shift x1 to x2, sample to x1 */
	b->x2 = b->x1;
	b->x1 = sample;

	/* shift y1 to y2, result to y1 */
	b->y2 = b->y1;
	b->y1 = result;

	return result;
}

void lpf_reset_eq_params(m_equalizer *bq, smp_type freq, smp_type srate, smp_type bandwidth){
	smp_type omega, sn, cs, alpha;
	smp_type a0, a1, a2, b0, b1, b2;

	/* setup variables */
	omega = 2 * M_PI * freq /srate;
	sn = sin(omega);
	cs = cos(omega);
	alpha = sn * sinh(M_LN2 /2 * bandwidth * omega /sn);
	
	   b0 = (1 - cs) /2;
	   b1 = 1 - cs;
	   b2 = (1 - cs) /2;
	   a0 = 1 + alpha;
	   a1 = -2 * cs;
	   a2 = 1 - alpha;

	/* precompute the coefficients */
	bq->a0 = b0 /a0;
	bq->a1 = b1 /a0;
	bq->a2 = b2 /a0;
	bq->a3 = a1 /a0;
	bq->a4 = a2 /a0;

	bq->_freq = freq;
	bq->_srate = srate;
	bq->_bandwidth = bandwidth;
}

void hpf_reset_eq_params(m_equalizer *bq, smp_type freq, smp_type srate, smp_type bandwidth){
	smp_type omega, sn, cs, alpha;
	smp_type a0, a1, a2, b0, b1, b2;

	/* setup variables */
	omega = 2 * M_PI * freq /srate;
	sn = sin(omega);
	cs = cos(omega);
	alpha = sn * sinh(M_LN2 /2 * bandwidth * omega /sn);
	
	   b0 = (1 + cs) /2;
	   b1 = -(1 + cs);
	   b2 = (1 + cs) /2;
	   a0 = 1 + alpha;
	   a1 = -2 * cs;
	   a2 = 1 - alpha;

	/* precompute the coefficients */
	bq->a0 = b0 /a0;
	bq->a1 = b1 /a0;
	bq->a2 = b2 /a0;
	bq->a3 = a1 /a0;
	bq->a4 = a2 /a0;

	bq->_freq = freq;
	bq->_srate = srate;
	bq->_bandwidth = bandwidth;
}

void bpf_reset_eq_params(m_equalizer *bq, smp_type freq, smp_type srate, smp_type bandwidth){
	smp_type omega, sn, cs, alpha;
	smp_type a0, a1, a2, b0, b1, b2;

	/* setup variables */
	omega = 2 * M_PI * freq /srate;
	sn = sin(omega);
	cs = cos(omega);
	alpha = sn * sinh(M_LN2 /2 * bandwidth * omega /sn);
	
	   b0 = alpha;
	   b1 = 0;
	   b2 = -alpha;
	   a0 = 1 + alpha;
	   a1 = -2 * cs;
	   a2 = 1 - alpha;

	/* precompute the coefficients */
	bq->a0 = b0 /a0;
	bq->a1 = b1 /a0;
	bq->a2 = b2 /a0;
	bq->a3 = a1 /a0;
	bq->a4 = a2 /a0;

	bq->_freq = freq;
	bq->_srate = srate;
	bq->_bandwidth = bandwidth;
}


/* sets up a BiQuad Filter */
m_equalizer *BiQuad_new(int type, smp_type dbGain, smp_type freq,
smp_type srate, smp_type bandwidth)
{
	m_equalizer *b;
	smp_type A, omega, sn, cs, alpha, beta;
	smp_type a0, a1, a2, b0, b1, b2;

	b = malloc(sizeof(m_equalizer));
	if (b == NULL)
	   return NULL;

	b->_dbgain = dbGain;
	b->_freq = freq;
	b->_srate = srate;
	b->_bandwidth = bandwidth;
	
	/* setup variables */
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
	   free(b);
	   return NULL;
	}

	/* precompute the coefficients */
	b->a0 = b0 /a0;
	b->a1 = b1 /a0;
	b->a2 = b2 /a0;
	b->a3 = a1 /a0;
	b->a4 = a2 /a0;

	/* zero initial samples */
	b->x1 = b->x2 = 0;
	b->y1 = b->y2 = 0;

	return b;
}