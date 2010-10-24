#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__
/**************************************************************************************
 *
 * Ricmar Technology GmbH   -   2010
 *
 * File example.h , 24.10.2010, created by muma
 *
 **************************************************************************************/

// Qt Includes

// Std Includes

// Linux Includes

// Windows Includes

// Library Includes

// Application Includes

// Defines

/**************************************************************************************/

 #include <QObject>

 class Counter : public QObject
 {
     Q_OBJECT

 public:
     Counter() { m_value = 0; }

     int value() const { return m_value; }

     int m_value;


 public slots:
     void setValue(int value);

 signals:
     void valueChanged(int newValue);

 private:

 };

#endif //__EXAMPLE_H__
