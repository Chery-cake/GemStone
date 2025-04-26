#pragma once

#include <cstddef>
#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>

namespace variables {

// Supported types
using VariantType = std::variant<int, float, bool, char, std::string>;

// Singleton class for thread-safe variable storage
class Storage {
public:
  // Delete copy/move constructors
  Storage(const Storage &) = delete;
  Storage &operator=(const Storage &) = delete;
  Storage(Storage &&) = delete;
  Storage &operator=(Storage &&) = delete;

  // Get singleton instance
  static Storage *getInstance();

  // Variable management

  // Set a global variable
  template <typename T> void set(const std::string &key, const T &value) {
    std::lock_guard<std::mutex> lock(mutex_);
    variables_[key] = value;
  };

  // Get a global variable
  template <typename T> T get(const std::string &key) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = variables_.find(key);
    if (it == variables_.end()) {
      throw std::runtime_error("Global variable not found: " + key);
    }

    try {
      return std::get<T>(it->second);
    } catch (const std::bad_variant_access &) {
      throw std::runtime_error("Type mismatch for global variable: " + key);
    }
  };

  bool exists(const std::string &key) const;
  void remove(const std::string &key);
  void clear();

  // Debug functions
  void dumpAll() const;
  std::size_t count() const;

  // Initialize Variable
  template <typename T> T init(const std::string &key, const T &value) {
    if (exists(key))
      return get<T>(key);
    set(key, value);
    return value;
  };

private:
  Storage() = default; // Private constructor
  static Storage *instancePtr;
  static std::mutex mutex_;
  std::unordered_map<std::string, VariantType> variables_;
};

// Convenience macros for easier acces
#define GLOBAL_SET(key, value)                                                 \
  variables::Storage::getInstance()->set(key, value)
#define GLOBAL_GET(type, key) variables::Storage::getInstance()->get<type>(key)
#define GLOBAL_EXISTS(key) variables::Storage::getInstance()->exists(key)
#define GLOBAL_INIT(type, key, value)                                          \
  variables::Storage::getInstance()->init<type>(key, value)

} // namespace variables
