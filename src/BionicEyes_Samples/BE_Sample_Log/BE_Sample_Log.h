//
// Created by ok_linux on 2021/4/8.
//

#ifndef BIONICEYES_BE_SAMPLE_LOG_H
#define BIONICEYES_BE_SAMPLE_LOG_H

#endif //BIONICEYES_BE_SAMPLE_LOG_H

#include "spdlog/spdlog.h"

class BE_spdlogSample
{
public:
    BE_spdlogSample(void *logger_ptr = NULL);
    ~BE_spdlogSample();

    void printSample();

private:
    bool logger_ptr_released;
    spdlog::logger *logger;
};