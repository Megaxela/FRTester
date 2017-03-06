//
// Created by megaxela on 03.03.17.
//

#ifndef FRTESTER_ABSTRACTSFABRIC_H
#define FRTESTER_ABSTRACTSFABRIC_H

#include <string>
#include "AbstractTestAction.h"

/**
 * @brief Фабрика для получения всех фабрик.
 */
template<typename Type, typename SerializeType>
class Fabric
{
public:

    typedef SerializeType NameType;
    typedef Type BaseElementType;

    /**
     * @brief Деструктор.
     */
    ~Fabric()
    {

    }

    /**
     * @brief Метод для получения объекта синглетон класса.
     */
    static Fabric<Type, SerializeType>& instance()
    {
        static Fabric<Type, SerializeType> t;
        return t;
    }

    /**
     * @brief Метод для регистрации типов.
     * @tparam T Тип.
     * @param name Название типа.
     */
    template<typename T>
    void registerObject(const SerializeType &name)
    {
        m_actions[name] = []()
        {
            return std::make_shared<T>();
        };
    }

    std::shared_ptr<Type> create(const SerializeType& name)
    {
        auto f = m_actions.find(name);
        if (f == m_actions.end())
        {
            throw std::invalid_argument("Can't find registred type \"" + name + "\".");
        }

        return f->second();
    }

    /**
     * @brief Метод для получения всех значений для сериализации.
     * @return
     */
    std::vector<SerializeType> getNames() const
    {
        std::vector<SerializeType> type;
        for (auto& element : m_actions)
        {
            type.push_back(element.first);
        }

        return type;
    }

private:
    /**
     * @brief Конструктор.
     */
    Fabric()
    {

    }

    std::map<
            SerializeType,
            std::function<
                    std::shared_ptr<
                            Type
                    >()
            >
    > m_actions;
};


#endif //FRTESTER_ACTIONSFABRIC_H
