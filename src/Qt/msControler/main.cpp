#include <QtGui/QApplication>
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Dialog w;
    if (w.start()) {
		w.show();
		return a.exec();
	}
	else return -1;
}
