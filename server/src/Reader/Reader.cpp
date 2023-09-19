#include "Reader.hpp"



Reader::Reader(asio::ip::udp::socket &socket, std::vector<std::unique_ptr<Client>> &clients): _socket(socket), _clients(clients)
{
    _thread = std::thread(&Reader::Clock, this);
}

Reader::~Reader()
{
}

void Reader::Clock()
{
    asio::ip::udp::endpoint sender;
    bool pass = false;
    enum { max_length = 1024 };
    char data[max_length];

    while (true) {
        pass = false;
        std::error_code ec;
        std::size_t length = this->_socket.receive_from(asio::buffer(data, max_length), sender, 0, ec);

        for (auto i = _clients.begin(); i != _clients.end(); i++) {
            if ((*i)->GetEndpoint() == sender) {
                std::cout << "client already exist" << std::endl;
                pass = true;
            }
        }

        if (!pass)
            _clients.push_back(std::make_unique<Client>(sender));
        if (ec) {
            std::cerr << "Error during receive: " << ec.message() << std::endl;
            continue;
        }
    }
}
