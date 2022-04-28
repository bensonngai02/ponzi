#include "stack.h"
#include "atom.h"


class Stack {
    public:
        Node * head;

    Stack(){
        this->head->atom = "nil";
        this->head->next = NULL;
    }

    void push(Atom new_atom) {
        Node * new_node = malloc(sizeof(*new_node));
        new_node->atom = new_atom;
        new_node->next = head;
        this->head = newNode;
    }

    Node * pop(){
        Node * popped = this->head;
        head = head->next;
        return popped;
    }

    void replace(Atom new_atom){
        this->head->atom = new_atom;
    }

    Node * peek(){
        return this->head;
    }

    void print(){
        fprintf(stderr, "PRINTING STACK: ");
        while (this->head != NULL) {
            if (isalnum(this->atom)) {
                if (isdigit(this->atom))
                    fprintf(stderr, "%ld -> ",head->atom);
                else 
                    fprintf(stderr, "%s ->", head->atom);
            }
            else
                (head->atom) ? fprintf(stderr, "true -> ") : fprintf(srderr, "false -> ");
            head = head->next;
        }
        fprintf(stderr, "\n---------------------\n");
    }       
};




