/****************************************************************************
*****************************************************************************
*																			*
*                     		msControl, GTK version 							*
*																			*
*****************************************************************************
*****************************************************************************/


#include <stdio.h>
#include <gtk/gtk.h>
#include "MidiShare.h"

/****************************************************************************
						Global variables for msControl  
*****************************************************************************/

static short 		gRefNum = -1;		// msControl MidiShare reference number

GtkObject 	*pitch, *velocity, *duration, 
			*channel, *port, 
			*prog, *vol, *mod, *pan;


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

	gtk_box_pack_start(GTK_BOX (hbox), label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX (hbox), scale, TRUE, TRUE,0);
	gtk_box_pack_start(GTK_BOX (hbox), spin, FALSE, FALSE,0);
	
	add(mbox, hbox);
}


/****************************************************************************
								Callbacks
*****************************************************************************/

	
void note_action (GtkAdjustment* adj, GtkWidget* data)
{
	MidiEvPtr e = MidiNewEv(typeNote);
	
	Pitch(e) = GTK_ADJUSTMENT(pitch)->value + 0.5; 
	Vel(e) = GTK_ADJUSTMENT(velocity)->value + 0.5; 
	Dur(e) = GTK_ADJUSTMENT(duration)->value + 0.5;
	Chan(e) = GTK_ADJUSTMENT(channel)->value + 0.5; 
	Port(e) = GTK_ADJUSTMENT(port)->value + 0.5; 
	
	MidiSendIm(gRefNum, e);
}
	
void prog_action (GtkAdjustment* adj, GtkWidget* data)
{
	MidiEvPtr e = MidiNewEv(typeProgChange);
	
	Pitch(e) = GTK_ADJUSTMENT(prog)->value + 0.5; 
	Chan(e) = GTK_ADJUSTMENT(channel)->value + 0.5; 
	Port(e) = GTK_ADJUSTMENT(port)->value + 0.5; 
	
	MidiSendIm(gRefNum, e);
}
	
void ctrl_action (GtkAdjustment* adj, long ctrlnumber)
{
	MidiEvPtr e = MidiNewEv(typeCtrlChange);
	
	Pitch(e) = ctrlnumber; 
	Vel(e) = GTK_ADJUSTMENT(vol)->value + 0.5; 
	Chan(e) = GTK_ADJUSTMENT(channel)->value + 0.5; 
	Port(e) = GTK_ADJUSTMENT(port)->value + 0.5; 
	
	MidiSendIm(gRefNum, e);
}


/****************************************************************************
								Main
*****************************************************************************/

int main(int argc, char *argv[] )
{
	GtkWidget *window, *vbox;

	// Initialisations
	gtk_init (&argc, &argv);
	gRefNum = MidiOpen("msControl");
	MidiConnect(gRefNum,0,1);
	MidiConnect(0,gRefNum,1);
	
	// Adjustments description  
	pitch 	= gtk_adjustment_new (60.0, 1.0, 128.0, 1.0, 8.0, 1.0);
	velocity= gtk_adjustment_new (64.0, 1.0, 128.0, 1.0, 8.0, 1.0);
	duration= gtk_adjustment_new (250.0, 50.0, 2000.0, 1.0, 50.0, 1.0);
	channel = gtk_adjustment_new (0.0, 0.0, 16.0, 1.0, 1.0, 1.0);
	port 	= gtk_adjustment_new (0.0, 0.0, 128.0, 1.0, 1.0, 1.0);

	prog	= gtk_adjustment_new (0.0, 0.0, 128.0, 1.0, 8.0, 1.0);
	vol		= gtk_adjustment_new (100.0, 0.0, 128.0, 1.0, 8.0, 1.0);
	mod		= gtk_adjustment_new (0.0, 0.0, 128.0, 1.0, 8.0, 1.0);
	pan		= gtk_adjustment_new (64.0, 0.0, 128.0, 1.0, 8.0, 1.0);


	// Window definition
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "msControl");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	vbox = gtk_vbox_new (FALSE, 10);
	
	// Add controllers
	add_control(vbox, "Pitch", pitch);
	add_control(vbox, "Vel", velocity);
	add_control(vbox, "Dur", duration);
	
	add(vbox, gtk_hseparator_new());

	add_control(vbox, "Prog", prog);
	add_control(vbox, "Vol", vol);
	add_control(vbox, "Mod", mod);
	add_control(vbox, "Pan", pan);
	
	add(vbox, gtk_hseparator_new());

	add_control(vbox, "Chan", channel);
	add_control(vbox, "Port", port);
	
	
	gtk_container_add(GTK_CONTAINER(window), vbox);	
	gtk_widget_show_all (window);
	
	// Connexion of signals for value adjustement
	gtk_signal_connect( pitch, "value_changed", GTK_SIGNAL_FUNC(note_action), NULL);
	gtk_signal_connect( velocity, "value_changed", GTK_SIGNAL_FUNC(note_action), NULL);
	gtk_signal_connect( duration, "value_changed", GTK_SIGNAL_FUNC(note_action), NULL);
	gtk_signal_connect( channel, "value_changed", GTK_SIGNAL_FUNC(note_action), NULL);
	gtk_signal_connect( port, "value_changed", GTK_SIGNAL_FUNC(note_action), NULL);

	gtk_signal_connect( prog, "value_changed", GTK_SIGNAL_FUNC(prog_action), NULL);
	gtk_signal_connect( vol, "value_changed", GTK_SIGNAL_FUNC(ctrl_action), (void*)7L);
	gtk_signal_connect( mod, "value_changed", GTK_SIGNAL_FUNC(ctrl_action), (void*)1L);
	gtk_signal_connect( pan, "value_changed", GTK_SIGNAL_FUNC(ctrl_action), (void*)10L);

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

