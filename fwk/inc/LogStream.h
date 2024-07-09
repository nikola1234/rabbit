#pragma once

#include "Noncopyable.h"
#include <string>
#include "Types.h"
#include <assert.h>
#include <string.h> // memcpy
#include <vector>
#include <map>
#include <utility>
#include <tuple>


namespace detail
{

  const int kSmallBuffer = 4000;
  const int kLargeBuffer = 4000*1000;

  template<int SIZE>
  class FixedBuffer : Noncopyable
  {
  public:
    FixedBuffer(): cur_(data_) {}

    ~FixedBuffer()
    {
      // TODO
      bzero();
    }

    void append(const char* /*restrict*/ buf, size_t len)
    {
      // FIXME: append partially
      if (implicit_cast<size_t>(avail()) > len)
      {
        memcpy(cur_, buf, len);
        cur_ += len;
      }
    }

    const char* data() const { return data_; }
    int length() const { return static_cast<int>(cur_ - data_); }

    // write to data_ directly
    char* current() { return cur_; }
    int avail() const { return static_cast<int>(end() - cur_); }
    void add(size_t len) { cur_ += len; }

    void reset() { cur_ = data_; }
    void bzero() { memZero(data_, sizeof data_); }  // memset with 0

    // for used by GDB
    const char* debugString();  // how to use it by GDB?
    // for used by unit test
    string toString() const { return string(data_, length()); }

  private:
    const char* begin() const {return data_ ;}  // TODO add it to support range for
    const char* end() const { return data_ + sizeof data_; }

    char data_[SIZE];
    char* cur_;
  };

}  // namespace detail

class LogStream : Noncopyable
{
  typedef LogStream self;
 public:
  typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer;

  self& operator<<(bool v)
  {
    buffer_.append(v ? "1" : "0", 1);
    return *this;
  }

  self& operator<<(short);
  self& operator<<(unsigned short);
  self& operator<<(int);
  self& operator<<(unsigned int);
  self& operator<<(long);
  self& operator<<(unsigned long);
  self& operator<<(long long);
  self& operator<<(unsigned long long);

  self& operator<<(const void*);

  self& operator<<(float v)
  {
    *this << static_cast<double>(v);
    return *this;
  }
  self& operator<<(double);
  // self& operator<<(long double);

  self& operator<<(char v)
  {
    buffer_.append(&v, 1);
    return *this;
  }

  // self& operator<<(signed char);
  // self& operator<<(unsigned char);

  self& operator<<(const char* str)
  {
    if (str)
    {
      buffer_.append(str, strlen(str));
    }
    else
    {
      buffer_.append("(null)", 6);
    }
    return *this;
  }

  self& operator<<(const unsigned char* str)
  {
    return operator<<(reinterpret_cast<const char*>(str));
  }

  self& operator<<(const std::string& v)
  {
    buffer_.append(v.c_str(), v.size());
    return *this;
  }

  template<typename T>
  LogStream& operator<<(const std::vector<T>& v)
  {
    *this << '[';
    bool first = true;
    for (const auto & e: v)
    {
      if (!first) {
          *this << ',';
      }
      *this << e;
      first = false;
    }
    *this << ']';
    return *this;
  }

  template<typename K, typename V>
  LogStream& operator<<(const std::map<K, V>& m)
  {
    *this << '[';
    bool first = true;
    for (const auto& [key, value] : m)
    {
      if (!first) {
          *this << ',';
      }

      *this << key << ":" << value;
      first = false;
    }
    *this << ']';
    return *this;
  }

  template<typename F, typename S>
  LogStream& operator<<(const std::pair<F, S>& p)
  {
    *this << '(';
    if (const auto& [first, second] = p; true)
    {
      *this << first << ":" << second;
    } else {
      *this << '-';
    }

    *this << ')';
    return *this;
  }

  template<typename... Args>
  LogStream& operator<<(const std::tuple<Args...>& t)
  {
    int size = static_cast<int>(sizeof...(Args));
    if (size > 0)
    {
      std::apply([this,size](auto&&... args) {
        int s  = size;
        *this<< '(';
        ((*this<< args << (s-- > 1 ? "," : "")), ...);
        *this<< ')';
      }, t);
    } else {
      *this << '-';
    }
      return *this;
  }


  void append(const char* data, int len) { buffer_.append(data, len); }
  const Buffer& buffer() const { return buffer_; }
  void resetBuffer() { buffer_.reset(); }

 private:

  template<typename T>
  void formatInteger(T);

  Buffer buffer_;

  static const int kMaxNumericSize = 48;
};
