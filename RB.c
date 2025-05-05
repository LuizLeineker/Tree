#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Produto{
    int id;
    char nome[20];
    int quantidade;
    float valor;
    
    char color;
    struct Produto* father;
    struct Produto* left;
    struct Produto* right;
}Produto;

//ROTATION
// ESQUERDA
Produto* rotacaoRR(Produto* raiz, Produto* x) {
    Produto* y = x->right;
    x->right = y->left;

    if(y->left != NULL)
        y->left->father = x;

    y->father = x->father;

    if(x->father == NULL)
        raiz = y;
    else if(x == x->father->left)
        x->father->left = y;
    else
        x->father->right = y;

    y->left = x;
    x->father = y;

    return raiz;
}

// DIREITA
Produto* rotacaoLL(Produto* raiz, Produto* y) {
    Produto* x = y->left;
    y->left = x->right;

    if(x->right != NULL)
        x->right->father = y;

    x->father = y->father;

    if(y->father == NULL)
        raiz = x;
    else if(y == y->father->left)
        y->father->left = x;
    else
        y->father->right = x;

    x->right = y;
    y->father = x;

    return raiz;
}





// CRUD

// CREATE - Insert
Produto* create(int id, char *nome, int quantidade, float valor){
    Produto* novo = (Produto*)malloc(sizeof(Produto));
    
    novo->id = id;
    strcpy(novo->nome, nome); 
    novo->quantidade = quantidade;
    novo->valor = valor;
   
    novo->father = NULL;
    novo->left = NULL;
    novo->right = NULL;
    return novo; 
}

Produto* inserir(Produto* raiz, Produto* novo){
    if (raiz == NULL)
        return novo;

    if (novo->id < raiz->id){
        raiz->left = inserir(raiz->left, novo);
        raiz->left->father = raiz;
    } else if (novo->id > raiz->id){
        raiz->right = inserir(raiz->right, novo);
        raiz->right->father = raiz;
    }
    
    return raiz;
}

Produto* corrigirInsercao(Produto* raiz, Produto* novo){
    Produto* pai = NULL;
    Produto* avo = NULL;

    while ((novo != raiz) && (novo->father->color == 'R')) {
        pai = novo->father;
        avo = pai->father;
        
        // Pai é filho do lado esquerdo
        if(pai == avo->left){
            Produto* tio = avo->right;

            // O tio é vermelho => recolorir
            if(tio != NULL && tio->color == 'R'){
                pai->color = 'B';
                tio->color = 'B';
                avo->color = 'R';
                novo = avo;
            }
            else {
                // Filho é do lado direito => rotação dupla
                if(novo == pai->right){
                    novo = pai;
                    raiz = rotacaoRR(raiz, novo);
                    pai = novo->father;
                }
                // Filho é do lado esquerdo => rotação simples
                pai->color = 'B';
                avo->color = 'R';
                raiz = rotacaoLL(raiz, avo);
            }
        }
        else {
            // Pai é filho do lado direito
            Produto* tio = avo->left;

            if(tio != NULL && tio->color == 'R'){
                pai->color = 'B';
                tio->color = 'B';
                avo->color = 'R';
                novo = avo;
            }
            else {
                if(novo == pai->left){
                    novo = pai;
                    raiz = rotacaoLL(raiz, novo);
                    pai = novo->father;
                }
                pai->color = 'B';
                avo->color = 'R';
                raiz = rotacaoRR(raiz, avo);
            }
        }
    }

    raiz->color = 'B';
    return raiz;
}

// Insert
Produto* insert(Produto* raiz, int id, char *nome, int quantidade, float valor){
    Produto* novo = create(id, nome, quantidade, valor);
    raiz = inserir(raiz, novo);
    return corrigirInsercao(raiz, novo);
}

// READ
// Read Completo
void list(Produto* raiz) {
    if(raiz == NULL){
        return;
    }
    
    list(raiz->left);
    
    printf("Nome: %s \n", raiz->nome);
    printf("Preço: %.2f \n", raiz->valor);
    printf("Produtos: %d \n", raiz->quantidade);
    printf("ID: %d \n", raiz->id);
    printf("C: %c \n",  raiz->color);
    printf("-------------------------------\n");

    list(raiz->right);
}

// Search
int search(Produto* raiz, int key){
    if(raiz == NULL){
        return 0;
    }
    
    if(raiz->id == key){
        printf("Nome: %s \n", raiz->nome);
        printf("Preço: %.2f \n", raiz->valor);
        printf("Produtos: %d \n", raiz->quantidade);
        printf("ID: %d \n", raiz->id);
        printf("C: %c \n",  raiz->color);
        printf("-------------------------------\n");
        return 1;
    }
    
    if(raiz->id > key)
        return search(raiz->left, key);
    else
        return search(raiz->right, key);
}

//DELETE
Produto* buscarID(Produto* raiz, int key) {
    if (raiz == NULL || raiz->id == key)
        return raiz;
    
    if (key < raiz->id)
        return buscarID(raiz->left, key);
    else
        return buscarID(raiz->right, key);
}


Produto* corrigirRemocao(Produto* raiz, Produto* x){
    while (x != raiz && x->color == 'B'){
        if(x == x->father->left){
            Produto* w = x->father->right;
            if (w->color == 'R'){
                w->color = 'B';
                x->father->color = 'R';
                raiz = rotacaoRR(raiz, x->father);
                w = x->father->right;
            }
            if((w->left == NULL || w->left->color == 'B') &&
                (w->right == NULL || w->right->color == 'B')){
                w->color = 'R';
                x = x->father;
            } else{
                if(w->right == NULL || w->right->color == 'B'){
                    if(w->left != NULL)
                        w->left->color = 'B';
                    w->color = 'R';
                    raiz = rotacaoLL(raiz, w);
                    w = x->father->right;
                }
                w->color = x->father->color;
                x->father->color = 'B';
                if(w->right != NULL)
                    w->right->color = 'B';
                raiz = rotacaoRR(raiz, x->father);
                x = raiz;
            }
        } else{
            Produto* w = x->father->left;
            if(w->color == 'R'){
                w->color = 'B';
                x->father->color = 'R';
                raiz = rotacaoLL(raiz, x->father);
                w = x->father->left;
            }
            if((w->right == NULL || w->right->color == 'B') &&
                (w->left == NULL || w->left->color == 'B')){
                w->color = 'R';
                x = x->father;
            } else{
                if(w->left == NULL || w->left->color == 'B'){
                    if (w->right != NULL)
                        w->right->color = 'B';
                    w->color = 'R';
                    raiz = rotacaoRR(raiz, w);
                    w = x->father->left;
                }
                w->color = x->father->color;
                x->father->color = 'B';
                if(w->left != NULL)
                    w->left->color = 'B';
                raiz = rotacaoLL(raiz, x->father);
                x = raiz;
            }
        }
    }
    x->color = 'B';
    return raiz;
}


void transplantar(Produto** raiz, Produto* u, Produto* v) {
    if(u->father == NULL){
        *raiz = v;
    } else if (u == u->father->left){
        u->father->left = v;
    }else {
        u->father->right = v;
    }
    if(v != NULL)
        v->father = u->father;
}

Produto* minimo(Produto* no){
    while (no->left != NULL)
        no = no->left;
    return no;
}

Produto* remover(Produto* raiz, int key){
    Produto* z = buscarID(raiz, key);
    if (z == NULL) {
        printf("ID %d não encontrado.\n", key);
        return raiz;
    }

    Produto* y = z;
    Produto* x;
    char yColor= y->color;

    if(z->left == NULL){
        x = z->right;
        transplantar(&raiz, z, z->right);
    } else if (z->right == NULL){
        x = z->left;
        transplantar(&raiz, z, z->left);
    } else{
        y = minimo(z->right);
        yColor = y->color;
        x = y->right;

        if(y->father == z){
            if (x != NULL) x->father = y;
        } else{
            transplantar(&raiz, y, y->right);
            y->right = z->right;
            if (y->right != NULL) y->right->father = y;
        }

        transplantar(&raiz, z, y);
        y->left = z->left;
        if (y->left != NULL) y->left->father = y;
        y->color = z->color;
    }

    free(z);

    if(yColor == 'B' && x != NULL)
        raiz = corrigirRemocao(raiz, x);

    return raiz;
}

// FREE
void freeTree(Produto* raiz){
    if(raiz != NULL){
        freeTree(raiz->left); 
        freeTree(raiz->right); 
        free(raiz); 
    }
}

int main() {
    Produto* raiz = NULL; 
    
    int id;
    char nome[20];
    int quantidade;
    float valor;
    
    int retorno;
    int opcode;
    
    do{ 
        printf("\n\n");
        printf("[ 1 ] - Create\n");
        printf("[ 2 ] - Delete\n");
        printf("[ 3 ] - Search\n");
        printf("[ 4 ] - Read\n");
        printf("[ 5 ] - Exit\n");
         
        printf("Informe um numero de acordo com as opcoes: ");
        scanf("%d", &opcode);
        printf("\n\n");
        switch (opcode){
         
        case 1:
            printf("Para adicionar o produto preencha as informações abaixo:\n\n");
            
            printf("Nome: ");
            scanf("%s", nome);
            
            printf("Preço: ");
            scanf("%f", &valor);
           
            printf("Quantidade: ");
            scanf("%d", &quantidade);
            
            printf("ID do produto: ");
            scanf("%d", &id);
            
            raiz = insert(raiz, id, nome, quantidade, valor); 
        break;
        
        case 2:
            printf("Insira o ID para remover: ");
            scanf("%d", &id);
            
            raiz = remover(raiz, id);
        break;
        
        case 3:
            printf("Insira o ID para a busca: ");
            scanf("%d", &id);
            
            retorno = search(raiz, id);
            
            if(retorno != 1)
                printf("Infelizmente não tem esse id no sistema!");
        break;
        
        case 4:
            if(raiz == NULL){ 
                printf("Infelizmente não tem nenhum dado no sistema!");
            }
            else{
                printf("----------  PRODUTOS ----------\n"); 
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
    freeTree(raiz);
    return 0;
}