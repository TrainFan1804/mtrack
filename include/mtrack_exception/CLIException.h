#ifndef CLIEXCEPTION_H
#define CLIEXCEPTION_H

#include <exception>
#include <string>

namespace mtrack
{
    class CLIException : public std::exception
    {
    public:
        CLIException(const std::string &msg)
            : _msg(msg) {};

        virtual const char *what() const throw()
        {
            return _msg.c_str();
        }

    private:
        std::string _msg;
    };
} // namespace mtrack

#endif
