#include <gtest/gtest.h>

#include <client_logger.h>

int main(
    int argc,
    char *argv[])
{
    std::ifstream paths_file("D:\\Study\\Programming\\GitHub\\mp_os\\mp_os\\logger\\client_logger\\tests\\paths.txt");
    if (!paths_file.is_open()) {
        std::cout << "invalid paths_file" << std::endl;
    }
    std::string conf_filename;
    std::string conf_path;
    {
        std::string line;
        while (std::getline(paths_file, line)) {
            if (line == "config path:") {
                std::getline(paths_file, conf_filename);
            }
            else if (line == "path in JSON:") {
                std::getline(paths_file, conf_path);
            }
        }
    }

    try {
        logger_builder *p_obj = new client_logger_builder;

        logger *log = p_obj->transform_with_configuration(conf_filename, conf_path)->build();

        log->information("LOOL");
        log->debug("KEEK");
        log->warning("sme_wrn");

        delete p_obj;
        delete log;
    }
    catch (std::exception er) {
        std::cout << er.what() << std::endl;
    }

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}