cmake_minimum_required(VERSION 3.16.3)
project(cpp_InternetRelayChat)

set(CMAKE_CXX_STANDARD 98)

add_executable(ft_irca

        src/Socket.cpp
        src/Socket.hpp

        src/Server.cpp
        src/Server.hpp

        src/Client.cpp
        src/Client.hpp


        src/commandPath/Command.cpp
        src/commandPath/Command.hpp

        src/commandPath/Invoker.cpp
        src/commandPath/Invoker.hpp

        src/main.cpp

        )
