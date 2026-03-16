#include "bst_sized.h"
#include "lib.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function for comparing a tree with an array
bool equalToArray(const BST* tree, const int expected[], size_t size)
{
    if (bstSize(tree) != size)
        return false;

    BSTIterator it = bstIteratorInit((BST*)tree);
    size_t i = 0;
    while (bstIteratorHasNext(&it) && i < size) {
        int* val = bstIteratorNext(&it);
        if (val == NULL || *val != expected[i]) {
            bstIteratorFree(&it);
            return false;
        }
        i++;
    }
    bstIteratorFree(&it);
    return i == size;
}

bool testInsertAndContains(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 15);

    bool ok = bstContains(tree, 10) && bstContains(tree, 5) && bstContains(tree, 15) && !bstContains(tree, 100) && bstSize(tree) == 3;

    bstFree(tree);
    free(tree);
    return ok;
}

bool testInsertDuplicate(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    bstInsert(tree, 42);
    int res = bstInsert(tree, 42);
    bool ok = (res == EEXIST) && bstSize(tree) == 1;

    bstFree(tree);
    free(tree);
    return ok;
}

bool testRemoveEmpty(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    bstDelete(tree, 123);
    bool ok = bstSize(tree) == 0;

    bstFree(tree);
    free(tree);
    return ok;
}

bool testRemoveLeaf(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 15);
    bstDelete(tree, 5);

    int expected[] = { 10, 15 };
    bool ok = equalToArray(tree, expected, 2) && bstIsValid(tree);

    bstFree(tree);
    free(tree);
    return ok;
}

bool testRemoveOneChild(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 3);
    bstDelete(tree, 5);

    int expected[] = { 3, 10 };
    bool ok = equalToArray(tree, expected, 2) && bstIsValid(tree);

    bstFree(tree);
    free(tree);
    return ok;
}

bool testRemoveRootWithOneChild(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstDelete(tree, 10);

    int expected[] = { 5 };
    bool ok = equalToArray(tree, expected, 1) && bstIsValid(tree) && tree->root->parent == NULL;

    bstFree(tree);
    free(tree);
    return ok;
}

bool testRemoveTwoChildren(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    bstInsert(tree, 50);
    bstInsert(tree, 30);
    bstInsert(tree, 70);
    bstInsert(tree, 20);
    bstInsert(tree, 40);
    bstInsert(tree, 35);
    bstInsert(tree, 45);
    bstDelete(tree, 30);

    int expected[] = { 20, 35, 40, 45, 50, 70 };
    bool ok = equalToArray(tree, expected, 6) && bstIsValid(tree);

    bstFree(tree);
    free(tree);
    return ok;
}

bool testRemoveRootWithTwoChildren(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    bstInsert(tree, 50);
    bstInsert(tree, 30);
    bstInsert(tree, 70);
    bstInsert(tree, 20);
    bstInsert(tree, 40);
    bstDelete(tree, 50);

    int expected[] = { 20, 30, 40, 70 };
    bool ok = equalToArray(tree, expected, 4) && bstIsValid(tree);

    bstFree(tree);
    free(tree);
    return ok;
}

bool testRemoveNonexistent(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    size_t oldSize = bstSize(tree);
    bstDelete(tree, 999);
    bool ok = bstSize(tree) == oldSize && bstContains(tree, 10) && bstContains(tree, 5);

    bstFree(tree);
    free(tree);
    return ok;
}

bool testKthMin(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 15);
    bstInsert(tree, 3);
    bstInsert(tree, 7);

    bool ok = (bstKthMin(tree, 1) == 3) && (bstKthMin(tree, 2) == 5) && (bstKthMin(tree, 3) == 7) && (bstKthMin(tree, 4) == 10) && (bstKthMin(tree, 5) == 15) && (bstKthMin(tree, 0) == 0) && (bstKthMin(tree, 6) == 0);

    bstFree(tree);
    free(tree);
    return ok;
}

bool testIteratorEmpty(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    BSTIterator it = bstIteratorInit(tree);
    bool ok = !bstIteratorHasNext(&it) && bstIteratorNext(&it) == NULL;
    bstIteratorFree(&it);

    bstFree(tree);
    free(tree);
    return ok;
}

bool testIterator(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    int values[] = { 10, 5, 15, 3, 7, 12, 18 };
    for (int i = 0; i < 7; ++i)
        bstInsert(tree, values[i]);

    int expected[] = { 3, 5, 7, 10, 12, 15, 18 };
    BSTIterator it = bstIteratorInit(tree);
    size_t i = 0;
    bool ok = true;
    while (bstIteratorHasNext(&it) && i < 7) {
        int* val = bstIteratorNext(&it);
        if (val == NULL || *val != expected[i]) {
            ok = false;
            break;
        }
        i++;
    }
    if (i != 7 || bstIteratorHasNext(&it))
        ok = false;
    bstIteratorFree(&it);

    bstFree(tree);
    free(tree);
    return ok;
}

bool testIsValid(void)
{
    BST* tree = newBST();
    if (!tree)
        return false;

    bstInsert(tree, 10);
    bstInsert(tree, 5);
    bstInsert(tree, 15);
    bool ok = bstIsValid(tree);

    // Artificially violating the BST property
    if (tree->root && tree->root->left)
        tree->root->left->value = 20;
    ok = ok && !bstIsValid(tree);

    bstFree(tree);
    free(tree);
    return ok;
}

bool testMerge(void)
{
    BST* tree1 = newBST();
    BST* tree2 = newBST();
    if (!tree1 || !tree2) {
        if (tree1) {
            bstFree(tree1);
            free(tree1);
        }
        if (tree2) {
            bstFree(tree2);
            free(tree2);
        }
        return false;
    }

    bstInsert(tree1, 10);
    bstInsert(tree1, 5);
    bstInsert(tree1, 15);

    bstInsert(tree2, 3);
    bstInsert(tree2, 7);
    bstInsert(tree2, 12);

    BST merged = bstMerge(tree1, tree2);
    int expected[] = { 3, 5, 7, 10, 12, 15 };
    bool ok = (merged.cardinality == 6) && equalToArray(&merged, expected, 6);

    bstFree(&merged);
    bstFree(tree1);
    free(tree1);
    bstFree(tree2);
    free(tree2);
    return ok;
}

// Running all the tests
bool testAll(void)
{
    return testInsertAndContains() && testInsertDuplicate() && testRemoveEmpty() && testRemoveLeaf() && testRemoveOneChild() && testRemoveRootWithOneChild() && testRemoveTwoChildren() && testRemoveRootWithTwoChildren() && testRemoveNonexistent() && testKthMin() && testIteratorEmpty() && testIterator() && testIsValid() && testMerge();
}

int main(int argc, char** argv)
{
    if (testAll()) {
        printf("All tests passed\n");
        return 0;
    } else {
        printf("Some tests failed\n");
        return 1;
    }
}