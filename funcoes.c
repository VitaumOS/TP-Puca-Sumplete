#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "funcoes.h"

void opcoes(){

    char opcao;
    char nome_arquivo[TAM];
    Geral g;
    Tabela t;
    Soma s;
    Jogador j;

    do{
        printf("Bem vindo ao Jogo SUMPLETE!\n");

        printf("0. Sair do Jogo!!!\n");
        printf("1. Começar um novo jogo\n");
        printf("2. Continuar um jogo salvo em arquivo\n");
        printf("3. Continuar o jogo atual\n");
        printf("4. Exibir o ranking\n");
        printf("Durante o jogo digite 'voltar' para retornar ao menu\n");

        printf("Escolha a opção: ");
        scanf(" %c", &opcao);
        limparBuffer();
        system("clear");

        

        switch(opcao){

            case '0': 
                break;
            case '1': 
                
                printf("Primeiramente, digite seu nome: ");
                fgets(j.nome, TAM, stdin);
                j.tam_nome=strlen(j.nome);
                j.nome[j.tam_nome-1]='\0';
                system("clear");

                printf("%s, Digite a dimensão que deseja (de 3 a 9): ", j.nome);
                scanf(" %s", t.tam_c);
                limparBuffer();
                system("clear");

                printf("Agora, digite a dificuldade:\nFácil - F\nMédio - M\nDifícil - D:\nDigite a dificuldade desejada: ");
                scanf(" %c", &t.dificuldade);
                limparBuffer();
                system("clear");
                
                g=jogo(t, s,j,0);

                break;
            case '2': 

                printf("Digite o nome do arquivo: ");
                scanf("%s", nome_arquivo);
                g=abreArquivo(nome_arquivo);
                g=jogo(g.t,g.s,g.j,1);

                break;
            case '3':

                if(g.parametro==1)
                    g=jogo(g.t, g.s, g.j,1);
                else 
                    printf("Você não começou um jogo!\n");
                break;
            case '4':

                //Aqui deverá ter uma função que mostre o ranking

                break;
            default:

                printf("Opção Inválida!\n");
                break;
        }
    }while(opcao!='0');
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

Geral abreArquivo(char * nome_arq){

    FILE *arq=fopen(nome_arq,"r");
    Geral g;
    fscanf(arq,"%d\n", &g.t.tam);
    int n=g.t.tam;
    g.t.mat=criaMatriz(g.t.tam);
    g.t.resposta=criaMatriz(g.t.tam);
    g.s.linha=criaVetor(g.t.tam);
    g.s.coluna=criaVetor(g.t.tam);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            fscanf(arq,"%d ", &g.t.mat[i][j]);
        }
        fprintf(arq,"\n");
    }

    for(int i=0; i<n; i++){
        fscanf(arq,"%d ",&g.s.linha[i]);
    }
    fprintf(arq,"\n");
    for(int i=0; i<n; i++){
        fscanf(arq,"%d ",&g.s.coluna[i]);
    }
    fprintf(arq,"\n");
    fscanf(arq,"%d\n", &g.t.quant_manter);
    int l, c;
    for(int i=0; i<g.t.quant_manter; i++){
        fscanf(arq,"%d %d\n",&l,&c);
        g.t.resposta[l][c]=1;
    }
    fscanf(arq,"%d\n", &g.t.quant_remover);
    for(int i=0; i<g.t.quant_remover; i++){
        fscanf(arq,"%d %d\n",&l,&c);
        g.t.resposta[l][c]=2;
    }
    fscanf(arq,"%s\n",g.j.nome);
    fscanf(arq,"%d", &g.j.TempoT);

    fclose(arq);
    return g;

}

int ** resposta(int l, char *op, int** resposta, int n){

    int linha, coluna;
    char lin[2], col[2];


    lin[0]=op[l+1];
    lin[1]='\0';

    col[0]=op[l+2];
    col[1]='\0';

    linha=atoi(lin);
    coluna=atoi(col);

    resposta[linha-1][coluna-1]=n;

    return resposta;
}

char * dividePalavra(char *op){
    int i=0;
    
    while(op[i]!=' ' && op[i]!='\n'){
        i++;
    }
    char *opcao=malloc((i+1)*sizeof(char));
    for(int j=0; j<i; j++)
        opcao[j]=op[j];

    opcao[i]='\0';
    return opcao;
}



Geral jogo(Tabela t, Soma s, Jogador j, int parametro){

    int vitoria=0;
    int r, len;
    char* opcao;

    Geral g;
    if(parametro==0){
        j.tempoI=time(NULL);
        t.quant_manter=0;
        t.quant_remover=0;

        t.tam = atoi(t.tam_c);
        t.mat=criaMatriz(t.tam);
        t.resposta=criaMatriz(t.tam);
        t.mat=geravalores(t.mat, t.tam);
        t.espelho=criarMatrizEspelho(t.tam);
        s=criaLinhaColuna(t);
    }

    while(vitoria == 0){

        montarTab(t,s);

        printf("Escolha o comando desejado: ");
        fgets(t.opcao, M, stdin);

        system("clear");

        opcao=dividePalavra(t.opcao);
        len=strlen(opcao);

        if(!strcmp(opcao, "manter") ){
            t.resposta=resposta(len, t.opcao, t.resposta,1);
            t.quant_manter++;
        }
        else if(!strcmp(opcao,"remover") ){
            t.resposta=resposta(len, t.opcao, t.resposta,2);
            t.quant_remover++;
        }
        else if(!strcmp(opcao,"dica")){
            t.resposta=dica(t);
        }
        else if(!strcmp(opcao,"resolver")){
            t.resposta=resolver(t);
            montarTab(t,s);
        }
        else if(!strcmp(opcao,"salvar")){
            j.tempoF=time(NULL)-j.tempoI;
            salvaArquivo(len,t,s,j);
            printf("Jogo Salvo!\n");
        }
        else if(!strcmp(opcao,"voltar")){
            g.t=t;
            g.j=j;
            g.s=s;
            g.parametro=1;
            g.j.tempoF=time(NULL)-j.tempoI;
            return g;
        }
        else{
            printf("Comando Inválido!\n");
        }

        vitoria=verificaVitoria(t);

    }
    j.tempoF=time(NULL)-j.tempoI;
    printf("VOCÊ GANHOU!\n");
    printf("Você terminou o jogo em %d segundos!", j.tempoF);
    g.parametro=0;
    return g;
}

void salvaArquivo(int l, Tabela t, Soma s, Jogador j){ //Essa função cria o arquivo com o nome desejado pelo jogador

    char nome[TAM];
    l++;//Pulando o espaço do comando 'salvar texto.txt'
    int n=0;
    
    while(t.opcao[l]!='\0'){ //Esse while vai pegar o nome escolhido pelo jogador para salvar
        nome[n]=t.opcao[l];
        n++;
        l++;
    }
    nome[n]='\0';
        

    FILE * arq=fopen(nome, "w");//Criação do arquivo com o nome escolhido pelo jogador

    fprintf(arq,"%d\n", t.tam); // Essa parte coloca no arquivo a dimensão da matriz e, em seguida, os valores da matriz
    for(int i=0; i<t.tam; i++){
        for(int j=0; j<t.tam; j++){

            fprintf(arq,"%d ",t.mat[i][j]);
        }
        fprintf(arq,"\n");
    }

    for(int i=0; i<t.tam; i++) //Aqui coloca as somas das linhas
        fprintf(arq,"%d ", s.linha[i]);
    fprintf(arq,"\n");
    for(int i=0; i<t.tam; i++) //Aqui coloca as somas das colunas
        fprintf(arq,"%d ",s.coluna[i]);

    fprintf(arq,"\n%d\n", t.quant_manter);//Aqui colocará a quantidade de valores que o jogador marcou para manter na tabela
    for(int i=0; i<t.tam; i++)
        for(int j=0; j<t.tam; j++)
            if(t.resposta[i][j]==1)
                fprintf(arq,"%d %d\n", i+1,j+1); //Aqui colocará cada linha e coluna que foi marcada para manter na tabela
    
    fprintf(arq,"%d\n", t.quant_remover);//Aqui colocará a quantidade de valores que o jogador marcou para manter na tabela
    for(int i=0; i<t.tam; i++)
        for(int j=0; j<t.tam; j++)
            if(t.resposta[i][j]==2)
                fprintf(arq,"%d %d\n", i+1,j+1);//Aqui colocará cada linha e coluna que foi marcada para manter na tabela

    fprintf(arq,"%s\n", j.nome); //Aqui colocará, respectivamente, o nome do jogador e seu tempo de jogo
    fprintf(arq,"%d", j.tempoF);

    fclose(arq);

}

int ** resolver(Tabela t){
    int n=t.tam;
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            t.resposta[i][j]=t.espelho[i][j];

    return t.resposta;
}

int ** dica(Tabela t){

    int n = t.tam,l,c;

    do{ 
        l = rand()%n;
        c = rand()%n;
        
        if(t.resposta[l][c]==0){
            t.resposta[l][c]=t.espelho[l][c];
            return t.resposta;
        }
        
    }while(t.resposta[l][c]!=0);

}

int verificaVitoria(Tabela t){

    int n=t.tam;

    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            if(t.resposta[i][j]!=t.espelho[i][j])
                return 0;


    return 1;

}

int ** criaMatriz(int n){ //Essa função cria uma matriz

    int **matriz=calloc(n,sizeof(int*));
    for(int i=0; i<n; i++){
        matriz[i]=calloc(n,sizeof(int));
    }

    return matriz;

}

int * criaVetor(int n){ //Essa função cria um vetor

    int *vet=malloc(n*sizeof(int));

    for(int i=0; i<n; i++)
        vet[i]=0;

    return vet;
}


void limpamatriz(int **mat, int n){ //Essa função limpa a matriz alocada dinamicamente

    for(int i=0; i<n; i++)
        free(mat[i]);
    free(mat);
}

void limpavetor(int *vet){ //Essa função limpa o vetor alocado dinamicamente

    free(vet);
}

void limpachar(char *op){

    free(op);
}

int ** geravalores(int **mat, int n){ //Essa função gera os valores aleatoriamente da matriz (AINDA PRECISA SER ALTERADA PARA AS DIFICULDADES)

    int m;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){

            m=rand()%9+1;
            mat[i][j]=m; 
        }
    }

    return mat;
}


int ** criarMatrizEspelho(int n){ //Essa função irá criar uma matriz que possuirá os resultados da matriz original
    
    int m;
    int **mat_esp=criaMatriz(n);
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            
            m=rand()%3+1;

            if(m!=3){ //Aqui, em 66% das vezes, um valor será escolhido para a soma
                mat_esp[i][j]=1;
            }
            else{//Nos outros 33%, o valor nn será escolhido, tendo o valor zero
                mat_esp[i][j]=2;
            }
        }
    }
    return mat_esp;
}

Soma criaLinhaColuna(Tabela tab){ //Essa função soma as linhas e colunas baseadas na matriz espelho

    Soma vet;
    vet.linha=criaVetor(tab.tam);
    vet.coluna=criaVetor(tab.tam);
    for(int i=0; i<tab.tam; i++){
        for(int j=0; j<tab.tam; j++){
            if(tab.espelho[i][j]==1){
                vet.linha[i]+=tab.mat[i][j];
                vet.coluna[j]+=tab.mat[i][j];
            }
        }
    }

    return vet;
}

void montarTab(Tabela t, Soma vet){ //Essa função monta a tabela do jogo com os valores gerados aleatoriamente
    
    int n = t.tam;
    
    //___________________________________________
    // Essa parte gera a parte de cima da tabela
    printf(TAB_TL);
    for(int k=0; k<n+1; k++){

        printf(TAB_HOR TAB_HOR TAB_HOR TAB_HOR);

        if(k==n)
            printf(TAB_TR);
        else
            printf(TAB_TJ);   
    }
    //___________________________________________

    //___________________________________________
    //Essa parte mostra os números da matriz junto com as partes intermediárias da tabela
    for(int i=0; i<n; i++){
        
        printf("\n");
        for(int j=0; j<n; j++){
            if(t.resposta[i][j]==1)
                printf(TAB_VER GREEN("  %d "), t.mat[i][j]);
            else if(t.resposta[i][j]==2)
                printf(TAB_VER RED("  %d "), t.mat[i][j]);
            else
                printf(TAB_VER "  %d ", t.mat[i][j]);
        }
        printf(TAB_VER BOLD(" %02d "), vet.linha[i]);
        printf(TAB_VER "\n");        

        if(i<n){
            printf(TAB_ML);
            for(int k=0; k<n+1; k++){

                printf(TAB_HOR TAB_HOR TAB_HOR TAB_HOR);
                if(k==n && i==n-1)
                    printf(TAB_BR);
                else if(k==n)
                    printf(TAB_MR );
                else
                    printf(TAB_MJ);
            }
        }
    }
    //___________________________________________
    printf("\n");
    for(int i=0; i<n; i++)
        printf( TAB_VER BOLD(" %02d "), vet.coluna[i]);
    printf(TAB_VER);
    printf("\n");


    //___________________________________________
    //Essa parte gera a parte debaixo da tabela
    printf(TAB_BL);
    for(int k=0; k<n; k++){
        printf(TAB_HOR TAB_HOR TAB_HOR TAB_HOR );
        if(k==n-1)
            printf(TAB_BR);
        else
            printf(TAB_BJ);
    }
    printf("\n");
    //___________________________________________
    
}