#include "rbtree.h"

using namespace std;

int main(int argc, char **argv)
{
    unsigned long key = 12;
    int val = 12123; 

    RBT<unsigned long, int> rbtree;

    rbtree.put(key, val);

    return 0;
}