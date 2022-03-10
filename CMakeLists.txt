cmake_minimum_required(VERSION 3.21)
project(cpp_InternetRelayChat)

set(CMAKE_CXX_STANDARD 98)

add_executable(ft_irca

        src/Socket.cpp
        src/Socket.hpp

        src/Server.cpp
        src/Server.hpp

        src/Client.cpp
        src/Client.hpp


        src/commandPath/ACommand.cpp
        src/commandPath/ACommand.hpp

        src/main.cpp

        )
