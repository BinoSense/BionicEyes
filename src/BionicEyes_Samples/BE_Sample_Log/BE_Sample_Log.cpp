#include "BE_Sample_Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

BE_spdlogSample::BE_spdlogSample(void *logger_ptr)
{
    if(logger_ptr == NULL)
    {
        logger_ptr_released = false;
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        //console_sink->set_level(spdlog::level::warn);
        logger = new spdlog::logger("BE_SampleConsole", {console_sink});
    } else
    {
        logger_ptr_released = true;
        logger = reinterpret_cast<spdlog::logger *>(logger_ptr);
    }

    logger->debug("BESampleLog: Object has been constructed");
    logger->info("BESampleLog: BE log sample starting...");
}

BE_spdlogSample::~BE_spdlogSample()
{
    logger->debug("BESampleLog: Object has been destructed");
    logger->flush();
    if(!logger_ptr_released)
    {
        delete logger;
    }
}

void BE_spdlogSample::printSample()
{
    //critical(致命错误) err(错误) warn(警告) info(信息) debug(调试) trace(跟踪)
    logger->trace("BESampleLog: This is print sample of be spdlog...");
    logger->debug("BESampleLog: This is print sample of be spdlog...");
    logger->info("BESampleLog: This is print sample of be spdlog...");
    logger->error("BESampleLog: This is print sample of be spdlog...");
    logger->critical("BESampleLog: This is print sample of be spdlog...");
}