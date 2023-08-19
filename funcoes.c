//Vitor de Oliveira Silva 23.1.4023
#include "funcoes.h"

void opcoes(){

    char opcao[4], op;
    char nome_arquivo[M];
    Geral g;
    g.parametro=0;

    system("clear");
    printf("Bem vindo ao Jogo SUMPLETE!\n");
    do{
        
        do{
            printf("0. Sair do Jogo\n");
            printf("1. Começar um novo jogo\n");
            printf("2. Continuar um jogo salvo em arquivo\n");
            printf("3. Continuar o jogo atual\n");
            printf("4. Exibir o ranking\n");
            printf("Durante o jogo digite 'voltar' para retornar ao menu\n");

            printf("Escolha a opção: ");
            
            fgets(opcao, 4, stdin);
            op = opcao[0];
            
            system("clear");
            
            if ((strlen(opcao) != 2) || (op < '0' || op > '4')) {
                printf(RED("Opção Inválida!\n"));
                limparBuffer();
            }

        } while ((strlen(opcao) != 2) || (op < '0' || op > '4'));


        switch(op){

            case '0': 

                if(g.parametro==2)
                    ranking(g.j.nome,g.j.tempoF,g.t.tam, 1);
                else if(g.parametro==1){
                    limpamatriz(&g.t.mat, g.t.tam);
                    limpamatriz(&g.t.resposta, g.t.tam);
                    limpamatriz(&g.t.gabarito, g.t.tam);
                    limpavetor(&g.s.linha);
                    limpavetor(&g.s.coluna);
                }

                printf("Obrigado por jogar!\n");
                break;
            case '1': 
                
                printf("Primeiramente, digite seu nome: ");
                fgets(g.j.nome, M, stdin);
                g.j.tam_nome=strlen(g.j.nome);
                g.j.nome[g.j.tam_nome-1]='\0';
                system("clear");

                do{

                    printf("%s, digite a dimensão que deseja (de 3 a 9): ", g.j.nome);

                    fgets(opcao, 4, stdin);
                    g.t.tam = atoi(opcao);

                    system("clear");
                    if(g.t.tam<3 || g.t.tam>9){
                        printf(RED("Valor inválido!\n"));
                        limparBuffer();
                    }

                }while(g.t.tam<3 || g.t.tam>9);
                if(g.t.tam>=5){
                    do{

                        printf("Agora, digite a dificuldade (Fácil | Médio | Difícil): ");
                        scanf(" %c", &g.t.dificuldade);
                        limparBuffer();
                        system("clear");
                        if(g.t.dificuldade!='F' && g.t.dificuldade!='M' && g.t.dificuldade!='D')
                            printf(RED("Valor inválido!\n"));
                        else if(g.t.tam<=6 && g.t.dificuldade=='D')
                            printf(RED("Dificuldade inválida!\n"));

                    }while((g.t.dificuldade!='F' && g.t.dificuldade!='M' && g.t.dificuldade!='D') || (g.t.tam<=6 && g.t.dificuldade=='D'));
                }
                else
                    g.t.dificuldade='F';
                
                g=jogo(g,0);

                break;
            case '2': 

                printf("Digite o nome do arquivo: ");
                scanf("%s", nome_arquivo);
                if(verificaNomeArquivo(nome_arquivo)){
                    g=abreArquivo(nome_arquivo);
                    if(g.parametro==1)
                        g=jogo(g,1);
                }
                limparBuffer();
                break;
            case '3':
                if(g.parametro==1)
                    g=jogo(g,1);
                else 
                    printf(RED("Você não tem um jogo iniciado!\n"));
                break;
            case '4':
                ranking(g.j.nome,g.j.tempoF,g.t.tam,0);

                break;
            default:

                printf(RED("Opção Inválida!\n"));
                break;
        }
    }while(op!='0');
}

int verificaNomeArquivo(char *arquivo){
    int tam,aux=0;
    char formato[6];
    tam=strlen(arquivo);
    for(int i=tam-4; i<=tam; i++){
        formato[aux]=arquivo[i];
        aux++;
    }
    if(!strcmp(formato,".txt"))
        return 1;
    else{
        printf("Formato incorreto!\n");
        return 0;
    }
}

Geral jogo(Geral g, int parametro){

    int vitoria=0;
    int r, tam;

    g.j.tempoI=time(NULL);

    
    if(parametro==0){
        g.j.tempoT=0;

        g.t.quant_manter=0;
        g.t.quant_remover=0;

        g.t.mat=criaMatriz(g.t.tam);
        g.t.resposta=criaMatriz(g.t.tam);
        g.t.mat=geravalores(g.t.mat, g.t.tam, g.t.dificuldade);
        g.t=criarMatrizEspelho(g.t);
        g.s=criaLinhaColuna(g.t);
    }

    montarTab(g.t,g.s);

    while(vitoria == 0){

        printf("Comandos possíveis:\n");
        printf(GREEN("manter") " (seguido da linha e coluna juntos)\n");
        printf(RED("remover") " (seguido da linha e coluna juntos)\n");
        printf("dica\n");
        printf("resolver\n");
        printf("salvar (seguido do nome do arquivo)\n");
        printf("voltar\n\n");
        printf("Escolha o comando desejado: ");

        fgets(g.t.opcao, M, stdin);
        tam=strlen(g.t.opcao);
        system("clear");

        if(strstr(g.t.opcao, "manter")!=NULL && tam==10){
            g.t.resposta=resposta(7, g.t.opcao, g.t.resposta,g.t.tam,1);
            g.t.quant_manter++;
        }
        else if(strstr(g.t.opcao, "remover")!=NULL && tam==11){
            g.t.resposta=resposta(8, g.t.opcao, g.t.resposta,g.t.tam,2);
            g.t.quant_remover++;
        }
        else if(strstr(g.t.opcao, "dica")!=NULL && tam==5){
            g.t.resposta=dica(g.t);
            g.t.quant_manter++;
        }
        else if(strstr(g.t.opcao, "resolver")!=NULL && tam==9){
            g.t.resposta=resolver(g.t);
        }
        else if(strstr(g.t.opcao, "salvar")!=NULL){
            g.j.tempoF=time(NULL)-g.j.tempoI;
            salvaArquivo(6,g.t,g.s,g.j);
            printf("Jogo Salvo!\n");
        }
        else if(strstr(g.t.opcao, "voltar")!=NULL && tam==7){
            g.parametro=1;
            g.j.tempoT=(time(NULL)-g.j.tempoI)+g.j.tempoT;
            return g;
        }
        else{
            printf(RED("Comando Inválido!\n"));
        }
        montarTab(g.t,g.s);

        vitoria=verificaVitoria(g.t, g.s);
    }
    g.j.tempoF=(time(NULL)-g.j.tempoI)+g.j.tempoT;
    printf("VOCÊ GANHOU!\n");
    printf("Você terminou o jogo em %d segundos!\n", g.j.tempoF);

    g.parametro=2;
    limpamatriz(&g.t.mat, g.t.tam);
    limpamatriz(&g.t.resposta, g.t.tam);
    limpamatriz(&g.t.gabarito, g.t.tam);
    limpavetor(&g.s.linha);
    limpavetor(&g.s.coluna);

    return g;
}

int ** resolver(Tabela t){ //Essa função copia os valores da matriz gabarito para a matriz resposta
    int n=t.tam;
    for(int i=0; i<n; i++)
        for(int j=0; j<n; j++)
            t.resposta[i][j]=t.gabarito[i][j];

    return t.resposta;
}

int ** dica(Tabela t){ // Essa função seleciona um valor aleatório (desde que não seja um escolhido pelo jogador) e marca na matriz resposta

    int n = t.tam,l,c;

    do{ 
        l = rand()%n;
        c = rand()%n;
        
        if((t.resposta[l][c]==2||t.resposta[l][c]==0) && t.gabarito[l][c]==1){
            t.resposta[l][c]=1;
            return t.resposta;
        }
    }while(t.gabarito[l][c]==2 || t.resposta[l][c]!=0);
}

int verificaVitoria(Tabela t, Soma s){ //Essa função irá comparar os valores colocados pelo jogador na matriz resposta com a matriz gabarito

    int n=t.tam, auxr=0,aux=0,somal,somac, subl, subc;

    for(int i=0; i<n; i++){
            somal=0;
            somac=0;
            subl=0;
            subc=0;
            for(int j=0; j<n; j++){
                if(t.resposta[i][j]==1)
                    somal+=t.mat[i][j];
                if(t.resposta[j][i]==1)
                    somac+=t.mat[j][i];
                subl+=t.mat[i][j];
                subc+=t.mat[j][i];
            }
            for(int j=0; j<n; j++){
                if(t.resposta[i][j]==2)
                    subl-=t.mat[i][j];
                if(t.resposta[j][i]==2)
                    subc-=t.mat[j][i];    
            }

            if(somal==s.linha[i])
                aux++;
            if(somac==s.coluna[i])
                aux++;
            if(subl==s.linha[i])
                auxr++;
            if(subc==s.coluna[i])
                auxr++;         
    }

    if(auxr==n*2 || aux==n*2)
        return 1;
    else
        return 0;
}

int ** geravalores(int **mat, int n, char d){ //Essa função gera os valores aleatoriamente da matriz 

    int m, quantnum, intervalo;
    if(d=='F' || d=='M'){
        quantnum=9;
        intervalo=1;
    }
    else{
        quantnum=19;
        intervalo=-9;
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){

            m=rand()%quantnum+intervalo;
            mat[i][j]=m; 
        }
    }
    return mat;
}


Tabela criarMatrizEspelho(Tabela t){ //Essa função irá criar uma matriz que possuirá os resultados da matriz original
    
    int m, n=t.tam, somal=0,aux=0; 
    int *somac=criaVetor(n);
    t.quant_remover_total=0;
    t.gabarito=criaMatriz(n);
    do{ 
        aux=0;
        for(int i=0; i<n; i++)
            somac[i]=0;
        for(int i=0; i<n; i++){       
            do{
                somal=0;
                for(int j=0; j<n; j++){
                    m=rand()%2;

                    if(m){ //Aqui, em 50% das vezes, um valor será escolhido para a soma
                        t.gabarito[i][j]=1;
                        somal+=t.mat[i][j];
                        somac[j]+=t.mat[i][j];
                    }
                    else{//Nos outros 50%, o valor nn será escolhido, tendo o valor zero
                        t.gabarito[i][j]=2;
                        t.quant_remover_total++;
                    }
                }

            }while(somal==0);
        }
        for(int i=0; i<n; i++)
            if(somac[i]==0)
                aux=1;
    }while(aux==1);
    free(somac);
    return t;
}

Soma criaLinhaColuna(Tabela tab){ //Essa função soma as linhas e colunas baseadas na matriz gabarito

    Soma vet;
    vet.linha=criaVetor(tab.tam);
    vet.coluna=criaVetor(tab.tam);
    for(int i=0; i<tab.tam; i++){
        for(int j=0; j<tab.tam; j++){
            if(tab.gabarito[i][j]==1){
                vet.linha[i]+=tab.mat[i][j];
                vet.coluna[j]+=tab.mat[i][j];
            }
        }
    }
    return vet;
}

void montarTab(Tabela t, Soma vet){ //Essa função monta a tabela do jogo com os valores gerados aleatoriamente
    
    int n = t.tam;
    int soma=0;
    
    //___________________________________________
    // Essa parte gera a parte de cima da tabela
    for(int i=0; i<8; i++)
        printf(" ");
    printf(TAB_TL);
    for(int k=0; k<n; k++){

        for(int i=0; i<7; i++)
            printf(TAB_HOR); //Repete o "teto" da tabela a quantidade desejada

        if(k==n-1)
            printf(TAB_TR);
        else
            printf(TAB_TJ);   
    }
    //___________________________________________

    printf("\n");
    //___________________________________________
    //Essa parte mostra os números da matriz junto com as partes intermediárias da tabela
    for(int i=0; i<n; i++){


        if(i==0){
            for(int j=0; j<8; j++)
                printf(" ");
            for(int j=1; j<=n; j++){
                printf(TAB_VER);
                printf("   %d   ",j);
            }
            printf(TAB_VER);
            printf("\n");

            printf(TAB_TL);

            for(int k=0; k<n+2; k++){

                for(int i=0; i<7; i++)
                    printf(TAB_HOR); 

                if(k==n+1)
                    printf(TAB_TR);
                else
                    printf(TAB_MJ);
            }
   
        }
        printf("\n");
        printf(TAB_VER);
        printf("   %d   ", i+1);
        for(int j=0; j<n; j++){
            printf(TAB_VER);
            
            if(t.mat[i][j]>=0)
                printf(" ");
            if(t.resposta[i][j]==1)
                printf( GREEN("  %d   "), t.mat[i][j]);
            else if(t.resposta[i][j]==2)
                printf( RED("  %d   "), t.mat[i][j]);
            else
                printf("  %d   ", t.mat[i][j]);
        }
        soma=0;
        for(int j=0; j<n; j++){
            if(t.resposta[i][j]==1)
                soma+=t.mat[i][j];
        }
        if(soma==vet.linha[i])
            printf(TAB_VER BOLD(GREEN("  %02d  ")), vet.linha[i]);
        else
            printf(TAB_VER BOLD("  %02d  "), vet.linha[i]);
        if(vet.linha[i]>-10)
            printf(" ");
        printf(TAB_VER "\n");        

        if(i<n){
            if(i==n-1)
                printf(TAB_BL);
            else
                printf(TAB_ML);
            for(int k=0; k<n+2; k++){

                for(int i=0; i<7; i++)
                    printf(TAB_HOR); 

                if(k==n+1 && i==n-1)
                    printf(TAB_BR);
                else if(k==n+1)
                    printf(TAB_MR );
                else
                    printf(TAB_MJ);
            }
        }
    }
    //___________________________________________
    printf("\n");
    for(int i=0; i<8; i++)
        printf(" ");
    for(int i=0; i<n; i++){
        soma=0;
        for(int j=0; j<n; j++){
            if(t.resposta[j][i]==1)
                soma+=t.mat[j][i];
        }
        if(soma==vet.coluna[i])
            printf( TAB_VER BOLD(GREEN("  %02d  ")), vet.coluna[i]);
        else
            printf( TAB_VER BOLD("  %02d  "), vet.coluna[i]);
        if(vet.coluna[i]>-10)
            printf(" ");
    }
    printf(TAB_VER);
    printf("\n");


    //___________________________________________
    //Essa parte gera a parte debaixo da tabela
    for(int i=0; i<8; i++)
        printf(" ");
    printf(TAB_BL);
    for(int k=0; k<n; k++){

        for(int i=0; i<7; i++)
            printf(TAB_HOR); 

        if(k==n-1)
            printf(TAB_BR);
        else
            printf(TAB_BJ);
    }
    printf("\n");
    //___________________________________________
    
}

int ** resposta(int l, char *op, int** resposta, int tam,int n){

    int linha, coluna;
    char lin[2], col[2];


    lin[0]=op[l];
    lin[1]='\0';

    col[0]=op[l+1];
    col[1]='\0';

    linha=atoi(lin)-1;
    coluna=atoi(col)-1;

    if(linha>=tam || linha<0 || coluna>=tam || coluna<0){
        n=0;
        printf("Valores de linha ou coluna inválido!\n");
    }
    else
        resposta[linha][coluna]=n;

    return resposta;
}

void ranking(char * nome,int tempo, int n, int param){ //Essa é a função geral do Ranking, que controla quando será necessário adicionar um novo jogador ou não

    Ranking ranking;

    ranking.nome=malloc(QUANTDIMENSOES*sizeof(char**));
    ranking.tempo=malloc(QUANTDIMENSOES*sizeof(int*));
    for(int i=0; i<QUANTDIMENSOES; i++){
        ranking.nome[i]=malloc(QUANTJOGADOR*sizeof(char*));
        ranking.tempo[i]=calloc(QUANTJOGADOR,sizeof(int));
        for(int j=0; j<QUANTJOGADOR; j++){
            ranking.nome[i][j]=malloc(M*sizeof(char));
            strcpy(ranking.nome[i][j], "");
        }
    }

    ranking=armazenaRanking(ranking);

    if(param){
        ranking=adicionaNovoRanking(nome,tempo,n,ranking);
        atualizaRanking(ranking);
    }
    else{
        mostraRanking(ranking);
    }
    
    for(int i=0; i<QUANTDIMENSOES; i++){
        for(int j=0; j<QUANTJOGADOR; j++)
            free(ranking.nome[i][j]);
        free(ranking.tempo[i]);
        free(ranking.nome[i]);
    }
    free(ranking.nome);
    free(ranking.tempo);

}

Ranking armazenaRanking(Ranking r) {
    FILE *arq = fopen("sumplete.ini", "r");
    char linha[M], nome[M], numero[2];
    strcpy(linha, "");
    int n, tam_total, aux, i;

    while (!feof(arq)) {
        i = -1;
        fgets(linha, M, arq);

        if (strstr(linha, "size") != NULL) {
            n = linha[7] - '0';
            n -= 3;
            do {
                i++;
                for (int k = 0; k < 2; k++) {
                    aux = 0;
                    fgets(linha, M, arq);
                    tam_total = strlen(linha);

                    if (strstr(linha, "player") != NULL) {
                        for (int j = 10; j < tam_total - 1; j++) {
                            nome[aux] = linha[j];
                            aux++;
                        }
                        nome[aux] = '\0';
                        strcpy(r.nome[n][i], nome);
                    } 
                    else if (strstr(linha, "time") != NULL) {
                        for (int j = 8; j < tam_total - 1; j++) {
                            numero[0] = linha[j];
                            numero[1] = '\0';
                            r.tempo[n][i] = r.tempo[n][i] * 10 + atoi(numero);
                        }
                    }
                }
            } while (r.tempo[n][i] != 0);
        }
    }
    fclose(arq);
    return r;
}


Ranking adicionaNovoRanking(char *nome, int tempo, int n, Ranking r) {//Essa função pega o novo jogador e seu tempo e o coloca na sua devida posição dentro das variáveis
    int i = 0, aux1 = tempo, aux2;
    n-=3;
    char c_aux1[M], c_aux2[M];
    strcpy(c_aux1, nome);

    while (i < QUANTJOGADOR) {
        if (tempo < r.tempo[n][i] || r.tempo[n][i]==0) {
            for(int j=i; j<QUANTJOGADOR; j++) {
                aux2 = r.tempo[n][j];
                strcpy(c_aux2, r.nome[n][j]);

                r.tempo[n][j] = aux1;
                strcpy(r.nome[n][j], c_aux1);

                aux1 = aux2;
                strcpy(c_aux1, c_aux2);
            }
            break; 
        }
        i++;
    }
    return r;
}

void atualizaRanking(Ranking r){ //Essa função atualiza o arquivo "sumplete.ini" quando tem-se um novo jogador
    FILE *arq=fopen("sumplete.ini","w");
    for(int i=0; i<QUANTDIMENSOES; i++){

        if(r.tempo[i][0]!=0){
            fprintf(arq, "size = %d\n", i+3);
            for(int j=0; j<QUANTJOGADOR; j++){
                if(r.tempo[i][j]!=0){
                    fprintf(arq,"player%d = %s\n", j+1, r.nome[i][j]);
                    fprintf(arq,"time%d = %d\n", j+1, r.tempo[i][j]);
                }
            }
            fprintf(arq,"\n\n");
        }
    }
    fclose(arq);
}

void mostraRanking(Ranking r){ //Essa função pega os valores armazenados do ranking e os mostra em ordem

    for(int i=0; i<QUANTDIMENSOES; i++){
        if(r.tempo[i][0]!=0){
            printf("Tamanho do Tabuleiro - %d\n", i+3);
            for(int j=0; j<QUANTJOGADOR; j++){
                if(r.tempo[i][j]!=0){
                    if(j+1==1 || j+1==2 || j+1==3){
                        printf(YELLOW("%d° Lugar - %s\n"), j+1, r.nome[i][j]);
                        printf(YELLOW("Tempo - %d\n"), r.tempo[i][j]);
                    }
                    else{
                        printf("%d° Lugar - %s\n", j+1, r.nome[i][j]);
                        printf("Tempo - %d\n", r.tempo[i][j]);
                    }
                }
            }
            printf("\n");
        }
    }
}

Geral abreArquivo(char *nome_arq) {
    FILE *arq = fopen(nome_arq, "r");
    Geral g;

    if (arq == NULL) {
        printf(RED("Erro ao abrir o arquivo\n"));
        g.parametro=0;
        return g;
    }

    fscanf(arq, "%d", &g.t.tam);
    int n = g.t.tam;
    g.t.mat = criaMatriz(g.t.tam);
    g.t.resposta = criaMatriz(g.t.tam);
    g.s.linha = criaVetor(g.t.tam);
    g.s.coluna = criaVetor(g.t.tam);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(arq, "%d ", &g.t.mat[i][j]);
        }
    }
    for (int i = 0; i < n; i++) {
        fscanf(arq, "%d ", &g.s.linha[i]);
    }

    for (int i = 0; i < n; i++) {
        fscanf(arq, "%d ", &g.s.coluna[i]);
    }

    fscanf(arq, "%d", &g.t.quant_manter);
    int l, c;
    for (int i = 0; i < g.t.quant_manter; i++) {
        fscanf(arq, "%d", &l);
        fscanf(arq, "%d", &c);
        g.t.resposta[l - 1][c - 1] = 1;
    }
    fscanf(arq, "%d", &g.t.quant_remover);
    for (int i = 0; i < g.t.quant_remover; i++) {
        fscanf(arq, "%d", &l);
        fscanf(arq, "%d", &c);
        g.t.resposta[l - 1][c - 1] = 2;
    }
    fgets(g.j.nome, M, arq);
    // Remove a quebra de linha (\n) do nome
    g.j.nome[strlen(g.j.nome)-1] = '\0';

    fscanf(arq, "%d", &g.j.tempoT);

    fclose(arq);

    g.parametro = 1;
    return g;
}


void salvaArquivo(int l, Tabela t, Soma s, Jogador j){ //Essa função cria o arquivo com o nome desejado pelo jogador

    char nome[M];
    l++;//Pulando o espaço do comando 'salvar texto.txt'
    int n=0;
    
    while(t.opcao[l]!='\n'){ //Esse while vai pegar o nome escolhido pelo jogador para salvar
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

void limparBuffer() { //Essa função limpa o buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void dividePalavra(char *op, char **opcao) {//Essa função irá dividir a primeira palavra de uma frase e retorná-la
    int i = 0;
    while (op[i] != ' ' && op[i] != '\n' && op[i] != '\0') {
        i++;
    }
    *opcao = malloc((i+1) * sizeof(char));
    for (int j = 0; j < i; j++)
        (*opcao)[j] = op[j];
    (*opcao)[i] = '\0';
}

int ** criaMatriz(int n){ //Essa função cria uma matriz

    int **matriz=malloc(n*sizeof(int*));
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


void limpamatriz(int ***mat, int n){ //Essa função limpa a matriz alocada dinamicamente

    for(int i=0; i<n; i++)
        free((*mat)[i]);
    free(*mat);
}

void limpavetor(int **vet){ //Essa função limpa o vetor alocado dinamicamente

    free(*vet);
}
