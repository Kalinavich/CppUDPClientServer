#pragma once

namespace Server {

    /// @brief Обертка RAII над сокетом
    class Socket {
    public:
        /// @brief Конструктор по умолчанию, дескриптор присваивается -1;
        Socket();
        /// @brief Конструктор по файловому дескриптору сокета
        /// @param descriptor файловый дескриптор
        Socket(int descriptor);
        /// @brief Конструктор перемещения. Старый сокет завершает работу.
        /// @param other перехватываемый сокет
        Socket(Socket&& other);

        Socket& operator=(const Socket& other) = delete;
        Socket(const Socket& other) = delete;
        ~Socket();

        /// @brief Оператор присваивания. Старый сокет завершает работу.
        /// @param other присваиваемый соке
        /// @return 
        Socket& operator=(Socket&& other);

        operator int() const;

        /// @brief Закрыть сокет
        void close();
        /// @brief Сбросить сокет
        /// @param descriptor 
        void reset(int descriptor = -1);
        /// @brief Завершить сокет
        /// @return Файловый дескриптор завершенного сокета
        int release();

    private:
        int descriptor = -1;
    };

    struct Pipe {
        Socket read;
        Socket write;

        Pipe();

        void close();
    };

}