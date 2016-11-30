//
// Created by megaxela on 14.11.16.
//

#ifndef HGENGINE_TIME_H
#define HGENGINE_TIME_H

#include <cinttypes>
#include <cmath>
#include <functional>
#include <chrono>
#include <thread>

/**
 * @brief Namespace with functions for work with time.
 */
namespace Time
{

    typedef long long time_t;

    /**
     * @brief Get milliseconds since epoch.
     * @return Number of milliseconds.
     */
    time_t getMilliseconds();

    /**
     * @brief Get microseconds since epoch.
     * @return Number of microseconds.
     */
    time_t getMicroseconds();

    /**
     * @brief Метод для получения времени с начала эпохи
     * в формате T.
     * @tparam T Формат времени из <chrono>
     * @return Количество времени.
     */
    template<typename T>
    time_t get()
    {
        return std::chrono::duration_cast< T >(
                std::chrono::system_clock::now()
                        .time_since_epoch()
        ).count();
    }

    /**
     * @brief Метод вычисляет время исполнения функции
     * в указанных единицах.
     * @tparam Тип времени из <chrono>
     * @param function
     * @return
     */
    template<typename T>
    time_t timeFunction(std::function<void()> function)
    {
        time_t before = get<T>();
        function();
        return get<T>() - before;
    }

    template<typename T>
    void sleep(time_t value)
    {
        std::this_thread::sleep_for(
            T(value)
        );
    }
};


#endif //HGENGINE_TIME_H
