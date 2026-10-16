#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* root = NULL;

/*Создание нового узла*/
struct Node* get_struct(int data)
{
    struct Node* r = (struct Node*)malloc(sizeof(struct Node));
    if (r == NULL) {
        printf("Ошибка выделения памяти\n");
        exit(0);
    }
    r->left = NULL;
    r->right = NULL;
    r->data = data;
    return r;
}

/*Создание дерева с дубликатами*/
struct Node* CreateTree(struct Node* root, int data)
{
    if (root == NULL)
        return get_struct(data);

    if (data < root->data)
        root->left = CreateTree(root->left, data);
    else
        root->right = CreateTree(root->right, data);

    return root;
}

/*Создание дерева БЕЗ дубликатов*/
struct Node* CreateTreeUnique(struct Node* root, int data)
{
    if (root == NULL)
        return get_struct(data);

    if (data < root->data)
        root->left = CreateTreeUnique(root->left, data);
    else if (data > root->data)
        root->right = CreateTreeUnique(root->right, data);
    else
        printf("Предупреждение: значение %d уже есть в дереве — пропущено\n", data);

    return root;
}

/*Проверка наличия значения*/
int Contains(struct Node* r, int value)
{
    if (r == NULL) return 0;
    if (r->data == value) return 1;
    if (value < r->data) return Contains(r->left, value);
    return Contains(r->right, value);
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

/*Поиск значения с определением уровня*/
struct Node* Search(struct Node* r, int value, int level, int* foundLevel)
{
    if (r == NULL) return NULL;

    if (r->data == value) {
        *foundLevel = level;
        return r;
    }

    if (value < r->data)
        return Search(r->left, value, level + 1, foundLevel);
    else
        return Search(r->right, value, level + 1, foundLevel);
}

/*Подсчёт вхождений*/
int CountOccurrences(struct Node* r, int value)
{
    if (r == NULL) return 0;
    int count = (r->data == value) ? 1 : 0;
    return count + CountOccurrences(r->left, value)
        + CountOccurrences(r->right, value);
}

/*Минимальный узел в поддереве*/
struct Node* FindMin(struct Node* r)
{
    while (r != NULL && r->left != NULL)
        r = r->left;
    return r;
}

/*Удаление узла по значению*/
struct Node* DeleteNode(struct Node* root, int value)
{
    if (root == NULL) return NULL;

    if (value < root->data) {
        root->left = DeleteNode(root->left, value);
    }
    else if (value > root->data) {
        root->right = DeleteNode(root->right, value);
    }
    else {
        if (root->left == NULL) {
            struct Node* temp = root->right;
            free(root);
            return temp;
        }

        if (root->right == NULL) {
            struct Node* temp = root->left;
            free(root);
            return temp;
        }

        struct Node* temp = FindMin(root->right);
        root->data = temp->data;
        root->right = DeleteNode(root->right, temp->data);
    }

    return root;
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

    /*Первое дерево (с дубликатами)*/
    printf("-1 - окончание построения дерева\n");
    while (start) {
        printf("Введите число: ");
        scanf_s("%d", &D);
        if (D == -1) { start = 0; break; }

        //if (Contains(root, D))
        //    printf("Предупреждение: значение % d уже есть в дереве(будет добавлено повторно)\n", D);

        root = CreateTree(root, D);
    }

    printf("\nПостроенное дерево:\n");
    print_tree(root, 0);

    /*Поиск с уровнем*/
    int value;
    printf("\nВведите значение для поиска: ");
    scanf_s("%d", &value);

    int foundLevel = -1;
    struct Node* found = Search(root, value, 0, &foundLevel);

    if (found != NULL)
        printf("Значение %d найдено на уровне %d (адрес: %p)\n",
            value, foundLevel, (void*)found);
    else
        printf("Значение %d не найдено\n", value);

    /*Подсчёт вхождений*/
    int cnt = CountOccurrences(root, value);
    printf("Число вхождений значения %d в дерево: %d\n", value, cnt);

    FreeTree(root);
    root = NULL;

    /*Второе дерево (без дубликатов)*/
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

    /*Удаление узлов*/
    printf("\nУдаление узлов (введите -1 для завершения):\n");
    while (1) {
        int delValue;
        printf("Введите значение для удаления: ");
        scanf_s("%d", &delValue);
        if (delValue == -1) break;

        if (Contains(root, delValue)) {
            root = DeleteNode(root, delValue);
            printf("Значение %d удалено.\n", delValue);
        }
        else {
            printf("Значение %d в дереве отсутствует — удалять нечего.\n", delValue);
        }
    }

    printf("\nДерево после удалений:\n");
    print_tree(root, 0);

    FreeTree(root);

    return 0;
}



