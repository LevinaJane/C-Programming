#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

struct Customer{
	char code[12];
    int category;
    char name[50];
};

struct Node{
	Customer data;
    struct Node *left;
    struct Node *right;
    int height;
} *root = NULL;

int getHeight(Node *node){
    if(node == NULL) return 0;
    return node -> height;
}

int getBF(Node *node){
    if(node == NULL) return 0;
    return getHeight(node -> left) - getHeight(node -> right);
}

void updateHeight(Node *node){
    int leftHeight = getHeight(node -> left);
    int rightHeight = getHeight(node -> right);
    node -> height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

Node *createNode(Customer customer){
    Node *newNode = (Node*) malloc(sizeof(Node));
    newNode -> data = customer;
    newNode -> left = NULL;
    newNode -> right = NULL;
    newNode -> height = 1;
    return newNode;
}

Node *rightRotate(Node *node){
    Node *newRoot = node -> left;
    node -> left = newRoot -> right;
    newRoot -> right = node;
    
    updateHeight(node);
    updateHeight(newRoot);
    
    return newRoot;
}

Node *leftRotate(Node* node){
    Node *newRoot = node -> right;
    node -> right = newRoot -> left;
    newRoot -> left = node;
    
    updateHeight(node);
    updateHeight(newRoot);
    
	return newRoot;
}

Node *rebalance(Node *node){
    if(node == NULL) return node;
    
    updateHeight(node);
    int balanceFactor = getBF(node);
    
    //left-left = RR
    if(balanceFactor > 1 && getBF(node -> left) >= 0){
        return rightRotate(node);
	}
    //left-right = LR
    if(balanceFactor > 1 && getBF(node -> left) < 0){
        node -> left = leftRotate(node -> left);
        return rightRotate(node);
    }
    //right-right = LL
    if(balanceFactor < -1 && getBF(node -> right) <= 0){
        return leftRotate(node);
	}
    
    //right-left = RL
    if(balanceFactor < -1 && getBF(node -> right) > 0){
        node -> right = rightRotate(node -> right);
        return leftRotate(node);
    }
    return node;
}

Node *insert(Node *root, Customer customer){
    if(root == NULL) return createNode(customer);
    
    if(customer.category < root -> data.category){
        root -> left = insert(root -> left, customer);
	} else if(customer.category > root -> data.category){
        root -> right = insert(root -> right, customer);
	} else{
        if(strcmp(customer.code, root -> data.code) < 0){
            root -> left = insert(root -> left, customer);
		} else{
            root -> right = insert(root -> right, customer);
		}
    }
    root = rebalance(root);
    return root;
}

Node *searching(Node *root, char *code){
    if(root == NULL || strcmp(root -> data.code, code) == 0) return root;
    
    if(strcmp(root -> data.code, code) < 0) return searching(root->right, code);
    return searching(root->left, code);
}

Node *deletion(Node *root, char *code){
    if(root == NULL) return root;
    
    if(strcmp(root -> data.code, code) < 0){
        root -> right = deletion(root -> right, code);
	} else if(strcmp(root -> data.code, code) > 0){
        root -> left = deletion(root -> left, code);
	} else{
        if(root -> left == NULL && root -> right == NULL){ //gapunya child
            free(root);
            root = NULL;
        } else if(root -> left == NULL){
            Node* temp = root;
            root = root -> right;
            free(temp);
        } else if(root -> right == NULL){
            Node* temp = root;
            root = root -> left;
            free(temp);
        } else{
            Node* temp = root -> right;
            while(temp -> left != NULL){
				temp = temp -> left;
			}
            root -> data = temp -> data;
            root -> right = deletion(root -> right, temp -> data.code);
        }
    }
    root = rebalance(root);
    return root;
}

void printPreOrder(Node *root){
	if(root != NULL){
		printf("|     %d    | %s  | %-30s |\n", root->data.category, root->data.code, root->data.name);
		printPreOrder(root->left);
		printPreOrder(root->right);
	}
}

void booking(){
	system("cls");
	puts("===== Booking Menu =====");
	puts("");
	puts("Category Number:");
	puts("1 - Ultimate Experience");
	puts("2 - Festival");
	puts("3 - Cat");
	puts("");
	puts("Format: [Category_Number]@[Name]");
	puts("Example: 2@Levina");
	puts("");
	
	char inp[50];
	bool isValid = false;
	do{
		printf("Insert your category number and name: ");
		scanf("%[^\n]", &inp);
		getchar();
		
		if(atoi(inp) < 1 || atoi(inp) > 3){
			isValid = false;
		} else if(inp[1] != '@'){
			isValid = false;
		} else{
			isValid = true;	
		}
	} while(!isValid);
	
	int category = atoi(inp);
	char name[50];
	
	char *cekSym = strchr(inp, '@');
    if (cekSym != NULL) {
        strcpy(name, cekSym + 1);
    }
	
	char code[12];
	int num = rand() % 10;
	char chars = 'A' + (rand() % 26);
	sprintf(code, "COLDPLAY-%d%c", num, chars);
	
	Customer customer;
    strcpy(customer.code, code);
    customer.category = category;
    strcpy(customer.name, name);
	
	root = insert(root, customer);
	
	puts("");
	puts("You have booked a ticket");
	printf("Booking Code: %s\n\n", code);
	printf("Press enter to continue...");
	getchar();
}

void deleteTicket(){
	system("cls");
	puts("===== Delete Menu =====");
	puts("");
	
	if(root == NULL){
		printf("No Data\n Please book a ticket first\n");
	} else{
		puts("------------------------------------------------------------");
		puts("| Category | Booking Code | Customer Name 		   |");
		puts("------------------------------------------------------------");
		printPreOrder(root);
		puts("------------------------------------------------------------");
		puts("");
	}
	puts("");
	printf("Press enter to continue...");
	getchar();
}

void showAll() {
	system("cls");
	puts("===== Show Menu =====");
	puts("");
	
	if(root == NULL){
		printf("No Data\n Please book a ticket first\n");
	} else{
		puts("------------------------------------------------------------");
		puts("| Category | Booking Code | Customer Name 		   |");
		puts("------------------------------------------------------------");
		printPreOrder(root);
		puts("------------------------------------------------------------");
	}
	puts("");
	printf("Press enter to continue...");
	getchar();
}

int main(){
	int choice;

	do{
		system("cls");
		puts("===== Coldplay Concert Ticket Sales System =====");
		printf("1. Book Ticket\n");
        printf("2. Delete Ticket\n");
        printf("3. Show All Tickets\n");
        printf("4. Exit\n");
		printf(">> ");
		scanf("%d", &choice); getchar();
		
		switch(choice){
		case 1:
			booking();
			break;
			
		case 2:
			deleteTicket();
			break;

		case 3:
			showAll();
			break;
	
		case 4:
			exit(0);
			break;
		}
	} while(choice != 4);
	
	return 0;
}

