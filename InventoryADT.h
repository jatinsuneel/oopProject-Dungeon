#ifndef INVENTORYADT_H
#define INVENTORYADT_H

#include <vector>
#include <string>

using namespace std;

class InventoryADT {
private:
    vector<string> items;

public:
    void add(string item);

    bool contains(string item) const;

    void display() const;
};

#endif