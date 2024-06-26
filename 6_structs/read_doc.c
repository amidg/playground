#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#define MAXWORD 1000 // max number of words
char* garbage[] = {"the", "to", "or", "with", "in", "is", "are", "not", "from", "on",  "a", "and", "an", "but", "no", "nor", "by", "of", "so"};
const char *text[] = { \
    "But I must explain to you how all this mistaken idea of denouncing pleasure and praising pain was born and I will give you a complete account of the system, and expound the actual teachings of the great explorer of the truth, the master-builder of human happiness.", \
    "No one rejects, dislikes, or avoids pleasure itself, because it is pleasure, but because those who do not know how to pursue pleasure rationally encounter consequences that are extremely painful.", \
    "Nor again is there anyone who loves or pursues or desires to obtain pain of itself, because it is pain, but because occasionally circumstances occur in which toil and pain can procure him some great pleasure.", \
    "To take a trivial example, which of us ever undertakes laborious physical exercise, except to obtain some advantage from it?", \
    "But who has any right to find fault with a man who chooses to enjoy a pleasure that has no annoying consequences, or one who avoids a pain that produces no resultant pleasure?", \
    "On the other hand, we denounce with righteous indignation and dislike men who are so beguiled and demoralized by the charms of pleasure of the moment, so blinded by desire, that they cannot foresee." \
};
#define SIZEOF(array) (sizeof(array)/sizeof(*array))

struct wnode {
    char *word; // word itself
    int *lines; // array of lines where word exists
    int count; // total number of occurances
    struct wnode* left; // binary tree left child pointer
    struct wnode* right; // binary tree right child pointer
};

int strcomp(const void*, const void*);
void strsort(char* [], size_t);
void intsort(int* [], size_t);
int binsearch(const char*, const char* [], size_t);
int is_garbage(const char*);
void to_lower_case(char*);
struct wnode* talloc(void);
struct wnode* addtree(struct wnode*, char*, int);
void treeprint(struct wnode*);

int main(int argc, char *argv[]) {
    // sort garbage before starting
    strsort(garbage, SIZEOF(garbage));

    /*
     * 1. Parse each line
     * 2. Find a word
     * 3. Add word to the binary tree
     */
    int i,j;
    char nword[MAXWORD] = ""; // new word
    struct wnode* root = NULL; 
    for (i=0; i<SIZEOF(text); i++) {
        for (j=0; *text[i] != '\0'; text[i]++) {
            if (*text[i] == ' ' ||
                *text[i] == '?' ||
                *text[i] == '.' ||
                *text[i] == ',') {
                /*
                 * If we found a word:
                 * 1. Check if word is garbage -> continue
                 * 2. If word is okay -> add node to the binary tree
                 */
                nword[j] = '\0'; // add string endline
                to_lower_case(nword);
                if (is_garbage(nword) == -1) {
                    root = addtree(root, nword, i);
                } 
                nword[j=0] = '\0'; // empty string
            } else {
                // if word not finished, keep iterating
                nword[j++] = *text[i];
            }
        }
    }
    treeprint(root);
    return 0;
}

/*
 * Function to compare strings, needed for the the qsort
 */
int strcomp(const void* a, const void* b) {
    // typecast from void to const char *
    return strcmp( *(const char**)a, *(const char**)b );
}

/*
 * Sorting function allows to sort strings in the descending order
 * Implements stdlib qsort algorithm
 */
void strsort(char* str[], size_t size) {
    qsort(str, size, sizeof(const char*), strcomp);
}

/*
 * Binary search is impossible without sorting because strings must be
 * sorted based on their lexical properties before we have a chance to 
 * iterate over them using binary search, otherwise only one side will work
 */
int binsearch(const char* word, const char* str[], size_t size) {
    int low, mid, high, cond;
    low = 0;
    high = size - 1;
    while (low <= high) {
        mid = (low + high)/2;
        if ( (cond = strcmp(word, str[mid])) < 0)
            high = mid - 1;
        else if (cond > 0)
            low = mid + 1;
        else if (cond == 0)
            return mid;
    }
    return -1;
}

/*
 * Check if word is a part of garbage list
 */
int is_garbage(const char *word) {
    return binsearch(word, garbage, SIZEOF(garbage));
}

void to_lower_case(char* word) {
    for ( ; *word != '\0'; word++)
        if (*word >= 65 && *word <= 90)
            *word += 32; // ASCII conversion? Any better idea?
}

/*
 * Function to add a node to a binary tree
 */
struct wnode* talloc(void) {
    return (struct wnode*) malloc(sizeof(struct wnode));
}

struct wnode* addtree(struct wnode* p, char *w, int line) {
    int cond,i,exists;
    int lines[MAXWORD]; // TODO: more efficient way?
    if (p==NULL) {  // a new word has arrived
        p=talloc(); // make a new node
        p->word=strdup(w); // comes from the string.h
        p->lines=&lines[0];
        p->count=1;
        p->left=p->right=NULL;
    } else if ((cond=strcmp(w,p->word))==0) {
        // check if line is already added to the list
        p->lines[p->count-1] = line;
        p->count += 1;
        //exists = 0;
        //for (i=0; p->lines[i] != -1; i++)
        //    exists += (int)(p->lines[i] == line);
        //if (!exists) {
        //    
        //}
        //*(p->lines) = line;
        //p->lines++;
    }
    else if (cond<0) //less than into left subtree
        p->left=addtree(p->left,w,line);
    else //greater than into right subtree
        p->right=addtree(p->right,w,line);
    return p;
}

void treeprint(struct wnode* p) {
    int count = 0;
    if (p!=NULL) {
        treeprint(p->left);
        //printf("%s: %d\n",p->word,p->count); // print word
        printf("%s: ",p->word); // print word
        for (int i=0; i < p->count; i++)
            printf("%d, ", p->lines[i]);
        printf("\n");
        treeprint(p->right);
    }
}
