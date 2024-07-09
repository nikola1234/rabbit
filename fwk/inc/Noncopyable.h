#pragma once

class Noncopyable
{
 public:
  Noncopyable(const Noncopyable&) = delete;
  Noncopyable& operator=(const Noncopyable&) = delete;

  // 可以定义移动构造函数和移动赋值操作符（如果需要）
  // Noncopyable(Noncopyable&&) = default;
  // Noncopyable& operator=(Noncopyable&&) = default;

 protected:
  Noncopyable() = default;
  ~Noncopyable() = default;
};

