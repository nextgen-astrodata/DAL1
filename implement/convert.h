// File: convert.h
#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <stdexcept>

class BadConversion : public std::runtime_error {
public:
  BadConversion(const std::string& s)
    : std::runtime_error(s)
    { }
};

template<typename T>
inline std::string stringify(const T& x)
{
  std::ostringstream o;
  if (!(o << x))
   throw BadConversion(std::string("stringify(")
		       + typeid(x).name() + ")");
   return o.str();
}

