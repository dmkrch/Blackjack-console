#pragma once

#include "Server.hpp"
#include "Table.hpp"
#include "Consts.hpp"

#include <vector>
#include <thread>
#include <string>

class Casino {
public:
    Casino();
    void run();

private:
    Server* _server;
    std::vector<Table> _tables;
    std::vector<std::thread> _tables_threads;
    int _maxTables;
};