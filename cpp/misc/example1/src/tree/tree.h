#include <string>
#include <vector>
#ifndef TREE_H
#define TREE_H
class Tree {
    public:
        // constructors
        Tree(): left(nullptr), right(nullptr), word("") {};
        Tree(const std::string& word): left(nullptr), right(nullptr), word(word) {};
        ~Tree() {delete left; delete right;};

        // getters this way guarantee object does not change its state
        Tree* get_left_tree() const { return left; };
        Tree* get_right_tree() const { return right; };
        std::string get_word() const { return word; };
        unsigned int get_count() const { return sizeof(indexes); };

        // setters
        void set_left(Tree* left) { left=left; };
        void set_right(Tree* right) { right=right; };
        void update_count(unsigned int position) {this->indexes.push_back(position);};
    private:
        Tree* left;
        Tree* right;
        std::string word;
        std::vector<unsigned int> indexes;
};
#endif // TREE_H
