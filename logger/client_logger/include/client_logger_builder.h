#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H

#include <logger_builder.h>
#include <client_logger.h>
#include <unordered_map>
#include <unordered_set>

#define CONSOLE_STREAM "console_stream"

class client_logger_builder final:
    public logger_builder
{
    std::string _format_out;
    std::unordered_map<std::string, std::unordered_set<logger::severity>> _path_sevs;
    std::unordered_set<logger::severity> _console_sevs;

public:

    client_logger_builder();

    client_logger_builder(std::string const& format);

    client_logger_builder(
        client_logger_builder const &other) = default;

    client_logger_builder &operator=(
        client_logger_builder const &other) = default;

    client_logger_builder(
        client_logger_builder &&other) noexcept = default;

    client_logger_builder &operator=(
        client_logger_builder &&other) noexcept = default;

    ~client_logger_builder() noexcept override = default;

public:

    logger_builder *add_file_stream(
        std::string const &stream_file_path,
        logger::severity severity) override;

    logger_builder *add_console_stream(
        logger::severity severity) override;

    logger_builder* transform_with_configuration(
        std::string const &configuration_file_path,
        std::string const &configuration_path) override;

    logger_builder *clear() override;

    [[nodiscard]] logger *build() const override;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_BUILDER_H