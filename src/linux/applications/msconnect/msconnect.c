/****************************************************************************
*****************************************************************************
*																			*
*                     		msConnect, version GTK							*
*																			*
*****************************************************************************
*****************************************************************************/


#include <stdio.h>
#include <gtk/gtk.h>
#include "MidiShare.h"

/****************************************************************************
						Les variables globales de msConnect 
*****************************************************************************/

static short 		gRefNum = -1;		// numéro de référence MidiShare de msConnect

static short		gCurRow = 0;		// numéro de ligne de l'application selectionnée à l'affichage
static short		gCurRef = 0;		// numéro de référence de l'application selectionnée à l'affichage

static GtkWidget*	gSrcList = 0;		// liste des sources
static GtkWidget*	gAppList = 0;		// liste des applications clientes
static GtkWidget*	gDstList = 0;		// liste des destinations

static int			gUpdateFlag=FALSE;	// Vrai quand un update des listes est en cours et 
										// qu'il ne faut pas tenir compte des callbacks engendrées
static int			gNeedRebuild=FALSE;	// Positionner par l'applAlarm pour indiquer un changement
										// dans la liste des applications clientes
static int			gNeedRefresh=FALSE;	// Positionner par l'ApplAlarm pour indiquer un changement 
										// de connexions

/****************************************************************************
								Mise a jour des listes
*****************************************************************************/

#define row2ref(row)  MidiGetIndAppl(1+row)

static void rebuildLists()
{
	short 	i, c;
	gchar* 	txt[2];

	//printf("rebuildlist\n");
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
	
	//printf("refreshlist\n");
	r = row2ref(gCurRow);		// refnum de l'appli dont ont va afficher les liens
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
			Callback de gestion des selections dans les listes
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
						Construction des listes
*****************************************************************************/

static GtkWidget* makelist(GtkWidget** plist, gchar* lstname,GtkSelectionMode mode, GtkSignalFunc selfun, GtkSignalFunc unselfun)
{
	static gchar* Colonnes[2];
	GtkWidget *scroll, *list;
	
	Colonnes[0] = "#";
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
				Surveillance des clients MidiShare
*****************************************************************************/
// MypplAlarm se contente de positionner des codes
// que la fonction periodique check_update exploite
// pour mettre a jour les listes
	
// Alarme de contexte MidiShare
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

// Fonction periodique GTK
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
						Definition des callbacks 
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

	// Initialisations
	gtk_init (&argc, &argv);
	gRefNum = MidiOpen("msConnect");

	// Construction de l'interface utilisateur
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "msConnect");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	hbox = gtk_hbox_new (FALSE, 10);
	add(hbox, makelist(&gSrcList, "Sources ->", GTK_SELECTION_MULTIPLE, GTK_SIGNAL_FUNC(select_src), GTK_SIGNAL_FUNC(unselect_src)));
	add(hbox, makelist(&gAppList, "Applications", GTK_SELECTION_BROWSE, GTK_SIGNAL_FUNC(select_appl), NULL));
	add(hbox, makelist(&gDstList, "-> Destinations",GTK_SELECTION_MULTIPLE, GTK_SIGNAL_FUNC(select_dst), GTK_SIGNAL_FUNC(unselect_dst)));
	
	initLists();
	
	gtk_container_add(GTK_CONTAINER(window), hbox);	
	gtk_widget_show_all (window);
	
	// connexion des signaux
	gtk_signal_connect(
			GTK_OBJECT(window), "delete_event", 
			GTK_SIGNAL_FUNC(my_delete_action), NULL
	);
	
	gtk_signal_connect(
			GTK_OBJECT(window), "destroy", 
			GTK_SIGNAL_FUNC(my_delete_action), NULL
	);
	
	// installation de la surveillance des appli MidiShare
	MidiSetApplAlarm(gRefNum, MyApplAlarm);
	gtk_timeout_add( 100, check_update, NULL);

	gtk_main ();

	return(0);
}

