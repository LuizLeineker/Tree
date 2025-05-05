// AVL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct No{
    char nome[20];
    char email[30];
    int id;
    
    struct No* left;
    struct No* right;
    int altura;
}No;

// Calculo altura
int height(No* no){
    if(no == NULL)
        return -1;
    return no->altura;
}

// Calcular o fator de balanceamento
int balanceamento(No* no){
    if (no == NULL)
        return 0;
    return height(no->left) - height(no->right);
}

// Maior Nó
int maior(int a, int b){
    return (a > b) ? a : b;
}

// ESQUERDA
No *rotacaoRR(No* raiz){
    No* no = raiz->right;
    raiz->right = no->left;
    no->left = raiz;
    raiz->altura = maior(height(raiz->left), height(raiz->right)) + 1;
    no->altura = maior(height(no->left), height(no->right)) + 1;
    return no;
}

//  DIREITA
No* rotacaoLL(No* raiz){
    No* no = raiz->left;
    raiz->left = no->right;
    no->right = raiz;
    raiz->altura = maior(height(raiz->left), height(raiz->right)) + 1;
    no->altura = maior(height(no->left), height(no->right)) + 1;
    return no;
}

// DUPLA ESQUERDA
No* rotacaoLR(No* raiz){
    raiz->left = rotacaoRR(raiz->left);
    return rotacaoLL(raiz);
}

// DUPLA DIREITA
No* rotacaoRL(No* raiz){
    raiz->right = rotacaoLL(raiz->right);
    return rotacaoRR(raiz);
}



// Create - Insert
No* create(char *nome, char *email, int id){
    No* new = (No*)malloc(sizeof(No));
    new->id = id;
    new->altura = 0;
    strcpy(new->nome, nome); 
    strcpy(new->email, email); 
    new->left = NULL;
    new->right = NULL;
    return new; 
}


// Insert
No* insert(No* raiz, char *nome, char *email, int id){
    if(raiz == NULL){
        return create(nome, email, id);
    }
    
    int cmp = strcmp(nome, raiz->nome);
    
    if(cmp < 0){
        raiz->left = insert(raiz->left, nome, email, id);
    }else if(cmp > 0){
        raiz->right = insert(raiz->right, nome, email, id);
    } else{
        return raiz; 
    }
    
    raiz->altura = maior(height(raiz->left), height(raiz->right)) + 1;

    int balancear = balanceamento(raiz);

    if(balancear > 1 && strcmp(nome, raiz->left->nome) < 0)
        return rotacaoLL(raiz);
    if(balancear < -1 && strcmp(nome, raiz->right->nome) > 0)
        return rotacaoRR(raiz);
    if(balancear > 1 && strcmp(nome, raiz->left->nome) > 0)
        return rotacaoLR(raiz);
    if(balancear < -1 && strcmp(nome, raiz->right->nome) < 0)
        return rotacaoRL(raiz);
    return raiz;
}

// Delete
// Menor Valor da Tree
No* menor(No* no) {
    No* atual = no;
    while (atual->left != NULL)
        atual = atual->left;
    return atual;
}

// Remove
No* remover(No* raiz, char *nome){
    if(raiz == NULL){
        return raiz;
    }
    int cmp = strcmp(nome, raiz->nome);
    if(cmp < 0){
        raiz->left = remover(raiz->left, nome);    
    }
    else if(cmp > 0){
        raiz->right = remover(raiz->right, nome);    
    }
    else{
        // Pelo menos um filho ou nenhum
        if (raiz->left == NULL || raiz->right == NULL){
            No* temp = raiz->left ? raiz->left : raiz->right;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        }
        // Filhos completos
        else {
            No* temp = menor(raiz->right);
            strcpy(raiz->nome, temp->nome);
            raiz->right = remover(raiz->right, temp->nome);
        }
    }

    if (raiz == NULL)
        return raiz;

    raiz->altura = maior(height(raiz->left), height(raiz->right)) + 1;

    int balancear = balanceamento(raiz);

    if(balancear > 1 && strcmp(nome, raiz->left->nome) < 0)
        return rotacaoLL(raiz);
    if(balancear < -1 && strcmp(nome, raiz->right->nome) > 0)
        return rotacaoRR(raiz);
    if(balancear > 1 && strcmp(nome, raiz->left->nome) > 0)
        return rotacaoLR(raiz);
    if(balancear < -1 && strcmp(nome, raiz->right->nome) < 0)
        return rotacaoRL(raiz);

    return raiz;
}


// Search
No* search(No* raiz, const char* keyname){
    if(raiz == NULL) 
        return NULL;

    int cmp = strcmp(keyname, raiz->nome);
    if(cmp == 0)
        return raiz;
    if(cmp < 0) 
        return search(raiz->left, keyname);
    return search(raiz->right, keyname);
}

// Read
void list(No *raiz){
    if (raiz != NULL){ 
        list(raiz->left);
        printf("Nome: %s\n", raiz->nome);
        printf("Email: %s\n", raiz->email);
        printf("ID: %d\n", raiz->id);
        printf("-------------------------------\n");
        list(raiz->right);
    }
}

// FREE
void freeTree(No* raiz){
    if(raiz != NULL){
        freeTree(raiz->left); 
        freeTree(raiz->right); 
        free(raiz); 
    }
}

int main() {
    No* raiz = NULL; 
    char nome[20];
    char email[30];
    int id;
    int opcode;
    int autoincrement = 0;
    
    do{ 
        printf("\n\n");
        printf("[ 1 ] - Insert\n");
        printf("[ 2 ] - Delete\n");
        printf("[ 3 ] - Search\n");
        printf("[ 4 ] - Read\n");
        printf("[ 5 ] - Exit\n");
         
        printf("Informe um numero de acordo com as opcoes: ");
        scanf("%d", &opcode);
        printf("\n\n");
        switch (opcode){
         
        case 1:
            printf("Para adicionar um novo usuario preencha as informações abaixo:\n\n");
            
            printf("Nome: ");
            scanf("%s", nome);
            
            printf("Email: ");
            scanf("%s", email);
            
            id = autoincrement;
            
            raiz = insert(raiz, nome, email, id); 
            
            autoincrement++;
        break;
        
        case 2:
            printf("Insira um nome para remover: ");
            scanf("%s", nome);
            raiz = remover(raiz, nome); 
        break;
        
        case 3:
            printf("Insira um nome: ");
            scanf("%s", nome);
            
            No* encontrado = search(raiz, nome);
            if(encontrado != NULL){
                printf("O nome inserido foi encontrado!\n");
                printf("Nome: %s | ID: %d | Email: %s\n", encontrado->nome, encontrado->id, encontrado->email);
            }else{
                printf("Infelizmente não foi possivel encontrar esse nome...\n");
            }
        break;
        
        case 4:
            if(raiz == NULL){ 
                printf("Infelizmente não tem nenhum dado no sistema!");
            }
            else{
                printf("----------- USERS -------------\n"); 
            list(raiz); 
            }
        break;
        
        case 5:
            printf("\nSaindo do Sistema!\n");
        break;
        
        default:
            printf("\nOperacao Invalida!\n");
        break;
         
        }
 
    }while(opcode != 5);
    return 0;
}