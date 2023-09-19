#pragma once

#include <thread>
#include "../Utils/Queue.hpp"
#include "Packet.hpp"

class Writer {
    public:
        Writer(asio::ip::udp::socket &socket, Queue<Packet> &queue);
        ~Writer();

    protected:
    private:
        void run();
        std::thread _thread;
        asio::ip::udp::socket &_socket;
        Queue<Packet> &_queue;
};
