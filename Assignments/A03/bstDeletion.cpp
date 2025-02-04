#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;
struct Node
{
    int data;
    Node *left;
    Node *right;
    Node(int x)
    {
        data = x;
        left = right = nullptr;
    }
};

class GraphvizBST
{
public:
    static void saveDotFile(const std::string &filename, const std::string &dotContent)
    {
        std::ofstream outFile(filename);
        if (outFile.is_open())
        {
            outFile << dotContent;
            outFile.close();
            std::cout << "DOT file saved: " << filename << std::endl;
        }
        else
        {
            std::cerr << "Error: Could not open file " << filename << std::endl;
        }
    }

    static std::string generateDot(const Node *root)
    {
        std::string dot = "digraph BST {\n";
        dot += "    node [fontname=\"Arial\"];\n";
        dot += generateDotHelper(root);
        dot += "}\n";
        return dot;
    }

private:
    static std::string generateDotHelper(const Node *node)
    {
        if (!node)
            return "";
        std::string result;
        if (node->left)
        {
            result += "    " + std::to_string(node->data) + " -> " + std::to_string(node->left->data) + " [label=\"L\"];\n";
            result += generateDotHelper(node->left);
        }
        else
        {
            std::string nullNode = "nullL" + std::to_string(node->data);
            result += "    " + nullNode + " [shape=point];\n";
            result += "    " + std::to_string(node->data) + " -> " + nullNode + ";\n";
        }
        if (node->right)
        {
            result += "    " + std::to_string(node->data) + " -> " + std::to_string(node->right->data) + " [label=\"R\"];\n";
            result += generateDotHelper(node->right);
        }
        else
        {
            std::string nullNode = "nullR" + std::to_string(node->data);
            result += "    " + nullNode + " [shape=point];\n";
            result += "    " + std::to_string(node->data) + " -> " + nullNode + ";\n";
        }
        return result;
    }
};

class Bst
{
    Node *root;

    void _print(Node *subroot)
    {
        if (subroot)
        {
            _print(subroot->left);
            cout << subroot->data << " ";
            _print(subroot->right);
        }
    }

    void _insert(Node *&subroot, int x)
    {
        if (!subroot)
        {
            subroot = new Node(x);
        }
        else
        {
            if (x < subroot->data)
            {
                _insert(subroot->left, x);
            }
            else
            {
                _insert(subroot->right, x);
            }
        }
    }

    void _delete(Node *&root, int key)
    {
        if (!root)
            return;
        if (key < root->data)
        {
            _delete(root->left, key);
        }
        else if (key > root->data)
        {
            
            _delete(root->right, key);

        }
        else if (!root->left || !root->right)
        {
            Node *temp = root->left ? root->left : root->right;
            if (!temp)
            {
                temp = root;
                root = nullptr;
            }
            else
            {
                *root = *temp;
            }
            delete temp;
        }
        else
        {
            Node *successor = _findMin(root->right);
            root->data = successor->data;
            _delete(root->right, successor->data);
        }
    }

    Node *_findMin(Node *node) //function to assist deletion
    {
        while (node->left)
            node = node->left;
        return node;
    }

    int _ipl(Node *root, int depth = 0)
    {
        if (!root)
            return 0; //base case
        return depth + _ipl(root->left, depth + 1) + _ipl(root->right, depth + 1);
    }

public:
    Bst()
    {
        root = nullptr;
    }

    void insert(int x)
    {
        _insert(root, x);
    }

    void print()
    {
        _print(root);
    }

    void deleteNode(int key)
    {
        _delete(root, key);
    }

    void saveDotFile(const std::string &filename)
    {
        std::string dotContent = GraphvizBST::generateDot(root);
        GraphvizBST::saveDotFile(filename, dotContent);
    }

    int ipl()
    {
        return _ipl(root);
    }
};

bool unique_val(int *arr, int n, int x){
    for(int i = 0; i < n; i++){
        if(arr[i] == x){
            return false;
        }
    }
    return true;
}

int main(){
    Bst tree; //creating object of class Bst
    int root = pow(2, 15) / 2; //root of the tree
    int max = pow(2, 15) - 1; //maximum value of the tree
    vector<int> arr; //vector to store the values of the tree
    arr.push_back(root); //pushing the root value to the vector
    tree.insert(root); //inserting the root value to the tree
    for (int i = 1; i < 5000; i++){
        int r = rand() % max;
        while (!unique_val(arr.data(), arr.size(), r)){
            r = rand() % max;
        } //generating random values and checking if they are unique
        tree.insert(r); //inserting the random value to the tree
        tree.deleteNode(r);
        arr.push_back(r); //pushing the random value to the vector
    }
    tree.print(); //printing the tree
    tree.saveDotFile("bst_snapshot.dot"); //saving the tree to a dot file
    

    Bst tree2;
    tree2.insert(10);
    tree2.insert(5);
    tree2.insert(15);
    tree2.insert(2);
    tree2.insert(7);
    tree2.insert(20);
    
    cout << "IPL of the tree is: " << tree2.ipl() << endl;
}
