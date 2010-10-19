#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
            ~Dialog();

	bool start();

public slots:
	void about();

	void setPitch(int);
	void setVel(int);
	void setChan(int);
	void setPort(int);

	void setProg(int);
	void setMod(int);
	void setVol(int);
	void setPan(int);

private:
    Ui::Dialog *ui;

	void sendNote(int pitch, int vel, int chan, int port);
	void sendProg (int prog, int chan, int port);
	void sendCtrl (int num, int val, int chan, int port);
	void readState ();
	void writeState();

    int fRefNum;
	int fPitch, fVel, fChan, fPort;
	int fProg, fMod, fVol, fPan;
};

#endif // DIALOG_H
