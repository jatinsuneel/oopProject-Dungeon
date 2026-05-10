#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <exception>
#include <string>

using namespace std;

class GameException : public exception {
private:
    string message;

public:
    GameException(string message);

    const char* what() const noexcept override;
};

#endif