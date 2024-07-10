#include <iostream>
#include <functional>
#include <unordered_map>
#include <map>
#include <type_traits>
#include <unistd.h>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "out/base.pb.h"

namespace
{
    constexpr int MAX_EVENTS = 10;
    constexpr int BUFFER_SIZE = 4096;
}

template<typename T, int I, typename... Args>
void setupTuple_(std::tuple<Args...> &tuple, base::Request request, T)
{
    assert(false && "111");
}

template<int I, typename... Args>
void setupTuple_(std::tuple<Args...> &tmutable_basic_valueuple, base::Request request, uint32_t value)
{
    auto parameter = request.parameter(I);
    auto basicValue = parameter.mutable_basic_value();
    std::get<I>(tuple) = basicValue->int_value();
}

template<int I, typename... Args>
void setupTuple_(std::tuple<Args...> &tuple, base::Request request, std::string value)
{
    auto parameter = request.parameter(I);
    auto basicValue = parameter.mutable_basic_value();
    std::get<I>(tuple) = basicValue->string_value();
}

template <int I = 0, typename... Args>
typename std::enable_if_t<I == std::tuple_size_v<std::tuple<Args...>>>
setupTuple(std::tuple<Args...> &tuple, base::Request request)
{
}

template <int I = 0, typename... Args>
typename std::enable_if_t<I != std::tuple_size_v<std::tuple<Args...>>>
setupTuple(std::tuple<Args...> &tuple, base::Request request)
{
    auto parameter = request.parameter(I);
    if(parameter.has_basic_value())
    {
        using CurrentTy = decltype(std::get<I>(tuple));
        setupTuple_<I>(tuple, request, std::get<I>(tuple));
    }
    else{
        assert(false&&"2222");
    }

    setupTuple<I + 1, Args...>(tuple, request);
}

class TcpServer
{
public:
    TcpServer(uint32_t port, std::function<void(const std::string &data, uint32_t)> func)
    {
        m_procFunc = func;

        m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        int reuse = 1;
        setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

        sockaddr_in serverAddress{};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);
        bind(m_serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress));

        listen(m_serverSocket, SOMAXCONN);

        epollFd = epoll_create1(0);

        epoll_event event{};
        event.events = EPOLLIN;
        event.data.fd = m_serverSocket;
        epoll_ctl(epollFd, EPOLL_CTL_ADD, m_serverSocket, &event);
    }

    ~TcpServer()
    {
        close(m_serverSocket);
        close(epollFd);
    }

    void wait()
    {
        m_numEvents = epoll_wait(epollFd, m_events, MAX_EVENTS, -1);
    }

    void step()
    {
        for (int i = 0; i < m_numEvents; i++)
        {
            if (m_events[i].data.fd == m_serverSocket)
            {
                sockaddr_in clientAddress{};
                socklen_t clientAddressLength = sizeof(clientAddress);
                int clientSocket = accept(m_serverSocket, reinterpret_cast<struct sockaddr *>(&clientAddress),
                                          &clientAddressLength);
                m_socketFdAddressMap[clientSocket] = clientAddress;

                epoll_event event{};
                event.events = EPOLLIN;
                event.data.fd = clientSocket;
                epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event);

                std::cout << "New client connected" << std::endl;
            }
            else
            {
                int bytesRead = read(m_events[i].data.fd, buffer, sizeof(buffer) - 1);
                if (bytesRead <= 0)
                {
                    epoll_ctl(epollFd, EPOLL_CTL_DEL, m_events[i].data.fd, nullptr);
                    close(m_events[i].data.fd);
                    std::cout << "Client disconnected" << std::endl;
                    m_socketFdAddressMap.erase(m_events[i].data.fd);
                }
                else
                {
                    std::string newString;
                    newString.resize(bytesRead);
                    std::copy_n(std::begin(buffer), bytesRead, newString.begin());
                    m_procFunc(newString, m_socketFdAddressMap[m_events[i].data.fd].sin_addr.s_addr);
                }
            }
        }
    }

    void run()
    {
        while (true)
        {
            wait();
            step();
        }
    }

private:
    char buffer[BUFFER_SIZE];
    epoll_event m_events[MAX_EVENTS];
    int m_serverSocket = 0;
    int epollFd = 0;
    int m_numEvents = 0;
    std::function<void(const std::string &data, uint32_t)> m_procFunc;
    std::map<uint32_t, sockaddr_in> m_socketFdAddressMap;
};

class TcpMessageHandler
{
public:
    void handleSocketData(const std::string &message, const uint32_t address)
    {
        std::cout << "TcpMessageHandler::handleSocketData " << address << " " << message.size() << std::endl;
        auto &info = m_info[address];
        info.buffer += message;
        while (true)
        {
            if (info.state == ParsingStateE::WAITING_FOR_LENGTH)
            {
                if (info.buffer.size() >= sizeof(uint32_t))
                {
                    info.length = (*(uint32_t *)(info.buffer.data()));
                    info.state = ParsingStateE::WAITING_FOR_DATA;
                    info.buffer = info.buffer.substr(4);
                    std::cout << "Get length success: " << info.length << std::endl;
                }
                else
                {
                    info.buffer = info.buffer;
                    break;
                }
            }
            else if (info.state == ParsingStateE::WAITING_FOR_DATA)
            {
                if (info.buffer.size() >= info.length)
                {
                    info.state = ParsingStateE::WAITING_FOR_LENGTH;
                    handleMessage(info.buffer.substr(0, info.length), address);
                    info.buffer = info.buffer.substr(info.length);
                    info.length = 0;
                }
                else
                {
                    info.buffer = info.buffer;
                    break;
                }
            }
        }
    }

    void handleDisconnect(const uint32_t address)
    {
        m_info.erase(address);
    }

    void handleMessage(std::string message, const uint32_t address)
    {
        base::Request request;
        request.ParseFromString(message);
        auto callback = m_nameCallbackMap[request.name()];
        callback(request);
    }

    template <typename... Args>
    void registerCallback(const std::string &name, std::function<void(Args...)> callback)
    {
        auto newCallback = [callback](base::Request request)
        {
            std::tuple<std::decay_t<Args>...> parameterTuple;
            setupTuple<0, std::decay_t<Args>...>(parameterTuple, request);
            std::apply(callback, parameterTuple);
        };
        m_nameCallbackMap[name] = newCallback;
    }

private:
    enum class ParsingStateE
    {
        WAITING_FOR_LENGTH,
        WAITING_FOR_DATA
    };
    struct Info
    {
        std::string buffer;
        ParsingStateE state = ParsingStateE::WAITING_FOR_LENGTH;
        uint32_t length = 0;
    };
    std::map<uint32_t, Info> m_info;
    std::map<std::string, std::function<void(base::Request)>> m_nameCallbackMap;
};

int main()
{
    TcpMessageHandler handler;
    std::function<void(std::string ,int )> callback = [](std::string a,int b)
    {
        std::cout << "First: " << a << " Second: " << b << std::endl;
    };
    handler.registerCallback("AAA", callback);
    TcpServer server(8080, std::bind(&TcpMessageHandler::handleSocketData, &handler, std::placeholders::_1, std::placeholders::_2));
    server.run();

    return 0;
}