#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>
#include <utility>

class GameException : public std::exception
{
protected:
    std::string message;

public:
    explicit GameException(std::string message) : message(std::move(message)) {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }
};

class InsufficientFundsException : public GameException
{
public:
    InsufficientFundsException(int cost, int available) : GameException("Fonduri insuficiente: cost = " + std::to_string(cost) + " | buget = " + std::to_string(available)) {}
};

class InvalidBuildingException : public GameException
{
public:
    explicit InvalidBuildingException(const std::string& reason) : GameException("Cladire invalida: " + reason) {}
};

class InvalidCommandException : public GameException
{
public:
    explicit InvalidCommandException(const std::string& reason) : GameException("Comanda invalida: " + reason) {}
};

class InvalidGameConfigurationException : public GameException
{
public:
    explicit InvalidGameConfigurationException(const std::string& reason) : GameException("Configurare joc invalida: " + reason) {}
};

#endif