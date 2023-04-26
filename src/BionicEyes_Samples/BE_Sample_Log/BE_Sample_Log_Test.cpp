//
// Created by ok_linux on 2021/4/8.
//

#include "evo_be_Common.h"
#include "BE_Sample_Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace evo_be;

int main()
{
    ////////////////////// create log file ///////////////////////
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);

    auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(evo_be::Be_Log_Path+"/Log_BE_Sample.txt", 23, 59, false);
    file_sink->set_level(spdlog::level::trace);

    spdlog::logger* logger = new spdlog::logger("BE_Log_Sample_sink", {console_sink, file_sink});
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
    //////////////////////////////////////////////////////////////

    cout<<"BE_LogSample with file and console output"<<endl;

    BE_spdlogSample beSpdlogSample(logger);

    beSpdlogSample.printSample();

    cout<<"BE_LogSample with console output"<<endl;

    BE_spdlogSample beSpdlogSample2;

    beSpdlogSample2.printSample();

    return 0;
}