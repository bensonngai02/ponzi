#include "expression.h"
#include "atom.h"

class Expression {
    public:
        Node * head;

    Expression(){
        this->head->atom->is_string = true;
        this->head->atom->val->string = "nil";
        this->head->next = NULL;
    }

    void push(Atom * new_atom) {
        Node * new_node = (Node *) malloc(sizeof(*new_node));
        new_node->atom = new_atom;
        new_node->next = head;
        this->head = new_node;
    }

    Node * pop(){
        Node * popped = this->head;
        this->head = this->head->next;
        return popped;
    }

    void replace(Atom * new_atom){
        this->head->atom = new_atom;
    }

    Node * peek(){
        return this->head;
    }

    void print(){
        fprintf(stderr, "PRINTING STACK: ");
        while (this->head != NULL) {
      
            if (this->head->atom->is_integer)
                fprintf(stderr, "%ld -> ", this->head->atom);
            else if (this->head->atom->is_string)
                fprintf(stderr, "%s ->", this->head->atom);
            else
                (head->atom) ? fprintf(stderr, "true -> ") : fprintf(stderr, "false -> ");
            head = head->next;
        }
        fprintf(stderr, "\n---------------------\n");
    }       
};




