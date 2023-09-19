#pragma once

#include <iostream>
#include <asio.hpp>
#include <thread>
#include "../Utils/Queue.hpp"

class Writer {
    public:
        class Packet {
            public:
                Packet(const asio::ip::udp::endpoint &endpoint, const std::string &message);

                const asio::ip::udp::endpoint &endpoint() const;
                const std::string &message() const;
                void setMessage(const std::string &message);

            protected:
            private:
                const asio::ip::udp::endpoint &_endpoint;
                std::string _message;
        };

        Writer(asio::ip::udp::socket &socket, Queue<Writer::Packet> &queue);
        ~Writer();

    protected:
    private:
        void run();
        std::thread _thread;
        asio::ip::udp::socket &_socket;
        Queue<Writer::Packet> &_queue;
};
