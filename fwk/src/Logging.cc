
#include "Logging.h"
#include <cstdio>
#include <iomanip>

Logger::LogLevel initLogLevel()
{
  /*
  if (::getenv("R_LOG_TRACE"))
    return Logger::LogLevel::TRACE;
  else if (::getenv("R_LOG_DEBUG"))
    return Logger::LogLevel::DEBUG;
  else
    return Logger::LogLevel::INFO;
  */
  return Logger::LogLevel::TRACE;
}

const char* LogLevelName[static_cast<std::uint32_t>(Logger::LogLevel::NUM_LOG_LEVELS)] =
{
  "TRACE ",
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ABN   ",
  "ERROR ",
};

void defaultOutput(const char* msg, int len)
{
  //fwrite 是用于写入文件的函数，它可以写入字符串、格式化数据等
  size_t n = std::fwrite(msg, sizeof(char), len, stdout);
  //FIXME check n
  (void)n;

  /*
   //std::puts() 函数接受一个以 null 结尾的字符数组（C 风格字符串），并将其输出到 stdout
   //puts 是一个简单的函数，用于将字符串写入到标准输出（stdout），并在字符串的末尾自动添加换行符。
    std::puts(msg);
  */
}

void defaultFlush()
{
  //std::fflush(stdout) is called to flush the output buffer and ensure the message is immediately displayed.
  // This can be useful in situations where you want to ensure that the output is displayed immediately,
  //rather than waiting for the buffer to be flushed automatically.
  fflush(stdout);
}

Logger::LogLevel g_logLevel = initLogLevel();
Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;

// #########################################

Logger::Impl::Impl(LogLevel level, const SourceFile& file, int line)
  :stream_(),
    level_(level),
    line_(line),
    file_(file)
{
  formatTime();
  stream_ << file_.toString() << ':' << line_ << ' ';
}


Logger::Logger(SourceFile file, int line)
  : impl_(LogLevel::INFO, file, line)
{
}

Logger::Logger(SourceFile file, int line, LogLevel level)
  : impl_(level, file, line)
{
}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func)
  : impl_(level, file, line)
{
  impl_.stream_ << func << ' ';
}

Logger::~Logger()
{
  impl_.finish();
  const LogStream::Buffer& buf(stream().buffer());
  g_output(buf.data(), buf.length());

  /* 这边就不terminate program了
  if (impl_.level_ == FATAL)
  {
    g_flush();
    abort();
  }
  */
}

void Logger::Impl::formatTime()
{
  std::time_t currentTime = std::time(nullptr);
  std::tm* localTime = std::localtime(&currentTime);

  std::ostringstream timeStream;
  timeStream << '['<< std::put_time(localTime, "%Y-%m-%d %H:%M:%S") << "]-";
  timeStream << std::setfill('0');
  stream_ << timeStream.str();
}

void Logger::Impl::finish()
{
  stream_ << '\n';
}

// #########################################

void Logger::setLogLevel(Logger::LogLevel level)
{
  g_logLevel = level;
}

void Logger::setOutput(OutputFunc out)
{
  g_output = out;
}

void Logger::setFlush(FlushFunc flush)
{
  g_flush = flush;
}





