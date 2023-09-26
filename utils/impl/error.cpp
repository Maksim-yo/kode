#include "utils/error.hpp"


BaseError::BaseError(std::string_view error) :  std::runtime_error {std::string {error}} {}

UnsupportedObjectType:: UnsupportedObjectType(std::string_view error) : BaseError(error) {}

NonExistingFile::NonExistingFile(std::string_view error) : BaseError(error) {}

OpenFileError::OpenFileError(std::string_view error) : BaseError(error) {}

IncorrectObjectRepresantion::IncorrectObjectRepresantion(std::string_view error) : BaseError(error) { }
