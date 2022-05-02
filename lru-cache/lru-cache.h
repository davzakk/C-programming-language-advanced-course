#pragma once

#include <string>
#include <unordered_map>
#include <list>
#include <iterator>

class LruCache {
public:
    LruCache(size_t max_size);

    void Set(const std::string& key, const std::string& value);

    bool Get(const std::string& key, std::string* value);

private:
    size_t max_sz_;
    std::list<std::pair<std::string, std::string>> ls_;
    std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>::iterator> m_;
};
