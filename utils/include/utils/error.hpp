#ifndef ERROR_H
#define ERROR_H
#include <stdexcept>

class BaseError: public std::runtime_error {

public:
    BaseError(std::string_view error = "");

};


class UnsupportedObjectType: public BaseError {

public:
    UnsupportedObjectType(std::string_view error = "");

};

class NonExistingFile: public BaseError {

public:
    NonExistingFile(std::string_view error = "") ;

};

class OpenFileError: public BaseError {

public:
    OpenFileError(std::string_view error = "");

};

class IncorrectObjectRepresantion: public BaseError {

  public:
    IncorrectObjectRepresantion(std::string_view error = "");
};
#endif // ERROR_H
