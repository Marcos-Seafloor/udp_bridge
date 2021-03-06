#include "udp_bridge/connection.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <sstream>

namespace udp_bridge
{

Connection::Connection(std::string const &host, uint16_t port):m_host(host),m_port(port)
{
    struct addrinfo hints = {0}, *addresses;
    
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;
    
    std::string port_string = std::to_string(port);
    
    int ret = getaddrinfo(host.c_str(), port_string.c_str(), &hints, &addresses);
    if(ret != 0)
        throw std::runtime_error(gai_strerror(ret));
    
    int error {0};
    for (struct addrinfo *address = addresses; address != nullptr; address = address->ai_next)
    {
        m_socket = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
        if(m_socket == -1)
        {
            error = errno;
            continue;
        }
        
        if(connect(m_socket, address->ai_addr, address->ai_addrlen) == 0)
            break;
        
        error = errno;
        close(m_socket);
        m_socket = -1;
    }
    freeaddrinfo(addresses);
    if(m_socket == -1)
        throw std::runtime_error(strerror(error));
}
    
    
Connection::~Connection()
{
    close(m_socket);
}

void Connection::send(std::vector<uint8_t> const &data)
{
    ::send(m_socket, data.data(), data.size(), 0);
}

std::string Connection::str() const
{
    std::stringstream ret;
    ret << m_host << ":" << m_port;
    return ret.str();
}



std::shared_ptr<Connection> ConnectionManager::getConnection(std::string const &host, uint16_t port)
{
    for(auto c: m_connections)
        if(c->m_host == host && c->m_port == port)
            return c;
    m_connections.push_back(std::shared_ptr<Connection>(new Connection(host,port)));
    return m_connections.back();
}

} // namespace udp_bridge
