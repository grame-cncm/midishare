/****************************************************************************
*****************************************************************************
*																	*
*                     		msPlayer, GTK version 							*
*																	*
*****************************************************************************
*****************************************************************************/


#include <stdio.h>
#include <gtk/gtk.h>
#include "MidiShare.h"
#include "Player.h"

/****************************************************************************
						Global variables for msPlayer
*****************************************************************************/

static short 	gRefNum = -1;		// msPlayer MidiShare reference number
static MidiSeqPtr gSequence;		// Sequence filled with the MIDIFile

GtkWidget *start,*stop,*cont, *pause1;


/****************************************************************************
						Callbacks definition
*****************************************************************************/

gint my_delete_action(GtkWidget* w, GdkEvent* ev, gpointer data) 
{
	ClosePlayer(gRefNum);
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
/*	
static void param_action (GtkAdjustment* adj, GtkWidget* data)
{
	gDelay = GTK_ADJUSTMENT(delay)->value + 0.5;
	gPitch = GTK_ADJUSTMENT(pitch)->value + 0.5;
	gVelocity = GTK_ADJUSTMENT(velocity)->value + 0.5;
	gChannel = GTK_ADJUSTMENT(channel)->value + 0.5;	
}
*/
/* Our usual callback function */
void startPlayer( GtkWidget *widget,gpointer   data )
{
  	StartPlayer(gRefNum);
}

/* Our usual callback function */
void stopPlayer( GtkWidget *widget,gpointer   data )
{
 	StopPlayer(gRefNum);
}


/* Our usual callback function */
void contPlayer( GtkWidget *widget,gpointer   data )
{
	ContPlayer(gRefNum);
}


/* Our usual callback function */
void pausePlayer( GtkWidget *widget,gpointer   data )
{
	PausePlayer(gRefNum);
}




/****************************************************************************
								Main
*****************************************************************************/

int main(int argc, char *argv[] )
{
	GtkWidget *window, *vbox;
 	MidiFileInfos info;

	// Initialisations
	gtk_init (&argc, &argv);

	gRefNum = OpenPlayer("msPlayer");
	MidiConnect (gRefNum, 0, true);
  	MidiConnect (0, gRefNum, true);

	gSequence = MidiNewSeq();
	MidiFileLoad(argv[1], gSequence,&info); 
	SetAllTrackPlayer(gRefNum,gSequence, info.clicks);


   	/* Create buttons */
        start = gtk_button_new_with_label ("Start");
	stop = gtk_button_new_with_label ("Stop");
	cont = gtk_button_new_with_label ("Play");
	pause1 = gtk_button_new_with_label ("Pause");
 
        /* Connect the "clicked" signal of the button to our callback */
        gtk_signal_connect (GTK_OBJECT (start), "clicked",
                               GTK_SIGNAL_FUNC (startPlayer), (gpointer) "cool button");

	/* Connect the "clicked" signal of the button to our callback */
        gtk_signal_connect (GTK_OBJECT (stop), "clicked",
                               GTK_SIGNAL_FUNC (stopPlayer), (gpointer) "cool button");

	 /* Connect the "clicked" signal of the button to our callback */
        gtk_signal_connect (GTK_OBJECT (cont), "clicked",
                               GTK_SIGNAL_FUNC (contPlayer), (gpointer) "cool button");

	/* Connect the "clicked" signal of the button to our callback */
        gtk_signal_connect (GTK_OBJECT (pause1), "clicked",
                               GTK_SIGNAL_FUNC (pausePlayer), (gpointer) "cool button");


	// Window definition
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "msPlayer");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	vbox = gtk_vbox_new (FALSE, 10);

	add (vbox,start);
	add (vbox,stop);
	add (vbox,cont);
	add (vbox,pause1);	

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

