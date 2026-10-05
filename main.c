#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_COUNT 100
#define SEARCH_COUNT 50
#define MIN_VALUE 0
#define MAX_VALUE 1000

typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;


/* 새로운 노드 생성 */
Node* createNode(int data)
{
    Node* newNode;

    newNode = (Node*)malloc(sizeof(Node));

    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}


/* BST 삽입 */
Node* insertBST(Node* root, int data, int* comparisonCount)
{
    Node* current;
    Node* parent;
    Node* newNode;

    if (root == NULL) {
        return createNode(data);
    }

    current = root;
    parent = NULL;

    while (current != NULL) {
        parent = current;

        (*comparisonCount)++;

        if (data < current->data) {
            current = current->left;
        }
        else if (data > current->data) {
            current = current->right;
        }
        else {
            return root;
        }
    }

    newNode = createNode(data);

    if (data < parent->data) {
        parent->left = newNode;
    }
    else {
        parent->right = newNode;
    }

    return root;
}


/* 서로 다른 100개의 데이터 생성 */
void generateData(int data[], int count)
{
    int i;
    int j;
    int value;
    int duplicate;

    i = 0;

    while (i < count) {
        value = rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE;
        duplicate = 0;

        for (j = 0; j < i; j++) {
            if (data[j] == value) {
                duplicate = 1;
                break;
            }
        }

        if (duplicate == 0) {
            data[i] = value;
            i++;
        }
    }
}


/* 50개의 탐색 대상 생성 */
void generateSearchKeys(int searchKeys[], int count)
{
    int i;

    for (i = 0; i < count; i++) {
        searchKeys[i] =
            rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE;
    }
}


/* 순차 탐색 */
int sequentialSearch(
    const int array[],
    int size,
    int key,
    int* comparisonCount
)
{
    int i;

    *comparisonCount = 0;

    for (i = 0; i < size; i++) {
        (*comparisonCount)++;

        if (array[i] == key) {
            return 1;
        }
    }

    return 0;
}


/* BST 탐색 */
int bstSearch(
    const Node* root,
    int key,
    int* comparisonCount
)
{
    const Node* current;

    *comparisonCount = 0;
    current = root;

    while (current != NULL) {
        (*comparisonCount)++;

        if (key == current->data) {
            return 1;
        }
        else if (key < current->data) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    return 0;
}


/* 배열 출력 */
void printArray(const int array[], int size)
{
    int i;

    for (i = 0; i < size; i++) {
        printf("%4d", array[i]);

        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
        else {
            printf(" ");
        }
    }
}


/* BST 메모리 해제 */
void freeTree(Node* root)
{
    if (root == NULL) {
        return;
    }

    freeTree(root->left);
    freeTree(root->right);

    free(root);
}


int main(void)
{
    int data[DATA_COUNT];
    int searchKeys[SEARCH_COUNT];

    Node* root;

    int bstBuildComparisons;
    int sequentialTotal;
    int bstTotal;

    int sequentialComparisons;
    int bstComparisons;

    int sequentialFound;
    int bstFound;

    int i;

    double sequentialAverage;
    double bstAverage;

    /*
     * 난수 초기화
     */
    srand((unsigned int)time(NULL));

    root = NULL;

    bstBuildComparisons = 0;
    sequentialTotal = 0;
    bstTotal = 0;


    /*
     * 1. 100개의 서로 다른 정수 생성
     */
    generateData(data, DATA_COUNT);

    printf("===============================================\n");
    printf(" Assignment 05: Search Comparison\n");
    printf("===============================================\n\n");

    printf("[1] Generated 100 Integers\n");
    printf("-----------------------------------------------\n");

    printArray(data, DATA_COUNT);


    /*
     * 2. 동일한 데이터로 BST 생성
     */
    for (i = 0; i < DATA_COUNT; i++) {
        root = insertBST(
            root,
            data[i],
            &bstBuildComparisons
        );
    }

    printf("\n[2] BST Construction\n");
    printf("-----------------------------------------------\n");
    printf("BST Build Comparisons : %d\n",
        bstBuildComparisons);


    /*
     * 3. 50개의 탐색 대상 생성
     */
    generateSearchKeys(searchKeys, SEARCH_COUNT);

    printf("\n[3] Generated 50 Search Keys\n");
    printf("-----------------------------------------------\n");

    printArray(searchKeys, SEARCH_COUNT);


    /*
     * 4. 각 탐색 대상에 대해
     *    Sequential Search와 BST Search 수행
     */
    printf("\n[4] Search Results\n");
    printf("===============================================================\n");

    printf("%-4s %-8s %-20s %-20s\n",
        "No.",
        "Key",
        "Sequential Search",
        "BST Search");

    printf("---------------------------------------------------------------\n");

    for (i = 0; i < SEARCH_COUNT; i++) {

        sequentialFound = sequentialSearch(
            data,
            DATA_COUNT,
            searchKeys[i],
            &sequentialComparisons
        );

        bstFound = bstSearch(
            root,
            searchKeys[i],
            &bstComparisons
        );

        sequentialTotal += sequentialComparisons;
        bstTotal += bstComparisons;

        printf("%-4d %-8d %-9s (%3d)         %-9s (%3d)\n",
            i + 1,
            searchKeys[i],
            sequentialFound ? "Found" : "Not Found",
            sequentialComparisons,
            bstFound ? "Found" : "Not Found",
            bstComparisons);
    }


    /*
     * 5. 평균 비교 횟수 계산
     */
    sequentialAverage =
        (double)sequentialTotal / SEARCH_COUNT;

    bstAverage =
        (double)bstTotal / SEARCH_COUNT;


    /*
     * 6. 결과 출력
     */
    printf("\n===============================================================\n");
    printf(" Performance Summary\n");
    printf("===============================================================\n");

    printf("\nNumber of data : %d\n", DATA_COUNT);
    printf("Number of searches : %d\n", SEARCH_COUNT);

    printf("\n[Sequential Search]\n");
    printf("Total comparisons   : %d\n",
        sequentialTotal);
    printf("Average comparisons : %.2f\n",
        sequentialAverage);

    printf("\n[BST Search]\n");
    printf("Total comparisons   : %d\n",
        bstTotal);
    printf("Average comparisons : %.2f\n",
        bstAverage);

    printf("\n[BST Construction]\n");
    printf("Build comparisons   : %d\n",
        bstBuildComparisons);

    printf("\n[BST Total Cost]\n");
    printf("Build + Search      : %d\n",
        bstBuildComparisons + bstTotal);

    printf("\n===============================================================\n");
    printf(" Comparison including BST construction cost\n");
    printf("===============================================================\n");

    printf("Sequential Search total : %d\n",
        sequentialTotal);

    printf("BST Build + Search total: %d\n",
        bstBuildComparisons + bstTotal);

    if (sequentialTotal >
        bstBuildComparisons + bstTotal) {

        printf("BST requires fewer comparisons in this experiment.\n");
    }
    else if (sequentialTotal <
        bstBuildComparisons + bstTotal) {

        printf("Sequential Search requires fewer comparisons in this experiment.\n");
    }
    else {
        printf("Both methods require the same number of comparisons.\n");
    }

    printf("\n===============================================================\n");


    /*
     * 7. 메모리 해제
     */
    freeTree(root);

    return 0;
}