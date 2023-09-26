#ifndef FILEHANDLER_HPP
#define FILEHANDLER_HPP
#include <memory>
#include <filesystem>
#include <sstream>

class FileHandler {
public:
    friend std::unique_ptr<FileHandler> createFileHanlder(std::filesystem::path path);


public:
    void write(std::stringstream data);
    std::stringstream read();

protected:
    FileHandler(std::filesystem::path path);

private:
    std::filesystem::path path;
};

std::unique_ptr<FileHandler> createFileHanlder(std::filesystem::path path);

#endif // FILEHANDLER_HPP
