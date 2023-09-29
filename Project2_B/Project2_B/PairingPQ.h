// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure
            // to initialize it here.
            explicit Node(const TYPE &val)
                : elt{ val }, child{ nullptr }, sibling{ nullptr },  previous{nullptr}
            {}

            // Description: Allows access to the element at that Node's
            // position.  There are two versions, getElt() and a dereference
            // operator, use whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data
            // members of this Node class from within the PairingPQ class.
            // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
            // function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            // TODO: Add one extra pointer (parent or previous) as desired.
            Node* previous;
    }; // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root{ nullptr }, count{ 0 } {
        // TODO: Implement this function.
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function.
        root = nullptr;
        count = 0;
        while (start != end) {
            this->addNode(*start);
            start++;
        }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare } {
        // TODO: Implement this function.
        this->count = 0;
        this->root = nullptr;
        std::deque<Node*> dq;
        dq.push_back(other.root);
        while (dq.size() > 0) {
            Node* tmp = dq.front();
            this->push(tmp->getElt());
            dq.pop_front();
            if (tmp->child != nullptr)
                dq.push_back(tmp->child);
            if (tmp->sibling != nullptr)
                dq.push_back(tmp->sibling);
        }
        // NOTE: The structure does not have to be identical to the original,
        //       but it must still be a valid pairing heap.
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        // TODO: Implement this function.
        // HINT: Use the copy-swap method from the "Arrays and Containers"
        // lecture.
        PairingPQ tmp(rhs);
        std::swap(root, tmp.root);
        std::swap(count, tmp.count);
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function.
        if (root == nullptr)
            return;
        std::deque<Node*> dq;
        dq.push_back(root);
        while (dq.size() > 0) {
            Node* tmp = dq.front();
            if (tmp->sibling != nullptr)
                dq.push_back(tmp->sibling);
            if (tmp->child != nullptr)
                dq.push_back(tmp->child);
            dq.pop_front();
            delete tmp;
        }
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant.  You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() { // Problems here
        // TODO: Implement this function.
        if (root == nullptr)
            return;
        std::deque<Node*> dq;
        dq.push_back(root);
        root = nullptr;
        while (dq.size() > 0) {
            Node* cur = dq.front();
            dq.pop_front();
            if (cur->child != nullptr)
                dq.push_back(cur->child);
            if (cur->sibling != nullptr)
                dq.push_back(cur->sibling);
            cur->child = nullptr;
            cur->previous = nullptr;
            cur->sibling = nullptr;
            root = meld(cur, root);
        }
    } // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely
    //              in the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        this->addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    // an element when the pairing heap is empty. Though you are welcome to
    // if you are familiar with them, you do not need to use exceptions in
    // this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
        if (root == nullptr)
            return;
        count--;
        Node* ptr = root->child;
        if (ptr == nullptr) {
            delete root;
            root = nullptr;
            return;
        }
        std::deque<Node*> dq;
        delete root;
        while (ptr != nullptr) {
            dq.push_back(ptr);
            ptr = ptr->sibling;
        }
        // before multi-pass, break the relation between nodes to make melding possible
        for (Node* ptr : dq) {
            ptr->previous = nullptr;
            ptr->sibling = nullptr;
        }
        // start multi-pass
        while (dq.size() > 1) {
            size_t num = dq.size() / 2;
            for (size_t i = 0; i < num; i++) {
                Node* a = dq.front();
                dq.pop_front();
                Node* b = dq.front();
                dq.pop_front();
                dq.push_back(meld(a, b));
            }
        }
        this->root = dq.front();
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function
        return root->getElt();
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function
        return this->count;
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function
        return !this->size();
    } // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value.  Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //              extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {
        // TODO: Implement this function
        bool left_most = false;
        if (node->previous->child == node)
            left_most = true;
        if (!left_most) {
            Node* sib = node->sibling;
            if (sib != nullptr)
                sib->previous = node->previous;
            Node* prev = node->previous;
            prev->sibling = node->sibling;
            node->previous = nullptr;
            node->sibling = nullptr;
            node->elt = new_value;
            meld(root, node);
        }
        else {
            Node* sib = node->sibling;
            if (sib != nullptr)
                sib->previous = node->previous;
            Node* prev = node->previous;
            prev->child = node->sibling;
            node->previous = nullptr;
            node->sibling = nullptr;
            node->elt = new_value;
            meld(root, node);
        }
        
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // Runtime: O(1)
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    Node* addNode(const TYPE &val) {
        // TODO: Implement this function
        Node* ptr = new Node(val);
        root = meld(root, ptr);
        count++;
        return ptr;
    } // addNode()


private:
    // TODO: Add any additional member variables or member functions you
    // require here.
    Node* root;
    size_t count;
    
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap
    // papers).
    Node* meld(Node* pa, Node* pb) {
        if (pa == nullptr)
            return pb;
        if (pb == nullptr)
            return pa;
        // No parent, No previous, No sibiling
        if (pa->sibling != nullptr || pa->previous != nullptr
            || pb->previous != nullptr || pb->sibling != nullptr) {
            return nullptr;
        }
        // if pb has higher priority, pb is gonna be the root
        Node* higher_ptr = nullptr;
        Node* lower_ptr = nullptr;
        if (this->compare(pa->getElt(), pb->getElt())) {
            higher_ptr = pb;
            lower_ptr = pa;
        }
        else {
            higher_ptr = pa;
            lower_ptr = pb;
        }

        if (higher_ptr->child == nullptr) {
            higher_ptr->child = lower_ptr;
            lower_ptr->previous = higher_ptr;
            return higher_ptr;
        }
        // now higher_ptr has a child, we make the older child be a sibiling to the new node
        // and the new node to be the new child of higher_ptr
        else {
            lower_ptr->sibling = higher_ptr->child;
            higher_ptr->child = lower_ptr;
            return higher_ptr;
        }

    }

    // NOTE: For member variables, you are only allowed to add a "root
    //       pointer" and a "count" of the number of nodes. Anything else
    //       (such as a deque) should be declared inside of member functions
    //       as needed.
};


#endif // PAIRINGPQ_H
