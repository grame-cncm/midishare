/****************************************************************************
*****************************************************************************
*																			*
*                     		msConnect, GTK version 							*
*																			*
*****************************************************************************
*****************************************************************************/


#include <stdio.h>
#include <gtk/gtk.h>
#include "MidiShare.h"
#include "msApplsTools.h"

/****************************************************************************
						Global variables for msConnect 
*****************************************************************************/

static short 		gRefNum = -1;		// msConnect MidiShare reference number

static short		gCurRow = 0;		// line number of the selected displayed aplication
static short		gCurRef = 0;		// reference number of the selected displayed aplication

static GtkWidget*	gSrcList = 0;		// sources list
static GtkWidget*	gAppList = 0;		// applications list
static GtkWidget*	gDstList = 0;		// destinations list

static int			gUpdateFlag=FALSE;	// true when a list update is being done and callbacks must be ignored
static int			gNeedRebuild=FALSE;	// set up by the applAlarm when the applications list has been changed
static int			gNeedRefresh=FALSE;	// set up by the applAlarm when connections have changed

/****************************************************************************
								Lists management
*****************************************************************************/

#define row2ref(row)  MidiGetIndAppl(1+row)

static void rebuildLists()
{
	short 	i, c;
	gchar* 	txt[2];

	gUpdateFlag=TRUE;	
	
	gtk_clist_freeze(GTK_CLIST(gSrcList));
	gtk_clist_freeze(GTK_CLIST(gAppList));
	gtk_clist_freeze(GTK_CLIST(gDstList));
	
	gtk_clist_clear(GTK_CLIST(gSrcList));
	gtk_clist_clear(GTK_CLIST(gAppList));
	gtk_clist_clear(GTK_CLIST(gDstList));
	
	c = MidiCountAppls();
	for (i=1; i <= c; i++) {
		txt[0] = g_strdup_printf("%d", MidiGetIndAppl(i));
		txt[1] = MidiGetName(MidiGetIndAppl(i));
		gtk_clist_append(GTK_CLIST(gSrcList), txt);
		gtk_clist_append(GTK_CLIST(gAppList), txt);
		gtk_clist_append(GTK_CLIST(gDstList), txt);
	}
	
	gtk_clist_thaw(GTK_CLIST(gSrcList));
	gtk_clist_thaw(GTK_CLIST(gAppList));
	gtk_clist_thaw(GTK_CLIST(gDstList));

	gUpdateFlag=FALSE;

}		

static void refreshLists()
{
	short r, i, c, a;
	
	r = row2ref(gCurRow);		// application number which lines are displayed
	if (r < 0) r = 0;
	
	gUpdateFlag=TRUE;
	
	gtk_clist_freeze(GTK_CLIST(gSrcList));
	gtk_clist_freeze(GTK_CLIST(gAppList));
	gtk_clist_freeze(GTK_CLIST(gDstList));
	
	gtk_clist_unselect_all(GTK_CLIST(gAppList));
	gtk_clist_unselect_all(GTK_CLIST(gSrcList));
	gtk_clist_unselect_all(GTK_CLIST(gDstList));
	
	for (i=0, c=MidiCountAppls(); i < c; i++) {
		a = row2ref(i);
		if (a == r)					gtk_clist_select_row (GTK_CLIST(gAppList), i, 0);
		if (MidiIsConnected(a, r))	gtk_clist_select_row (GTK_CLIST(gSrcList), i, 0);
		if (MidiIsConnected(r, a)) 	gtk_clist_select_row (GTK_CLIST(gDstList), i, 0);
	}
	
	gtk_clist_thaw(GTK_CLIST(gSrcList));
	gtk_clist_thaw(GTK_CLIST(gAppList));
	gtk_clist_thaw(GTK_CLIST(gDstList));
		
	gUpdateFlag=FALSE;

}		

/****************************************************************************
			List selections callbacks
 ***************************************************************************/

static void select_src(GtkWidget *widget, gint row, gint column, GdkEventButton *event, gpointer data)
{
	if (!gUpdateFlag) MidiConnect(row2ref(row), row2ref(gCurRow), 1);
}

static void unselect_src(GtkWidget *widget, gint row, gint column, GdkEventButton *event, gpointer data)
{
	if (!gUpdateFlag) MidiConnect(row2ref(row), row2ref(gCurRow), 0);
}

static void select_appl(GtkWidget *widget, gint row, gint column, GdkEventButton *event, gpointer data)
{
	if (!gUpdateFlag) { gCurRow = row; gCurRef = row2ref(row); refreshLists(); }
}

static void select_dst(GtkWidget *widget, gint row, gint column, GdkEventButton *event, gpointer data)
{
	if (!gUpdateFlag) MidiConnect(row2ref(gCurRow), row2ref(row), 1);
}

static void unselect_dst(GtkWidget *widget, gint row, gint column, GdkEventButton *event, gpointer data)
{
	if (!gUpdateFlag) MidiConnect(row2ref(gCurRow), row2ref(row), 0);
}


/****************************************************************************
						List construction
*****************************************************************************/

static GtkWidget* makelist(GtkWidget** plist, gchar* lstname,GtkSelectionMode mode, GtkSignalFunc selfun, GtkSignalFunc unselfun)
{
	static gchar* Colonnes[2];
	GtkWidget *scroll, *list;
	
	Colonnes[0] = "#  ";
	Colonnes[1] = lstname;

	scroll = gtk_scrolled_window_new(NULL, NULL);	
	list = gtk_clist_new_with_titles(2, Colonnes);
	gtk_clist_column_title_passive(GTK_CLIST(list),0);
	gtk_clist_column_title_passive(GTK_CLIST(list),1);
	*plist = list;

	gtk_clist_set_selection_mode(GTK_CLIST(list), mode);
	if (selfun) gtk_signal_connect(GTK_OBJECT(list),"select_row", selfun, NULL);
	if (unselfun) gtk_signal_connect(GTK_OBJECT(list),"unselect_row", unselfun, NULL);


	gtk_container_add(GTK_CONTAINER(scroll), list);
	gtk_widget_show (list);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_usize(scroll, 150, 100);
	return scroll;
}
	
static void initLists()
{
	gUpdateFlag = TRUE;
	rebuildLists();
	gCurRow=0; 
	gCurRef=0;
	refreshLists();
	gUpdateFlag = FALSE;
}


/****************************************************************************
				MidiShare clients management
*****************************************************************************/
// MypplAlarm updates globals variables that the periadical check_update
// function use to update the lists
	
// MidiShare context alarm
void MyApplAlarm (short refNum, long msg)
{
	unsigned int code = msg & 0xFFFF;
	unsigned int ref = msg >> 16;

	switch (code) {
		
		case MIDIOpenAppl :
		case MIDICloseAppl : 
			if (ref == gCurRef) { gCurRef = 0; gCurRow = 0; }
			gNeedRebuild = TRUE;
			return;
			
		case MIDIChgConnect :
			gNeedRefresh = TRUE;
			return;
	}
}

// GTK periodical function 
gint check_update( gpointer data )
{
	if (gNeedRebuild) {
		gNeedRebuild = FALSE;
		gNeedRefresh = FALSE;
		gUpdateFlag = TRUE;
			rebuildLists();
			refreshLists();
		gUpdateFlag = FALSE;
	} else if (gNeedRefresh) {
		gNeedRefresh = FALSE;
		gUpdateFlag = TRUE;
			refreshLists();
		gUpdateFlag = FALSE;
	}
		
	return TRUE;
}

										
/****************************************************************************
						Callbacks  definition
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

int main(int argc, char *argv[] )
{
	GtkWidget *window, *hbox;
	char applName[256];

	// Initialisations
	gtk_init (&argc, &argv);
	StripPath (argv[0], applName);
	
	CheckMidiShare (applName);
	gRefNum = MidiOpen(applName);

	// User interface construction
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), applName);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	hbox = gtk_hbox_new (FALSE, 10);
	add(hbox, makelist(&gSrcList, "Sources ->", GTK_SELECTION_MULTIPLE, GTK_SIGNAL_FUNC(select_src), GTK_SIGNAL_FUNC(unselect_src)));
	add(hbox, makelist(&gAppList, "Applications", GTK_SELECTION_BROWSE, GTK_SIGNAL_FUNC(select_appl), NULL));
	add(hbox, makelist(&gDstList, "-> Destinations",GTK_SELECTION_MULTIPLE, GTK_SIGNAL_FUNC(select_dst), GTK_SIGNAL_FUNC(unselect_dst)));
	
	initLists();
	
	gtk_container_add(GTK_CONTAINER(window), hbox);	
	gtk_widget_show_all (window);
	
	// signal connexion
	
	gtk_signal_connect(
			GTK_OBJECT(window), "destroy", 
			GTK_SIGNAL_FUNC(my_delete_action), NULL
	);
	
	// MidiShare applAlarm initialisation
	MidiSetApplAlarm(gRefNum, MyApplAlarm);
	gtk_timeout_add( 100, check_update, NULL);

	gtk_main ();

	return(0);
}

