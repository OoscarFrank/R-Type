#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <map>
#include <bitset>
#include <asio.hpp>

#include "Utils/Instruction.hpp"
#include "Utils/Scheduling.hpp"
#include "Utils/Queue.hpp"
#include "Utils/Stream.hpp"

class Client
{
private:
    asio::ip::udp::socket &_socket;
    asio::ip::udp::endpoint _endpoint;
    Stream _streamIn;
    std::mutex _resendMutex;

    size_t _lastActivity;
    std::chrono::system_clock::time_point _lastPing = std::chrono::system_clock::now();
    u_short _cmdNbr;
    u_short _lastCmdNbrRecieved;
    const std::vector<Commands> _outCommands;
    const std::vector<Commands> _inCommands;
    std::map<u_short, Stream> _sentPackets;
    bool _initialised = false;

public:
    /**
     * @brief Construct a new Client object
     * 
     * @param socket 
     * @param endpoint 
     */
    Client(asio::ip::udp::socket &socket, asio::ip::udp::endpoint endpoint);
    ~Client();
    Client(const Client &client) = delete;
    Client(Client &&client) = delete;
    Client &operator=(const Client &client) = delete;
    Client &operator=(Client &&client) = delete;

    /**
     * @brief Get the endpoint of the client
     * 
     * @return const asio::ip::udp::endpoint&
     */
    const asio::ip::udp::endpoint &getEndpoint() const;
    
    /**
     * @brief Get the Stream In object
     * 
     * @return Stream& 
     */
    Stream &getStreamIn();

    /**
     * @brief get the new instruction from the client
     * 
     * @return std::pair<size_t, Stream> 
     */
    std::pair<size_t, Stream> getNextInst();

    /**
     * @brief send a Stream to the client
     * 
     * @param message 
     */
    void send(const Stream &message);

    /**
     * @brief Resend a packet to the client
     * 
     * @param cmdNbr 
     */
    void resend(u_short cmdNbr);

    /**
     * @brief check if the client is alive
     * 
     * @return true 
     * @return false 
     */
    bool isAlive();

    /**
     * @brief Refresh the client activity
     * 
     */
    void ping();
};

#endif
