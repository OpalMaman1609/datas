// #include "StreamingDBa1.h"
// // #include <iostream>

int main() {
    streaming_database *obj = new streaming_database();
    obj->add_movie(1, Genre::DRAMA, 6, true);
    std::cout << "HELLO?";
    return 0;
    // std::cout << "HELLO?";
}