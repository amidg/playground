#include <iostream>
#include <string>
#include "tree/tree.h"

const std::string randomWords[50] = {
    "apple", "banana", "cherry", "date", "elderberry",
    "fig", "grape", "honeydew", "kiwi", "lemon",
    "apple", "banana", "cherry", "date", "elderberry",
    "fig", "grape", "honeydew", "kiwi", "lemon",
    "apple", "banana", "cherry", "date", "elderberry",
    "fig", "grape", "honeydew", "kiwi", "lemon",
    "apple", "banana", "cherry", "date", "elderberry",
    "fig", "grape", "honeydew", "kiwi", "lemon"
};

int main(int argc, char* argv[]) {
    Tree* root = new Tree(randomWords[0]);
    Tree* next_word = root;
    for (int i = 1; i < (int)(sizeof(randomWords)/sizeof(randomWords[0])); i++) {
        if (randomWords[i] == root->get_word()) {
            root->update_count(i);
        } else if (randomWords[i] < )

    }
    return 0;
}
