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
GtkWidget *gInternalSync,*gClockSync,*gSMPTESync;
GtkWidget *gBar,*gBeat,*gUnit;
GtkWidget *gMin,*gSec,*gMilli;

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
 					redrawButtons															
*****************************************************************************/

static void redrawButtons()
{
	PlayerState state;
	Boolean st;

	GetStatePlayer (gRefNum, &state);
	st = (state.state == kIdle)||(state.state == kPause);

	gtk_widget_set_sensitive(cont, st);
	gtk_widget_set_sensitive(stop, !st);
	gtk_widget_set_sensitive(pause1, !st);
}



/****************************************************************************
					Callbacks
*****************************************************************************/

void startPlayer( GtkWidget *widget,gpointer   data )
{
  	StartPlayer(gRefNum);
	redrawButtons();
}

void stopPlayer( GtkWidget *widget,gpointer   data )
{
 	StopPlayer(gRefNum);
	redrawButtons();
}


void contPlayer( GtkWidget *widget,gpointer   data )
{
	ContPlayer(gRefNum);
	redrawButtons();
}


void pausePlayer( GtkWidget *widget,gpointer   data )
{
	PausePlayer(gRefNum);
	redrawButtons();
}

void synchroPlayer( GtkWidget *button,char*  data )
{
	if (GTK_TOGGLE_BUTTON(button) == gInternalSync)
		SetSynchroInPlayer(gRefNum, kInternalSync);
	
	if (GTK_TOGGLE_BUTTON(button) == gClockSync)
		SetSynchroInPlayer(gRefNum, kClockSync);

	if (GTK_TOGGLE_BUTTON(button) == gSMPTESync)
		SetSynchroInPlayer(gRefNum, kSMPTESync);
}



/****************************************************************************
					User Interface Creation
*****************************************************************************/

 GtkWidget *createTransportBox( gint  horizontal,
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

         start = gtk_button_new_with_label ("Start");
         gtk_container_add (GTK_CONTAINER (bbox), start);

         cont = gtk_button_new_with_label ("Play");
         gtk_container_add (GTK_CONTAINER (bbox), cont);

         pause1 = gtk_button_new_with_label ("Pause");
         gtk_container_add (GTK_CONTAINER (bbox), pause1);

 	 stop = gtk_button_new_with_label ("Stop");
         gtk_container_add (GTK_CONTAINER (bbox), stop);


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


         return(frame);
  }


 GtkWidget *createSynchroBox( gint  horizontal,
                               char *title,
                               gint  spacing,
                               gint  child_w,
                               gint  child_h,
                               gint  layout )
       {
         GtkWidget *frame;
         GtkWidget *bbox;
 	 GSList *group;

      
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

         gInternalSync = gtk_radio_button_new_with_label (NULL,"Internal");
	 gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (gInternalSync), TRUE);
         gtk_container_add (GTK_CONTAINER (bbox), gInternalSync);
	 group = gtk_radio_button_group (GTK_RADIO_BUTTON (gInternalSync));

         gClockSync = gtk_radio_button_new_with_label (group,"Clock");
         gtk_container_add (GTK_CONTAINER (bbox), gClockSync);
	 group = gtk_radio_button_group (GTK_RADIO_BUTTON (gClockSync));

         gSMPTESync = gtk_radio_button_new_with_label (group,"SMPTE");
         gtk_container_add (GTK_CONTAINER (bbox), gSMPTESync);

	
	 /* Connect the "clicked" signal of the button to our callback */
        gtk_signal_connect (GTK_OBJECT (gInternalSync), "clicked",
                               GTK_SIGNAL_FUNC (synchroPlayer), (gpointer) "int\n");

	/* Connect the "clicked" signal of the button to our callback */
        gtk_signal_connect (GTK_OBJECT (gClockSync), "clicked",
                               GTK_SIGNAL_FUNC (synchroPlayer), (gpointer) "clock\n");

	 /* Connect the "clicked" signal of the button to our callback */
        gtk_signal_connect (GTK_OBJECT (gSMPTESync), "clicked",
                               GTK_SIGNAL_FUNC (synchroPlayer), (gpointer) "smpte \n");

         return(frame);
}


 GtkWidget *createDateBBUBox( gint  horizontal,
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
           bbox = gtk_hbox_new (TRUE,1);
         else
           bbox = gtk_vbox_new (TRUE,1);

         gtk_container_set_border_width (GTK_CONTAINER (bbox), 5);
         gtk_container_add (GTK_CONTAINER (frame), bbox);

         gBar = gtk_entry_new();
	 gtk_entry_set_max_length(gBar,4);
	 gtk_entry_set_text(gBar,"1");
	 gtk_widget_set_usize(gBar, 30 ,16);
         gtk_box_pack_start (GTK_CONTAINER (bbox), gBar,FALSE,0,0);
 	 gBeat = gtk_entry_new();
	 gtk_entry_set_text(gBeat,"1");
	 gtk_entry_set_max_length(gBeat,4);
         gtk_widget_set_usize(gBeat, 30 ,16);
         gtk_box_pack_start (GTK_CONTAINER (bbox), gBeat,FALSE,0,0);
	 gUnit = gtk_entry_new();
	 gtk_entry_set_max_length(gUnit,4);
	 gtk_entry_set_text(gUnit,"1");
	 gtk_widget_set_usize(gUnit, 30 ,16);
         gtk_box_pack_start (GTK_CONTAINER (bbox), gUnit,FALSE,0,0);

         return(frame);
}

GtkWidget *createDateMSBox( gint  horizontal,
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
           bbox = gtk_hbox_new (TRUE,1);
         else
           bbox = gtk_vbox_new (TRUE,1);

         gtk_container_set_border_width (GTK_CONTAINER (bbox), 5);
         gtk_container_add (GTK_CONTAINER (frame), bbox);

         gMin = gtk_entry_new();
	 gtk_entry_set_max_length(gMin,4);
	 gtk_entry_set_text(gMin,"0");
	 gtk_widget_set_usize(gMin, 30 ,16);
         gtk_box_pack_start (GTK_CONTAINER (bbox), gMin,FALSE,0,0);
 	 gSec = gtk_entry_new();
	 gtk_entry_set_text(gSec,"0");
	 gtk_entry_set_max_length(gSec,4);
         gtk_widget_set_usize(gSec, 30 ,16);
         gtk_box_pack_start (GTK_CONTAINER (bbox), gSec,FALSE,0,0);
	 gMilli = gtk_entry_new();
	 gtk_entry_set_max_length(gMilli,4);
	 gtk_entry_set_text(gMilli,"0");
	 gtk_widget_set_usize(gMilli, 30 ,16);
         gtk_box_pack_start (GTK_CONTAINER (bbox), gMilli,FALSE,0,0);

         return(frame);
}



/****************************************************************************
					Periodical fucntion
*****************************************************************************/


gint displayState( gpointer data )
{
	PlayerState state;
	char buffer[16];
	long   t , hh, mn, ss, nnn;

	GetStatePlayer (gRefNum, &state);

	t = state.date;
	nnn = t%1000; t /= 1000;
        ss = t%60; t /= 60;
        mn = t%60; t /= 60;
        hh = t%60; 
    
	sprintf(buffer,"%d",state.bar);
	gtk_entry_set_text(gBar,buffer );
	sprintf(buffer,"%d",state.beat);
	gtk_entry_set_text(gBeat, buffer);
	sprintf(buffer,"%d",state.unit);
	gtk_entry_set_text(gUnit, buffer);

	sprintf(buffer, "%d", mn);
	gtk_entry_set_text(gMin, buffer);
	sprintf(buffer, "%d", ss);
	gtk_entry_set_text(gSec, buffer);
	sprintf(buffer, "%d", nnn);
	gtk_entry_set_text(gMilli, buffer);

        return TRUE;
}


/****************************************************************************
								Main
*****************************************************************************/

int main(int argc, char *argv[] )
{
	GtkWidget *window, *vbox;
 	MidiFileInfos info;
	PlayerState state;

	// Initialisations
	gtk_init (&argc, &argv);

	gRefNum = OpenPlayer("msPlayer");
	MidiConnect (gRefNum, 0, true);
  	MidiConnect (0, gRefNum, true);

	gSequence = MidiNewSeq();
	MidiFileLoad(argv[1], gSequence,&info); 
	SetAllTrackPlayer(gRefNum,gSequence, info.clicks);
	
	// Window definition
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "msPlayer");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	vbox = gtk_vbox_new (FALSE, 10);

	gtk_box_pack_start (GTK_BOX (vbox),
                  createDateBBUBox (TRUE, "Date : Bar Beat Unit", 1, 15, 20, GTK_BUTTONBOX_START),
                             TRUE, TRUE, 0);

	gtk_box_pack_start (GTK_BOX (vbox),
                  createDateMSBox (TRUE, "Date : Min Sec Ms", 1, 15, 20, GTK_BUTTONBOX_START),
                             TRUE, TRUE, 0);


	gtk_box_pack_start (GTK_BOX (vbox),
                  createSynchroBox (TRUE, "Synchro", 20, 55, 20, GTK_BUTTONBOX_SPREAD),
                             TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (vbox),
                  createTransportBox (TRUE, "Transport", 20, 55, 20, GTK_BUTTONBOX_SPREAD),
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
	gtk_timeout_add( 250, displayState, NULL);
	gtk_main ();

	return(0);
}

