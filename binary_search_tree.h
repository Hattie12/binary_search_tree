/*BST, by  HP*/

#ifndef BinarySearchTree_H
#define BinarySearchTree_H

#include <iostream>
#include <fstream>
#include <functional>

using namespace std;
template<class T> class BinarySearchTree
{
public:
    //init to null
    BinarySearchTree() { root = nullptr; }

    //destructor
    ~BinarySearchTree() {
        bool isLeft = false;
        filterAndRemove([](T t) -> bool {return true; });
    }

    //public alias functions
    void insert(const T& t) {
        _insert(root, t);
    }

    void traverse(vector<T>& vec) const {
        _traverse(root, vec);
        return;
    }
    void deleteNode(const T& val) {
        _findandDeleteNode(root, val);
        return;
    }
    void applyToInterval(const T& a, const T& b, function<void(const T)> fun) const {
        _applyToInterval(root, a, b, fun);
        return;
    }
    void filterAndRemove(function<bool(const T)> isIn) {
        _filterAndRemove(root, isIn);
        return;
    }

    bool verifyBinarySearchTree() const {
        return (_verifyBinarySearchTree(root));
    }


private: 

    //(Node struct is inspired by (but modified) from one given to my algorithms class)
    struct Node
    {
        friend class BinarySearchTree;

        //constructors
        Node() {leftLeaf = nullptr;  rightLeaf = nullptr;}
        Node(const T& num) {nodeData = num;  leftLeaf = nullptr;  rightLeaf = nullptr;}
        Node(Node* leftLeaf, Node* rightLeaf, const T& num) : leftLeaf{leftLeaf}, rightLeaf{rightLeaf}, nodeData{num} {}

        //left leaf, right leaf, actual number
        T     nodeData;
        Node* leftLeaf;
        Node* rightLeaf;
    };

    Node* root;

    //checks that the tree is in order
    //takes the root
    //returns t/f whether the tree is correct
    bool _verifyBinarySearchTree(Node* root) const {

        vector<T> orderedVec; //holder vec
        _traverse(root, orderedVec); //fill up vec with order of bst

        vector<int>::iterator smallEle = begin(orderedVec); //start itr

        //make sure every element is less than the next
        for (; smallEle + 1 < end(orderedVec); smallEle++) {
            if (!(*smallEle < *(smallEle + 1))) {
                //if its not, the tree is not sorted
                return false;
            }
        }

        return true;
    }

    //applies func to each value in tree within a range
      //takes the tree root, lower bound, upper bound, and the func to be applied
      //returns nothing
    void _applyToInterval(Node* currentNode, const T& a, const T& b, function<void(const T)> func) const {

        //if the tree is empty
        if (currentNode == nullptr) {
            //do nothing
            return;
        }

        //if its right of correct range, move left
        if (currentNode->nodeData > b) _applyToInterval(currentNode->leftLeaf, a, b, func);

        //if its left of correct range, move right
        else if (currentNode->nodeData < a) _applyToInterval(currentNode->rightLeaf, a, b, func);

        //otherwise if the pointer is in the range
        else {
            //apply func
            func(currentNode->nodeData);

            //check on left/right
            _applyToInterval(currentNode->rightLeaf, a, b, func);
            _applyToInterval(currentNode->leftLeaf, a, b, func);
        }

        return;
    }

    //deletes nodes where the function returns true
    //takes the root, the root of the root, the function, and whether root is left node of its root
    //returns nothing
    void _filterAndRemove(Node*& root, function<bool(const T)> isIn) {

        //if the tree is empty
        if (root == nullptr) {
            //do nothing
            return;
        }

        //move left/right
        _filterAndRemove(root->leftLeaf, isIn);
        _filterAndRemove(root->rightLeaf, isIn);

        //if root is bad
        if (!isIn(root->nodeData)) {
            //delete it 
            _deleteNode(root);
        }

        return;
    }

    //finds and deletes value
    //takes root and val to delete
    //returns nothing
    void _findandDeleteNode(Node*& p, const T& val) {
        //if its an empty tree
        if (p == nullptr) {
            //do nothing
            return;
        }
        //if the bad node is to the left
        if (p->nodeData > val) {
            //move left
            _findandDeleteNode(p->leftLeaf, val);
        }

        //if the bad node is to the right
        else if (p->nodeData < val) {
            //move right
            _findandDeleteNode(p->rightLeaf, val);
        }
        else
        {
            _deleteNode(p);
        }


        return;
    }

    //deletes a node
    //takes pointer to a node pointer of node to delete
    //returns nothing
    void _deleteNode(Node*& badNode) {

        //if the tree is empty
        if (badNode == nullptr) {
            return;
        }

        //if the node has two children
        if ((badNode)->leftLeaf != nullptr && (badNode)->rightLeaf != nullptr) {
            //set the value of the current node equal to the smallest on the right side
            (badNode)->nodeData = _findSmallestNode((badNode)->rightLeaf)->nodeData;

            //and delete the smallestNode
            _findandDeleteNode(badNode->rightLeaf, badNode->nodeData);
        }

        //otherwise the node has either a left or right child or no children
        else {
            //new an old node to store badNode
            Node* tempNode = badNode;

            //replace badNode with its left/right kid directly
            badNode = (((badNode)->leftLeaf == nullptr) ? ((badNode)->rightLeaf) : ((badNode)->leftLeaf));

            //delete tempNode for no memory leakage
            delete tempNode;
        }

        return;
    }

    //finds the smallest node under the root
    //takes root under which to find the smallest node
    //returns pointer to node pointer of smallest root
    Node* _findSmallestNode(Node*& leftNode) {

        //if there is a node to the left
        if ((leftNode)->leftLeaf != nullptr) {
            //follow it
            _findSmallestNode(((leftNode)->leftLeaf));
        }
        //otherwise this is the leftmost node
        else {
            //and therefore the smallest
            return (leftNode);
        }
    }

    //traverses the three inorder, modifies vec input to contain ordered elements
    //takes root and vec by reference
    //returns nothing
    void _traverse(Node* currentNode, vector<T>& orderVec) const {

        //if the tree is empty
        if (currentNode == nullptr) {
            //do nothing
            return;
        }
        //if root has a left node
        if (currentNode->leftLeaf != nullptr) {
            //if it has a left node, that must be traversed first
            _traverse(currentNode->leftLeaf, orderVec);
        }

        //time to record the value of root node
        orderVec.push_back(currentNode->nodeData);

        //if root has a right node
        if (currentNode->rightLeaf != nullptr) {
            //it should be traversed last
            _traverse(currentNode->rightLeaf, orderVec);
        }

        return;
    }

    //inserts value into the tree
    //takes the value to insert and the root
    //returns nothing
    void _insert(Node*& currentNode, const T t) {

        //if its the very first node
        if (currentNode == nullptr) {
            currentNode = new Node(t);
            return;
        }

        //if the new data should be to the left
        if (currentNode->nodeData > t) {
            //send it to the left
            _insert(currentNode->leftLeaf, t);
        }
        //otherwise the new node should go to the right
        else {
            //send it to the right
            _insert(currentNode->rightLeaf, t);
        }
    }
};


#endif
