#include <iostream>
#include "CLFileHandle.h"
using namespace std;

int main() {
    CLFileHandle* fh = new CLFileHandle("test.txt");
    // fh->WriteToFile("hello world");
    fh->WriteToFile("hello world,hello world,hello world,hello world,hello world,hello world,hello world");
    fh->ReadFromFile(2, 5);
}