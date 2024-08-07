#include <iostream>

template <typename T>
class Node {
    T value;
    std::unique_ptr<Node> next;
    std::weak_ptr<Node> prev; // prevents circular references
public:
    Node() { next = nullptr; prev = nullptr; };
    Node(T value): value(value) { next = nullptr; prev = nullptr; };
    Node(const Node& other) {
        value = other.value;
        std::unique_ptr<Node> new_next(new Node<T>);

    }
    Node(Node&& other) {
        if (*this == other) return;
        delete next;
        delete prev;
        this->next(std::move(other->next));
        this->prev(std::move(other->prev));
        this->value = other.value;
        other->next = nullptr; // can also use delete
        other->prev = nullptr;
        other->value = 0;
    }
    ~Node() { delete this->next; delete this->prev; };

    // value  manipulation
    const T get_value() const { return value; };
    void set_value(T value) { value = value; };

    // overloads
    Node& operator=(Node&& other) {
        if (this != &other) {
            // Free existing resource
            delete prev;
            delete next;

            // Transfer ownership of the resource
            prev = other.prev;
            next = other.next;
            value = other.value;

            // Nullify the source
            other.prev = nullptr;
            other.next = nullptr;
            other.value = 0;
        }
        return *this;
    }

    // manipulate nodes
    bool add_node(Node&& n) {
        // find last node in sequence
        if (next == nullptr) {
            Node<T>* temp = new Node<T>(n);
            next = temp;
            temp->prev = this;
        } else
            add_node(n);
    }

    // read data
    int get_depth() {
        int result = 0;
        while (next != nullptr)
            ++result;
        return result;
    }
};

int main(int argc, char* argv[]) {
    Node<int>* root = new Node<int>(100);
    std::cout << root->get_value() << std::endl;

    Node<int>* temp;
    for (int i = 1; i < 100; i++) {
        temp = new Node<int>(i+100);
        root->add_node(temp);
    }

    std::cout << root->get_depth() << std::endl;
    return 0;
}
