//
// Created by megaxela on 05.04.17.
//

#ifndef FRTESTER_CONNECTION_H
#define FRTESTER_CONNECTION_H

#include <string>
#include <json.hpp>

using nlohmann::json;

class FRDriver;

/**
 * @brief Класс, описывающий соединение с
 * ККТ.
 */
class Connection
{
public:
    enum Type
    {
        Unknown,
        COM,
        TCP
    };

    /**
     * @brief Конструктор.
     */
    Connection(Type type);

    /**
     * @brief Деструктор.
     */
    virtual ~Connection();

    /**
     * @brief Метод для получения названия соединения.
     * Для разных соединений название может формироваться
     * по-разному.
     * @return Строковое представление названия соединения.
     */
    virtual std::string name() const = 0;

    /**
     * @brief Метод для выполнения подключения по
     * подключения. Разные соединения могут подключаться
     * по-разному.
     * @return Результат подключения.
     */
    virtual bool performConnection(FRDriver* driver) = 0;

    /**
     * @brief Метод для преобразования соединения в JSON.
     * @return JSON структура.
     */
    virtual json toJson() const = 0;

    /**
     * @brief Метод для получения соединения из json структуры.
     * @param j JSON структура.
     * @return Умный указатель с соединением.
     */
    static std::shared_ptr<Connection> fromJson(json j);

    /**
     * @brief Метод для получения типа соединения.
     * @return Тип соединения.
     */
    Type type() const;

private:
    Type m_type;
};


#endif //FRTESTER_CONNECTION_H
