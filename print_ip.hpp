/**
 * @file print_ip.hpp
 * @brief Реализация функции print_ip с использованием SFINAE.
 *
 * Этот файл содержит шаблонные функции print_ip, которые
 * выводят данные для: целочисленных, строк, std::vector и std::list.
 * Также содержит вспомогательные структуры для SFINAE.
 */
#pragma once
#include <iostream>
#include <cstdint>
#include <type_traits>
#include <vector>
#include <list>
#include <type_traits>

// --- Вспомогательные структуры для SFINAE ---

/**
 * @brief Определяет, является ли тип std::vector.
 *
 * Используется для SFINAE-проверки в шаблонных функциях.
 */
template <typename T> struct is_std_vector : std::false_type {};

/**
 * @brief Частичная специализация is_std_vector для std::vector.
 * @tparam T_Elem Тип элементов в векторе.
 * @tparam Alloc Тип аллокатора вектора.
 */
template <typename T_Elem> struct is_std_vector<std::vector<T_Elem>> : std::true_type {};

/**
 * @brief Определяет, является ли тип std::list.
 *
 * Используется для SFINAE-проверки в шаблонных функциях.
 */
template <typename T> struct is_std_list : std::false_type {};

/**
 * @brief Частичная специализация is_std_list для std::list.
 * @tparam T_Elem Тип элементов в списке.
 * @tparam Alloc Тип аллокатора списка.
 */
template <typename T_Elem> struct is_std_list<std::list<T_Elem>> : std::true_type {};

/**
 * @brief Определяет, является ли тип std::tuple.
 *
 * Используется для SFINAE-проверки в шаблонных функциях.
 */
template<typename T> struct is_same_tuple : std::false_type {};

/**
 * @brief Определяет, является ли все элементы одного типа.
 *
 * Используется для SFINAE-проверки в шаблонных функциях.
 */
template<typename T, typename... Args> struct all_same 
{
    static constexpr bool value = (std::is_same<T, Args>::value && ...);
};

/**
 * @brief Определяет, является ли все элементы одного типа.
 *
 * Используется для SFINAE-проверки в шаблонных функциях.
 */
template<typename T, typename... Args>
struct is_same_tuple<std::tuple<T, Args...>> : all_same<T, Args...> {};

// --- Шаблонные функции print_ip ---

/**
 * @brief Печатает целочисленное значение побайтово с разделителями (.).
 *
 * @details Эта функция обрабатывает различные целочисленные типы (int8_t, int16_t, int32_t, int64_t, unsigned int и т.д.).
 * Она извлекает каждый байт числа, начиная со старшего, и выводит его как беззнаковое
 * десятичное число, разделяя точки. Работает с использованием SFINAE через std::is_integral.
 *
 * @tparam T Тип целочисленного значения. Должен быть целым числом.
 * @param value Значение для печати.
 */
template <typename T>
typename std::enable_if<std::is_same<T, int8_t>::value || std::is_same<T, int16_t>::value 
    || std::is_same<T, int32_t >::value || std::is_same<T, int64_t >::value, void>::type
print_ip(T value)
{
    constexpr int num_bytes = sizeof(T);

    for (int i = num_bytes - 1; i >= 0; --i)
    {
        unsigned char byte = static_cast<unsigned char>((value >> (i * 8)) & 0xFF);
        std::cout << static_cast<int>(byte); 
        if (i > 0)
        {
            std::cout << ".";
        }
    }
    std::cout << std::endl;
}

/**
 * @brief Печатает std::string.
 *
 * @details Эта функция просто выводит содержимое строки как есть.
 * Работает с использованием SFINAE через std::is_same.
 *
 * @tparam T Тип должен быть std::string.
 * @param value Строка для печати.
 */
template <typename T>
typename std::enable_if<std::is_same<T, std::string>::value, void>::type
print_ip(T value)
{
    std::cout << value<< std::endl;
}

/**
 * @brief Печатает содержимое контейнеров std::vector или std::list.
 *
 * @details Эта функция проходит по всем элементам контейнера (vector или list),
 * выводя каждый элемент через точку. Работает с любыми типами элементов,
 * которые сами по себе могут быть напечатаны.
 * Работает с использованием SFINAE через комбинацию is_std_vector и is_std_list.
 *
 * @tparam T Тип контейнера. Должен быть std::vector или std::list.
 * @param value Контейнер для печати.
 */
template <typename T>
typename std::enable_if<is_std_vector<T>::value || is_std_list<T>::value>::type
print_ip(const T& value)
{
    bool first = true;
    for (const auto& item : value)
    {
        if (!first) std::cout << ".";
        std::cout << item;
        first = false;
    }
    std::cout << std::endl;
}

/**
 * @brief Печатает содержимое контейнеров std::tuple
 *
 * @details Эта функция проходит по всем элементам контейнера (tuple),
 * выводя каждый элемент через точку. Работает при условии что в tuple,
 * лежат одинаковые типы.
 * Работает с использованием SFINAE через комбинацию is_same_tuple.
 *
 * @tparam T Тип контейнера. Должен быть std::tuple.
 * @param value Контейнер для печати.
 */
template<typename T> typename std::enable_if<is_same_tuple<T>::value>::type
print_ip(const T& t)
{

    std::apply([](auto&&... args)
        {
            bool first = true;
            ((std::cout << (first ? "" : ".") << args , first = false ), ...);
            std::cout << std::endl;
        }
        , t);
}
