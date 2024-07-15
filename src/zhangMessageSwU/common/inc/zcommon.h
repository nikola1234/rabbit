#pragma once

#include "base.pb.h"

#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <type_traits>
#include <functional>
#include <string>
#include <map>

namespace O_O
{

    template <int I, typename... Args>
    std::enable_if_t<std::is_same_v<std::tuple_element_t<I, std::tuple<Args...>>, bool>>
    setupTuple_(std::tuple<Args...> &tuple, const base::ParameterType &parameter)
    {
        auto basicValue = parameter.basic_value();
        std::get<I>(tuple) = basicValue.bool_value();
    }

    template <int I, typename... Args>
    std::enable_if_t<std::is_same_v<std::tuple_element_t<I, std::tuple<Args...>>, int>>
    setupTuple_(std::tuple<Args...> &tuple, const base::ParameterType &parameter)
    {
        auto basicValue = parameter.basic_value();
        std::get<I>(tuple) = basicValue.int_value();
    }

    template <int I, typename... Args>
    std::enable_if_t<std::is_same_v<std::tuple_element_t<I, std::tuple<Args...>>, std::string>>
    setupTuple_(std::tuple<Args...> &tuple, const base::ParameterType &parameter)
    {
        auto basicValue = parameter.basic_value();
        std::get<I>(tuple) = basicValue.string_value();
    }

    template <int I, typename... Args>
    std::enable_if_t<std::is_same_v<std::tuple_element_t<I, std::tuple<Args...>>, std::vector<bool>>>
    setupTuple_(std::tuple<Args...> &tuple, const base::ParameterType &parameter)
    {
        auto &v = std::get<I>(tuple);
        auto arrayValue = parameter.array_value();
        v.resize(arrayValue.value_size());
        for (auto i = 0; i < arrayValue.value_size(); i++)
        {
            v[i] = arrayValue.value(i).bool_value();
        }
    }

    template <int I, typename... Args>
    std::enable_if_t<std::is_same_v<std::tuple_element_t<I, std::tuple<Args...>>, std::vector<int>>>
    setupTuple_(std::tuple<Args...> &tuple, const base::ParameterType &parameter)
    {
        auto &v = std::get<I>(tuple);
        auto arrayValue = parameter.array_value();
        v.resize(arrayValue.value_size());
        for (auto i = 0; i < arrayValue.value_size(); i++)
        {
            v[i] = arrayValue.value(i).int_value();
        }
    }

    template <int I, typename... Args>
    std::enable_if_t<std::is_same_v<std::tuple_element_t<I, std::tuple<Args...>>, std::vector<std::string>>>
    setupTuple_(std::tuple<Args...> &tuple, const base::ParameterType &parameter)
    {
        auto &v = std::get<I>(tuple);
        auto arrayValue = parameter.array_value();
        v.resize(arrayValue.value_size());
        for (auto i = 0; i < arrayValue.value_size(); i++)
        {
            v[i] = arrayValue.value(i).string_value();
        }
    }

    class TcpMessagehandler
    {
    public:
        static void sendWithLength(const int fd, const std::string &data)
        {
            uint32_t length = data.size();
            send(fd, (char *)(&length), sizeof(uint32_t), 0);
            send(fd, data.data(), data.size(), 0);
        }

        void handleSocketData(const std::string &message, const int fd)
        {
            auto &info = m_info[fd];
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
                        m_onMessageCallback(info.buffer.substr(0, info.length), fd);
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

        void handleDisconnect(const int fd)
        {
            m_info.erase(fd);
        }

        std::function<void(std::string, int)> m_onMessageCallback;

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
        std::map<int, Info> m_info;
    };

    class RpcMessageHandler : public TcpMessagehandler
    {
    public:
        RpcMessageHandler()
        {
            TcpMessagehandler::m_onMessageCallback = std::bind(&RpcMessageHandler::handleMessage, this, std::placeholders::_1, std::placeholders::_2);
        }

        void handleMessage(const std::string &message, const int fd)
        {
            base::Request request;
            request.ParseFromString(message);
            auto callback = m_nameCallbackMap[request.name()];
            callback(request, fd);
        }

        template <int I = 0, typename... Args>
        typename std::enable_if_t<I == std::tuple_size_v<std::tuple<Args...>>>
        setupTuple(std::tuple<Args...> &tuple, base::Request &request)
        {
        }

        template <int I = 0, typename... Args>
        typename std::enable_if_t<I != std::tuple_size_v<std::tuple<Args...>>>
        setupTuple(std::tuple<Args...> &tuple, base::Request &request)
        {
            auto parameter = request.parameter(I);
            setupTuple_<I>(tuple, parameter);
            setupTuple<I + 1, Args...>(tuple, request);
        }

        template <typename ReturnType, typename CallbackType, typename tupleType>
        std::enable_if_t<std::is_same_v<ReturnType, void>, std::string>
        applyAndGetReturnMessage(CallbackType callback, tupleType parameterTuple)
        {
            std::apply(callback, parameterTuple);

            base::Confirm confirm;
            base::ParameterType *resultValue = new base::ParameterType;
            confirm.set_allocated_result(resultValue);

            std::string str;
            confirm.SerializeToString(&str);
            return str;
        }

        template <typename ReturnType, typename CallbackType, typename tupleType>
        std::enable_if_t<std::is_same_v<ReturnType, bool>, std::string>
        applyAndGetReturnMessage(CallbackType callback, tupleType parameterTuple)
        {
            bool result = std::apply(callback, parameterTuple);
            base::Confirm confirm;

            base::ParameterType *resultValue = new base::ParameterType;
            base::BasicValueType *basicValue = new base::BasicValueType;
            basicValue->set_bool_value(result);
            resultValue->set_allocated_basic_value(basicValue);
            confirm.set_allocated_result(resultValue);

            std::string str;
            confirm.SerializeToString(&str);
            return str;
        }

        template <typename ReturnType, typename CallbackType, typename tupleType>
        std::enable_if_t<std::is_same_v<ReturnType, int>, std::string>
        applyAndGetReturnMessage(CallbackType callback, tupleType parameterTuple)
        {
            int result = std::apply(callback, parameterTuple);
            base::Confirm confirm;

            base::ParameterType *resultValue = new base::ParameterType;
            base::BasicValueType *basicValue = new base::BasicValueType;
            basicValue->set_int_value(result);
            resultValue->set_allocated_basic_value(basicValue);
            confirm.set_allocated_result(resultValue);

            std::string str;
            confirm.SerializeToString(&str);
            return str;
        }

        template <typename ReturnType, typename CallbackType, typename tupleType>
        std::enable_if_t<std::is_same_v<ReturnType, std::string>, std::string>
        applyAndGetReturnMessage(CallbackType callback, tupleType parameterTuple)
        {
            std::string result = std::apply(callback, parameterTuple);
            base::Confirm confirm;

            base::ParameterType *resultValue = new base::ParameterType;
            base::BasicValueType *basicValue = new base::BasicValueType;
            basicValue->set_string_value(result);
            resultValue->set_allocated_basic_value(basicValue);
            confirm.set_allocated_result(resultValue);

            std::string str;
            confirm.SerializeToString(&str);
            return str;
        }

        template <typename ReturnType, typename CallbackType, typename tupleType>
        std::enable_if_t<std::is_same_v<ReturnType, std::vector<std::string>>, std::string>
        applyAndGetReturnMessage(CallbackType callback, tupleType parameterTuple)
        {
            std::vector<std::string> result = std::apply(callback, parameterTuple);
            base::Confirm confirm;

            base::ParameterType *resultValue = new base::ParameterType;
            base::ArrayValueType *arrayValue = new base::ArrayValueType;
            for (auto & s : result)
            {
                auto value = arrayValue->add_value();
                value->set_string_value(s);
            }
            resultValue->set_allocated_array_value(arrayValue);
            confirm.set_allocated_result(resultValue);

            std::string str;
            confirm.SerializeToString(&str);
            return str;
        }

        template <typename ReturnType, typename CallbackType, typename tupleType>
        std::enable_if_t<std::is_same_v<ReturnType, std::vector<bool>>, std::string>
        applyAndGetReturnMessage(CallbackType callback, tupleType parameterTuple)
        {
            std::vector<bool> result = std::apply(callback, parameterTuple);
            base::Confirm confirm;

            base::ParameterType *resultValue = new base::ParameterType;
            base::ArrayValueType *arrayValue = new base::ArrayValueType;
            for (auto & s : result)
            {
                auto value = arrayValue->add_value();
                value->set_bool_value(s);
            }
            resultValue->set_allocated_array_value(arrayValue);
            confirm.set_allocated_result(resultValue);

            std::string str;
            confirm.SerializeToString(&str);
            return str;
        }

        template <typename ReturnType, typename CallbackType, typename tupleType>
        std::enable_if_t<std::is_same_v<ReturnType, std::vector<int>>, std::string>
        applyAndGetReturnMessage(CallbackType callback, tupleType parameterTuple)
        {
            std::vector<int> result = std::apply(callback, parameterTuple);
            base::Confirm confirm;

            base::ParameterType *resultValue = new base::ParameterType;
            base::ArrayValueType *arrayValue = new base::ArrayValueType;
            for (auto & s : result)
            {
                auto value = arrayValue->add_value();
                value->set_int_value(s);
            }
            resultValue->set_allocated_array_value(arrayValue);
            confirm.set_allocated_result(resultValue);

            std::string str;
            confirm.SerializeToString(&str);
            return str;
        }

        template <typename ReturnType, typename... Args>
        void registerCallback(const std::string &name, std::function<ReturnType(Args...)> callback)
        {
            auto newCallback = [this, callback](base::Request request, int fd)
            {
                std::tuple<std::decay_t<Args>...> parameterTuple;
                setupTuple<0, std::decay_t<Args>...>(parameterTuple, request);
                auto resultData = applyAndGetReturnMessage<ReturnType, decltype(callback), decltype(parameterTuple)>(callback, parameterTuple);
                sendWithLength(fd, resultData);
            };
            m_nameCallbackMap[name] = newCallback;
        }

    private:
        std::map<std::string, std::function<void(base::Request, int)>> m_nameCallbackMap;
    };

    namespace
    {
        constexpr int MAX_EVENTS = 10;
        constexpr int BUFFER_SIZE = 4096;
    }

    class TcpServer
    {
    public:
        TcpServer(int port, std::function<void(const std::string &data, int)> func, std::function<void(int)> disconnectedFunc)
        {
            m_procFunc = func;
            m_disconnectedFunc = disconnectedFunc;

            m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

            int reuse = 1;
            setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

            sockaddr_in serverAddress{};
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = INADDR_ANY;
            serverAddress.sin_port = htons(port);
            bind(m_serverSocket, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress));

            listen(m_serverSocket, SOMAXCONN);

            m_epollFd = epoll_create1(0);

            epoll_event event{};
            event.events = EPOLLIN;
            event.data.fd = m_serverSocket;
            epoll_ctl(m_epollFd, EPOLL_CTL_ADD, m_serverSocket, &event);
        }

        ~TcpServer()
        {
            close(m_serverSocket);
            close(m_epollFd);
        }

        void wait()
        {
            m_numEvents = epoll_wait(m_epollFd, m_events, MAX_EVENTS, -1);
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
                    epoll_ctl(m_epollFd, EPOLL_CTL_ADD, clientSocket, &event);

                    std::cout << "New client connected" << std::endl;
                }
                else
                {
                    int bytesRead = read(m_events[i].data.fd, buffer, sizeof(buffer) - 1);
                    if (bytesRead <= 0)
                    {
                        epoll_ctl(m_epollFd, EPOLL_CTL_DEL, m_events[i].data.fd, nullptr);
                        close(m_events[i].data.fd);
                        std::cout << "Client disconnected" << std::endl;
                        m_socketFdAddressMap.erase(m_events[i].data.fd);
                        m_disconnectedFunc(m_events[i].data.fd);
                    }
                    else
                    {
                        std::string newString;
                        newString.resize(bytesRead);
                        std::copy_n(std::begin(buffer), bytesRead, newString.begin());
                        m_procFunc(newString, m_events[i].data.fd);
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
        int m_epollFd = 0;
        int m_numEvents = 0;
        std::function<void(const std::string &data, int)> m_procFunc;
        std::function<void(int)> m_disconnectedFunc;
        std::map<int, sockaddr_in> m_socketFdAddressMap;
    };

    // Client
    template <typename T>
    void setupParameter_(base::Request &request, T value)
    {
        assert(false && "1");
    }

    template <>
    void setupParameter_<bool>(base::Request &request, bool value)
    {
        auto parameter = request.add_parameter();
        auto basic_value = parameter->mutable_basic_value();
        basic_value->set_bool_value(value);
    }

    template <>
    void setupParameter_<int32_t>(base::Request &request, int32_t value)
    {
        auto parameter = request.add_parameter();
        auto basic_value = parameter->mutable_basic_value();
        basic_value->set_int_value(value);
    }

    template <>
    void setupParameter_<std::string>(base::Request &request, std::string value)
    {
        auto parameter = request.add_parameter();
        auto basic_value = parameter->mutable_basic_value();
        basic_value->set_string_value(value);
    }

    template <>
    void setupParameter_<std::vector<bool>>(base::Request &request, std::vector<bool> value)
    {
        auto parameter = request.add_parameter();
        auto array_value = parameter->mutable_array_value();
        for (auto i : value)
        {
            auto v = array_value->add_value();
            v->set_bool_value(i);
        }
    }

    template <>
    void setupParameter_<std::vector<int>>(base::Request &request, std::vector<int> value)
    {
        auto parameter = request.add_parameter();
        auto array_value = parameter->mutable_array_value();
        for (auto i : value)
        {
            auto v = array_value->add_value();
            v->set_int_value(i);
        }
    }

    template <>
    void setupParameter_<std::vector<std::string>>(base::Request &request, std::vector<std::string> value)
    {
        auto parameter = request.add_parameter();
        auto array_value = parameter->mutable_array_value();
        for (auto i : value)
        {
            auto v = array_value->add_value();
            v->set_string_value(i);
        }
    }

    template <int I = 0, typename... Args>
    typename std::enable_if_t<I == std::tuple_size_v<std::tuple<Args...>>>
    setupParameter(base::Request &request, std::tuple<Args...> tuple)
    {
    }

    template <int I = 0, typename... Args>
    typename std::enable_if_t<I != std::tuple_size_v<std::tuple<Args...>>>
    setupParameter(base::Request &request, std::tuple<Args...> tuple)
    {
        setupParameter_(request, std::get<I>(tuple));
        setupParameter<I + 1>(request, tuple);
    }

    template <typename... Args>
    base::Request rpcCall(std::string name, Args... args)
    {
        base::Request request;
        request.set_name(name);
        auto tuple = std::make_tuple(args...);
        setupParameter<0, Args...>(request, tuple);
        return request;
    }

    class RpcClient
    {
    public:
        RpcClient(const std::string &address, const uint16_t port) : m_address(address), m_port(port)
        {
        }
        ~RpcClient()
        {
            if (m_socket != 0)
            {
                close(m_socket);
            }
        }

        bool start()
        {
            assert(m_socket == 0);
            m_socket = socket(AF_INET, SOCK_STREAM, 0);

            sockaddr_in serverAddress{};
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_port = htons(m_port);
            if (inet_pton(AF_INET, m_address.data(), &(serverAddress.sin_addr)) <= 0)
            {
                return false;
            }

            if (connect(m_socket, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress)) < 0)
            {
                return false;
            }
            return true;
        }

        void stop()
        {
            close(m_socket);
        }

        template <typename T, typename... Args>
        T rpcCall(std::string name, Args... args)
        {
            T rt;

            base::Request request;
            request.set_name(name);
            auto tuple = std::make_tuple(args...);
            setupParameter<0, Args...>(request, tuple);

            std::string msg;
            request.SerializeToString(&msg);
            TcpMessagehandler::sendWithLength(m_socket, msg);

            bool received = false;
            TcpMessagehandler tcpMessagehandler;
            tcpMessagehandler.m_onMessageCallback = [&received, &rt](std::string rcvdData, int f)
            {
                received = true;

                base::Confirm confirm;
                confirm.ParseFromString(rcvdData);
                auto result = confirm.result();

                std::tuple<T> tuple_;
                setupTuple_<0>(tuple_, result);
                rt = std::get<0>(tuple_);
            };

            while (!received)
            {
                char recvBuffer[4096];
                auto recvDataSize = read(m_socket, recvBuffer, 4096);
                std::string newString;
                newString.resize(recvDataSize);
                std::copy_n(std::begin(recvBuffer), recvDataSize, newString.begin());
                tcpMessagehandler.handleSocketData(newString, m_socket);
            }
            return rt;
        }

    private:
        std::string m_address;
        uint16_t m_port;
        int m_socket = 0;
    };
}