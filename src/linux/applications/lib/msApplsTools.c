
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "MidiShare.h"

//---------------------------------------------------------
void StripPath (char * name, char *dst)
{
	char *ptr = strrchr (name, '/');
	if (!ptr) ptr = name;
	else ptr++;
	while (*ptr) *dst++ = *ptr++;
	*dst = 0;
}


//---------------------------------------------------------
static short gClickedIndex;
static GtkWidget * gDlog;
static void AlertCallback (GtkWidget * button, short index)
{
	if (index >= 0)
		gtk_widget_destroy (gDlog);
	gClickedIndex = index;
	gtk_main_quit();
}


#define gtkconnect(o,str,f, arg)  gtk_signal_connect(GTK_OBJECT(o),str,GTK_SIGNAL_FUNC(f),(gpointer)arg);

//---------------------------------------------------------
static void CreateButton (GtkWidget * dlog, const char * label, int index, short defaut) 
{
	GtkWidget *button;
	
	button = gtk_button_new_with_label (label);
	gtkconnect (button, "clicked", AlertCallback, index);
	gtkconnect (button, "clicked", gtk_widget_destroy, dlog);
	GTK_WIDGET_SET_FLAGS(button, GTK_CAN_DEFAULT);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dlog)->action_area), button, TRUE, TRUE, 0);
	if (defaut)
		gtk_widget_grab_default (button);
}


//---------------------------------------------------------
short Alert (char *dlogTitle, char* msg, char* button[], short defaut)
{
	GtkWidget * label;
	short i=0;
	
	gClickedIndex = -1;
	gDlog = gtk_dialog_new();
	gtk_window_set_title (GTK_WINDOW(gDlog), dlogTitle);
	gtk_window_set_position (GTK_WINDOW(gDlog), GTK_WIN_POS_MOUSE);
	gtkconnect (gDlog, "destroy", AlertCallback, -1);
	
	while (button[i]) {
		CreateButton (gDlog, button[i], i, i==defaut);
		i++;
	}
	label = gtk_label_new (msg);
	gtk_container_set_border_width(GTK_CONTAINER(GTK_DIALOG(gDlog)->vbox), 10);
	gtk_box_pack_start (GTK_BOX(GTK_DIALOG(gDlog)->vbox), label, TRUE, TRUE, 0);
	
	gtk_window_set_position (GTK_WINDOW(gDlog), GTK_WIN_POS_MOUSE);
	gtk_window_set_policy (GTK_WINDOW(gDlog), FALSE, FALSE, TRUE);
	gtk_widget_show_all (gDlog);
	gtk_main();
	return gClickedIndex;
}


//---------------------------------------------------------
void CheckMidiShare (char * applname) 
{
	if (!MidiShare()) {
		char msg[512], *button[] = { "ok", 0 };
		char *notInstalled = "MidiShare is not installed.";
		sprintf (msg, "\nCan't launch \"%s\" !\n%s\n", applname, notInstalled);
		Alert (applname, msg, button, 0);
		gtk_exit (1);
	}
}
