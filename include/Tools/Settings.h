//
// Created by xelam on 30.11.2016.
//

#ifndef FRTESTER_SETTINGS_H
#define FRTESTER_SETTINGS_H

#include <QVariant>

/**
 * @brief Класс, отвечающий за хранение настроек.
 */
class Settings
{
public:

    /**
     * @brief Деструктор.
     */
    ~Settings();

    /**
     * @brief Статический метод для получения объекта
     * класса.
     * @return Объект класса с настройками.
     */
    static Settings& instance();

    /**
     * @brief Метод для получения значения.
     * @param name Название поля.
     * @param def Значение поля по-умолчанию.
     * @return Значение поля.
     */
    QString getValue(const QString &name, const QString &def) const;

    /**
     * @brief Метод для установки значения.
     * @param name Название поля.
     * @param value Значение поля.
     */
    void setValue(const QString &name, const QString &value);

    /**
     * @brief Метод для установки местоположения файла
     * с настройками.
     * @param name
     */
    void setFilename(const QString &name);

    /**
     * @brief Метод для загрузки настроек из указанного файла.
     */
    bool load();

    /**
     * @brief Метод для сохранения настроек в файл.
     */
    bool save();

private:
    /**
     * @brief Приватный конструктор, ибо Singleton.
     */
    Settings();

    QString m_filename;

    QMap<QString, QString> m_data;
};


#endif //FRTESTER_SETTINGS_H
