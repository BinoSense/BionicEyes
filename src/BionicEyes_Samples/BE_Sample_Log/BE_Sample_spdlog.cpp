//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

// spdlog usage example
/*
参考文献
https://blog.csdn.net/haojie_superstar/article/details/89383433?ops_request_misc=&request_id=&biz_id=102&utm_source=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-0

要创建线程安全的loggers，使用带 _mt 后缀的工厂函数，例如：
auto logger = spdlog::basic_logger_mt(...);
要创建单线程的loggers，使用带 _st 后缀的工厂函数，例如：
auto logger = spdlog::basic_logger_st(...);

对于sinks，以 _mt 后缀结尾的是线程安全的，比如：daily_file_sink_mt

以_st 后缀结尾的是非线程安全的，比如：daily_file_sink_st

*/

#include <cstdio>
#include <evo_be_Common.h>

void stdout_logger_example();
void basic_example();
void rotating_example();
void daily_example();
void async_example();
void binary_example();
void trace_example();
void multi_sink_example();
void user_defined_example();
void err_handler_example();
void syslog_example();

#include "spdlog/spdlog.h"

int main(int, char *[])
{
    //首先你可以使用set_level函数来设置当前的日志等级，所有的等级如下（从大到小）：
    //critical(致命错误) err(错误) warn(警告) info(信息) debug(调试) trace(跟踪)
    //最后显示出来的信息只会是你指定等级及其左边的等级日志，比如将等级设置为warn:
    spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);
    //占位符，长度为8
    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    //10进制，16进制，oct 8进制，2进制
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);

    //输出为1.23
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    //括号里的数字代表后面参数的顺序，
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    //左对齐，保证30字符宽度
    spdlog::info("{:>8} aligned, {:<8} aligned", "right", "left");

    // Runtime log levels
    //设置日志等级
    spdlog::set_level(spdlog::level::info); // Set global log level to info
    spdlog::debug("This message should not be displayed!");
    spdlog::set_level(spdlog::level::trace); // Set specific logger's log level
    spdlog::debug("This message should be displayed..");

    // Customize msg format for all loggers
    //设置格式
    spdlog::set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
    spdlog::info("This an info message with custom format");
    spdlog::set_pattern("%+"); // back to default format，返回默认格式
    spdlog::set_level(spdlog::level::info);

    // Backtrace support
    // Loggers can store in a ring buffer all messages (including debug/trace) for later inspection.
    // When needed, call dump_backtrace() to see what happened:
    //记录器可以将所有消息（包括调试/跟踪）存储在环形缓冲区中，以供以后检查。
    //需要时，调用dump_backtrace（）查看发生了什么：

    spdlog::enable_backtrace(10); // create ring buffer with capacity of 10  messages
    for (int i = 0; i < 100; i++)
    {
        spdlog::debug("Backtrace message {}", i); // not logged..
    }
    // e.g. if some error happened:
    spdlog::dump_backtrace(); // log them now!

    try
    {
        stdout_logger_example();
        basic_example();
        rotating_example();
        daily_example();
        async_example();
        binary_example();
        multi_sink_example();
        user_defined_example();
        err_handler_example();
        trace_example();

        // Flush all *registered* loggers using a worker thread every 3 seconds.
        //每隔3秒使用工作线程刷新所有*已注册*的日志记录器。
        // note: registered loggers *must* be thread safe for this to work correctly!
        //注意：注册的记录器*必须*是线程安全的，这样才能正常工作！
        spdlog::flush_every(std::chrono::seconds(3));

        // Apply some function on all registered loggers
        //对所有注册的记录器应用某些功能
        spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l) { l->info("End of example."); });

        // Release all spdlog resources, and drop all loggers in the registry.
        //释放所有spdlog资源，并在注册表中删除所有记录器。
        // This is optional (only mandatory if using windows + async log).
        //这是可选的（仅在使用windows+异步日志时是必需的）。
        spdlog::shutdown();
    }

        // Exceptions will only be thrown upon failed logger or sink construction (not during logging).
    catch (const spdlog::spdlog_ex &ex)
    {
        std::printf("Log initialization failed: %s\n", ex.what());
        return 1;
    }
}

#include "spdlog/sinks/stdout_color_sinks.h"
// or #include "spdlog/sinks/stdout_sinks.h" if no colors needed.
void stdout_logger_example()
{
    // Create color multi threaded logger.
    //创建彩色多线程记录器。
    auto console = spdlog::stdout_color_mt("console");
    // or for stderr:
    // auto console = spdlog::stderr_color_mt("error-logger");
}

#include "spdlog/sinks/basic_file_sink.h"
void basic_example()
{
    // Create basic file logger (not rotated).
    //创建基本文件记录器（不旋转）。
    auto my_logger = spdlog::basic_logger_mt("file_logger", evo_be::Be_Log_Path+"/basic-log.txt");
}

#include "spdlog/sinks/rotating_file_sink.h"
void rotating_example()
{
    // Create a file rotating logger with 5mb size max and 3 rotated files.
    //创建一个最大大小为5mb的文件滚动记录器和3个滚动文件。
    auto rotating_logger = spdlog::rotating_logger_mt("some_logger_name", evo_be::Be_Log_Path+"/rotating.txt", 20 * 5, 3);
}

#include "spdlog/sinks/daily_file_sink.h"
void daily_example()
{
    // Create a daily logger - a new file is created every day on 2:30am.
    auto daily_logger = spdlog::daily_logger_mt("daily_logger", evo_be::Be_Log_Path+"/daily.txt", 2, 30);
}

//使用工厂函数创建异步logger：
#include "spdlog/async.h"
void async_example()
{
    // Default thread pool settings can be modified *before* creating the async logger:
    //在*创建异步记录器之前*可以修改默认线程池设置：
    // spdlog::init_thread_pool(32768, 1); // queue with max 32k items 1 backing thread.
    //使用最多32k个项目1的后台线程排队。
    auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", evo_be::Be_Log_Path+"/async_log.txt");
    // alternatively:
    // auto async_file = spdlog::create_async<spdlog::sinks::basic_file_sink_mt>("async_file_logger", "logs/async_log.txt");

    for (int i = 1; i < 101; ++i)
    {
        async_file->info("Async message #{}", i);
    }
}

// Log binary data as hex.
// Many types of std::container<char> types can be used.
// Iterator ranges are supported too.
// Format flags:
// {:X} - print in uppercase.用大写印刷。
// {:s} - don't separate each byte with space.不要用空格分隔每个字节。
// {:p} - don't print the position on each line start.不要在每行开始处打印位置。
// {:n} - don't split the output to lines.不要将输出拆分为行。

#include "spdlog/fmt/bin_to_hex.h"
void binary_example()
{
    std::vector<char> buf(80);
    for (int i = 0; i < 80; i++)
    {
        buf.push_back(static_cast<char>(i & 0xff));
    }
    spdlog::info("Binary example: {}", spdlog::to_hex(buf));
    spdlog::info("Another binary example:{:n}", spdlog::to_hex(std::begin(buf), std::begin(buf) + 10));
    // more examples:
    // logger->info("uppercase: {:X}", spdlog::to_hex(buf));
    // logger->info("uppercase, no delimiters: {:Xs}", spdlog::to_hex(buf));
    // logger->info("uppercase, no delimiters, no position info: {:Xsp}", spdlog::to_hex(buf));
}

// Compile time log levels.日志宏定义
// #define SPDLOG_ACTIVE_LEVEL to required level (e.g. SPDLOG_LEVEL_TRACE)
void trace_example()
{
    // trace from default logger
    SPDLOG_TRACE("Some trace message.. {} ,{}", 1, 3.23);
    // debug from default logger
    SPDLOG_DEBUG("Some debug message.. {} ,{}", 1, 3.23);

    // trace from logger object
    auto logger = spdlog::get("file_logger");
    SPDLOG_LOGGER_TRACE(logger, "another trace message");
}

// A logger with multiple sinks (stdout and file) - each with a different format and log level.
//创建一个对应多个sink的logger，每一个sink都有独有的格式和日志级别
void multi_sink_example()
{
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::warn);
    console_sink->set_pattern("[multi_sink_example] [%^%l%$] %v");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(evo_be::Be_Log_Path+"/multisink.txt", true);
    file_sink->set_level(spdlog::level::trace);

    spdlog::logger logger("multi_sink", {console_sink, file_sink});
    logger.set_level(spdlog::level::debug);
    logger.warn("this should appear in both console and file");
    logger.info("this message should not appear in the console, only in the file");
}
//
////创建一个由多个loggers共享同一个输出文件的sink
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
int main2(int, char* [])
{
    try
    {
        auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(evo_be::Be_Log_Path+"/logfile", 23, 59);
        // create synchronous  loggers
        auto net_logger = std::make_shared<spdlog::logger>("net", daily_sink);
        auto hw_logger  = std::make_shared<spdlog::logger>("hw",  daily_sink);
        auto db_logger  = std::make_shared<spdlog::logger>("db",  daily_sink);

        net_logger->set_level(spdlog::level::critical); // independent levels
        hw_logger->set_level(spdlog::level::debug);

        // globally register the loggers so so the can be accessed using spdlog::get(logger_name)
        spdlog::register_logger(net_logger);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
}
//
//// User defined types logging by implementing operator<<
#include "spdlog/fmt/ostr.h" // must be included
struct my_type
{
    int i;
    template<typename OStream>
    friend OStream &operator<<(OStream &os, const my_type &c)
    {
        return os << "[my_type i=" << c.i << "]";
    }
};

void user_defined_example()
{
    spdlog::info("user defined type: {}", my_type{14});
}

// Custom error handler. Will be triggered on log failure.
void err_handler_example()
{
    // can be set globally or per logger(logger->set_error_handler(..))
    spdlog::set_error_handler([](const std::string &msg) { printf("*** Custom log error handler: %s ***\n", msg.c_str()); });
}

//// syslog example (linux/osx/freebsd)
//#ifndef _WIN32
//#include "spdlog/sinks/syslog_sink.h"
//void syslog_example()
//{
//    std::string ident = "spdlog-example";
//    auto syslog_logger = spdlog::syslog_logger_mt("syslog", ident, LOG_PID);
//    syslog_logger->warn("This is warning that will end up in syslog.");
//}
//#endif
//
//// Android example.
//#if defined(__ANDROID__)
//#include "spdlog/sinks/android_sink.h"
//void android_example()
//{
//    std::string tag = "spdlog-android";
//    auto android_logger = spdlog::android_logger_mt("android", tag);
//    android_logger->critical("Use \"adb shell logcat\" to view this message.");
//}
//
//#endif