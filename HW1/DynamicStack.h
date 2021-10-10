#ifndef DYNAMICSTACK_H
#define DYNAMICSTACK_H

/* The stack implementation was copied and slightly modified from 4-StacksAndQueues.ppt */
/* An Underflow class was added to the copied code for it to work as intended */

class Underflow {
public:
    Underflow()
        : message("ERROR: Stack is empty.")
    {}

    const char* what() { return message; }

private:
    const char* message;
};

template <class Object>
class Stack {

public:
    Stack();
    Stack(const Stack& rhs);
    ~Stack();

    bool isEmpty() const;
    bool isFull() const;
    void makeEmpty();

    void pop();
    void push(const Object& x);
    Object topAndPop();
    const Object & top() const;

    const Stack & operator=(const Stack& rhs);

private:
    struct ListNode {
        Object element;
        ListNode* next;

        ListNode(const Object& theElement, ListNode* n = NULL)
            : element(theElement), next(n) { }
    };

    ListNode* topOfStack;
};

#include "DynamicStack.cpp"  
#endif