/****************************************************************************
*****************************************************************************
*																	*
*                     		msSync, GTK version 							*
*																	*
*****************************************************************************
*****************************************************************************/


#include <stdio.h>
#include <gtk/gtk.h>
#include "MidiShare.h"
#include "msApplsTools.h"


/****************************************************************************
						Global variables for msSync
*****************************************************************************/

static short 			gRefNum = -1;		// msSync MidiShare reference number
static MidiFilterPtr  	gFilter;			// Midi filter
static long 			gTempoSpeed = 60;   // Tempo in BPM
static long     		gRemain = 0;        
static long     		gTempoUnitNum = 4;
static long     		gTempoUnitDenum = 4;
static Boolean  		gStartStop=FALSE;

static MidiEvPtr 		gClockTask = 0;     // Task to play Clock events

GtkWidget *gStart, *gCont, *gStop;
GtkObject* gTempo;


/****************************************************************************
			RcvAlarm									  
*****************************************************************************/

void  ReceiveEvents(short refNum)
{
	MidiEvPtr e;
	short n;

	for (n=MidiCountEvs(refNum); n>0; --n) {
		e = MidiGetEv(refNum);
		switch ( EvType(e) ) {
			case typeStart :
				gtk_button_clicked ((GtkButton*)gStart);
				break;
			case typeStop :
				gtk_button_clicked ((GtkButton*)gStop);
				break;
			case typeContinue :
				gtk_button_clicked ((GtkButton*)gCont);
				break;
		}
		MidiFreeEv(e);
	}
 }

/****************************************************************************
   			Filter configuration		  
*****************************************************************************/

void InstallFilter( )
{
	short i;
	gFilter = MidiNewFilter();

  	if (gFilter) {
 		for (i=0;i<256;i++) {
         		MidiAcceptType(gFilter,i,false);
         		MidiAcceptPort(gFilter,i,true);
 		}

		for (i=0;i<16;i++) MidiAcceptChan(gFilter,i,true);

  		MidiAcceptType(gFilter,typeStart,true);
  		MidiAcceptType(gFilter,typeStop,true);
		MidiAcceptType(gFilter,typeContinue,true);
		MidiAcceptType(gFilter,typeTempo,true);
	
  		MidiSetFilter(gRefNum,gFilter);

	}
}

/****************************************************************************
 					ClockTask															
*****************************************************************************/

void   ClockTask( long date, short ref, long u1, long u2, long u3 )
{
	MidiEvPtr e;
	long diffT,tp1,tp2;
	
	if (( e = MidiNewEv(typeClock) )) {
		MidiSendIm(ref,e);
		tp1 = 60000*gTempoUnitDenum + gRemain;
		tp2 = 24*gTempoUnitNum*gTempoSpeed;
		diffT = tp1 / tp2;
		gRemain = tp1 % tp2 ;
		gClockTask = MidiTask(ClockTask, date+diffT, ref, 0, 0 ,0 );
	}
}

/****************************************************************************
 					redrawButtons															
*****************************************************************************/

static void redrawButtons()
{
	gtk_widget_set_sensitive(gCont, !gStartStop);
	gtk_widget_set_sensitive(gStop, gStartStop);
}

/****************************************************************************
						Callbacks definition
*****************************************************************************/

gint my_delete_action(GtkWidget* w, GdkEvent* ev, gpointer data) 
{
	
	MidiForgetTask(&gClockTask);
	//if(gFilter) MidiFreeFilter(gFilter);
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

void tempo_action (GtkAdjustment* adj, GtkWidget* data)
{
	gTempoSpeed = GTK_ADJUSTMENT(gTempo)->value + 0.5; 
}
	
void startSynchro( GtkWidget *widget,gpointer   data )
{
	MidiSendIm(gRefNum,MidiNewEv(typeStart));
	if (!gStartStop) {
		gStartStop = TRUE;
		ClockTask(MidiGetTime(), gRefNum, 0, 0, 0 );
	}
  	redrawButtons();
}

void stopSynchro( GtkWidget *widget,gpointer   data )
{
 	MidiForgetTask(&gClockTask);
	gStartStop = FALSE;
	gRemain = 0;
	MidiSendIm(gRefNum,MidiNewEv(typeStop));
	redrawButtons();
}

void contSynchro( GtkWidget *widget,gpointer   data )
{
	MidiSendIm(gRefNum,MidiNewEv(typeContinue));
	gStartStop = TRUE;
	ClockTask(MidiGetTime(), gRefNum, 0, 0, 0 );
	redrawButtons();
}


 GtkWidget *create_bbox( gint  horizontal,
                               char *title,
                               gint  spacing,
                               gint  child_w,
                               gint  child_h,
                               gint  layout )
       {
         GtkWidget *frame;
         GtkWidget *bbox;
      
         frame = gtk_frame_new (title);

         if (horizontal)
           bbox = gtk_hbutton_box_new ();
         else
           bbox = gtk_vbutton_box_new ();

         gtk_container_set_border_width (GTK_CONTAINER (bbox), 5);
         gtk_container_add (GTK_CONTAINER (frame), bbox);

         /* Set the appearance of the Button Box */
         gtk_button_box_set_layout (GTK_BUTTON_BOX (bbox), layout);
         gtk_button_box_set_spacing (GTK_BUTTON_BOX (bbox), spacing);
         gtk_button_box_set_child_size (GTK_BUTTON_BOX (bbox), child_w, child_h);

         gStart = gtk_button_new_with_label ("Start");
         gtk_container_add (GTK_CONTAINER (bbox), gStart);
	
         gCont = gtk_button_new_with_label ("Play");
         gtk_container_add (GTK_CONTAINER (bbox), gCont);

       	 gStop = gtk_button_new_with_label ("Stop");
         gtk_container_add (GTK_CONTAINER (bbox), gStop);


	 /* Connect the "clicked" signal of the button to our callback */
         gtk_signal_connect (GTK_OBJECT (gStart), "clicked",
                               GTK_SIGNAL_FUNC (startSynchro), (gpointer) "cool button");

	 /* Connect the "clicked" signal of the button to our callback */
         gtk_signal_connect (GTK_OBJECT (gStop), "clicked",
                               GTK_SIGNAL_FUNC (stopSynchro), (gpointer) "cool button");

	 /* Connect the "clicked" signal of the button to our callback */
         gtk_signal_connect (GTK_OBJECT (gCont), "clicked",
                               GTK_SIGNAL_FUNC (contSynchro), (gpointer) "cool button");


         return(frame);
 }

/****************************************************************************
								Main
*****************************************************************************/

int main(int argc, char *argv[] )
{
	GtkWidget *window, *vbox;
 	char applName[256];

	// Initialisations
	gtk_init (&argc, &argv);
	StripPath (argv[0], applName);
	
	CheckMidiShare (applName);
	gRefNum = MidiOpen(applName);
	MidiConnect (gRefNum, 0, true);
  	MidiConnect (0, gRefNum, true);
	InstallFilter(gRefNum);
	MidiSetRcvAlarm(gRefNum, ReceiveEvents);

	// Adjustments description  
	gTempo 	= gtk_adjustment_new (120.0, 10.0, 241.0, 1.0, 8.0, 1.0);

	// Window definition
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), applName);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	vbox = gtk_vbox_new (FALSE, 10);

	// Add controllers
	add_control(vbox, "Tempo", gTempo);
	
	// Connexion of signals for value adjustement
	gtk_signal_connect(gTempo, "value_changed", GTK_SIGNAL_FUNC(tempo_action), NULL);
	

  	gtk_box_pack_start (GTK_BOX (vbox),
                  create_bbox (TRUE, "", 20, 55, 20, GTK_BUTTONBOX_SPREAD),
                             TRUE, TRUE, 0);

	gtk_container_add(GTK_CONTAINER(window), vbox);	
	gtk_widget_show_all (window);
	
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

