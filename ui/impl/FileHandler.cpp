#include "ui/FileHandler.hpp"
#include "utils/error.hpp"
#include <fstream>
#include <QDebug>

FileHandler::FileHandler(std::filesystem::path path) : path(path) {
    if (!std::filesystem::exists(path))
        throw NonExistingFile("ERROR: non existing file: " + path.string());
}

std::stringstream FileHandler::read(){

    std::ifstream file(path, std::ios::binary);
    std::stringstream data;
    if (file.is_open()) {

        data << file.rdbuf();
        file.close();
    }
    else
        throw OpenFileError("couldn't open file for reading");
    return data;

}

void FileHandler::write(std::stringstream data){

    std::ofstream file(path, std::ios::binary | std::ios::app);

    if (file) {

        file << data.rdbuf();
    }
    else
        throw OpenFileError("couldn't open file for writing");
}


std::unique_ptr<FileHandler> createFileHanlder(std::filesystem::path path){

    std::unique_ptr<FileHandler> file_handler;
    try {
        file_handler = std::unique_ptr<FileHandler>(new FileHandler(path));
    }

    catch(const BaseError& err){

        qDebug() << err.what();
    }
    return file_handler;


}
