#include <iostream>

int main(int, char**) {
    std::cout << "Hello, world!\n";

    {
        std::cout << "Press any key to continue...\n";
        getchar();
    }

    return 0;
}
