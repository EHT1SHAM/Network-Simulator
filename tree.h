#pragma once
#include "Queue.h"
#include <iostream>
#include <iomanip>
template <typename T>
class SplayNode { //splay tree is self adjusting bin tree ie avl tree
public:
	T data;
	SplayNode* left;
	SplayNode* right;

	SplayNode() {
		data = {};
		left = NULL;
		right = NULL;
	}
    SplayNode(T data) {
        data = data;
    }
};

template <class T>
class SplayTree {

	SplayNode<T>* root;
public:
	SplayTree() {
		root = NULL;
	}
    SplayNode<T>* getRoot() {
        return root;
    }
    void findMax() {
        if (!root) {
            return;
        }
        SplayNode<T>* searchPtr = root;
        while (searchPtr->right) {
            searchPtr = searchPtr->right;
        }
        splay(searchPtr->data);
    }
    void findMin() {
        if (!root) {
            return;
        }
        SplayNode<T>* searchPtr = root;
        while (searchPtr->left) {
            searchPtr = searchPtr->left;
        }
        splay(searchPtr->data);
    }
    void insert(T data) {
        root = insert(root, data);
    }

    void deleteTree() {
        findMax();
        while (root) {
            remove(root->data);
        }
    }


    SplayNode<T>* insert(SplayNode<T>* nodePtr, T data) {//inserts a node into the tree
        if (!nodePtr) {
            nodePtr = new SplayNode<T>;
            nodePtr->data = data;
            return nodePtr;
        }
        nodePtr = splay(nodePtr, data);

        if (nodePtr->data == data) {
            return nodePtr;
        }
        SplayNode<T>* newNode = new SplayNode<T>;
        newNode->data = data;


        if (nodePtr->data > data) {
            newNode->right = nodePtr;
            newNode->left = nodePtr->left;
            nodePtr->left = NULL;
        }
        else {
            newNode->left = nodePtr;
            newNode->right = nodePtr->right;
            nodePtr->right = NULL;
        }

        return newNode;
    }
    
    void remove(T data) {
        root = remove(root, data);
    }
    SplayNode<T>* remove(SplayNode<T>* nodePtr, T data) {
        SplayNode<T>* tempTree = new SplayNode<T>;
        if (!nodePtr) {
            return NULL;
        }
        nodePtr = splay(nodePtr, data);
        if (nodePtr->data != data) {
            return nodePtr;
        }

        if (!root->right && !root->left && root->data == data) {
            delete root;
            return NULL;
        }

        if (!nodePtr->left) {
            tempTree = nodePtr->right;
        }
        else {
            tempTree = splay(nodePtr->left, data);
            tempTree->right = nodePtr->right;
        }
        delete nodePtr;
        return tempTree;
    }

    void splay(T data) {
        root = splay(root, data);
    }
    SplayNode<T>* doubleRotateLeft(SplayNode<T>* toRotate) {
        toRotate->left = rotateRight(toRotate->left);

        return rotateLeft(toRotate);
    }
    SplayNode<T>* splay(SplayNode<T>* nodePtr, T data) { 
        
        if (!nodePtr) {
            return nodePtr;
        }

        SplayNode<T>* tempTree = new SplayNode<T>;
        SplayNode<T>* rightTreeTemp = NULL, *leftTreeTemp = NULL;
        rightTreeTemp = leftTreeTemp = tempTree;


        while (1) {
            if (data < nodePtr->data) {
                if (!nodePtr->left) {
                    break;
                }
                if (data < nodePtr->left->data) {
                    nodePtr = rotateLeft(nodePtr);
                }
                if (!nodePtr->left) {
                    break;
                }
                rightTreeTemp->left = nodePtr;
                rightTreeTemp = nodePtr;
                nodePtr = nodePtr->left;
            }
            else if (data > nodePtr->data) {
                if (!nodePtr->right) {
                    break;
                }
                if (data > nodePtr->right->data) {
                    nodePtr = rotateRight(nodePtr);
                }
                if (!nodePtr->right) {
                    break;
                }
                leftTreeTemp->right = nodePtr;
                leftTreeTemp = nodePtr;
                nodePtr = nodePtr->right;
            }
            else {
                break;
            }
        }
        leftTreeTemp->right = nodePtr->left;
        rightTreeTemp->left = nodePtr->right;
        nodePtr->left = tempTree->right;
        nodePtr->right = tempTree->left;

        return nodePtr;
    }
    

    SplayNode<T>* doubleRotateRight(SplayNode<T>* toRotate) {
        toRotate->right = rotateLeft(toRotate->right);

        return rotateRight(toRotate);
    }

    SplayNode<T>* rotateRight(SplayNode<T>* toRotate) {

        SplayNode<T>* temp = toRotate->right;
        toRotate->right = temp->left;
        temp->left = toRotate;

        return temp;
    }
   void levelOrderTraversal() {
        if (!root) {
            return;
        }

        Queue<SplayNode<T>*>* treeQueue = new Queue<SplayNode<T>*>;
        treeQueue->enqueue(root);
        while (!treeQueue->isEmpty()) {
            SplayNode<T>* currNode = treeQueue->Front();
            std::cout  << "--> ";
            std::cout << std::setw(8) << currNode->data << std::endl;
            treeQueue->dequeue();

            if (currNode->left) {
                treeQueue->enqueue(currNode->left);
            }

            if (currNode->right) {
                treeQueue->enqueue(currNode->right);
            }


        }
        delete treeQueue;
    }
    SplayNode<T>* rotateLeft(SplayNode<T>* toRotate) {

        SplayNode<T>* temp = toRotate->left;
        toRotate->left = temp->right;
        temp->right = toRotate;

        return temp;
    }
    void preorder(SplayNode<T>* r) {

        if (r != NULL) {
            preorder(r->left);
            std::cout << r->data << std::endl;
            preorder(r->right);
        }

        else {
            return;
        }
    }

 

    ~SplayTree() {
        deleteTree();
    }
};