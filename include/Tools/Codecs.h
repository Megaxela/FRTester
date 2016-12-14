//
// Created by megaxela on 13.12.16.
//

#ifndef FRTESTER_CODECS_H
#define FRTESTER_CODECS_H

#include <QString>
#include <QByteArray>
#include <QMap>

/**
 * @brief Синглетон, управляющий преобразованием
 * текста из одной кодировки в другую с использованием
 * Qt.
 */
class Codecs
{
public:
    /**
     * @brief Деструктор.
     */
    ~Codecs();

    /**
     * @brief Метод для создания объекта класса.
     * @return Объект класса.
     */
    static Codecs& instance();

    /**
     * @brief Метод для конвертации текста из одной кодировки
     * в другую.
     * @param from Исходная кодировка.
     * @param to Конечная кодировка.
     * @param data Преобразовываемые данные.
     * @return Результат преобразования.
     */
    QByteArray convert(std::string from, std::string to, QByteArray data);

    QString convert(std::string from, QByteArray data);

private:
    /**
     * @brief Приватный конструктор.
     */
    Codecs();

    QMap<std::string, QTextCodec*> m_codecs;
};


#endif //FRTESTER_CODECS_H
