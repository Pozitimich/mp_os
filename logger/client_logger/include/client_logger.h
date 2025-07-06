#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include "client_logger_builder.h"

#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <memory>

class client_logger final:
    public logger
{
    friend class client_logger_builder;
    
    using fstream_map = std::map<std::string, std::pair<std::ofstream* const, size_t>>;
    static fstream_map _all_fstreams;
    std::vector<std::pair<logger::severity, std::vector<fstream_map::iterator>>> _local_fstreams;
    std::vector<logger::severity> _console_sevs;

    std::string _format_out;

    client_logger(
            const std::unordered_map<std::string, 
                std::unordered_set<logger::severity>> &configuration,
            const std::unordered_set<logger::severity> &console_stream,
            std::string const &format);
    
    void remove_all_local_references();
    void remove_reference(const fstream_map::iterator &map_iter);

public:

    client_logger(
        client_logger const &other);

    client_logger &operator=(
        client_logger const &other);

    client_logger(
        client_logger &&other) noexcept;

    client_logger &operator=(
        client_logger &&other) noexcept;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H