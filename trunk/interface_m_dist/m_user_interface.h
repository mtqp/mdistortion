#ifndef __M_USER_INTERFACE_H__
#define __M_USER_INTERFACE_H__

#include <gtk/gtk.h>

typedef struct _m_user_interface{
	//-m_dist-//
	GtkWindow*		 m_distortion;
	GtkContainer* 	 box_eq;
		GtkToggleButton* eq_onoff;
		GtkContainer* 	 extra_effects;
			GtkToggleButton* delay;
			GtkToggleButton* hall;
	GtkContainer* 	 eq_bars;
	GtkContainer* 	 distors;
		GtkRadioButton*  hell_sqrt;
	GtkScaleButton*  volume;
	//-m_delay-//
	GtkWindow*		 m_delay;
	GtkToggleButton* dl_onoff;
	GtkContainer* 	 dl_sets;
	//-m_hall-//
	GtkWindow*		 m_hall;
	GtkToggleButton* hl_onoff;
	GtkContainer*	 chintensity;
} m_user_interface;

#endif
