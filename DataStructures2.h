#if !defined(HDR)
#define HDR
#include <stdlib.h>
#include <stdio.h>

//////////////////////////////////////////////
/* Structs */
typedef struct Resources {// 0 = false, 1 = true
        int resource_A;
        int resource_B;
        int resource_C;
        int resource_D;
}Resources;

typedef struct Job {
        long mtype;
        int number;
        struct tm *creation_date;
        int execution_time;
        int memory_requirement;
        Resources resources;
}Job;
// structure for message queue
typedef struct message {
        long mtype;
        int memory;
        int P_1;
        Resources resources;
} message;
////////////////////////////////////////////////
/* define Node*/
typedef struct Node {
        Job data;
        struct Node* next;
}Node;

Node* new_node(Job data){
        Node* node = (Node*) calloc(1,sizeof(Node));
        node->data = data;
        return node;
};

/* define LinkedList*/
typedef struct LinkedList {
        Node* head;
}LinkedList;

LinkedList* new_linked_list(){
        LinkedList* list = (LinkedList*) calloc(1,sizeof(LinkedList));
        return list;
}
int size(LinkedList* list){
        int result = 0;
        Node* node = list->head;
        while(node) {
                result++;
                node = node->next;
        }
        return result;
}
int is_empty(LinkedList* list){
        return !list->head;
}


//
// void enqueue(Queue* queue, void* data){
//         insert_at_back(queue->buffer, data);
// }

void insert_at_back(LinkedList* list, Job data){
        Node* node = new_node(data);
        if(!node) {
                return;
                puts("if(!node)");
        }
        Node* tmp = list->head;
        if(!tmp) { // if there are no node in list
                list->head = node;
        }
        else{
                while(tmp->next) {
                        tmp = tmp->next;
                }
                tmp->next = node;
        }
}







Job remove_from_front(LinkedList* list){
        Job result;
        Node* tmp = list->head;
        // if(!tmp)
        //         return NULL;

        result = tmp->data;
        list->head = tmp->next;
        free(tmp);
        return result;
}





/* define Queue*/
typedef struct Queue {
        LinkedList *buffer;
}Queue;
Queue* new_queue(){
        Queue* queue = (Queue*) calloc(1,sizeof(Queue));
        queue->buffer = new_linked_list();
        return queue;
}
void enqueue(Queue* queue, Job data){
        insert_at_back(queue->buffer, data);
}

Job dequeue(Queue* queue){

        return remove_from_front(queue->buffer);
}
#endif
