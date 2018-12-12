#if !defined(HDR)
#define HDR
#include <stdlib.h>
/* define Node*/
typedef struct Node {
  void* data;
  struct Node* next;
}Node;

Node* new_node(void *data){
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
  while(node){
    result++;
    node = node->next;
  }
  return result;
}
int is_empty(LinkedList* list){
  return !list->head;
}

void insert_at_back(LinkedList* list, void* data){
  Node* node = new_node(data);
  if(!node)
    return;
  Node* tmp = list->head;
  if(!tmp) // if there are no node in list
    list->head = node;
  else{
    while(tmp->next){
      tmp = tmp->next;
    }
    tmp->next = node;
  }
}
void* remove_from_front(LinkedList* list){
  void* result;
  Node* tmp = list->head;
  if(!tmp)
    return NULL;

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
void enqueue(Queue* queue, void* data){
  insert_at_back(queue->buffer, data);
}
void* dequeue(Queue* queue){
  return remove_from_front(queue->buffer);
}
#endif
