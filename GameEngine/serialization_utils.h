#pragma once
#ifndef SERIALIZATION_UTILS_H
#define SERIALIZATION_UTILS_H

#include <fstream>
#include <cereal/archives/json.hpp>
#include <string>
#include <stdexcept>

template <typename T>
void saveToFile(const T& obj, const std::string& filename, const std::string& objName) {
    std::ofstream os(filename);
    if (!os.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }
    cereal::JSONOutputArchive archive(os);
    archive(cereal::make_nvp(objName, obj));
}

template <typename T>
T loadFromFile(const std::string& filename, const std::string& objName) {
    std::ifstream is(filename);
    if (!is.is_open()) {
        throw std::runtime_error("Could not open file for reading: " + filename);
    }
    try {
        cereal::JSONInputArchive archive(is);
        T obj;
        archive(cereal::make_nvp(objName, obj));
        return obj;
    }
    catch (const std::exception& e) {
        std::cerr << "Error during deserialization: " << e.what() << std::endl;
        throw; // Rethrow to handle further up
    }
}


#endif