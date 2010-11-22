
#include <QMessageBox>
#include <QMenuBar>

#include "dialog.h"
#include "ui_dialog.h"
#include "MidiShare.h"
#include "states.h"

static const char * profileName 		= "msControler.ini";
static const char * StateSectionName	= "Application state";
static const char * portEntryName		= "port";
static const char * chanEntryName		= "chan";
static const char * progChgeEntryName	= "progChge";
static const char * volEntryName		= "vol";
static const char * modulationEntryName = "modulation";
static const char * panEntryName		= "pan";
static const char * pitchEntryName		= "pitch";
static const char * velEntryName		= "vel";

Dialog::Dialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::Dialog)
{
	QMenuBar * menu = new QMenuBar();
    QMenu * helpMenu = menu->addMenu(tr("&Help"));
	QAction * aboutAct = new QAction(tr("&About"), this);
    helpMenu->addAction(aboutAct);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
	
	fRefNum = 0;
	fPitch = fVel = fChan = fPort = 0;
	fProg = fMod = fVol = fPan = 0;

    ui->setupUi(this);
	readState();
	ui->pitchSlider->setSliderPosition (fPitch);
	ui->velSlider->setSliderPosition (fVel);
	ui->chanSlider->setSliderPosition (fChan);
	ui->portSlider->setSliderPosition (fPort);
	
	ui->progSlider->setSliderPosition (fProg);
	ui->modSlider->setSliderPosition (fMod);
	ui->volSlider->setSliderPosition (fVol);
	ui->panSlider->setSliderPosition (fPan);

    
	connect(ui->pitchSlider, SIGNAL(valueChanged(int)), this, SLOT(setPitch(int)));
	connect(ui->velSlider, SIGNAL(valueChanged(int)), this, SLOT(setVel(int)));
	connect(ui->chanSlider, SIGNAL(valueChanged(int)), this, SLOT(setChan(int)));
	connect(ui->portSlider, SIGNAL(valueChanged(int)), this, SLOT(setPort(int)));
	
	connect(ui->progSlider, SIGNAL(valueChanged(int)), this, SLOT(setProg(int)));
	connect(ui->modSlider, SIGNAL(valueChanged(int)), this, SLOT(setMod(int)));
	connect(ui->volSlider, SIGNAL(valueChanged(int)), this, SLOT(setVol(int)));
	connect(ui->panSlider, SIGNAL(valueChanged(int)), this, SLOT(setPan(int)));
}

bool Dialog::start()
{
    if (!MidiShare()) {
		QMessageBox msg;
		msg.critical (0, "Error", "Can't launch msControler:\nMidiShare is not installed", QMessageBox::Abort);		
		return false;
	}
    fRefNum = MidiOpen("msControler");
	if (fRefNum > 0) {
		MidiConnect (fRefNum, 0, true);
		return true;
	}
	return false;
}

Dialog::~Dialog()
{
    writeState();
	MidiClose(fRefNum);
    delete ui;
}

//_______________________________________________________________________________
void Dialog::sendNote(int pitch, int vel, int chan, int port)
{
	MidiEvPtr e = MidiNewEv(typeNote);
	if (e) {
		Pitch(e) = pitch;
		Vel(e) = vel;
		Chan(e) = chan;
		Port(e) = port;
		Dur(e) = 200;
		MidiSendIm (fRefNum, e);
	}
}

void Dialog::sendProg (int prog, int chan, int port)
{
	MidiEvPtr e = MidiNewEv (typeProgChange);
	if (e) {
		Port(e) = port;
		Chan(e) = chan;
		Data(e)[0] = prog;
		MidiSendIm (fRefNum, e);
		sendNote (fPitch, fVel, fChan, fPort);
	}
}

void Dialog::sendCtrl (int num, int val, int chan, int port)
{
	MidiEvPtr e = MidiNewEv (typeCtrlChange);
	if (e) {
		Port(e) = port;
		Chan(e) = chan;
		Data(e)[0] = num;
		Data(e)[1] = val;
		MidiSendIm (fRefNum, e);
		sendNote (fPitch, fVel, fChan, fPort);
	}
}

//_______________________________________________________________________________
void Dialog::about()
{ 
	int version = MidiGetVersion();
	char about[512];
	sprintf (about, "msControler\nA MidiShare application\nCopyright (c) Grame 1989-2010\n\nMidiShare v.%d.%d", version/100, version%100);
	QMessageBox::about (0, "msControler", about);
}

//_______________________________________________________________________________
void Dialog::setPitch(int value)	{ sendNote (fPitch = value, fVel, fChan, fPort); }
void Dialog::setVel(int value)		{ sendNote (fPitch, fVel = value, fChan, fPort); }
void Dialog::setChan(int value)		{ sendNote (fPitch, fVel, fChan = value, fPort); }
void Dialog::setPort(int value)		{ sendNote (fPitch, fVel, fChan, fPort = value); }

void Dialog::setProg(int value)		{ sendProg (fProg = value, fChan, fPort); }
void Dialog::setMod(int value)		{ sendCtrl (1, fMod = value, fChan, fPort); }
void Dialog::setVol(int value)		{ sendCtrl (7, fVol = value, fChan, fPort); }
void Dialog::setPan(int value)		{ sendCtrl (10, fPan = value, fChan, fPort); }


//_______________________________________________________________________________
void Dialog::readState ()
{
	int x, y;
	if (ReadPos (profileName, &x, &y))
		move (x, y);
	fPort = ReadIntState (profileName, StateSectionName, portEntryName, 0);
	fChan = ReadIntState (profileName, StateSectionName, chanEntryName, 0);
	fProg = ReadIntState(profileName, StateSectionName, progChgeEntryName, 0);
	fVol  = ReadIntState (profileName, StateSectionName, volEntryName, 127);
	fMod  = ReadIntState (profileName, StateSectionName, modulationEntryName, 0);
	fPan  = ReadIntState (profileName, StateSectionName, panEntryName, 64);
	fPitch= ReadIntState (profileName, StateSectionName, pitchEntryName, 60);
	fVel  = ReadIntState (profileName, StateSectionName, velEntryName, 100);
}

//_______________________________________________________________________________
void Dialog::writeState ()
{
	WritePos  (profileName, pos().x(), pos().y());
	WriteIntState (profileName, StateSectionName, portEntryName, fPort);
	WriteIntState (profileName, StateSectionName, chanEntryName, fChan);
	WriteIntState (profileName, StateSectionName, progChgeEntryName, fProg);
	WriteIntState (profileName, StateSectionName, volEntryName, fVol);
	WriteIntState (profileName, StateSectionName, modulationEntryName, fMod);
	WriteIntState (profileName, StateSectionName, panEntryName, fPan);
	WriteIntState (profileName, StateSectionName, pitchEntryName, fPitch);
	WriteIntState (profileName, StateSectionName, velEntryName, fVel);
}


