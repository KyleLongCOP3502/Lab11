#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie node structure
struct TrieNode {
    struct TrieNode* children[26]; 
    int isEndOfWord; 
    int count; 
};

// Initializes a new TrieNode
struct TrieNode* createNode() {
    struct TrieNode* pNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    pNode->isEndOfWord = 0;
    pNode->count = 0;
    for (int i = 0; i < 26; i++) {
        pNode->children[i] = NULL;
    }
    return pNode;
}

// Inserts a word into the Trie
void insert(struct TrieNode* root, char* word) {
    struct TrieNode* current = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
    current->count++;
}

// Returns the number of occurrences of a word in the Trie
int numberOfOccurances(struct TrieNode* root, char* word) {
    struct TrieNode* current = root;
    for (int i = 0; i < strlen(word); i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            return 0; 
        }
        current = current->children[index];
    }
    if (current != NULL && current->isEndOfWord) {
        return current->count;
    }
    return 0; 
}

// Deallocates memory for the Trie
void deallocateTrie(struct TrieNode* root) {
    if (!root) {
        return;
    }
    for (int i = 0; i < 26; i++) {
        if (root->children[i]) {
            deallocateTrie(root->children[i]);
        }
    }
    free(root);
}

// Reads the dictionary file and returns the number of words read
int readDictionary(char* filename, char** pInWords) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file\n");
        exit(1);
    }
    int numWords = 0;
    char word[256];
    while (fscanf(file, "%s", word) == 1) {
        pInWords[numWords] = strdup(word);
        numWords++;
    }
    fclose(file);
    return numWords;
}

int main(void) {
    char* inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct TrieNode* pTrie = createNode();
    for (int i = 0; i < numWords; i++) {
        insert(pTrie, inWords[i]);
    }

    // Test with some words
    char* pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }

    deallocateTrie(pTrie);
    return 0;
}
