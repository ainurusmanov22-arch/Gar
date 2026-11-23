#include <iostream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    return 0;
}