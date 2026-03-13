#include "bst_sized.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
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

int bstInsert(BST* tree, int value)
{
    BSTNode** insert = &tree->root;
    BSTNode* parent = nullptr;

    while (*insert != nullptr) {
        parent = *insert;

        if (value < (**insert).value) {
            insert = &(**insert).left;
        } else if (value > (**insert).value) {
            insert = &(**insert).right;
        } else {
            return EEXIST;
        }
    }

    BSTNode* newNode = malloc(sizeof(BSTNode));

    if (newNode == nullptr) {
        return ENOMEM;
    }

    *newNode = {
        .value = value,
        .parent = parent,
        .left = nullptr,
        .right = nullptr
    };

    *insert = newNode;
    tree->cardinality++;

    return 0;
}

bool bstContains(BST* tree, int value)
{
    BSTNode** node = &tree->root;

    while (*node != nullptr) {
        if (value < (**node).value) {
            node = &(**node).left;
        } else if (value > (**node).value) {
            node = &(**node).right;
        } else {
            return true;
        }
    }

    return false;
}

void bstNodeFree(BSTNode** node)
{
    if (node == nullptr) {
        return;
    }

    bstNodeFree(&(**node).left);
    bstNodeFree(&(**node).right);
    free(*node);
    *node = nullptr;
}

void bstFree(BST* tree)
{
    tree->cardinality = 0;
    bstNodeFree(tree->root);
    tree->root = nullptr;
}

void bstInorderInner(const BSTNode* node)
{
    if (node == nullptr) {
        return;
    }

    bstInorderInner(node->left);
    printf("%d ", node->value);
    bstInorderInner(node->right);
}

void bstInorder(const BST* tree)
{
    bstInorderInner(tree->root);
    printf("\n");
}

void bstPreorderInner(const BSTNode* node)
{
    if (node == nullptr) {
        return;
    }

    printf("%d ", node->value);
    bstPreorderInner(node->left);
    bstPreorderInner(node->right);
}

void bstPostorder(const BST* tree)
{
    bstPreorderInner(tree->root);
    printf("\n");
}

void bstPostorderInner(const BSTNode* node)
{
    if (node == nullptr) {
        return;
    }

    bstPostorderInner(node->left);
    bstPostorderInner(node->right);
    printf("%d ", node->value);
}

void bstPostorder(const BST* tree)
{
    bstPostorderInner(tree->root);
    printf("\n");
}

size_t bstNodeHeight(const BSTNode* node)
{
    if (node == nullptr) {
        return 0;
    }

    size_t left = bstNodeHeight(node->left);
    size_t right = bstNodeHeight(node->right);

    return 1 + (left > right ? left : right);
}

size_t bstHeight(const BST* tree)
{
    return bstNodeHeight(tree->root);
}

size_t bstSize(const BST* tree)
{
    return tree->cardinality;
}

// unsound for an empty tree
int bstMin(const BST* tree)
{
    const BSTNode* node = tree->root;

    while (node->left != nullptr) {
        node = node->left;
    }

    return node->value;
}

// unsound for an empty tree
int bstMax(const BST* tree)
{
    const BSTNode* node = tree->root;

    while (node->right != nullptr) {
        node = node->right;
    }

    return node->value;
}
