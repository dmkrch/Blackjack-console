#include <iostream>
#include <vector>
#include <thread>
#include "Client.hpp"

int main() {
    try
    {
        Client cl;
        cl.run();
           
        return 0;
    }
    catch (const std::exception& ex)
    {
 
    }
    catch (...)
    {

    }

    return 0;
}
