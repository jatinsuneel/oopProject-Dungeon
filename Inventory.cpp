#include "InventoryADT.h"
#include <iostream>
#include <algorithm>

using namespace std;

void InventoryADT::add(string item) {
    items.push_back(item);
}

bool InventoryADT::contains(string item) const {
    return find(items.begin(),
                items.end(),
                item) != items.end();
}

void InventoryADT::display() const {
    cout << "ADT Inventory: ";

    if (items.empty()) {
        cout << "Empty";
    } else {
        for (string item : items) {
            cout << item << " ";
        }
    }

    cout << endl;
}