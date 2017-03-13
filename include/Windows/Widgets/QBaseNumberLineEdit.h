//
// Created by megaxela on 30.01.17.
//

#ifndef FRTESTER_QBASENUMBERLINEEDIT_H
#define FRTESTER_QBASENUMBERLINEEDIT_H

#include <QLineEdit>
#include <cstdint>

class QBaseNumberLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    QBaseNumberLineEdit(QWidget*);

    virtual ~QBaseNumberLineEdit();

signals:
    void newUInt8ValueAccepted(uint8_t value);
    void newInt8ValueAccepted(int8_t value);
    void newUInt16ValueAccepted(uint16_t value);
    void newInt16ValueAccepted(int16_t value);
    void newUInt32ValueAccepted(uint32_t value);
    void newInt32ValueAccepted(int32_t value);
    void newUInt64ValueAccepted(uint64_t value);
    void newInt64ValueAccepted(int64_t value);

};


#endif //FRTESTER_QBASENUMBERLINEEDIT_H
