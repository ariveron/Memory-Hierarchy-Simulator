#ifndef STRING_MANIPULATION_H
#define STRING_MANIPULATION_H

#include <string>
#include <algorithm>
#include <locale>

// Trim white-space from start of string, in-place
static inline void ltrim(std::string& s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](auto c) {
          return !std::isspace(c);
        }));
}

// Trim white-space from end of string, in-place
static inline void rtrim(std::string& s)
{
  s.erase(std::find_if(s.rbegin(), s.rend(), [](auto c) {
          return !std::isspace(c);
        }).base(), s.end());
}

// Trim white-space from both ends of string, in-place
static inline void trim(std::string& s)
{
  ltrim(s);
  rtrim(s);
}

// Trim white-space from start of string, out-of-place
static inline std::string ltrim_copy(std::string s)
{
  ltrim(s);
  return s;
}

// Trim white-space from end of string, out-of-place
static inline std::string rtrim_copy(std::string s)
{
  rtrim(s);
  return s;
}

// Trim white-space from both ends of string, out-of-place
static inline std::string trim_copy(std::string s)
{
  trim(s);
  return s;
}

// Convert all characters in string to uppercase, in-place
static inline void toupper(std::string& s)
{
  std::transform(s.begin(), s.end(), s.begin(), [](auto& c) { return static_cast<char>(std::toupper(static_cast<int>(c))); });
}

// Convert all characters in string to lowercase, in-place
static inline void tolower(std::string& s)
{
  std::transform(s.begin(), s.end(), s.begin(), [](auto& c) { return static_cast<char>(std::tolower(static_cast<int>(c))); });
}

// Convert all characters in string to uppercase, out-of-place
static inline std::string toupper_copy(std::string s)
{
  toupper(s);
  return s;
}

// Convert all characters in string to lowercase, out-of-place
static inline std::string tolower_copy(std::string s)
{
  tolower(s);
  return s;
}

#endif
