#ifndef TOWERNATOR_EXCEPTIONS_H
#define TOWERNATOR_EXCEPTIONS_H

#include <exception>
#include <string>
#include <stdexcept>
namespace Exceptions {

    class TowernatorException : public std::exception {
    private:
        std::string m_message;

    public:
        explicit TowernatorException(const std::string& message)
            : m_message(message) {
        }

        const char* what() const noexcept override {
            return m_message.c_str();
        }
    };

}

#endif