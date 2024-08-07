#include <iostream>
#include <algorithm>

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Utility function to compute height and update diameter
int height(TreeNode* node, int& diameter) {
    if (node == nullptr) return 0;

    int leftHeight = height(node->left, diameter);
    int rightHeight = height(node->right, diameter);

    // Update diameter if the path through this node is longer
    diameter = std::max(diameter, leftHeight + rightHeight);

    // Return height of the node
    return 1 + std::max(leftHeight, rightHeight);
}

int findDiameter(TreeNode* root) {
    int diameter = 0;
    height(root, diameter);
    return diameter;
}

int main() {
    // Example usage
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->right = new TreeNode(6);

    int diameter = findDiameter(root);
    std::cout << "Diameter of the binary tree is: " << diameter << std::endl;

    // Clean up memory (not shown for brevity)
    return 0;
}
