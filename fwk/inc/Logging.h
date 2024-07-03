#pragma once

#include "LogStream.h"
#include <string>
#include <cstring>
#include <iostream>
class Logger
{

public:

    enum class LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ABN,
        ERROR,
        NUM_LOG_LEVELS,
    };

    class SourceFile
    {
    public:
        template<int N>
        SourceFile(const char (&arr)[N])
        : filename_(arr),
            size_(N-1)
        {
            const char* slash = std::strrchr(filename_, '/'); // builtin function
            if (slash)
            {
                filename_ = slash + 1;
                size_ -= static_cast<int>(filename_ - arr);
            }
        }

        explicit SourceFile(const char* filename)
            : filename_(filename)
        {
            if (filename == nullptr)
            {
                filename_ = "unknown file";
            }
            const char* slash = std::strrchr(filename, '/');
            if (slash)
            {
                filename_ = slash + 1;
            }
            size_ = static_cast<int>(std::strlen(filename_));
        }

        std::string toString()
        {
            return std::string(filename_);
        }

        const char* filename_;
        int size_;
    };

    Logger(SourceFile file, int line);
    Logger(SourceFile file, int line, LogLevel level);
    Logger(SourceFile file, int line, LogLevel level, const char* func);
    ~Logger();

    LogStream& stream() { return impl_.stream_; }

    static LogLevel logLevel();
    static void setLogLevel(LogLevel level);

    typedef void (*OutputFunc)(const char* msg, int len);
    typedef void (*FlushFunc)();

    static void setOutput(OutputFunc);
    static void setFlush(FlushFunc);

private:
    class Impl
    {
    public:
        typedef Logger::LogLevel LogLevel;
        Impl(LogLevel level, const SourceFile& file, int line);
        void formatTime();
        void finish();

        std::string time_;
        LogStream stream_;
        LogLevel level_;
        int line_;
        SourceFile file_;
    };

    Impl impl_;

};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel()
{
  return g_logLevel;
}

#define LOG_TRACE if (Logger::logLevel() <= Logger::LogLevel::TRACE) Logger(__FILE__, __LINE__, Logger::LogLevel::TRACE, __func__).stream()
#define LOG_DEBUG if (Logger::logLevel() <= Logger::LogLevel::DEBUG) Logger(__FILE__, __LINE__, Logger::LogLevel::DEBUG, __func__).stream()
#define LOG_INFO if (Logger::logLevel() <= Logger::LogLevel::INFO) Logger(__FILE__, __LINE__).stream()
#define LOG_WARN Logger(__FILE__, __LINE__, Logger::LogLevel::WARN).stream()
#define LOG_ABN Logger(__FILE__, __LINE__, Logger::LogLevel::ABN).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::LogLevel::ERROR).stream()