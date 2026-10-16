#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* root = NULL;

/*Создание дерева (с дубликатами)*/
struct Node* CreateTree(struct Node* root, struct Node* r, int data)
{
    if (r == NULL) {
        r = (struct Node*)malloc(sizeof(struct Node));
        if (r == NULL) {
            printf("Ошибка выделения памяти\n");
            exit(0);
        }
        r->left = NULL;
        r->right = NULL;
        r->data = data;
        if (root == NULL) return r;

        if (data < root->data) root->left = r;
        else root->right = r;
        return r;
    }

    if (data < r->data)
        CreateTree(r, r->left, data);
    else
        CreateTree(r, r->right, data);

    return root;
}

/*Создание дерева БЕЗ дубликатов (задание 3)*/
struct Node* CreateTreeUnique(struct Node* root, int data)
{
    if (root == NULL) {
        root = (struct Node*)malloc(sizeof(struct Node));
        if (root == NULL) {
            printf("Ошибка выделения памяти\n");
            exit(0);
        }
        root->left = NULL;
        root->right = NULL;
        root->data = data;
        return root;
    }

    if (data < root->data)
        root->left = CreateTreeUnique(root->left, data);
    else if (data > root->data)
        root->right = CreateTreeUnique(root->right, data);
    /* если data == root->data ничего не делаем */

    return root;
}

/*Вывод дерева (повёрнуто на 90°)*/
void print_tree(struct Node* r, int l)
{
if (r == NULL) return;

print_tree(r->right, l + 1);
for (int i = 0; i < l; i++) printf("    ");
printf("%d\n", r->data);
print_tree(r->left, l + 1);
}

/*Задание 1: поиск значения*/
struct Node* Search(struct Node* r, int value)
{
    if (r == NULL) return NULL;
    if (r->data == value) return r;
    if (value < r->data) return Search(r->left, value);
    return Search(r->right, value);
}

/*Задание 2: подсчёт вхождений*/
int CountOccurrences(struct Node* r, int value)
{
    if (r == NULL) return 0;
    int count = (r->data == value) ? 1 : 0;
    return count + CountOccurrences(r->left, value)
        + CountOccurrences(r->right, value);
}

/*Освобождение памяти*/
void FreeTree(struct Node* r)
{
    if (r == NULL) return;
    FreeTree(r->left);
    FreeTree(r->right);
    free(r);
}

int main()
{
    setlocale(LC_ALL, "");

    int D, start = 1;

    printf("-1 - окончание построения дерева\n");
    while (start) {
        printf("Введите число: ");
        scanf_s("%d", &D);
        if (D == -1) { start = 0; break; }
        root = CreateTree(root, root, D);
    }

    printf("\nПостроенное дерево:\n");
    print_tree(root, 0);

    /*Задание 1*/
    int value;
    printf("\nВведите значение для поиска: ");
    scanf_s("%d", &value);

    struct Node* found = Search(root, value);
    if (found != NULL)
        printf("Значение %d найдено (адрес: %p)\n", value, (void*)found);
    else
        printf("Значение %d не найдено\n", value);

    /*Задание 2*/
    int cnt = CountOccurrences(root, value);
    printf("Число вхождений значения %d в дерево: %d\n", value, cnt);

    FreeTree(root);
    root = NULL;

    /*Задание 3: пример построения без дубликатов*/
    printf("\n--- Построение дерева без дубликатов ---\n");
    printf("-1 - окончание построения дерева\n");
    start = 1;
    while (start) {
        printf("Введите число: ");
        scanf_s("%d", &D);

        if (D == -1) { start = 0; break; }
        root = CreateTreeUnique(root, D);
    }

    printf("\nДерево без дубликатов:\n");
    print_tree(root, 0);

    FreeTree(root);

    return 0;
}