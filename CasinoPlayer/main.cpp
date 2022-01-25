#include <iostream>
#include <vector>
#include <thread>
#include "Client.hpp"
#include "CasinoPlayer.hpp"

int main() {
    try
    {
        CasinoPlayer player;
        player.run();
           
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
