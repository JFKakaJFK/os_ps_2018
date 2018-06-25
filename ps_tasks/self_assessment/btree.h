#ifndef _BTREE_H_GUARD_
#define _BTREE_H_GUARD_
// only include the code if the header guard is not already set

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/// Stores a binary, sorted tree of integers, in which each element may only occur once
struct btree{
  struct btree_node *root;
  int size, min, max;
};

typedef struct btree btree;

/// A single node in the binary tree
struct btree_node{
  int data;
  struct btree_node *left, *right, *parent;
};

typedef struct btree_node btree_node;

/// Creates and returns a new binary tree
/// must be deallocated by btree_destroy
btree* btree_create(){
  btree* t = malloc(sizeof(*t));
  t->root = NULL;
  t->size = 0;
  t->min = 0;
  t->max = 0;
  return t;
}

// recursively destroys a tree
static void destroy(btree_node* n){
  if(n){
    destroy(n->left);
    destroy(n->right);
    free(n);
    n=NULL;
  }
}

/// Destroys and deallocates all memory for the given tree 't'
void btree_destroy(btree* t){
  destroy(t->root);
  free(t);
  t = NULL;
}

// inserts a new node, allocating the space and setting its values
static btree_node* insert(btree_node* parent, const int d){
  btree_node* child = malloc(sizeof(*child));
  child->data = d;
  child->left = NULL;
  child->right = NULL;
  child->parent = parent;
  return child;
}

/// Inserts the given number 'd' into tree 't'
/// if the number is already in 't', no changes are made
void btree_insert(btree* t, int d){
  t->size++;
  // if the tree is empty, set the root
  if(!t->root){
    t->root = insert(NULL, d);
    t->min = d;
    t->max = d;
  } else {
    // find the node to append the new data
    btree_node *ptr = t->root;
    while(ptr){
      if(d < ptr->data){
        if(ptr->left){
          ptr = ptr->left;
        } else {
          ptr->left = insert(ptr, d);
          if(d < t->min){
            t->min = d;
          } 
          break;
        }
      } else if(d > ptr->data){
        if(ptr->right){
          ptr = ptr->right;
        } else {
          ptr->right = insert(ptr, d);
          if(d > t->max){
            t->max = d;
          }
          break;
        }
      } else {
        t->size--;
        break;
      }
    }
  }
}

static btree_node* find_node(const btree* t, const int d){
  if(!t->size || d < t->min || d > t->max){
    return NULL;
  }
  btree_node *ptr = t->root;
  while(ptr){
    if(ptr->data == d){
      return ptr;
    } else if(d < ptr->data){
      ptr = ptr->left;
    } else {
      ptr = ptr->right;
    }
  }
  return NULL;
}

/// Removes the given number 'd' from tree 't' 
void btree_remove(btree* t, int d){
  btree_node *ptr = find_node(t, d);
  if(ptr){
    // case 1: ptr has no children or one child
    if(!ptr->left || !ptr->right){
      // if ptr is root, the only child is the new root or the tree is empty
      if(ptr == t->root){
        if(ptr->left){
          t->root = ptr->left;
          t->max = (t->root)->data;
        } else if(ptr->right){
          t->root = ptr->right;
          t->min = (t->root)->data;
        } else {
          t->root = NULL;
          t->min = 0;
          t->max = 0;
        }
      } else {
        // for left childs:
        // if the only child is right, check if the min changed
        if((ptr->parent)->left == ptr){
          if(ptr->left){
            (ptr->parent)->left = ptr->left;
            (ptr->left)->parent = ptr->parent;
          } else if(ptr->right){
            (ptr->parent)->left = ptr->right;
            (ptr->right)->parent = ptr->parent;
            if(ptr->data == t->min){
              t->min = (ptr->right)->data;
            }
          } else {
            (ptr->parent)->left = NULL;
            if(ptr->data == t->min){
              t->min = (ptr->parent)->data;
            }
          }
        } else {
          // for right childs:
          // check if the max changed if the only child is left
          if(ptr->left){
            (ptr->parent)->right = ptr->left;
            (ptr->left)->parent = ptr->parent;
            if(ptr->data == t->max){
              t->max = (ptr->left)->data;
            }
          } else if(ptr->right){
            (ptr->parent)->right = ptr->right;
            (ptr->right)->parent = ptr->parent;
          } else {
            (ptr->parent)->right = NULL;
            if(ptr->data == t->max){
              t->max = (ptr->parent)->data;
            }
          }
        }
      }
    } else {
      // find the successor, swap the node and its successor
      // reattach the successors right child and delete the node
      btree_node *successor = ptr->left;
      while(successor->right){
        successor = successor->right;
      }
      if(successor == ptr->left){
        successor->parent = ptr->parent;
        successor->right = ptr->right;
        (successor->right)->parent = successor;
        if(ptr == t->root){
          t->root = successor;
        }
        if(t->max == ptr->data){
          t->max = successor->data;
        }
      } else {
        (successor->parent)->right = successor->left;
        if(successor->left){
          // reattach the successors left child
          (successor->left)->parent = successor->parent;
        }
        // attach the nodes children to the successor
        successor->left = ptr->left;
        (successor->left)->parent = successor;
        successor->right = ptr->right;
        (successor->right)->parent = successor;
        // attach the successor to the nodes parent, if that is not null
        if(ptr == t->root){
          t->root = successor;
        } else {
          if((ptr->parent)->right == ptr){
            (ptr->parent)->right = successor;
          } else {
            (ptr->parent)->left = successor;
          }
        }
        successor->parent = ptr->parent;
      }
    }
    t->size--;
    free(ptr);
    ptr = NULL;
  }
}

/// Returns the smallest number in tree 't'
int btree_minimum(const btree* t){
  return t->min;
}

/// Returns the largest number in tree 't'
int btree_maximum(const btree* t){
  return t->max;
}

/// Returns true if the given tree 't' contains 'd', false otherwise
bool btree_contains(const btree* t, int d){
  return find_node(t, d);
}

/// Returns the number of elements of the given btree 't'
/// NOTE: should complete in O(1) time
size_t btree_size(const btree* t){
  return t->size;
}

static void print_tree(btree_node* n, FILE* out){
  fprintf(out, "(");
  if(n->left){
    print_tree(n->left, out);
    fprintf(out, ", ");
  }
  fprintf(out, "%d", n->data);
  if(n->right){
    fprintf(out, ", ");
    print_tree(n->right, out);
  }
  fprintf(out, ")");
}

/// Prints the given btree 't' to the supplied output stream 'out'
/// output format: ([LEFT], VAL, [RIGHT]) : [SIZE]
/// example empty: ( NIL ) : 0
/// example 3,4,7 in a balanced tree: ((3), 4, (7)) : 3
void btree_print(const btree* t, FILE* out){
  if(!t->root){
    fprintf(out, "( NIL ) : %d\n", t->size);
  } else {
    fprintf(out, "(");
    if((t->root)->left){
      print_tree((t->root)->left, out);
      fprintf(out, ", ");
    }
    fprintf(out, "%d", (t->root)->data);
    if((t->root)->right){
      fprintf(out, ", ");
      print_tree((t->root)->right, out);
    }
    fprintf(out, ") : %d\n", t->size);
  }
}

#endif // _BTREE_H_GUARD_
