#include "IdGenerater.hpp"

#include <thread>
#include <vector>
#include <iostream>

using namespace std;

int main() {
    cout << "end11" << endl;
    vector<thread> ths;
    for (int i = 0; i < 10; ++i) {
        cout << i << ", " << id_generater::Id_Generater::get_instance()->next_id() << endl;
        ths.emplace_back(thread([](){
            // cout << i << ", " << id_generater::Id_Generater::get_instance()->next_id() << endl;
            cout << 111 << endl;
        }));
    }


    for (int i = 0; i < 10; ++i) {
        ths[i].join();
    }
    cout << "end" << endl;
    return 0;
}