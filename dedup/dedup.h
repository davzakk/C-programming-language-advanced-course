#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>

using std::string;

std::vector<std::unique_ptr<string>> Duplicate(const std::vector<std::shared_ptr<string>>& items) {
    std::vector<string> vals;
    std::vector<std::unique_ptr<string>> ans;
    for (auto i : items) {
        vals.push_back(string(*i));
    }
    for (int i = 0; i < static_cast<int>(vals.size()); ++i) {
        ans.push_back(std::make_unique<string>(vals[i]));
    }
    return ans;
}

std::vector<std::shared_ptr<string>> DeDuplicate(
    const std::vector<std::unique_ptr<string>>& items) {
    std::vector<std::shared_ptr<string>> ans;
    std::map<string, string> vals;
    std::map<string, std::shared_ptr<string>> aaa;
    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        string x = *items[i];
        vals[x] = x;
        aaa[x] = std::make_shared<string>(x);
    }
    for (int i = 0; i < static_cast<int>(items.size()); ++i) {
        ans.push_back(aaa[*items[i]]);
    }
    return ans;
}
