#pragma once

#include <iostream>
#include <asio.hpp>

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
