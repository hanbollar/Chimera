/****************************************************************************
** Meta object code from reading C++ file 'mygl.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../PoissonSampler/src/mygl.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mygl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyGL_t {
    QByteArrayData data[13];
    char stringdata0[208];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyGL_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyGL_t qt_meta_stringdata_MyGL = {
    {
QT_MOC_LITERAL(0, 0, 4), // "MyGL"
QT_MOC_LITERAL(1, 5, 18), // "sig_ResizeToCamera"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 14), // "sig_DisableGUI"
QT_MOC_LITERAL(4, 40, 11), // "timerUpdate"
QT_MOC_LITERAL(5, 52, 19), // "slot_poissonClicked"
QT_MOC_LITERAL(6, 72, 19), // "slot_loadPoissonObj"
QT_MOC_LITERAL(7, 92, 13), // "slot_viewPBVH"
QT_MOC_LITERAL(8, 106, 1), // "b"
QT_MOC_LITERAL(9, 108, 12), // "slot_viewOBJ"
QT_MOC_LITERAL(10, 121, 32), // "slot_calculateVolumeOfPoissonObj"
QT_MOC_LITERAL(11, 154, 26), // "slot_calculateCenterOfMass"
QT_MOC_LITERAL(12, 181, 26) // "slot_calculateVertexOffset"

    },
    "MyGL\0sig_ResizeToCamera\0\0sig_DisableGUI\0"
    "timerUpdate\0slot_poissonClicked\0"
    "slot_loadPoissonObj\0slot_viewPBVH\0b\0"
    "slot_viewOBJ\0slot_calculateVolumeOfPoissonObj\0"
    "slot_calculateCenterOfMass\0"
    "slot_calculateVertexOffset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyGL[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   64,    2, 0x06 /* Public */,
       3,    1,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   72,    2, 0x08 /* Private */,
       5,    0,   73,    2, 0x0a /* Public */,
       6,    0,   74,    2, 0x0a /* Public */,
       7,    1,   75,    2, 0x0a /* Public */,
       9,    1,   78,    2, 0x0a /* Public */,
      10,    0,   81,    2, 0x0a /* Public */,
      11,    0,   82,    2, 0x0a /* Public */,
      12,    0,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MyGL::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyGL *_t = static_cast<MyGL *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_ResizeToCamera((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->sig_DisableGUI((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->timerUpdate(); break;
        case 3: _t->slot_poissonClicked(); break;
        case 4: _t->slot_loadPoissonObj(); break;
        case 5: _t->slot_viewPBVH((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->slot_viewOBJ((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->slot_calculateVolumeOfPoissonObj(); break;
        case 8: _t->slot_calculateCenterOfMass(); break;
        case 9: _t->slot_calculateVertexOffset(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MyGL::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyGL::sig_ResizeToCamera)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (MyGL::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyGL::sig_DisableGUI)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject MyGL::staticMetaObject = {
    { &GLWidget277::staticMetaObject, qt_meta_stringdata_MyGL.data,
      qt_meta_data_MyGL,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MyGL::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyGL::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyGL.stringdata0))
        return static_cast<void*>(const_cast< MyGL*>(this));
    return GLWidget277::qt_metacast(_clname);
}

int MyGL::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GLWidget277::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void MyGL::sig_ResizeToCamera(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MyGL::sig_DisableGUI(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
