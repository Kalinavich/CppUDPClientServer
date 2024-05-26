#pragma once

#include <optional>
#include <string>
#include <stdint.h>

namespace Tools
{
    class Address {
    public:
        Address() = delete;
        /// @brief Коструктор адреса
        /// @param ip ip
        /// @param port порт
        Address(uint32_t ip, uint16_t port);
        

        /// @brief Преобразовать ip в виде текста в байтовую последовательность шириной 32
        /// @param str строка
        /// @return ip адрес
        static std::optional<uint32_t> parseIPString(const std::string& str);

        /// @brief Получить адрес из строки
        /// @param str адрес
        /// @return адрес
        static std::optional<Address> getAddressFromString(const std::string& str);

        /// @brief Получить порт адреса
        /// @return порт
        uint16_t GetPort();

        /// @brief Получить ip адреса
        /// @return 
        uint32_t GetIp();

    private:
        uint32_t ip;
        uint16_t port;
    };
}