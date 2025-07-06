#include "../include/client_logger_builder.h"

#include <fstream>
#include <vector>

#include <algorithm> 
#include <cctype>
#include <string> 

#include <nlohmann/json.hpp>

client_logger_builder::client_logger_builder() :
    _format_out("[%s] %m") {}

client_logger_builder::client_logger_builder(const std::string &format) :
    _format_out(format) {}

logger_builder *client_logger_builder::add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) {
    _path_sevs[stream_file_path].insert(severity);
    return this;
}

logger_builder *client_logger_builder::add_console_stream(
        logger::severity severity) {
    _console_sevs.insert(severity);
    return this;
}

logger_builder* client_logger_builder::transform_with_configuration(
        std::string const &configuration_file_path,
        std::string const &configuration_path) {

    std::ifstream stream(configuration_file_path);

    if (!stream.is_open()) {
        throw std::runtime_error("File error " + configuration_file_path);
    }
    std::vector<std::string> data_path_components;

    for (size_t ind = 0; ind < configuration_path.size(); ) {
        size_t tmp_ind = std::min(configuration_path.find(':', ind), configuration_path.size());

        std::string component = configuration_path.substr(ind, tmp_ind - ind);
        if (component.size() == 0) {
            throw std::runtime_error("Invalid in-JSON configuration path");
        }

        data_path_components.push_back(std::move(component));
        ind = tmp_ind + 1;
    }

    nlohmann::json config;
    try {
        config = nlohmann::json::parse(stream);
    }
    catch (const std::exception&) {
        throw std::runtime_error("Invalid JSON file");
    }

    for (auto path_elem : data_path_components) {
        config = config[path_elem];
    }

    clear();
    try { 
        _format_out = config["format"];
        config = config["files"];
    }
    catch (const std::exception&) {
        throw std::runtime_error("Invalid or non-existent JSON configuration");
    }
    for (auto &[file_path, severities] : config.items()) {
        for (std::string sev_str : severities) {
            std::transform(sev_str.begin(), sev_str.end(), sev_str.begin(),
                [](unsigned char c) { return std::tolower(c); }
            );
            logger::severity severity = string_to_severity(sev_str);

            if (file_path == CONSOLE_STREAM) {
                add_console_stream(severity);
            }
            else {
                add_file_stream(file_path, severity);
            }
        }
    }
    return this;
}

logger_builder *client_logger_builder::clear() {
    _format_out = "[%s]: %m";
    _path_sevs.clear();
    return this;
}

logger *client_logger_builder::build() const {
    return new client_logger(_path_sevs, _console_sevs,_format_out);
}