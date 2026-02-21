#include "bst_sized.h"
#include <assert.h>
#include <stdlib.h>
#include <errno.h>

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

int bstInsert(BST tree, int value)
{
    BSTNode* node = tree->root;

    BSTNode* next = node.left;

    while (next != nullptr) {
        if (value < node->value) {
            next = node.left;
        } else {
            next = node.right;
        }
    }

    BSTNode* newNode = malloc(sizeof(BSTNode));

    if (newNode == nullptr) {
        return ENOMEM;
    }

    newNode = {
        .value = value;
        .parent = node;
        .left = nullptr;
        .right = nullptr;
    };

    node.left = newNode;
}
