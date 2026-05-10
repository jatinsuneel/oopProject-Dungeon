#include "GameException.h"

GameException::GameException(string message) {
    this->message = message;
}

const char* GameException::what() const noexcept {
    return message.c_str();
}