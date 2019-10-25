#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct LLNode LLnode;

struct LLNode{
	char* filename;
	FILE* file = NULL;
	LLNode* next = NULL;
};

void LLFree(LLNode* root){
	LLNode* node = root;
	while (node){
		LLNode* next = node->next;
		fclose(node->file);
		free(next);
		node = next;
	}
}

void LLPushFront(LLNode** root, LLNode node){
	LLNode* n = (LLNode*) malloc(sizeof(LlNode));
	n->file = node.file;
	n->next = *root;
	*root = n;
}

void LLForEach(LLNode* root, void (*func)(LLNode*)){

}

void LLForEachStrArg (LLNode* root, void (*func)(LLNode*, const char*), const char* str){
	LLNode* node = root;
	while (node){
		LLNode* next = node->next;
		func(node, str);
		node = next;
	}
}

void OpenFile(LLNode* node, const char* mode){
	node->file = fopen(node->filename, mode);
}

int main(int argv, char* argc[]){
	bool append = false;
	bool ignoreInterrupts = false;
	bool ignoreFlags = false;
	// parse flags
	LLNode* root = NULL;
	for (int i = 1; i < argv; ++i){
		char* str = argc[i];
		size_t len = strlen(str);
		if (str[0] == "-" && !ignoreFlags && len > 1){
			for (size_t j = 1; j < len; ++j){
				switch (str[j]){
					case 'a':
						append = true;
						break;
					case 'i':
						ignoreInterrupts = true;
						break;
					case '-':
						if ( j == 1)
							if ( len == 2)
								ignoreFlags = true;
							break;
						else 
							fprintf (stderr, "Unrecognized option: %s\n", str);
							break;
					default:
						fprintf( stderr, "Invalid option: %c\n", str[j]);
						break;
					}
				}	
		}
		else {
			LLNode node;
			node.filename = str;
			if (root == NULL)
				root = (LLNode*) malloc(sizeof(LLNode));
			LLPushFront(&root, node);
		}
	}

	// open files
	char[2] mode = {append?'a':'w', 0};
	LLForEachStrArg(root, OpenFile, mode);

	// write to files
	// read from stin in chunks

	// free memory and close files
	LLFree(root);	
}
