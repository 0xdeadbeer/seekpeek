#include <stdio.h>

struct dom_structure {
  void *root_node; 
  size_t nodes; 

  void *title; 
}; 

struct dom_title {
  char *title; 
  size_t len; 
}; 

struct document_node {
  void *parent_dom;  
  void *first_node; 
};

struct element_node {
  void *attribute_nodes; 
  size_t attribute_num; 
  void *next_node; 
}; 

struct attribute_node {
  void *value; 
  size_t len; 
}; 

struct text_node {
  void *text; 
  size_t len; 
}; 

struct comment_node {
  void *comment; 
  size_t len; 
}; 
