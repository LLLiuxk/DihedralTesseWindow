/****************************************************************************
** Meta object code from reading C++ file 'dihedraltessewindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../dihedraltessewindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dihedraltessewindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DihedralTesseWindow_t {
    QByteArrayData data[7];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DihedralTesseWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DihedralTesseWindow_t qt_meta_stringdata_DihedralTesseWindow = {
    {
QT_MOC_LITERAL(0, 0, 19), // "DihedralTesseWindow"
QT_MOC_LITERAL(1, 20, 7), // "changed"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 28), // "on_actionLoadInput_triggered"
QT_MOC_LITERAL(4, 58, 24), // "on_actionClear_triggered"
QT_MOC_LITERAL(5, 83, 8), // "openfile"
QT_MOC_LITERAL(6, 92, 13) // "winRectupdate"

    },
    "DihedralTesseWindow\0changed\0\0"
    "on_actionLoadInput_triggered\0"
    "on_actionClear_triggered\0openfile\0"
    "winRectupdate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DihedralTesseWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    1,   42,    2, 0x0a /* Public */,
       6,    0,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,

       0        // eod
};

void DihedralTesseWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DihedralTesseWindow *_t = static_cast<DihedralTesseWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->changed(); break;
        case 1: _t->on_actionLoadInput_triggered(); break;
        case 2: _t->on_actionClear_triggered(); break;
        case 3: _t->openfile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->winRectupdate(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DihedralTesseWindow::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DihedralTesseWindow::changed)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject DihedralTesseWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_DihedralTesseWindow.data,
      qt_meta_data_DihedralTesseWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DihedralTesseWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DihedralTesseWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DihedralTesseWindow.stringdata0))
        return static_cast<void*>(const_cast< DihedralTesseWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int DihedralTesseWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DihedralTesseWindow::changed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
