#include <gtest/gtest.h>

#include <fraction.h>
#include <client_logger.h>
#include <operation_not_supported.h>

logger *create_logger(
    std::vector<std::pair<std::string, logger::severity>> const &output_file_streams_setup,
    bool use_console_stream = true,
    logger::severity console_stream_severity = logger::severity::debug)
{
    logger_builder *builder = new client_logger_builder();
    
    if (use_console_stream)
    {
        builder->add_console_stream(console_stream_severity);
    }
    
    for (auto &output_file_stream_setup: output_file_streams_setup)
    {
        builder->add_file_stream(output_file_stream_setup.first, output_file_stream_setup.second);
    }
    
    logger *built_logger = builder->build();
    
    delete builder;
    
    return built_logger;
}

TEST(positive_tests, test1)
{
    
}

int main(
    int argc,
    char **argv)
{
    fraction eps("1", "10000000000000000000000000000000000000000000000000000000000000000000");
    
	
	std::cout << (fraction("1", "2").ln(eps)) << std::endl;
	
	
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}