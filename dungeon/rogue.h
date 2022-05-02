#pragma once

#include "dungeon.h"

#include <stdexcept>
#include <catch.hpp>
#include <util.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <set>

std::set<std::string> keys;
std::vector<Room*> used, prev_used;
Room* ans = nullptr;

bool In(Room* r) {
    for (Room* i : used) {
        if (r == i) {
            return true;
        }
    }
    return false;
}

void Dfs(Room* room) {
    if (room->IsFinal()) {
        ans = room;
    }
    if (ans != nullptr) {
        return;
    }
    if (!In(room)) {
        used.push_back(room);
    }
    for (size_t i = 0; i < room->NumKeys(); ++i) {
        keys.insert(room->GetKey(i));
    }

    for (size_t i = 0; i < room->NumDoors(); ++i) {
        if (room->GetDoor(i)->IsOpen()) {
            if (!In(room->GetDoor(i)->GoThrough())) {
                Dfs(room->GetDoor(i)->GoThrough());
            }
        } else {
            for (std::string j : keys) {
                if (room->GetDoor(i)->TryOpen(j)) {
                    if (!In(room->GetDoor(i)->GoThrough())) {
                        Dfs(room->GetDoor(i)->GoThrough());
                    }
                    break;
                }
            }
        }
    }
}

Room* FindFinalRoom(Room* starting_room) {
    keys = {}, used = {}, prev_used = {}, ans = nullptr;
    while (true) {
        prev_used = used;
        used = {};
        Dfs(starting_room);
        if (ans != nullptr || prev_used == used) {
            return ans;
        }
    }
}
