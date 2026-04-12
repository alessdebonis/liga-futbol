#include "league.h"

int main() {
    ConfigLeague config = readConfig("data/config.txt");
    runProgram(config);
    return 0;
}