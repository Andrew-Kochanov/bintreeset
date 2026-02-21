#include "bst_sized.h"
#include <assert.h>
#include <stdlib.h>

typedef struct BSTNode {
    int value;
    struct BSTNode* parent;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

static_assert(sizeof(BSTNode) == sizeof(BSTNodeStandin__));

typedef struct BST {
    BSTNode* root;
    size_t cardinality;
} BST;

static_assert(sizeof(BST) == sizeof(BSTStandin__));
