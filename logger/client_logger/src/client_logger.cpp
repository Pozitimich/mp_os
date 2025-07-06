#include "../include/client_logger.h"
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>

std::map<std::string, std::pair<std::ofstream* const, size_t>> client_logger::_all_fstreams{};

client_logger::client_logger(
        const std::unordered_map<std::string, 
                std::unordered_set<logger::severity>> &configuration,
        const std::unordered_set<logger::severity> &console_sevs,
        const std::string &format) :
            _console_sevs(console_sevs.begin(), console_sevs.end()), 
            _format_out(format) {
    for (const auto &path_sevs : configuration) {
        std::string file_path = std::filesystem::weakly_canonical(path_sevs.first).string();
        fstream_map::iterator map_iter = _all_fstreams.find(file_path);
        if (map_iter == _all_fstreams.end()) {
            std::ofstream *fstream = nullptr;
            try {
                fstream = new std::ofstream(file_path);

                if (!fstream->is_open()) {
                    throw std::runtime_error("File error: " + file_path);
                }
            }
            catch (const std::exception&) {
                delete fstream;
                for (auto sev_map : _local_fstreams) {
                    for (auto map_iter : sev_map.second) {
                        remove_reference(map_iter);
                    }
                }

                throw;
            }
            map_iter = _all_fstreams.insert(std::make_pair(std::move(file_path), 
                    std::make_pair(fstream, size_t(0)))).first;
        }
        ++map_iter->second.second;

        for (const logger::severity sev : path_sevs.second) {
            bool found = false;
            for (auto& sev_iters : _local_fstreams) {
                if (sev_iters.first == sev) {
                    sev_iters.second.push_back(map_iter);
                    found = true;
                    break;
                }
            }
            if (!found) {
                _local_fstreams.emplace_back(sev, std::vector<fstream_map::iterator>{map_iter});
            }
        }
    }
    _local_fstreams.shrink_to_fit();

}

client_logger::client_logger(
    client_logger const &other) :
    _format_out(other._format_out),
    _local_fstreams(other._local_fstreams),
    _console_sevs(other._console_sevs) {
   for (auto &sev_files : _local_fstreams) {
        for (auto &path_stream : sev_files.second) { 
            ++path_stream->second.second;
        }
   }
}

client_logger &client_logger::operator=(
    client_logger const &other) {
    if (this == &other) {
        return *this;
    }

    remove_all_local_references();

    _format_out = other._format_out;
    _local_fstreams = other._local_fstreams;
    _console_sevs = other._console_sevs;

    for (auto& sev_iters : _local_fstreams) {
        for (auto& map_iter : sev_iters.second) {
            ++map_iter->second.second;   
        }
    }

    return *this;
}


client_logger::client_logger(client_logger &&other) noexcept :
    _format_out(std::move(other._format_out)),
    _local_fstreams(std::move(other._local_fstreams)),
    _console_sevs(std::move(other._console_sevs)) {}

client_logger &client_logger::operator=(
    client_logger &&other) noexcept {
    if (this == &other) {
        return *this;
    }

    remove_all_local_references();

    _format_out = std::move(other._format_out);
    _local_fstreams = std::move(other._local_fstreams);
    _console_sevs = std::move(other._console_sevs);

    return *this;
}

client_logger::~client_logger() noexcept {
    remove_all_local_references();
}

logger const *client_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept {

        std::string result = _format_out;
        //--------------
        std::time_t now = std::time(nullptr);
        char date[11];
        char time[9];
        std::strftime(date, sizeof(date), "%d.%m.%Y", std::localtime(&now));
        std::strftime(time, sizeof(time), "%H:%M:%S", std::localtime(&now));
        std::string d(date);
        std::string t(time);
        //--------------
        result = std::regex_replace(result, std::regex("\\%d"), d);
        result = std::regex_replace(result, std::regex("\\%t"), t);
        result = std::regex_replace(result, std::regex("\\%s"), logger::severity_to_string(severity));
        result = std::regex_replace(result, std::regex("\\%m"), text);

        for (const auto &sev_iters : _local_fstreams) {
            if (severity == sev_iters.first) {
                for (const auto& iter : sev_iters.second) {
                    *iter->second.first << result << std::endl;
                }
            }
        }
        for (const logger::severity console_sev : _console_sevs) {
            if (console_sev == severity) {
                std::cout << result << std::endl;
            }
        }
        return this;
}

void client_logger::remove_reference(const fstream_map::iterator &map_iter) {
    if (--map_iter->second.second == 0) {
        map_iter->second.first->flush();
        delete map_iter->second.first;
        _all_fstreams.erase(map_iter);
    }
}

void client_logger::remove_all_local_references() {
    for (auto& sev_iters : _local_fstreams) {
        for (auto& map_iter : sev_iters.second) {
            remove_reference(map_iter);   
        }
    }
}