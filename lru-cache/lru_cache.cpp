#include "lru_cache.h"
#include <string>
#include <unordered_map>
#include <list>
#include <iterator>

LruCache::LruCache(size_t max_size) {
    max_sz_ = max_size;
    ls_ = std::list<std::pair<std::string, std::string>>();
    m_ =
        std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>::iterator>();
}

void LruCache::Set(const std::string& key, const std::string& value) {
    if (m_.find(key) != m_.end()) {
        ls_.erase(m_[key]);
        ls_.push_back({key, value});
        m_[key] = --ls_.end();
    } else {
        ls_.push_back({key, value});
        m_[key] = --ls_.end();
        if (m_.size() > max_sz_) {
            m_.erase(ls_.begin()->first);
            ls_.pop_front();
        }
    }
}

bool LruCache::Get(const std::string& key, std::string* value) {  // может тут аперсант
    if (m_.find(key) != m_.end()) {
        *value = m_[key]->second;
        ls_.push_back({key, *value});
        ls_.erase(m_[key]);
        m_[key] = --ls_.end();
        return true;
    }
    return false;
}
