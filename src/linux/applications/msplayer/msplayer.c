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
#include "msApplsTools.h"

/****************************************************************************
						Global variables for msPlayer
*****************************************************************************/

static short 	gRefNum = -1;		// msPlayer MidiShare reference number
static MidiSeqPtr gSequence;		// Sequence filled with the MIDIFile

GtkWidget *gStart,*gStop,*gCont, *gPause;
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

/****************************************************************************
 					redrawButtons															
*****************************************************************************/

static void redrawButtons()
{
	PlayerState state;
	Boolean st1,st2;

	GetStatePlayer (gRefNum, &state);
	st1 = (state.state == kIdle)||(state.state == kPause);
	st2 = (state.state == kIdle);

	gtk_widget_set_sensitive(gCont, st1);
	gtk_widget_set_sensitive(gStop, !st1);
	gtk_widget_set_sensitive(gPause, !st1);
	
	gtk_widget_set_sensitive(gInternalSync, st2);
	gtk_widget_set_sensitive(gClockSync, st2);
	gtk_widget_set_sensitive(gSMPTESync, st2);
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
	if (GTK_BUTTON(button) == GTK_BUTTON(gInternalSync))
		SetSynchroInPlayer(gRefNum, kInternalSync);
	
	if (GTK_BUTTON(button) == GTK_BUTTON(gClockSync))
		SetSynchroInPlayer(gRefNum, kClockSync);

	if (GTK_BUTTON(button) == GTK_BUTTON(gSMPTESync))
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

         gStart = gtk_button_new_with_label ("Start");
         gtk_container_add (GTK_CONTAINER (bbox), gStart);

         gCont = gtk_button_new_with_label ("Play");
         gtk_container_add (GTK_CONTAINER (bbox), gCont);

         gPause = gtk_button_new_with_label ("Pause");
         gtk_container_add (GTK_CONTAINER (bbox), gPause);

 	 gStop = gtk_button_new_with_label ("Stop");
         gtk_container_add (GTK_CONTAINER (bbox), gStop);


	 /* Connect the "clicked" signal of the button to our callback */
        gtk_signal_connect (GTK_OBJECT (gStart), "clicked",
                               GTK_SIGNAL_FUNC (startPlayer), (gpointer) "cool button");

	/* Connect the "clicked" signal of the button to our callback */
        gtk_signal_connect (GTK_OBJECT (gStop), "clicked",
                               GTK_SIGNAL_FUNC (stopPlayer), (gpointer) "cool button");

	 /* Connect the "clicked" signal of the button to our callback */
        gtk_signal_connect (GTK_OBJECT (gCont), "clicked",
                               GTK_SIGNAL_FUNC (contPlayer), (gpointer) "cool button");

	/* Connect the "clicked" signal of the button to our callback */
        gtk_signal_connect (GTK_OBJECT (gPause), "clicked",
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
	 gtk_frame_set_label_align(GTK_FRAME(frame), 0.5,0.5);

         if (horizontal)
           bbox = gtk_hbox_new (TRUE,1);
         else
           bbox = gtk_vbox_new (TRUE,1);

         gtk_container_set_border_width (GTK_CONTAINER (bbox), 5);
         gtk_container_add (GTK_CONTAINER (frame), bbox);

         gBar = gtk_entry_new();
	 gtk_widget_set_sensitive(gBar, FALSE);
	 gtk_entry_set_max_length(GTK_ENTRY(gBar),4);
	 gtk_entry_set_text(GTK_ENTRY(gBar),"1");
	 gtk_widget_set_usize(gBar, 30 ,16);
         gtk_box_pack_start (GTK_BOX (bbox), gBar,FALSE,0,0);
 	 gBeat = gtk_entry_new();
	 gtk_widget_set_sensitive(gBeat, FALSE);
	 gtk_entry_set_text(GTK_ENTRY(gBeat),"1");
	 gtk_entry_set_max_length(GTK_ENTRY(gBeat),4);
         gtk_widget_set_usize(gBeat, 30 ,16);
         gtk_box_pack_start (GTK_BOX (bbox), gBeat,FALSE,0,0);
	 gUnit = gtk_entry_new();
	 gtk_widget_set_sensitive(gUnit, FALSE);
	 gtk_entry_set_max_length(GTK_ENTRY(gUnit),4);
	 gtk_entry_set_text(GTK_ENTRY(gUnit),"1");
	 gtk_widget_set_usize(gUnit, 30 ,16);
         gtk_box_pack_start (GTK_BOX (bbox), gUnit,FALSE,0,0);

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
	 gtk_frame_set_label_align(GTK_FRAME(frame), 0.5,0.5);

         if (horizontal)
           bbox = gtk_hbox_new (TRUE,1);
         else
           bbox = gtk_vbox_new (TRUE,1);

         gtk_container_set_border_width (GTK_CONTAINER (bbox), 5);
         gtk_container_add (GTK_CONTAINER (frame), bbox);

         gMin = gtk_entry_new();
	 gtk_widget_set_sensitive(gMin, FALSE);
	 gtk_entry_set_max_length(GTK_ENTRY(gMin),4);
	 gtk_entry_set_text(GTK_ENTRY(gMin),"0");
	 gtk_widget_set_usize(gMin, 30 ,16);
         gtk_box_pack_start (GTK_BOX (bbox), gMin,FALSE,0,0);
 	 gSec = gtk_entry_new();
	 gtk_widget_set_sensitive(gSec, FALSE);
	 gtk_entry_set_text(GTK_ENTRY(gSec),"0");
	 gtk_entry_set_max_length(GTK_ENTRY(gSec),4);
         gtk_widget_set_usize(gSec, 30 ,16);
         gtk_box_pack_start (GTK_BOX (bbox), gSec,FALSE,0,0);
	 gMilli = gtk_entry_new();
	 gtk_widget_set_sensitive(gMilli, FALSE);
	 gtk_entry_set_max_length(GTK_ENTRY(gMilli),4);
	 gtk_entry_set_text(GTK_ENTRY(gMilli),"0");
	 gtk_widget_set_usize(gMilli, 30 ,16);
         gtk_box_pack_start (GTK_BOX (bbox), gMilli,FALSE,0,0);

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
	gtk_entry_set_text(GTK_ENTRY(gBar),buffer );
	sprintf(buffer,"%d",state.beat);
	gtk_entry_set_text(GTK_ENTRY(gBeat), buffer);
	sprintf(buffer,"%d",state.unit);
	gtk_entry_set_text(GTK_ENTRY(gUnit), buffer);

	sprintf(buffer, "%ld", mn);
	gtk_entry_set_text(GTK_ENTRY(gMin), buffer);
	sprintf(buffer, "%ld", ss);
	gtk_entry_set_text(GTK_ENTRY(gSec), buffer);
	sprintf(buffer, "%ld", nnn);
	gtk_entry_set_text(GTK_ENTRY(gMilli), buffer);

	redrawButtons();

        return TRUE;
}


/****************************************************************************
								Main
*****************************************************************************/

int main(int argc, char *argv[] )
{
	GtkWidget *window, *vbox;
 	MidiFileInfos info;
	char applName[256];
	
	// Initialisations
	gtk_init (&argc, &argv);
	StripPath (argv[0], applName);
	
	CheckMidiShare (applName);

	if (argc!=2) {
		printf("Usage : %s <midifile> \n", applName);
		return 0;
	}

	gRefNum = OpenPlayer(applName);
	MidiConnect (gRefNum, 0, true);
  	MidiConnect (0, gRefNum, true);

	gSequence = MidiNewSeq();
	MidiFileLoad(argv[1], gSequence,&info); 
	SetAllTrackPlayer(gRefNum,gSequence, info.clicks);
	
	// Window definition
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), applName);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);


	vbox = gtk_table_new(3,5, TRUE);
	gtk_table_attach(GTK_TABLE(vbox),createDateBBUBox (TRUE, "Bar  Beat  Unit", 1, 15, 20, GTK_BUTTONBOX_START),
                              0,3,0,1,GTK_EXPAND,GTK_EXPAND, 5,5);
	
	
	gtk_table_attach(GTK_TABLE(vbox),createDateMSBox (TRUE, "Min  Sec  Ms", 1, 15, 20, GTK_BUTTONBOX_START),
                              0,3,1,2,GTK_EXPAND,GTK_EXPAND, 5,5);
	
	
	gtk_table_attach(GTK_TABLE(vbox), createSynchroBox (FALSE, "Synchro", 5, 40, 20, GTK_BUTTONBOX_START),
                              3,5,0,2,GTK_EXPAND,GTK_EXPAND, 5,5);
	
	
	gtk_table_attach(GTK_TABLE(vbox),createTransportBox (TRUE, "Transport", 5, 40, 20, GTK_BUTTONBOX_SPREAD),
                              0,5,2,3,GTK_EXPAND,GTK_EXPAND, 5,5);
	
	
	gtk_container_add(GTK_CONTAINER(window), vbox);	
	gtk_widget_show_all (window);
	
	// Connexion of signals
		gtk_signal_connect(
			GTK_OBJECT(window), "destroy", 
			GTK_SIGNAL_FUNC(my_delete_action), NULL
	);
	gtk_timeout_add( 250, displayState, NULL);
	gtk_main ();

	return(0);
}

