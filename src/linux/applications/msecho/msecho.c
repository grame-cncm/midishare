/****************************************************************************
*****************************************************************************
*																	*
*                     		msEcho, GTK version 							*
*																	*
*****************************************************************************
*****************************************************************************/


#include <stdio.h>
#include <gtk/gtk.h>
#include "MidiShare.h"

/****************************************************************************
						Global variables for msEcho
*****************************************************************************/

static short 	gRefNum = -1;		// msEcho MidiShare reference number
MidiFilterPtr	gFilter;			// Midi filter

GtkObject 	*pitch, *velocity, *delay,*channel; 
			
static short  gDelay = 1000;		  
static short  gPitch = 0;			  
static short  gVelocity = 0;
static short  gChannel = 0;


static void EchoTask( long d,short ref,long el,long a2, long a3);

/****************************************************************************
						Callbacks definition
*****************************************************************************/

gint my_delete_action(GtkWidget* w, GdkEvent* ev, gpointer data) 
{
	MidiFreeFilter(gFilter);
	MidiClose(gRefNum);
	gtk_main_quit();
	return FALSE;
}


static void add(GtkWidget *box, GtkWidget *obj)
{
	gtk_box_pack_start_defaults (GTK_BOX (box), obj);
}

static void add_control(GtkWidget* mbox, gchar* name, GtkObject* adjustment)
{
	GtkWidget	*hbox, *label, *scale, *spin;
	
	hbox = gtk_hbox_new (FALSE, 10);
	label = gtk_label_new(name);
	gtk_label_set_justify (GTK_LABEL (label),GTK_JUSTIFY_LEFT);
  	gtk_widget_set_usize (GTK_WIDGET (label), 40, 20);
	
  	scale = gtk_hscale_new (GTK_ADJUSTMENT (adjustment));
	gtk_scale_set_draw_value(GTK_SCALE(scale), FALSE);
  	gtk_widget_set_usize (GTK_WIDGET (scale), 150, 20);
	spin = gtk_spin_button_new(GTK_ADJUSTMENT (adjustment), 0.5, 0);
 	gtk_widget_set_usize (GTK_WIDGET (spin), 60, 20);

	gtk_box_pack_start(GTK_BOX (hbox), label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (hbox), scale, TRUE, TRUE,0);
	gtk_box_pack_start(GTK_BOX (hbox), spin, FALSE, FALSE,0);
	
	add(mbox, hbox);
}


/****************************************************************************
					Callbacks
*****************************************************************************/
	
static void param_action (GtkAdjustment* adj, GtkWidget* data)
{
	gDelay = GTK_ADJUSTMENT(delay)->value + 0.5;
	gPitch = GTK_ADJUSTMENT(pitch)->value + 0.5;
	gVelocity = GTK_ADJUSTMENT(velocity)->value + 0.5;
	gChannel = GTK_ADJUSTMENT(channel)->value + 0.5;	
}

/****************************************************************************
				Application receive alarm 
*****************************************************************************/

static void EchoAlarm ( short r)
{
  MidiEvPtr e;
  unsigned long d;
  
  while ((e = MidiGetEv(r))) {
         d = Date(e) + gDelay;
         if ( (gChannel!=0) && (gChannel!=Chan(e)+1) )
                MidiFreeEv(e);
         else if ( EvType(e) == typeNote ) {
                if( !MidiTask(EchoTask,d,r,(long)e,0,0))
                        MidiFreeEv(e);
         }
         else if ( Vel(e) > 0 )  {
	        EvType(e) = typeNote;
                Dur(e) = 100;
                if( !MidiTask(EchoTask,d,r,(long)e,0,0))
                        MidiFreeEv(e);
         }
         else MidiFreeEv(e);
  }
}	

/****************************************************************************
				Real time echo management task
*****************************************************************************/

static void EchoTask( long d,short ref,long el,long a2, long a3)
{
  MidiEvPtr e;
  short v,p;

  e= (MidiEvPtr)el;
  v = Vel(e)+gVelocity;
  p = Pitch(e)+gPitch;
  if ( ((v>0)&&(v<128)) && ((p>=0)&&(p<128)) ) {
         Vel(e) = v;
         Pitch(e) = p;
         MidiSendAt(ref, MidiCopyEv(e), d);
         if( !MidiTask(EchoTask, d+gDelay, ref, el, 0, 0))
                MidiFreeEv(e);
  }
  else MidiFreeEv(e);
}

/****************************************************************************
				For incoming events filtering
*****************************************************************************/

static void InstallFilter(void) {

  short i;
  
  gFilter = MidiNewFilter();
  
  if (gFilter) {

 	for (i=0;i<256;i++) {
         	MidiAcceptType(gFilter,i,false);
         	MidiAcceptPort(gFilter,i,true);
 	}
  	MidiAcceptType(gFilter,typeNote,true);
  	MidiAcceptType(gFilter,typeKeyOn,true);

  	for (i=0;i<16;i++) MidiAcceptChan(gFilter,i,true);
  	MidiSetFilter(gRefNum,gFilter);
  }
}


/****************************************************************************
								Main
*****************************************************************************/

int main(int argc, char *argv[] )
{
	GtkWidget *window, *vbox;

	// Initialisations
	gtk_init (&argc, &argv);

	gRefNum = MidiOpen(argv[0]);
	MidiSetRcvAlarm(gRefNum, EchoAlarm);
	InstallFilter();
  	MidiConnect (gRefNum, 0, true);
  	MidiConnect (0, gRefNum, true);
	
	// Adjustments description  
	pitch 	= gtk_adjustment_new (0.0, -127.0, 128.0, 1.0, 8.0, 1.0);
	velocity= gtk_adjustment_new (0.0, -127.0, 128.0, 1.0, 8.0, 1.0);
	delay= gtk_adjustment_new (1000.0, 0.0, 2000.0, 1.0, 50.0, 1.0);
	channel = gtk_adjustment_new (0.0, 0.0, 16.0, 1.0, 1.0, 1.0);
	

	// Window definition
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "msEcho");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	vbox = gtk_vbox_new (FALSE, 10);
	
	// Add controllers
	add_control(vbox, "Pitch", pitch);
	add_control(vbox, "Vel", velocity);
	add_control(vbox, "Delay", delay);

	add(vbox, gtk_hseparator_new());

	add_control(vbox, "Chan", channel);
	
	gtk_container_add(GTK_CONTAINER(window), vbox);	
	gtk_widget_show_all (window);
	
	// Connexion of signals for value adjustement
	gtk_signal_connect( pitch, "value_changed", GTK_SIGNAL_FUNC(param_action), NULL);
	gtk_signal_connect( velocity, "value_changed", GTK_SIGNAL_FUNC(param_action), NULL);
	gtk_signal_connect( delay, "value_changed", GTK_SIGNAL_FUNC(param_action), NULL);
	gtk_signal_connect( channel, "value_changed", GTK_SIGNAL_FUNC(param_action), NULL);
	
	// Connexion of signals
	gtk_signal_connect(
			GTK_OBJECT(window), "delete_event", 
			GTK_SIGNAL_FUNC(my_delete_action), NULL
	);
	
	gtk_signal_connect(
			GTK_OBJECT(window), "destroy", 
			GTK_SIGNAL_FUNC(my_delete_action), NULL
	);
	
	gtk_main ();

	return(0);
}

