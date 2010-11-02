/****************************************************************************
** Meta object code from reading C++ file 'browser.h'
**
** Created: Wed Feb 20 20:20:52 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../browser.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'browser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_Browser[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,
      23,   18,    8,    8, 0x0a,
      55,   45,    8,    8, 0x0a,
      84,    8,    8,    8, 0x0a,
     102,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Browser[] = {
    "Browser\0\0finish()\0link\0activateLink(QString)\0"
    "link,menu\0popupForLink(QString,QMenu*)\0"
    "startWebBrowser()\0getMails()\0"
};

const QMetaObject Browser::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Browser,
      qt_meta_data_Browser, 0 }
};

const QMetaObject *Browser::metaObject() const
{
    return &staticMetaObject;
}

void *Browser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Browser))
	return static_cast<void*>(const_cast< Browser*>(this));
    return QWidget::qt_metacast(_clname);
}

int Browser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: finish(); break;
        case 1: activateLink((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: popupForLink((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QMenu*(*)>(_a[2]))); break;
        case 3: startWebBrowser(); break;
        case 4: getMails(); break;
        }
        _id -= 5;
    }
    return _id;
}
