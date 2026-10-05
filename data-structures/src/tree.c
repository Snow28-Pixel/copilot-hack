#include "ds.h"

#include <stdlib.h>

static bool build_preorder(const int *items, size_t count, size_t *index,
                           int null_marker, BiNode **root)
{
    BiNode *node;

    if (*index >= count) {
        return false;
    }
    if (items[*index] == null_marker) {
        ++*index;
        *root = NULL;
        return true;
    }
    node = (BiNode *)malloc(sizeof(*node));
    if (node == NULL) {
        return false;
    }
    node->data = items[(*index)++];
    node->left = NULL;
    node->right = NULL;
    if (!build_preorder(items, count, index, null_marker, &node->left) ||
        !build_preorder(items, count, index, null_marker, &node->right)) {
        bitree_destroy(node);
        return false;
    }
    *root = node;
    return true;
}

bool bitree_build_preorder(const int *items, size_t count, size_t *index,
                           int null_marker, BiNode **root)
{
    if (items == NULL || index == NULL || root == NULL || *index > count) {
        return false;
    }
    *root = NULL;
    return build_preorder(items, count, index, null_marker, root);
}

void bitree_destroy(BiNode *root)
{
    if (root == NULL) {
        return;
    }
    bitree_destroy(root->left);
    bitree_destroy(root->right);
    free(root);
}

void bitree_preorder(const BiNode *root, TreeVisitFn visit, void *context)
{
    if (root == NULL || visit == NULL) {
        return;
    }
    visit(root->data, context);
    bitree_preorder(root->left, visit, context);
    bitree_preorder(root->right, visit, context);
}

void bitree_inorder(const BiNode *root, TreeVisitFn visit, void *context)
{
    if (root == NULL || visit == NULL) {
        return;
    }
    bitree_inorder(root->left, visit, context);
    visit(root->data, context);
    bitree_inorder(root->right, visit, context);
}

void bitree_postorder(const BiNode *root, TreeVisitFn visit, void *context)
{
    if (root == NULL || visit == NULL) {
        return;
    }
    bitree_postorder(root->left, visit, context);
    bitree_postorder(root->right, visit, context);
    visit(root->data, context);
}

size_t bitree_size(const BiNode *root)
{
    if (root == NULL) {
        return 0;
    }
    return 1 + bitree_size(root->left) + bitree_size(root->right);
}

size_t bitree_height(const BiNode *root)
{
    size_t left_height;
    size_t right_height;

    if (root == NULL) {
        return 0;
    }
    left_height = bitree_height(root->left);
    right_height = bitree_height(root->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}
