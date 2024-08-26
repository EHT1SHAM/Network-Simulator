#pragma once
#include <iostream>

template <typename T>
class QueueNode {
    public:
    T value;
    QueueNode* next;

    QueueNode() {
        value = {};
        next = NULL;
    }
};

template <class T>
class Queue {
    QueueNode<T>* front;
    QueueNode<T>* rear;

public:
    Queue() {
        front = NULL;
        rear = NULL;
    }
    bool isEmpty() {
        return !(front && rear);
    }
    void enqueue(T& data) {
        if (isEmpty()) {
            QueueNode<T>* toInsert = new QueueNode<T>;
            toInsert->value = data;
            front = toInsert;
            rear = toInsert;
            return;
        }
        QueueNode<T>* toInsert = new QueueNode<T>;
        toInsert->value = data;
        rear->next = toInsert;
        rear = rear->next;
        return;
    }
    void dequeue() {
        if (isEmpty()) {
            return;
        }
        if (front == rear) {
            QueueNode<T>* toDelete = front;
            delete toDelete;
            toDelete = NULL;
            front = NULL;
            rear = NULL;
            return;
        }
        QueueNode<T>* toDelete = front;
        front = front->next;
        delete toDelete;
        toDelete = NULL;
        
    }
    void virtualDequeue() {
        if (isEmpty()) {
            return;
        }
        if (front == rear) {
            //QueueNode<T>* toDelete = front;
            //delete toDelete;
            //toDelete = NULL;
            front = NULL;
            rear = NULL;
            return;
        }
        //QueueNode<T>* toDelete = front;
        front = front->next;
        //delete toDelete;
        //toDelete = NULL;
    }
    T Front() {
        return front->value;
    }
    void makeNull() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    void print() {
        QueueNode<T>* temp = front;

        while (temp) {
            std::cout << temp->value << std::endl;
            temp = temp->next;
        }
    }
    int getQueueSize() {

        QueueNode<T>* temp = front;
        int size = 0;
        while (temp) {

            //std::cout << size << std::endl;
            size++;
            temp = temp->next;
        }
        return size;
    }
    ~Queue() {
        makeNull();
    }
};
