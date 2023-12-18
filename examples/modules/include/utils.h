#pragma once
#include <iostream>

#define STDLOGPROCESS(msg) std::cout << "(STDLOGPROCESS)" << #msg << std::endl
#define STDLOGFUNCTION(msg) std::cout << "function-" << __FUNCTION__ << msg << std::endl;

template <class T>
class Prompter {
  static T value_;
public:
  Prompter() = delete;
  static void prompt(const std::string& message) {
    std::cout << message << std::endl;
    std::cin >> value_;
  }
  static T value() { return value_; }
};
template <class T>
T Prompter<T>::value_;

template <class T>
T prompt(const std::string& message) {
  std::cout << message << std::endl;
  T value;
  std::cin >> value;
  return value;
}

struct YN {
  bool value;
  operator bool() const { return value; }
};

std::istream& operator>>(std::istream& is, YN& yn) {
  std::string s;
  is >> s;
  yn.value = (s == "y" || s == "Y");
  return is;
}
std::ostream& operator<<(std::ostream& os, const YN& yn) {
  os << (yn.value ? "True" : "False");
  return os;
}