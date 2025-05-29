#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H

#include <exception>
#include <string>

namespace mtrack
{
    class DatabaseException : public std::exception
    {
    public:
        DatabaseException(const std::string &msg)
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
