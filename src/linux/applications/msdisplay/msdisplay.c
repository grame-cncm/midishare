/****************************************************************************
*****************************************************************************
*																			*
*                     		msDisplay, GTK version			    *
*																			*
*****************************************************************************
*****************************************************************************/

#include <stdio.h>
#include <gtk/gtk.h>
#include "MidiShare.h"
#include "msApplsTools.h"

/****************************************************************************
						Global variables for msDisplay  
*****************************************************************************/

#define MAXEV 100

static short 		gRefNum = -1;		// msDisplay MidiShare reference number
static GtkWidget*	gEventList = 0;		// received events list
static long		gCount = 0;		// received events number


static int min (int a, int b) { return (a < b) ?a : b;}

/****************************************************************************
						Build the display list
*****************************************************************************/
static GtkWidget* makeEventList(GtkWidget** plist)
{
	int c;
	static gchar* Colonnes[]={"#", "hh:mm:ss.nnn", "Port", "Chan", "Type", "Data"};
	GtkWidget *scroll, *list;

	scroll = gtk_scrolled_window_new(NULL, NULL);	
	list = gtk_clist_new_with_titles(6, Colonnes);
	for (c=0; c<6; c++) gtk_clist_column_title_passive(GTK_CLIST(list),c);
	*plist = list;
	
	gtk_clist_set_column_auto_resize(GTK_CLIST(list), 0, TRUE);
	gtk_clist_set_column_auto_resize(GTK_CLIST(list), 4, TRUE);
	
	gtk_container_add(GTK_CONTAINER(scroll), list);
	gtk_widget_show (list);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_widget_set_usize(scroll, 300, 100);
	return scroll;
}


/****************************************************************************
				Display received events
*****************************************************************************/
static char* gEvNames_0_19[] = {
		"Note","KeyOn","KeyOff","KeyPress","CtrlChange","ProgChange",
		"ChanPress","PitchWheel","SongPos","SongSel","Clock","Start",
		"Continue","Stop","Tune","ActiveSens","Reset","SysEx","Stream", "Private"
		};

static char* gEvNames_128_150[] = {
		"Process", "DProcess", "QuarterFrame", "Ctrl14b", 
		"NonRegParam", "RegParam", "SeqNum", "Text", "Copyright", "SeqName", 
		"InstrName", "Lyric", "Marker", "CuePoint", "ChanPrefix", "EndTrack", 
		"Tempo", "SMPTEOffset", "TimeSign", "KeySign", "Specific", "PortPrefix", 
		"RcvAlarm","ApplAlarm"
		};

static char* EvName(MidiEvPtr e)
{
	if (EvType(e) < typePrivate) return gEvNames_0_19[EvType(e)];
	if (EvType(e) < typeProcess) return "Private";	
	if (EvType(e) < typeReserved) return gEvNames_128_150[EvType(e) - typeProcess];	
	return "Undefined";
}

static char * DatasToText(MidiEvPtr e , char *buff)
{
	char *ptr= buff;
	int i, max;

	*ptr= 0;
	switch (EvType(e)) {
		case typeTextual:
		case typeCopyright:
		case typeSeqName:
		case typeInstrName:
		case typeLyric:
		case typeMarker:
		case typeCuePoint:
			for( i=0, max= min(MidiCountFields(e), 18); i< max; i++)
				*ptr++= (char)MidiGetField(e, i);
			*ptr= 0;
			break;
		case typeSMPTEOffset:
			for( i=0; i< 6; i++, ptr+=3) 
				sprintf( ptr, "%-2ld ", MidiGetField(e, i));
			break;
	
		default:
			for( i=0, max= min(MidiCountFields(e), 5); i< max; i++, ptr+=4)
				sprintf( ptr, "%-3ld ", MidiGetField(e, i));
	}
	return buff;
}


void display_1_event(MidiEvPtr e)
{
		long 	t, hh, mn, ss, nnn;
		char* 	txt[6];
		char	buf0[8];
		char	buf1[16];
		char	buf2[8];
		char	buf3[8];
		char 	buf4[50];
		
		t = Date(e);
		nnn = t%1000; t /= 1000;
		ss = t%60; t /= 60;
		mn = t%60; t /= 60;
		hh = t%60; 
		
		txt[0] = buf0; sprintf(buf0, "%ld", gCount);
		txt[1] = buf1; sprintf(buf1, "%02ld:%02ld:%02ld.%03ld", hh,mn,ss,nnn);
		txt[2] = buf2; sprintf(buf2, "%d", Port(e));
		txt[3] = buf3; sprintf(buf3, "%d", Chan(e));
		txt[4] = EvName(e);
		txt[5] = DatasToText(e, buf4);
		gtk_clist_append(GTK_CLIST(gEventList), txt);
		gCount++; 
		if (gCount>MAXEV) gtk_clist_remove(GTK_CLIST(gEventList),0);
}

// GTK periodical function 

gint display_events( gpointer data )
{
	MidiEvPtr e;
	long c = MidiCountEvs(gRefNum);
	
	// if we are too late, remove some events
	while (c > MAXEV) { MidiFreeEv(MidiGetEv(gRefNum)); c--; }
	
	if (c>0) {
	
		while ( (c>0) && (e=MidiGetEv(gRefNum)) ) {
			display_1_event(e);
			MidiFreeEv(e);
			c--;
		}

		gtk_clist_moveto(GTK_CLIST(gEventList), (gCount>MAXEV) ? MAXEV-1 : gCount-1, 0, 0.0, 1.0);
	}
	return TRUE;
}

										
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

int main(int argc, char *argv[] )
{
	GtkWidget *window, *vbox;
	char applName[256];

	// Initialisations
	gtk_init (&argc, &argv);
	StripPath (argv[0], applName);
	
	CheckMidiShare (applName);
	gRefNum = MidiOpen(applName);
	MidiConnect (0,gRefNum,1);

	// User interface construction
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), applName);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	vbox = gtk_vbox_new (FALSE, 10);
	add(vbox, makeEventList(&gEventList));
	
	gtk_container_add(GTK_CONTAINER(window), vbox);	
	gtk_widget_show_all (window);
	
	// Connexion of signal	
	gtk_signal_connect(
			GTK_OBJECT(window), "destroy", 
			GTK_SIGNAL_FUNC(my_delete_action), NULL
	);
	
	// Install the periodical function which displays received events
	gtk_timeout_add( 100, display_events, NULL);

	gtk_main ();

	return(0);
}

