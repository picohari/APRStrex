/****************************************************************************
** Meta object code from reading C++ file 'emulkbd.h'
**
** Created: Sun Nov 10 18:49:08 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "emulkbd.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'emulkbd.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EmulKbd[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_EmulKbd[] = {
    "EmulKbd\0"
};

void EmulKbd::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData EmulKbd::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EmulKbd::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_EmulKbd,
      qt_meta_data_EmulKbd, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EmulKbd::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EmulKbd::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EmulKbd::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EmulKbd))
        return static_cast<void*>(const_cast< EmulKbd*>(this));
    return QFrame::qt_metacast(_clname);
}

int EmulKbd::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_EmulKey[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x09,
      22,    8,    8,    8, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_EmulKey[] = {
    "EmulKey\0\0keyPressed()\0keyReleased()\0"
};

void EmulKey::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EmulKey *_t = static_cast<EmulKey *>(_o);
        switch (_id) {
        case 0: _t->keyPressed(); break;
        case 1: _t->keyReleased(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData EmulKey::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EmulKey::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_EmulKey,
      qt_meta_data_EmulKey, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EmulKey::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EmulKey::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EmulKey::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EmulKey))
        return static_cast<void*>(const_cast< EmulKey*>(this));
    return QPushButton::qt_metacast(_clname);
}

int EmulKey::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
