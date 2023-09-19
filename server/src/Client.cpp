#include "Client.hpp"
#include "Utils/Instruction.hpp"

Client::Client(asio::ip::udp::endpoint endpoint)
{
    this->_endpoint = endpoint;
    this->buffer = "";
}

Client::~Client()
{
}

const asio::ip::udp::endpoint &Client::getEndpoint() const
{
    return this->_endpoint;
}

void Client::pushBuffer(const std::string &data)
{
    this->buffer += data;
}

std::pair<size_t, std::string> Client::getNextInst()
{

    if (this->buffer.size() == 0)
        return std::make_pair(0, "");
    std::vector<inst_t> inst = INST;

    std::pair<size_t, std::string> out;


    for (auto i = inst.begin(); i != inst.end(); ++i) {
        if (this->buffer[0] == std::to_string(i->_inst)[0]) {       //! remove std::to_string()[0]
            if (this->buffer.size() < i->_size + 1)
                return std::make_pair(0, "");
            out.first = i->_inst;
            out.second = this->buffer.substr(1, i->_size);
            this->buffer = this->buffer.substr(i->_size + 1);
            return out;
        }
    }
    this->buffer = "";
    return std::make_pair(0, "");
}