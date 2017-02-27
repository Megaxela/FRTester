//
// Created by megaxela on 27.02.17.
//

#include "include/Windows/Widgets/QHexLineEdit.h"


QHexLineEdit::QHexLineEdit(QWidget *parent, uint32_t maxSize) :
    QLineEdit(parent),
    m_size(0)
{
    setHexSize(maxSize);
}

void QHexLineEdit::setHexSize(uint32_t sz)
{
    m_size = sz;

    QString mask;
    for (uint32_t i = 0; i < sz; ++i)
    {
        mask += "HH ";
    }

    setInputMask(mask);
}

uint32_t QHexLineEdit::hexSize() const
{
    return m_size;
}


void QHexLineEdit::onEditingFinished()
{
    QString hexString = text().simplified();

    hexString.replace(" ", "");

    m_value = ByteArray::fromHex(hexString.toStdString());

    emit byteArrayChanged(m_value);
}

void QHexLineEdit::setByteArray(ByteArray array)
{
    m_value = array;
    displayValue();
}

void QHexLineEdit::displayValue()
{
    auto s = m_value.toHex();

    setText(QString::fromStdString(s));
}
