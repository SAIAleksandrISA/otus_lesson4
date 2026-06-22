#pragma once
#include <iostream>
#include <cstdint>
#include <type_traits>
#include <vector>
#include <list>

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

template <typename T>
typename std::enable_if<std::is_same<T, std::string>::value, void>::type
print_ip(T value)
{
    std::cout << value<< std::endl;
}

template <typename T> struct is_std_vector : std::false_type {};
template <typename T_Elem> struct is_std_vector<std::vector<T_Elem>> : std::true_type {};

template <typename T> struct is_std_list : std::false_type {};
template <typename T_Elem> struct is_std_list<std::list<T_Elem>> : std::true_type {};

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