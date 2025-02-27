// Jacob Bradley
// 2/27/25
// Purpose: To implement a binary search tree (BST) with asymmetric and symmetric deletion methods, and to analyze the Internal Path Length (IPL) of the BST after a series of insertions and deletions. The program also generates a DOT file for visualization using Graphviz.
// The program includes a utility for generating random integers, checking for duplicates, and recording IPL history during the experiment. The main function runs the experiment for different tree sizes and deletion methods, outputting the IPL values at specified intervals.


#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <unordered_set>

using namespace std;

// Node structure for the BST.
struct Node {
    int data;
    Node *left;
    Node *right;
    Node(int x) : data(x), left(nullptr), right(nullptr) {}
};

// Graphviz utility for visualization.
class GraphvizBST {
public:
    static void saveDotFile(const std::string &filename, const std::string &dotContent) {
        std::ofstream outFile(filename);
        if (outFile.is_open()) {
            outFile << dotContent;
            outFile.close();
            std::cout << "DOT file saved: " << filename << std::endl;
        } else {
            std::cerr << "Error: Could not open file " << filename << std::endl;
        }
    }

    static std::string generateDot(const Node *root) {
        std::string dot = "digraph BST {\n";
        dot += "    node [fontname=\"Arial\"];\n";
        dot += generateDotHelper(root);
        dot += "}\n";
        return dot;
    }

private:
    static std::string generateDotHelper(const Node *node) {
        if (!node)
            return "";
        std::string result;
        if (node->left) {
            result += "    " + std::to_string(node->data) + " -> " + std::to_string(node->left->data) + " [label=\"L\"];\n";
            result += generateDotHelper(node->left);
        } else {
            std::string nullNode = "nullL" + std::to_string(node->data);
            result += "    " + nullNode + " [shape=point];\n";
            result += "    " + std::to_string(node->data) + " -> " + nullNode + ";\n";
        }
        if (node->right) {
            result += "    " + std::to_string(node->data) + " -> " + std::to_string(node->right->data) + " [label=\"R\"];\n";
            result += generateDotHelper(node->right);
        } else {
            std::string nullNode = "nullR" + std::to_string(node->data);
            result += "    " + nullNode + " [shape=point];\n";
            result += "    " + std::to_string(node->data) + " -> " + nullNode + ";\n";
        }
        return result;
    }
};

// Binary Search Tree class.
class Bst {
    Node *root;
    // Used for symmetric deletion toggle: true means use successor; false means use predecessor.
    bool symmetricToggle;

    // Helper function for insertion.
    void _insert(Node *&subroot, int x) {
        if (!subroot) {
            subroot = new Node(x);
        } else {
            if (x < subroot->data) {
                _insert(subroot->left, x);
            } else {
                _insert(subroot->right, x);
            }
        }
    }

    // Internal function to compute Internal Path Length (IPL).
    int _ipl(Node *node, int depth = 0) const {
        if (!node)
            return 0;
        return depth + _ipl(node->left, depth + 1) + _ipl(node->right, depth + 1);
    }

    // Find minimum (leftmost) node.
    Node* findMin(Node *node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }
    
    // Find maximum (rightmost) node.
    Node* findMax(Node *node) {
        while (node && node->right) {
            node = node->right;
        }
        return node;
    }

    // Asymmetric deletion helper: always replace a node with two children using its successor.
    Node* _deleteAsymmetric(Node *&node, int key) {
        if (!node) return node;
        if (key < node->data) {
            node->left = _deleteAsymmetric(node->left, key);
        } else if (key > node->data) {
            node->right = _deleteAsymmetric(node->right, key);
        } else { // found node to delete
            if (!node->left) {
                Node *temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node *temp = node->left;
                delete node;
                return temp;
            } else {
                // Always use successor.
                Node *succ = findMin(node->right);
                node->data = succ->data;
                node->right = _deleteAsymmetric(node->right, succ->data);
            }
        }
        return node;
    }

    // Symmetric deletion helper: for a node with two children, alternate between successor and predecessor.
    Node* _deleteSymmetric(Node *&node, int key) {
        if (!node) return node;
        if (key < node->data) {
            node->left = _deleteSymmetric(node->left, key);
        } else if (key > node->data) {
            node->right = _deleteSymmetric(node->right, key);
        } else { // found node to delete
            if (!node->left) {
                Node *temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node *temp = node->left;
                delete node;
                return temp;
            } else {
                if (symmetricToggle) {
                    // Replace with successor.
                    Node *succ = findMin(node->right);
                    node->data = succ->data;
                    node->right = _deleteSymmetric(node->right, succ->data);
                } else {
                    // Replace with predecessor.
                    Node *pred = findMax(node->left);
                    node->data = pred->data;
                    node->left = _deleteSymmetric(node->left, pred->data);
                }
            }
        }
        return node;
    }

public:
    Bst() : root(nullptr), symmetricToggle(true) {}

    // Insert a value into the BST.
    void insert(int x) {
        _insert(root, x);
    }

    // Delete using asymmetric deletion (always using successor replacement).
    void deleteAsymmetric(int key) {
        root = _deleteAsymmetric(root, key);
    }

    // Delete using symmetric deletion (alternates between successor and predecessor).
    void deleteSymmetric(int key) {
        root = _deleteSymmetric(root, key);
        symmetricToggle = !symmetricToggle;
    }

    // Compute the Internal Path Length (IPL).
    int ipl() const {
        return _ipl(root);
    }

    // Optional: Inorder print for debugging.
    void print() const {
        _print(root);
        cout << endl;
    }

    // Save the BST as a DOT file for Graphviz.
    void saveDotFile(const std::string &filename) {
        std::string dotContent = GraphvizBST::generateDot(root);
        GraphvizBST::saveDotFile(filename, dotContent);
    }
    
private:
    void _print(Node *node) const {
        if (!node)
            return;
        _print(node->left);
        cout << node->data << " ";
        _print(node->right);
    }
};

// Utility function to generate a random integer within a range.
int getRandom(int low, int high) {
    return low + rand() % (high - low + 1);
}

// Checks if the key is already present in the vector.
bool keyExists(const vector<int>& keys, int key) {
    return std::find(keys.begin(), keys.end(), key) != keys.end();
}

// Runs an experiment on a BST of size n with a given deletion method, while recording IPL history.
// deletionMethod: 'A' for asymmetric, 'S' for symmetric.
// numCycles: total number of I/D pairs.
// recordInterval: record the IPL every 'recordInterval' cycles.
vector<int> runExperimentHistory(int n, char deletionMethod, int numCycles, int recordInterval) {
    Bst tree;
    vector<int> keys;
    vector<int> iplHistory;
    int maxRange = n * 10;

    // Populate the tree with n unique random values.
    while (keys.size() < (size_t)n) {
        int r = getRandom(0, maxRange);
        if (!keyExists(keys, r)) {
            keys.push_back(r);
            tree.insert(r);
        }
    }
    // Record initial IPL (0 I/D pairs).
    iplHistory.push_back(tree.ipl());

    // Perform I/D cycles.
    for (int i = 1; i <= numCycles; i++) {
        // Randomly select a key to delete.
        int idx = getRandom(0, keys.size() - 1);
        int keyToDelete = keys[idx];

        // Delete using the chosen method.
        if (deletionMethod == 'A') {
            tree.deleteAsymmetric(keyToDelete);
        } else if (deletionMethod == 'S') {
            tree.deleteSymmetric(keyToDelete);
        }

        // Remove the key from our vector.
        keys[idx] = keys.back();
        keys.pop_back();

        // Insert a new unique random value.
        int newKey;
        do {
            newKey = getRandom(0, maxRange);
        } while (keyExists(keys, newKey));
        keys.push_back(newKey);
        tree.insert(newKey);

        // Record IPL every recordInterval cycles.
        if (i % recordInterval == 0) {
            iplHistory.push_back(tree.ipl());
        }
    }

    // Save the final state of the tree as a DOT file.
    // File name includes tree size and deletion method.
    string filename = "bst_size_" + to_string(n) + "_" + (deletionMethod == 'A' ? "asym" : "sym") + ".dot";
    tree.saveDotFile(filename);

    return iplHistory;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    // Define the tree sizes to experiment with.
    vector<int> treeSizes = {64, 128, 256, 512, 1024, 2048};

    // Experiment parameters.
    int numCycles = 1000;         // Total number of I/D pairs per experiment.
    int recordInterval = 100;     // Record the IPL every 100 cycles.

    // Deletion Methods: 'A' (asymmetric) and 'S' (symmetric).
    for (char method : {'A', 'S'}) {
        cout << "=== Experiment: " << (method == 'A' ? "Asymmetric Deletion" : "Symmetric Deletion") << " ===" << endl;
        for (int n : treeSizes) {
            // Run the experiment and get the IPL history.
            vector<int> iplHistory = runExperimentHistory(n, method, numCycles, recordInterval);

            // Output the recorded IPL values.
            cout << "Tree Size: " << n << ", Deletion Method: " << (method == 'A' ? "Asymmetric" : "Symmetric") << endl;
            cout << "I/D Pair Count\tIPL" << endl;
            int cycle = 0;
            cout << cycle << "\t\t" << iplHistory[0] << endl;
            for (size_t i = 1; i < iplHistory.size(); i++) {
                cycle += recordInterval;
                cout << cycle << "\t\t" << iplHistory[i] << endl;
            }
            cout << endl;
        }
    }

    return 0;
}
