#include <stdio.h>
#include <stdlib.h>

#define MAX_TREE_HT 50

// A Huffman tree node
struct MinHNode {
    char item;
    unsigned freq;
    struct MinHNode *left, *right;
};

// A Min Heap: Collection of min-heap (or Hufmman tree) nodes
struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHNode **array;
};

// A utility function to create a new min-heap node
struct MinHNode* newNode(char item, unsigned freq) {
    struct MinHNode* temp = (struct MinHNode*)malloc(sizeof(struct MinHNode));
    temp->left = temp->right = NULL;
    temp->item = item;
    temp->freq = freq;
    return temp;
}

// A utility function to create a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHNode**)malloc(minHeap->capacity * sizeof(struct MinHNode*));
    return minHeap;
}

// A utility function to swap two min-heap nodes
void swapMinHNode(struct MinHNode** a, struct MinHNode** b) {
    struct MinHNode* t = *a;
    *a = *b;
    *b = t;
}

// A standard min-heapify function
void minHeapify(struct MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

// A utility function to check if the size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap) {
    return (minHeap->size == 1);
}

// A standard function to extract minimum value node from heap
struct MinHNode* extractMin(struct MinHeap* minHeap) {
    struct MinHNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// A utility function to insert a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHNode* minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

// A standard function to build min heap
void buildMinHeap(struct MinHeap* minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

// A utility function to check if this node is leaf
int isLeaf(struct MinHNode* root) {
    return !(root->left) && !(root->right);
}

// Creates a min-heap of capacity equal to size and inserts all character of data[]
struct MinHeap* createAndBuildMinHeap(char item[], int freq[], int size) {
    struct MinHeap* minHeap = createMinHeap(size);

    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(item[i], freq[i]);

    minHeap->size = size;
    buildMinHeap(minHeap);

    return minHeap;
}

// The main function that builds Huffman tree
struct MinHNode* buildHuffmanTree(char item[], int freq[], int size) {
    struct MinHNode *left, *right, *top;
    struct MinHeap* minHeap = createAndBuildMinHeap(item, freq, size);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

// Prints huffman codes from the root of Huffman Tree.
void printCodes(struct MinHNode* root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (isLeaf(root)) {
        printf("%c: ", root->item);
        for (int i = 0; i < top; ++i)
            printf("%d", arr[i]);
        printf("\n");
    }
}

// The main function that builds a Huffman Tree and print codes by traversing the built Huffman Tree
void HuffmanCodes(char item[], int freq[], int size) {
    struct MinHNode* root = buildHuffmanTree(item, freq, size);

    int arr[MAX_TREE_HT], top = 0;
    printCodes(root, arr, top);
}

int main() {
    char arr[] = { 'A', 'B', 'C', 'D' };
    int freq[] = { 5, 1, 6, 3 };
    int size = sizeof(arr) / sizeof(arr[0]);

    printf("Char | Huffman code\n");
    printf("--------------------\n");

    HuffmanCodes(arr, freq, size);

    return 0;
}
