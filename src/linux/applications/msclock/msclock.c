/****************************************************************************
*****************************************************************************
*																			*
*                     		msClock, GTK version 							*
*																			*
*****************************************************************************
*****************************************************************************/


#include <stdio.h>
#include <gtk/gtk.h>
#include "MidiShare.h"
#include "msApplsTools.h"

/****************************************************************************
						Global variables for msControl  
*****************************************************************************/

static short 		gRefNum = -1;		// msControl MidiShare reference number

GtkWidget 	*gHours, *gMin, *gSec, *gMls;
GtkWidget 	*gRun, *gReset, *gText;
guint	gTimeID;
int		gLoopFlag = 0;
Boolean running = false;
unsigned long	gTimeOffset;

typedef struct {
	short	hours, min, sec, mls;
} msTime, *msTimePtr;
msTime gDisplayedTime = { 0 };

/****************************************************************************
						Callbacks definition
*****************************************************************************/

gint my_delete_action(GtkWidget* w, GdkEvent* ev, gpointer data) 
{
	MidiClose(gRefNum);
	gtk_main_quit();
	return FALSE;
}

/****************************************************************************
								Main
*****************************************************************************/
static void add(GtkWidget *box, GtkWidget *obj)
{
	gtk_box_pack_start_defaults (GTK_BOX (box), obj);
}

static GtkWidget * part_time (char *name, GtkWidget ** dst)
{
	GtkWidget	*vbox, *label, *value;
	
	vbox = gtk_vbox_new (TRUE, 10);
	label = gtk_label_new (name);
	gtk_label_set_justify (GTK_LABEL (label),GTK_JUSTIFY_CENTER);
  	gtk_widget_set_usize (GTK_WIDGET (label), 40, 20);

	value = gtk_label_new ("0");
	gtk_label_set_justify (GTK_LABEL (label),GTK_JUSTIFY_CENTER);
  	gtk_widget_set_usize (GTK_WIDGET (label), 40, 20);

	gtk_box_pack_start(GTK_BOX (vbox), label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (vbox), value, FALSE, FALSE, 0);
	*dst = value;
	return vbox;
}

static void add_time (GtkWidget* mbox)
{
	GtkWidget	*hbox;
	hbox = gtk_hbox_new (TRUE, 10);
	gtk_box_pack_start(GTK_BOX (hbox), part_time("Hrs", &gHours), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (hbox), part_time("Min", &gMin), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (hbox), part_time("Sec", &gSec), FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (hbox), part_time("Mls", &gMls), FALSE, FALSE, 0);
	add(mbox, hbox);
}

static void add_buttons (GtkWidget* mbox)
{
	GtkWidget	*hbox;
	gRun 	= GTK_WIDGET(gtk_toggle_button_new_with_label (" Run "));
  	gtk_widget_set_usize (gRun, 60, 20);
	gReset      = GTK_WIDGET(gtk_button_new_with_label (" Reset "));
  	gtk_widget_set_usize (gReset, 60, 20);
	hbox = gtk_hbox_new (TRUE, 10);
	gtk_box_pack_start(GTK_BOX (hbox), gRun, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (hbox), gReset, FALSE, FALSE,0);
	
	add(mbox, hbox);
}

/****************************************************************************
	current time display
*****************************************************************************/
static void Time2msTime (unsigned long time, msTimePtr t)
{
	unsigned long tmp;
	t->mls = time % 1000;
	tmp = time / 1000;
	t->sec = tmp % 60;
	tmp = tmp / 60;
	t->min = tmp % 60;
	t->hours = tmp / 60;
}

static void display (GtkWidget * w, int value)
{
	char text[255];
	sprintf (text, "%d", value);
	gtk_label_set_text (GTK_LABEL(w), text);
}

static gboolean display_time (gpointer unused)
{
	msTime current;
	Time2msTime (MidiGetTime() - gTimeOffset, &current);
	if (gDisplayedTime.hours != current.hours) {
		display (gHours, current.hours);
		gDisplayedTime.hours = current.hours;
	}
	if (gDisplayedTime.min != current.min) {
		display (gMin, current.min);
		gDisplayedTime.min = current.min;
	}
	if (gDisplayedTime.sec != current.sec) {
		display (gSec, current.sec);
		gDisplayedTime.sec = current.sec;
	}
	if (gDisplayedTime.mls != current.mls) {
		display (gMls, current.mls);
		gDisplayedTime.mls = current.mls;
	}
	return TRUE;
}

/****************************************************************************
	start / stop actions
*****************************************************************************/
static void start (Boolean toggle) {
	if (!running) {
		running = true;	
		gTimeOffset = MidiGetTime ();
		gTimeID = gtk_timeout_add( 10, display_time, NULL);
		if (toggle) {
			gLoopFlag++;
			gtk_toggle_button_set_active ((GtkToggleButton *)gRun, TRUE);
		}
	}
}

static void stop (Boolean toggle) { 
	if (running && ((MidiGetTime() - gTimeOffset) > 100)) {
		running = false;
		gtk_timeout_remove (gTimeID); 
		display_time(0);
		if (toggle) {
			gLoopFlag++;
			gtk_toggle_button_set_active ((GtkToggleButton *)gRun, FALSE);
		}
	}
}

static void reset () 	{ gTimeOffset = MidiGetTime(); display_time(0); }
	
static void start_action (GtkWidget* w)
{
	MidiSendIm (gRefNum, MidiNewEv (typeStart));
	start (FALSE);
}
	
static void stop_action (GtkWidget* w)
{
	MidiSendIm (gRefNum, MidiNewEv (typeStop));
	stop (FALSE);
}

/****************************************************************************
								Callbacks
*****************************************************************************/
static void start_stop_action (GtkWidget* w, void * unused)
{
	if (gLoopFlag) {
		gLoopFlag = 0;
		return;
	}
	if (running) stop_action (w);
	else 		 start_action (w);
}
	
static void reset_action (GtkWidget* w, void * unused)
{
	MidiSendIm (gRefNum, MidiNewEv (typeReset));
	reset ();
}
	
static void rcv_alarm (short refnum)
{
	MidiEvPtr e = MidiGetEv (refnum);
	while (e) {
		switch (EvType(e)) {
			case typeStart:	start(TRUE);
				break;
			case typeStop:	stop(TRUE);
				break;
			case typeReset:	reset();
				break;
		}
		MidiFreeEv (e);
		e = MidiGetEv (refnum);
	}
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
	MidiSetRcvAlarm (gRefNum,rcv_alarm);
	MidiConnect(gRefNum,0,1);
	MidiConnect(0,gRefNum,1);

	// Window definition
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), applName);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	vbox = gtk_vbox_new (FALSE, 10);
	
	// Add controllers
	add_time (vbox);
	add(vbox, gtk_hseparator_new());
	add_buttons (vbox);
	
	gtk_container_add(GTK_CONTAINER(window), vbox);	
	gtk_widget_show_all (window);
	
	gtk_signal_connect(GTK_OBJECT(gRun), "clicked", GTK_SIGNAL_FUNC(start_stop_action), NULL);
	gtk_signal_connect(GTK_OBJECT(gReset), "clicked", GTK_SIGNAL_FUNC(reset_action), NULL);

	// Connexion of signals	
	gtk_signal_connect(
			GTK_OBJECT(window), "destroy", 
			GTK_SIGNAL_FUNC(my_delete_action), NULL
	);
	
	gtk_main ();

	return(0);
}

