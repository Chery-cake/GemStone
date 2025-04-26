#include "variables.h"

#include <iostream>
#include <mutex>
#include <string>

using namespace variables;
using std::lock_guard, std::mutex, std::string;

Storage *Storage::instancePtr = nullptr;
mutex Storage::mutex_ = mutex();

// Singleton instance access
Storage *Storage::getInstance() {
  lock_guard<mutex> lock(mutex_);
  if (instancePtr == nullptr) {
    instancePtr = new Storage();
  }
  return instancePtr;
}

bool Storage::exists(const string &key) const {
  lock_guard<mutex> lock(mutex_);
  return variables_.find(key) != variables_.end();
}

void Storage::remove(const string &key) {
  lock_guard<mutex> lock(mutex_);
  variables_.erase(key);
}

void Storage::clear() {
  lock_guard<mutex> lock(mutex_);
  variables_.clear();
}

void Storage::dumpAll() const {
  lock_guard<mutex> lock(mutex_);
  std::cout << "=== Global Store Contents ===" << std::endl;
  for (const auto &[key, value] : variables_) {
    std::cout << key << " = ";
    std::visit([](auto &&arg) { std::cout << arg; }, value);
    std::cout << std::endl;
  }
}

size_t Storage::count() const {
  lock_guard<mutex> lock(mutex_);
  return variables_.size();
}
