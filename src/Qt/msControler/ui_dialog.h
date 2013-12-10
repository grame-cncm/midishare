/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created: Thu Oct 14 16:17:48 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QGroupBox>
#include <QHeaderView>
#include <QSlider>
#include <QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGroupBox *groupBox_2;
    QSpinBox *spinBox_2;
    QSpinBox *spinBox;
    QSlider *chanSlider;
    QSlider *portSlider;
    QGroupBox *groupBox_3;
    QSpinBox *spinBox_3;
    QSpinBox *spinBox_4;
    QSpinBox *spinBox_5;
    QSpinBox *spinBox_6;
    QSlider *progSlider;
    QSlider *volSlider;
    QSlider *modSlider;
    QSlider *panSlider;
    QGroupBox *groupBox_4;
    QSpinBox *spinBox_7;
    QSpinBox *spinBox_8;
    QSlider *pitchSlider;
    QSlider *velSlider;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->setWindowModality(Qt::ApplicationModal);
        Dialog->resize(340, 240);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Dialog->sizePolicy().hasHeightForWidth());
        Dialog->setSizePolicy(sizePolicy);
        Dialog->setMinimumSize(QSize(340, 240));
        Dialog->setMaximumSize(QSize(340, 240));
        Dialog->setWindowOpacity(1);
        Dialog->setModal(true);
        groupBox_2 = new QGroupBox(Dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 0, 81, 231));
        QFont font;
        font.setPointSize(9);
        groupBox_2->setFont(font);
        spinBox_2 = new QSpinBox(groupBox_2);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setGeometry(QRect(40, 30, 31, 25));
        QFont font1;
        font1.setPointSize(10);
        spinBox_2->setFont(font1);
        spinBox_2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox_2->setMaximum(255);
        spinBox = new QSpinBox(groupBox_2);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(10, 30, 31, 25));
        spinBox->setFont(font1);
        spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox->setMaximum(15);
        chanSlider = new QSlider(groupBox_2);
        chanSlider->setObjectName(QString::fromUtf8("chanSlider"));
        chanSlider->setGeometry(QRect(14, 60, 24, 160));
        chanSlider->setMaximum(15);
        chanSlider->setPageStep(2);
        chanSlider->setOrientation(Qt::Vertical);
        portSlider = new QSlider(groupBox_2);
        portSlider->setObjectName(QString::fromUtf8("portSlider"));
        portSlider->setGeometry(QRect(44, 60, 24, 160));
        portSlider->setMaximum(255);
        portSlider->setPageStep(10);
        portSlider->setOrientation(Qt::Vertical);
        groupBox_3 = new QGroupBox(Dialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(100, 0, 141, 231));
        groupBox_3->setFont(font);
        spinBox_3 = new QSpinBox(groupBox_3);
        spinBox_3->setObjectName(QString::fromUtf8("spinBox_3"));
        spinBox_3->setGeometry(QRect(10, 30, 31, 25));
        spinBox_3->setFont(font1);
        spinBox_3->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox_3->setMaximum(127);
        spinBox_4 = new QSpinBox(groupBox_3);
        spinBox_4->setObjectName(QString::fromUtf8("spinBox_4"));
        spinBox_4->setGeometry(QRect(40, 30, 31, 25));
        spinBox_4->setFont(font1);
        spinBox_4->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox_4->setMaximum(127);
        spinBox_5 = new QSpinBox(groupBox_3);
        spinBox_5->setObjectName(QString::fromUtf8("spinBox_5"));
        spinBox_5->setGeometry(QRect(70, 30, 31, 25));
        spinBox_5->setFont(font1);
        spinBox_5->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox_5->setMaximum(127);
        spinBox_6 = new QSpinBox(groupBox_3);
        spinBox_6->setObjectName(QString::fromUtf8("spinBox_6"));
        spinBox_6->setGeometry(QRect(100, 30, 31, 25));
        spinBox_6->setFont(font1);
        spinBox_6->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox_6->setMaximum(127);
        progSlider = new QSlider(groupBox_3);
        progSlider->setObjectName(QString::fromUtf8("progSlider"));
        progSlider->setGeometry(QRect(14, 60, 24, 160));
        progSlider->setMaximum(127);
        progSlider->setPageStep(10);
        progSlider->setOrientation(Qt::Vertical);
        volSlider = new QSlider(groupBox_3);
        volSlider->setObjectName(QString::fromUtf8("volSlider"));
        volSlider->setGeometry(QRect(44, 60, 24, 160));
        volSlider->setMaximum(127);
        volSlider->setPageStep(10);
        volSlider->setOrientation(Qt::Vertical);
        modSlider = new QSlider(groupBox_3);
        modSlider->setObjectName(QString::fromUtf8("modSlider"));
        modSlider->setGeometry(QRect(74, 60, 24, 160));
        modSlider->setMaximum(127);
        modSlider->setPageStep(10);
        modSlider->setOrientation(Qt::Vertical);
        panSlider = new QSlider(groupBox_3);
        panSlider->setObjectName(QString::fromUtf8("panSlider"));
        panSlider->setGeometry(QRect(104, 60, 24, 160));
        panSlider->setMaximum(127);
        panSlider->setPageStep(10);
        panSlider->setOrientation(Qt::Vertical);
        groupBox_4 = new QGroupBox(Dialog);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(250, 0, 81, 231));
        groupBox_4->setFont(font);
        spinBox_7 = new QSpinBox(groupBox_4);
        spinBox_7->setObjectName(QString::fromUtf8("spinBox_7"));
        spinBox_7->setGeometry(QRect(40, 30, 31, 25));
        spinBox_7->setFont(font1);
        spinBox_7->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox_7->setMaximum(127);
        spinBox_8 = new QSpinBox(groupBox_4);
        spinBox_8->setObjectName(QString::fromUtf8("spinBox_8"));
        spinBox_8->setGeometry(QRect(10, 30, 31, 25));
        spinBox_8->setFont(font1);
        spinBox_8->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBox_8->setMaximum(127);
        pitchSlider = new QSlider(groupBox_4);
        pitchSlider->setObjectName(QString::fromUtf8("pitchSlider"));
        pitchSlider->setGeometry(QRect(14, 60, 24, 160));
        pitchSlider->setMaximum(127);
        pitchSlider->setPageStep(10);
        pitchSlider->setOrientation(Qt::Vertical);
        velSlider = new QSlider(groupBox_4);
        velSlider->setObjectName(QString::fromUtf8("velSlider"));
        velSlider->setGeometry(QRect(44, 60, 24, 160));
        velSlider->setMaximum(127);
        velSlider->setPageStep(10);
        velSlider->setOrientation(Qt::Vertical);
        QWidget::setTabOrder(spinBox_8, spinBox_7);
        QWidget::setTabOrder(spinBox_7, spinBox);
        QWidget::setTabOrder(spinBox, spinBox_2);
        QWidget::setTabOrder(spinBox_2, spinBox_3);
        QWidget::setTabOrder(spinBox_3, spinBox_4);
        QWidget::setTabOrder(spinBox_4, spinBox_5);
        QWidget::setTabOrder(spinBox_5, spinBox_6);

        retranslateUi(Dialog);
        QObject::connect(chanSlider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
        QObject::connect(spinBox, SIGNAL(valueChanged(int)), chanSlider, SLOT(setValue(int)));
        QObject::connect(portSlider, SIGNAL(valueChanged(int)), spinBox_2, SLOT(setValue(int)));
        QObject::connect(spinBox_2, SIGNAL(valueChanged(int)), portSlider, SLOT(setValue(int)));
        QObject::connect(progSlider, SIGNAL(valueChanged(int)), spinBox_3, SLOT(setValue(int)));
        QObject::connect(spinBox_3, SIGNAL(valueChanged(int)), progSlider, SLOT(setValue(int)));
        QObject::connect(spinBox_4, SIGNAL(valueChanged(int)), volSlider, SLOT(setValue(int)));
        QObject::connect(volSlider, SIGNAL(valueChanged(int)), spinBox_4, SLOT(setValue(int)));
        QObject::connect(modSlider, SIGNAL(valueChanged(int)), spinBox_5, SLOT(setValue(int)));
        QObject::connect(spinBox_5, SIGNAL(valueChanged(int)), modSlider, SLOT(setValue(int)));
        QObject::connect(spinBox_6, SIGNAL(valueChanged(int)), panSlider, SLOT(setValue(int)));
        QObject::connect(panSlider, SIGNAL(valueChanged(int)), spinBox_6, SLOT(setValue(int)));
        QObject::connect(pitchSlider, SIGNAL(valueChanged(int)), spinBox_8, SLOT(setValue(int)));
        QObject::connect(spinBox_8, SIGNAL(valueChanged(int)), pitchSlider, SLOT(setValue(int)));
        QObject::connect(spinBox_7, SIGNAL(valueChanged(int)), velSlider, SLOT(setValue(int)));
        QObject::connect(velSlider, SIGNAL(valueChanged(int)), spinBox_7, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "msControl"));
        groupBox_2->setTitle(QApplication::translate("Dialog", "Chan     Port"));
        groupBox_3->setTitle(QApplication::translate("Dialog", "Prog     Vol     Mod     Pan"));
        groupBox_4->setTitle(QApplication::translate("Dialog", "Pitch    Vel"));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
