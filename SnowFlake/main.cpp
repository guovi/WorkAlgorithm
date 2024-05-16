#include "IdGenerater.hpp"

#include <thread>
#include <vector>
#include <iostream>

using namespace std;

int main() {
    vector<thread> ths;
    for (int i = 0; i < 100; ++i) {
        ths.emplace_back(thread([=](){
            cout << id_generater::Id_Generater::get_instance()->next_id() << endl;
        }));
    }

    for (int i = 0; i < 100; ++i) {
        ths[i].join();
    }
    return 0;
}