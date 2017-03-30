//
// Created by megaxela on 30.01.17.
//

#ifndef FRTESTER_QNUMBERTEXTEDIT_H
#define FRTESTER_QNUMBERTEXTEDIT_H

#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <Tools/Logger.h>
#include "QBaseNumberLineEdit.h"

/**
 * @brief Класс, описывающий текстовое поле для
 * ввода чисел.
 * @tparam T Тип вводимого числа
 */
template <typename T>
class QNumberLineEdit : public QBaseNumberLineEdit
{
public:
    /**
     * @brief Конструктор.
     * @param parent Родительский виджет.
     * @param min Минимальное значение.
     * @param max Максимальное значение.
     */
    QNumberLineEdit(QWidget* parent, T min=std::numeric_limits<T>::min(), T max=std::numeric_limits<T>::max()) :
            QBaseNumberLineEdit(parent),
            m_lastAcceptedValue(min),
            m_minimum(min),
            m_maximum(max)
    {
        connect(this,
                &QLineEdit::editingFinished,
                this,
                &QNumberLineEdit::onEditingFinished);
    }

    /**
     * @brief Метод для установки минимального значения поля.
     * @param value Минимальное значение поля.
     */
    void setMinimum(T value)
    {
        m_minimum = value;
        if (m_lastAcceptedValue < m_minimum)
        {
            m_lastAcceptedValue = m_minimum;
            setText(QString::number(m_lastAcceptedValue));
            raiseSignal();
        }
    }

    /**
     * @brief Метод для установки максимального значения поля.
     * @param value Максимальное значение поля.
     */
    void setMaximum(T value)
    {
        m_maximum = value;
        if (m_lastAcceptedValue > m_maximum)
        {
            m_lastAcceptedValue = m_maximum;
            setText(QString::number(m_lastAcceptedValue));
            raiseSignal();
        }
    }

    /**
     * @brief Метод для получения числового значения.
     * @return Возвращает последнее принятое значение.
     */
    T numberValue() const
    {
        return m_lastAcceptedValue;
    }

    /**
     * @brief Метод для устновки числового значения.
     * @param value Числовое значение.
     */
    void setNumberValue(T value)
    {
        if (value >= m_minimum && value <= m_maximum)
        {
            m_lastAcceptedValue = value;
            setText(QString::number(m_lastAcceptedValue));
            raiseSignal();
        }
        else
        {
            Error(
                    "Невозможно принять значение " +
                    std::to_string(value) +
                    " поскольку оно не удовлетворяет условию " +
                    std::to_string(m_minimum) +
                    " <= " +
                    std::to_string(value) +
                    " <= " +
                    std::to_string(m_maximum)
            );
        }
    }

private slots:
    /**
     * @brief Слот, вызываемый при окончании ввода значения в поле.
     */
    void onEditingFinished()
    {
        // Получение значения
        auto value = text();

        bool ok = false;

        T numberValue;

        if (m_minimum < 0)
        {
            numberValue = static_cast<T>(value.toULongLong(&ok));
        }
        else
        {
            numberValue = static_cast<T>(value.toLongLong(&ok));
        }

        if (!ok)
        {
            // todo: Использовать RTTI для получения названия типа.
            Error("Значение " + value.toStdString() + " не является числом.");
            return;
        }

        if (numberValue >= m_minimum && numberValue <= m_maximum)
        {
            m_lastAcceptedValue = numberValue;
            setText(QString::number(m_lastAcceptedValue));
            raiseSignal();
        }
        else
        {
            Error(
                    "Значение не удовлетворяет условию " +
                    std::to_string(m_minimum) +
                    " <= " +
                    std::to_string(numberValue) +
                    " <= " +
                    std::to_string(m_maximum)
            );
        }
    };

private:
    void raiseSignal()
    {
        if (std::is_same<T, uint8_t>::value)
        {
            emit newUInt8ValueAccepted(m_lastAcceptedValue);
        }
        else if (std::is_same<T, int8_t>::value)
        {
            emit newInt8ValueAccepted(m_lastAcceptedValue);
        }
        else if (std::is_same<T, uint16_t>::value)
        {
            emit newUInt16ValueAccepted(m_lastAcceptedValue);
        }
        else if (std::is_same<T, int16_t>::value)
        {
            emit newInt16ValueAccepted(m_lastAcceptedValue);
        }
        else if (std::is_same<T, uint32_t>::value)
        {
            emit newUInt32ValueAccepted(m_lastAcceptedValue);
        }
        else if (std::is_same<T, int32_t>::value)
        {
            emit newInt32ValueAccepted(m_lastAcceptedValue);
        }
        else if (std::is_same<T, uint64_t>::value)
        {
            emit newUInt64ValueAccepted(m_lastAcceptedValue);
        }
        else if (std::is_same<T, int64_t>::value)
        {
            emit newInt64ValueAccepted(m_lastAcceptedValue);
        }
        else
        {
            Critical("Неподдерживаемый числовой тип. Не удается вызвать сигнал.");
        }

    }

    T m_lastAcceptedValue;
    T m_minimum;
    T m_maximum;
};


#endif //FRTESTER_QNUMBERTEXTEDIT_H
