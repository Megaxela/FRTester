//
// Created by xelam on 30.11.2016.
//

#ifndef FRTESTER_SETTINGS_H
#define FRTESTER_SETTINGS_H

#include <map>
#include <string>

/**
 * @brief Класс, отвечающий за хранение настроек.
 */
class Settings
{
public:
    class Names
    {
    public:
        const static std::string testsPath;
        const static std::string triggersPath;
    };


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
    std::string getValue(const std::string &name, const std::string &def);

    /**
     * @brief Метод для установки значения.
     * @param name Название поля.
     * @param value Значение поля.
     */
    void setValue(const std::string &name, const std::string &value);

    /**
     * @brief Метод для установки местоположения файла
     * с настройками.
     * @param name
     */
    void setFilename(const std::string &name);

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

    std::string m_filename;

    std::map<std::string, std::string> m_data;
};


#endif //FRTESTER_SETTINGS_H
