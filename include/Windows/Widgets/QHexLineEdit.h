//
// Created by megaxela on 27.02.17.
//

#ifndef FRTESTER_QHEXLINEEDIT_H
#define FRTESTER_QHEXLINEEDIT_H

#include <QLineEdit>
#include <include/Tools/ByteArray.h>

/**
 * @brief Класс, описывающий текстовое поле
 * для ввода массива байт.
 */
class QHexLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     * @param parent Родительский виджет.
     * @param maxSize Максимальный размер массива байт.
     */
    QHexLineEdit(QWidget* parent, uint32_t maxSize = 256);

    /**
     * @brief Метод для указания размера.
     * @param sz Значения размера.
     */
    void setHexSize(uint32_t sz);

    /**
     * @brief Метод для получения размера массива байт.
     * @return Размер массива байт.
     */
    uint32_t hexSize() const;

    /**
     * @brief Метод для передачи значения в виде массива байт.
     * @param array Массив байт.
     */
    void setByteArray(ByteArray array);

signals:
    void byteArrayChanged(ByteArray array);

private slots:

    void onEditingFinished();

private:
    void displayValue();

    uint32_t m_size;
    ByteArray m_value;
};


#endif //FRTESTER_QHEXLINEEDIT_H
