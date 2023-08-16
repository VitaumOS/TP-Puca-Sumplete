#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct {
    int *linha;
    int *coluna;
} Soma;

typedef struct {
    int **mat;
    int **gabarito;
    int **resposta;
    int tam;
} Tabela;

int **criarMatriz(int n) {
    int **matriz = calloc(n, sizeof(int *));
    for (int i = 0; i < n; i++) {
        matriz[i] = calloc(n, sizeof(int));
    }
    return matriz;
}

int *criaVetor(int n) {
    int *vet = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        vet[i] = 0;
    return vet;
}

int **criarMatrizEspelho(int n) {
    int **mat_esp = criarMatriz(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int m = rand() % 3 + 1;
            if (m != 3) {
                mat_esp[i][j] = 1;
            } else {
                mat_esp[i][j] = 2;
            }
        }
    }
    return mat_esp;
}

int **geraValores(int **mat, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int m = rand() % 9 + 1;
            mat[i][j] = m;
        }
    }
    return mat;
}

Soma criaLinhaColuna(Tabela tab) {
    Soma vet;
    vet.linha = criaVetor(tab.tam);
    vet.coluna = criaVetor(tab.tam);
    for (int i = 0; i < tab.tam; i++) {
        for (int j = 0; j < tab.tam; j++) {
            if (tab.gabarito[i][j] == 1) {
                vet.linha[i] += tab.mat[i][j];
                vet.coluna[j] += tab.mat[i][j];
            }
        }
    }
    return vet;
}

void gabarito(Tabela t, Soma s){
    int n=pow(2,t.tam), soma=0, quant,aux;
    
    int *linhaF=malloc(t.tam*sizeof(int));

    int **matgab=calloc(t.tam,sizeof(int*));
    for(int i=0; i<t.tam; i++)
        matgab[i]=calloc(t.tam,sizeof(int));
    
    int **linha=calloc(n,sizeof(int*));
    for(int i=0; i<n; i++)
        linha[i]=calloc((t.tam+1),sizeof(int));
    do{
        for(int i=0; i<t.tam; i++){

            quant=0;
            for(int k=0; k<n; k++){
                soma=0;
                linha[k][t.tam]=0;
                for(int j=0; j<t.tam; j++){
                    if((k>>j)&1){
                        soma+=t.mat[i][j];
                        linha[k][j]=1;   
                    }
                    else
                        linha[k][j]=2;
                }
                for(int j=0; j<t.tam; j++)
                    if(matgab[i][j]!=linha[k][j] && matgab[i][j]!=0)
                        linha[k][t.tam]=2;
                if(soma==s.linha[i] && linha[k][t.tam]!=2){
                    quant++;
                    linha[k][t.tam]=1;
                }  
            }
            if(quant==1){
                for(int k=0; k<n; k++){
                    if(linha[k][t.tam]==1){
                        for(int j=0; j<t.tam; j++){
                         
                            linhaF[j]=linha[k][j];
                        }
                    }
                    
                }
            }   
            else{
                
                for(int j=0; j<t.tam; j++)
                    linhaF[j]=0; 
            }

            for(int j=0; j<t.tam; j++){
                if(matgab[i][j]==0)
                    matgab[i][j]=linhaF[j];
            }

            for(int x=0; x<t.tam; x++)
                printf("%d ", linhaF[x]);  
            printf("Possibilidades: %d\n", quant); 
        
        }

        printf("\n");

        for(int i=0; i<t.tam; i++){

            quant=0;
            for(int k=0; k<n; k++){
                soma=0;
                linha[k][t.tam]=0;
                for(int j=0; j<t.tam; j++){
                    if((k>>j)&1){
                        soma+=t.mat[j][i];
                        linha[k][j]=1;
                    }
                    else
                        linha[k][j]=2;
                }
                for(int j=0; j<t.tam; j++)
                    if(matgab[j][i]!=linha[k][j] && matgab[j][i]!=0)
                        linha[k][t.tam]=2;
                if(soma==s.coluna[i] && linha[k][t.tam]!=2){
                    quant++;
                    linha[k][t.tam]=1;
                }  
            }
            if(quant==1){
                for(int k=0; k<n; k++){
                    if(linha[k][t.tam]==1){
                        for(int j=0; j<t.tam; j++){
                            
                            linhaF[j]=linha[k][j];
                        }
                    }
                    
                }
            }   
            else{
                
                for(int j=0; j<t.tam; j++)
                    linhaF[j]=0; 
            }

            for(int j=0; j<t.tam; j++)
                if(matgab[j][i]==0)
                    matgab[j][i]=linhaF[j];

            for(int x=0; x<t.tam; x++)
                printf("%d ", linhaF[x]);  
            printf("Possibilidades: %d\n", quant); 
        
        }
        printf("\n");
        for(int i=0; i<t.tam; i++){
            for(int j=0; j<t.tam; j++)
                printf("%d ", matgab[i][j]);
            printf("\n");
        }
        aux=0;
        for(int i=0; i<t.tam; i++)
            for(int j=0; j<t.tam; j++)
                if(matgab[i][j]!=0)
                    aux++;
    }while(aux!=(t.tam*t.tam));
                

    

   

    for(int i=0; i<n; i++)
        free(linha[i]);
    free(linha);
    free(linhaF);
}

int main() {
    srand(time(NULL));
    Tabela t;
    Soma s;
    
    t.tam = 5;
    t.mat = criarMatriz(t.tam);
    t.resposta = criarMatriz(t.tam);
    t.mat = geraValores(t.mat, t.tam);
    t.gabarito = criarMatrizEspelho(t.tam);
    s = criaLinhaColuna(t);
    
    for (int i = 0; i < t.tam; i++) {
        for (int j = 0; j < t.tam; j++)
            printf("%d ", t.mat[i][j]);
        printf("\n");
    }
    printf("\n");printf("\n");
    for (int i = 0; i < t.tam; i++) {
        printf("%d ", s.linha[i]);
    }
    printf("\n");printf("\n");
    
    for (int i = 0; i < t.tam; i++) {
        printf("%d ", s.coluna[i]);
    }
    printf("\n");printf("\n");
    for (int i = 0; i < t.tam; i++) {
        for (int j = 0; j < t.tam; j++)
            printf("%d ", t.gabarito[i][j]);
        printf("\n");
    }
    printf("\n");printf("\n");

    gabarito(t,s);
    
    // Liberar memória alocada
    for (int i = 0; i < t.tam; i++) {
        free(t.mat[i]);
        free(t.resposta[i]);
        free(t.gabarito[i]);
    }
    
    free(t.mat);
    free(t.resposta);
    free(t.gabarito);
    free(s.linha);
    free(s.coluna);

    return 0;
}
