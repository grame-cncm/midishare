/****************************************************************************
** Meta object code from reading C++ file 'dialog.h'
**
** Created: Thu Oct 14 15:54:18 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Dialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x0a,
      16,    7,    7,    7, 0x0a,
      30,    7,    7,    7, 0x0a,
      42,    7,    7,    7, 0x0a,
      55,    7,    7,    7, 0x0a,
      68,    7,    7,    7, 0x0a,
      81,    7,    7,    7, 0x0a,
      93,    7,    7,    7, 0x0a,
     105,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Dialog[] = {
    "Dialog\0\0about()\0setPitch(int)\0setVel(int)\0"
    "setChan(int)\0setPort(int)\0setProg(int)\0"
    "setMod(int)\0setVol(int)\0setPan(int)\0"
};

const QMetaObject Dialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Dialog,
      qt_meta_data_Dialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Dialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Dialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Dialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Dialog))
        return static_cast<void*>(const_cast< Dialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Dialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: about(); break;
        case 1: setPitch((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: setVel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: setChan((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: setPort((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: setProg((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: setMod((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: setVol((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: setPan((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
