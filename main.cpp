#include <iostream>
#include <cassert>

#define DELFAIL -1
#define DELOK 1

class CList;
class Node;

typedef int Elem_t;

class Node {

public:
    Elem_t data_;

    Node* next_;
    Node* prev_;

    CList* List_;

    void node_dump ();
};

class CList{

public:
    Node* head_;
    Node* tail_;

    int num_of_elem_;

    explicit CList ();
    ~CList ();
    Node* node_ins (Node* elem, Elem_t data);
    int node_del (Node* elem);
    void CList_dump();
    void CList_graph();
};

Node* CList::node_ins(Node* elem, Elem_t data) {

    Node* new_elem = new Node;

    assert(new_elem != nullptr);

    new_elem -> data_ = data;

    this -> num_of_elem_++;

    if (elem == nullptr) {
        this -> head_ = new_elem;
        new_elem -> next_ = nullptr;
        new_elem -> prev_ = nullptr;
        new_elem -> List_ = this;
        return new_elem;
    }

    new_elem -> prev_ = elem;
    new_elem -> next_ = elem -> next_;

    if (elem -> next_ != nullptr)
        elem -> next_ -> prev_ = new_elem;
    else
        this -> tail_ = new_elem;

    elem -> next_ = new_elem;

    new_elem -> List_ = elem -> List_;

    return new_elem;
}

int CList::node_del (Node* elem){

    if (elem == nullptr)
        return DELFAIL;

    if (elem -> next_ == nullptr && elem -> prev_ == nullptr) {
        delete elem;
        this -> head_ = nullptr;
        this -> tail_ = nullptr;
        this -> num_of_elem_--;
        return DELOK;
    }

    if (elem -> prev_ != nullptr)
        elem -> prev_ -> next_ = elem -> next_;
    else
        this -> head_ = elem -> next_;

    if (elem -> next_ != nullptr)
        elem -> next_ -> prev_ = elem -> prev_;
    else
        this -> tail_ = elem -> prev_;

    delete elem;

    this -> num_of_elem_--;

    return DELOK;
}

void Node::node_dump (){

    fprintf(stderr, "Node: 0x%p\n", this);
    fprintf(stderr, "Node data: %d\n", this -> data_);
    fprintf(stderr, "Node next: 0x%p\n", this -> next_);
    fprintf(stderr, "Node prev: 0x%p\n", this -> prev_);
    fprintf(stderr, "Node CList: 0x%p\n", this -> List_);
    fprintf(stderr, "------------------------\n");
}

CList::CList() :
    head_ (nullptr),
    tail_ (nullptr),
    num_of_elem_ (0)
{

}

CList::~CList() {

    Node* elem = this -> tail_;
    Node* save_prev = nullptr;

    if (elem != nullptr) {
        while (elem -> prev_ != nullptr) {
            save_prev = elem -> prev_;
            delete elem;
            elem = save_prev;
        }
    }

    delete this -> head_;
}

void CList::CList_dump() {

    Node* elem = this -> head_;

    fprintf(stderr, "CList: 0x%p\n", this);
    fprintf(stderr, "CList head: 0x%p\n", this -> head_);
    fprintf(stderr, "CList tail: 0x%p\n", this -> tail_);
    fprintf(stderr, "num_of_elem: %d\n\n", this -> num_of_elem_);

    int i = 0;

    while (elem != nullptr && i < this -> num_of_elem_) {
        elem -> Node::node_dump();
        elem = elem -> next_;
        i++;
    }
}

void CList::CList_graph() {

    Node* elem = this -> head_;

    FILE* dms = fopen("dmszsr.txt", "w");
    fprintf(dms, "digraph {\n");
    fprintf(dms, "Node_%p [shape = record, label = \"{ Data = %d | Next = 0x%p | Prev = 0x%p }\"]\n", elem, elem -> data_, elem-> next_, elem -> prev_);
    fprintf(dms, "}");
    fclose(dms);

    system("dot -Tpng -odmszsr.png dmszsr.txt");
    system("start dmszsr.png");
}

int main() {

    CList l1;
    Node* head = l1.node_ins(nullptr, 10);
    Node* elem1 = l1.node_ins(head, 20);
    Node* elem2 = l1.node_ins(elem1, 30);
    Node* elem3 = l1.node_ins(elem2, 40);
    l1.node_del(elem2);
    l1.CList_graph();

    /*FILE* dms = fopen("dmszsr.txt", "w");
    fprintf(dms, "digraph {\n");
    fprintf(dms, "Node_%p [shape = record, label = \"{ Data = %d | Next = 0x%p | Prev = 0x%p }\"]\n", head, head -> data_, head-> next_, head -> prev_);
    fprintf(dms, "}");
    fclose(dms);

    system("dot -Tpng -odmszsr.png dmszsr.txt");
    system("start dmszsr.png"); */

    /*Node* tail = head -> node_ins(20);
    Node* elem1 = head -> node_ins(30);
    Node* elem2 = head -> node_ins(40);
    l1.CList_dump();*/

    return 0;
}
