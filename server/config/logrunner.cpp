#include "logrunner.h"
#include "configuration.h"

void LoadLogger()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/service.log", true);

    auto logger = std::make_shared<spdlog::logger>("multi_logger", spdlog::sinks_init_list{console_sink, file_sink});
    spdlog::set_default_logger(logger); 

    spdlog::set_level(spdlog::level::from_str(server_config.loglevel));
    spdlog::set_pattern("%Y-%m-%d %H:%M:%S [%!] [%^%l%$] %v");
}