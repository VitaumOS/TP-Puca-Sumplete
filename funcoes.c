//Vitor de Oliveira Silva 23.1.4023
#include "funcoes.h"

void opcoes(){ //Função em que terá o menu inicial do jogo

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
            
            if ((strlen(opcao) != 2) || (op < '0' || op > '4')) { //Isso evita que o jogador escolha uma opção errada
                printf(RED("Opção Inválida!\n"));
                limparBuffer();
            }

        } while ((strlen(opcao) != 2) || (op < '0' || op > '4')); //Usuário ficará no loop enquanto ele digitar a opção errada

        switch(op){ //Pega o valor digitado pelo usuário
            case '0': //Caso '0': Fecha o jogo e libera a memória (caso necessário) antes de fechar

                if(g.parametro==2)
                    ranking(g.j.nome,g.j.tempoF,g.t.tam, 1);
                else if(g.parametro==1 || g.parametro==3){
                    limpamatriz(&g.t.mat, g.t.tam);
                    limpamatriz(&g.t.resposta, g.t.tam);
                    limpamatriz(&g.t.gabarito, g.t.tam);
                    limpavetor(&g.s.linha);
                    limpavetor(&g.s.coluna);
                }

                printf("Obrigado por jogar!\n");
                break;
            case '1': //Caso '1': Inicia um novo jogo
                
                //Primeiro, pede o nome do jogador
                printf("Primeiramente, digite seu nome: ");
                fgets(g.j.nome, M, stdin);
                g.j.tam_nome=strlen(g.j.nome);
                g.j.nome[g.j.tam_nome-1]='\0';
                system("clear");
                do{//Aqui, ele ficará num loop caso digite o tamanho de dimensão errada
                    printf("%s, digite a dimensão que deseja (de 3 a 9): ", g.j.nome);

                    fgets(opcao, 4, stdin);
                    g.t.tam = atoi(opcao); //Para evitar problemas na entrada, o usuário digita uma string e depois a converte em inteiro

                    system("clear");
                    if(g.t.tam<3 || g.t.tam>9){
                        printf(RED("Valor inválido!\n"));
                        limparBuffer();
                    }
                }while(g.t.tam<3 || g.t.tam>9);
                if(g.t.tam>=5){ //O menu de dificuldades só aparece caso o tamanho do tabuleiro seja maior ou igual a 5
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
                else //Caso seja abaixo de 5 o tamanho do tabuleiro, automaticamente a dificuldade será no fácil
                    g.t.dificuldade='F';
                
                g=jogo(g,0); //Acessa a função principal com o parâmetro 0 (iniciando novo jogo)

                break;
            case '2': //Caso '2': Pega o nome do arquivo.txt e o coloca no jogo

                printf("Digite o nome do arquivo: ");
                scanf("%s", nome_arquivo);
                if(verificaNomeArquivo(nome_arquivo)){//Verifica se o arquivo é do formato texto (txt)
                    g=abreArquivo(nome_arquivo); //Abre o arquivo e coloca todas as informações dele em variáveis
                    if(g.parametro==1 || g.parametro==3)
                        g=jogo(g,g.parametro);
                }
                limparBuffer();
                system("clear");
                break;
            case '3':// Caso '3': Continua um jogo já iniciado
                if(g.parametro==1 || g.parametro==3) //Verifica os parâmetros 1 (iniciou um jogo mas não finalizou) e 2 (iniciou um jogo sem gabarito mas não finalizou)
                    g=jogo(g,g.parametro);
                else 
                    printf(RED("Você não tem um jogo iniciado!\n"));
                break;
            case '4': //Caso '4': Acessa o ranking do jogo (apenas para visualizar)

                ranking(g.j.nome,g.j.tempoF,g.t.tam,0);
                break;
            default: // Default: digitou a opção errada

                printf(RED("Opção Inválida!\n"));
                break;
        }
    }while(op!='0');
}

int verificaNomeArquivo(char *arquivo){ // Essa função verifica se o formato digitado pelo usuário é do tipo texto (.txt)
    
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

Geral jogo(Geral g, int parametro){ //Nessa função ocorre o jogo em si

    int vitoria=0;
    int r, tam;

    g.j.tempoI=time(NULL);
    
    if(parametro==0){ // Inicia todas as variáveis quando o jogo foi iniciado pela primeira vez
        g.j.tempoT=0;

        g.t.quant_manter=0;
        g.t.quant_remover=0;

        g.t.mat=criaMatriz(g.t.tam);
        g.t.resposta=criaMatriz(g.t.tam);
        g.t.mat=geravalores(g.t.mat, g.t.tam, g.t.dificuldade);
        g.t=criarMatrizGabarito(g.t);
        g.s=criaLinhaColuna(g.t);
    }
    if(g.parametro==3) // Quando o jogo foi criado por arquivo e não foi possível criar o gabarito, essa mensagem irá aparecer
        printf("Não foi possível criar a Matriz Gabarito. Você poderá jogar normalmente, mas as opções 'Dica' e 'Resolver' estarão indisponíveis!\n");

    montarTab(g.t,g.s); // Monta o tabuleiro do jogo

    while(vitoria == 0){ //O jogo será mantido até que o usuário ganhe o jogo

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

        if(strstr(g.t.opcao, "manter")!=NULL && tam==10){ // Função "manter": Ela pega as coordenadas escolhidas pelo usuário a mantém na soma
            g.t.resposta=resposta(7, g.t.opcao, g.t.resposta,g.t.tam,1);
            g.t.quant_manter++;
        }
        else if(strstr(g.t.opcao, "remover")!=NULL && tam==11){// Função "remover": Ela pega as coordenadas escolhidas pelo usuário a remove da soma
            g.t.resposta=resposta(8, g.t.opcao, g.t.resposta,g.t.tam,2);
            g.t.quant_remover++;
        }
        else if(strstr(g.t.opcao, "dica")!=NULL && tam==5){ // Função "dica": escolhe um valor aleatório da matriz gabarito que entra na soma e a marca
            if(g.parametro==3)// Caso não tenha a matriz gabarito, não será possível acessar as dicas
                printf("Você não pode utilizar a função dica!\n");
            else{
                g.t.resposta=dica(g.t);
                g.t.quant_manter++;
            }
        }
        else if(strstr(g.t.opcao, "resolver")!=NULL && tam==9){ // Função "resolver": pega os valores da matriz gabarito e coloca na matriz resposta
            if(g.parametro==3) // Caso não tenha a matriz gabarito, não será possível resolver o jogo
                printf("Você não pode utilizar a função resolver!\n");
            else
                g.t.resposta=resolver(g.t);
        }
        else if(strstr(g.t.opcao, "salvar")!=NULL){ // Função "salvar": dado o nome do arquivo de texto, as informações do jogo são salvas
            g.j.tempoF=time(NULL)-g.j.tempoI;
            if(salvaArquivo(6,g.t,g.s,g.j))
                printf("Jogo Salvo!\n");
        }
        else if(strstr(g.t.opcao, "voltar")!=NULL && tam==7){// Função "voltar": o jogo volta para o menu principal
            if(g.parametro!=3)
                g.parametro=1;
            g.j.tempoT=(time(NULL)-g.j.tempoI)+g.j.tempoT;
            return g;
        }
        else{
            printf(RED("Comando Inválido!\n"));
        }
        montarTab(g.t,g.s);

        vitoria=verificaVitoria(g.t, g.s); //A função verifica se o jogo está completo (a soma dos valores selecionados é igual as somas das linhas/colunas)
    }
    g.j.tempoF=(time(NULL)-g.j.tempoI)+g.j.tempoT;//marca o tempo em que o jogo foi finalizado
    printf("VOCÊ GANHOU!\n");
    printf("Você terminou o jogo em %d segundos!\n", g.j.tempoF);

    g.parametro=2;
    // libera todas as matrizes e vetores usadas
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
    if(d=='F' || d=='M'){ // Caso a dificuldade seja Fácil ou Média, os valores serão de 1 a 9
        quantnum=9;
        intervalo=1;
    }
    else{ // Caso a dificuldade seja Difícil, os valores serão de -9 a 9
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


Tabela criarMatrizGabarito(Tabela t){ //Essa função irá criar uma matriz que possuirá os resultados da matriz original
    
    int m, n=t.tam, somal=0,auxl=0,aux=0;
    int *auxc=criaVetor(n);
    t.quant_remover_total=0;
    t.gabarito=criaMatriz(n);
    switch(t.dificuldade){
        case 'F': // Caso F (Fácil): Pode ter soma das linhas/colunas igual a zero ou o valor total da linha/coluna
            for(int i=0; i<n; i++){       
                        for(int j=0; j<n; j++){
                            m=rand()%2;
                            if(m){ //Aqui, em 50% das vezes, um valor será escolhido para a soma
                                t.gabarito[i][j]=1;
                            }
                            else{//Nos outros 50%, o valor nn será escolhido, tendo o valor zero
                                t.gabarito[i][j]=2;
                                t.quant_remover_total++;
                            }
                        }
            }
            break;
        case 'M':// Caso M (Médio): Não pode ter soma das linhas/colunas igual a zero ou o valor total da linha/coluna
            do{ 
                aux=0;
                for(int i=0; i<n; i++)
                    auxc[i]=0;
                for(int i=0; i<n; i++){       
                    do{
                        auxl=0;
                        for(int j=0; j<n; j++){
                            m=rand()%2;

                            if(m){ //Aqui, em 50% das vezes, um valor será escolhido para a soma
                                t.gabarito[i][j]=1;
                                auxl++;
                                auxc[j]++;
                            }
                            else{//Nos outros 50%, o valor nn será escolhido, tendo o valor zero
                                t.gabarito[i][j]=2;
                                t.quant_remover_total++;
                            }
                        }

                    }while(auxl==0 || auxl==n);
                }
                for(int i=0; i<n; i++)
                    if(auxc[i]==0 || auxc[i]==n)
                        aux=1;
            }while(aux==1);
            break;
        case 'D': // Caso D (Difícil): Pode ter soma das linhas/colunas igual a zero , mas não igual ao valor total da linha/coluna
            do{ 
                aux=0;
                for(int i=0; i<n; i++)
                    auxc[i]=0;

                for(int i=0; i<n; i++){       
                    do{
                        auxl=0;
                        for(int j=0; j<n; j++){
                            m=rand()%2;

                            if(m){ //Aqui, em 50% das vezes, um valor será escolhido para a soma
                                t.gabarito[i][j]=1;
                                auxl++;
                                auxc[j]++;     
                            }
                            else{//Nos outros 50%, o valor nn será escolhido, tendo o valor zero
                                t.gabarito[i][j]=2;
                                t.quant_remover_total++;
                            }
                        }
                    }while(auxl==n);
                }
                for(int i=0; i<n; i++)
                    if(auxc[i]==n)
                        aux=1;
            }while(aux==1);
            break;
    }    
    free(auxc);
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

    printf("\n");
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
            
            if(t.mat[i][j]>=0) //Para igualar o espaço na matriz, caso o número não tenha um sinal negativo, ele pulará um espaço
                printf(" ");
            if(t.resposta[i][j]==1) // Se o valor foi escolhido pelo usuário, então ele ficará verde
                printf( GREEN("  %d   "), t.mat[i][j]);
            else if(t.resposta[i][j]==2) // Se o valor foi removido pelo usuário, então ele ficará vermelho
                printf( RED("  %d   "), t.mat[i][j]);
            else
                printf("  %d   ", t.mat[i][j]);
        }
        soma=0;
        for(int j=0; j<n; j++){
            if(t.resposta[i][j]==1)
                soma+=t.mat[i][j];
        }
        if(soma==vet.linha[i]) //Se a soma dos valores selecionados pelo usuário deem a soma da linha, a soma da linha ficará verde
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

    printf("\n");
    for(int i=0; i<8; i++)
        printf(" ");
    for(int i=0; i<n; i++){
        soma=0;
        for(int j=0; j<n; j++){
            if(t.resposta[j][i]==1)
                soma+=t.mat[j][i];
        }
        if(soma==vet.coluna[i])//Se a soma dos valores selecionados pelo usuário deem a soma da coluna, a soma da coluna ficará verde
            printf( TAB_VER BOLD(GREEN("  %02d  ")), vet.coluna[i]);
        else
            printf( TAB_VER BOLD("  %02d  "), vet.coluna[i]);
        if(vet.coluna[i]>-10)
            printf(" ");
    }
    printf(TAB_VER);
    printf("\n");

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
}

int ** resposta(int l, char *op, int** resposta, int tam,int n){ // Essa função marcará como mantido ou removido o número escolhido pelo usuário

    int linha, coluna;
    char lin[2], col[2];

    lin[0]=op[l];
    lin[1]='\0';

    col[0]=op[l+1];
    col[1]='\0';
    //Como os valores digitados pelo usuário eram caracteres, vamos convertêlo de caracter para inteiro (atoi)
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
    //Aloca dinamicamente as variáveis do nome dos usuários e seus tempos
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

    ranking=armazenaRanking(ranking); //Primeiro, armazenaremos todos os valores que estão no arquivo "sumplete.ini"

    if(param){ // Se o parâmetro for 1, ele irá adicionar um novo nome e tempo em seu devido tamanho e em sua devida posição
        ranking=adicionaNovoRanking(nome,tempo,n,ranking);
        atualizaRanking(ranking); // Aqui ele atualiza o arquivo sumplete.ini com o novo nome e tempo
    }
    else{
        mostraRanking(ranking); // Essa função irá mostrar todos os nomes e tempos em seus devidos tamanhos e posições
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

Ranking armazenaRanking(Ranking r) {// Essa função pega os valores de tamanho, jogador e tempo

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
                    else
                        break;
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
            fprintf(arq,"\n");
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

Geral abreArquivo(char *nome_arq) { // Abre e armazena todas as informações do arquivo em variáveis

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
            fscanf(arq, "%d", &g.t.mat[i][j]);
        }
    }
    for (int i = 0; i < n; i++) {
        fscanf(arq, "%d", &g.s.linha[i]);
    }
    for (int i = 0; i < n; i++) {
        fscanf(arq, "%d", &g.s.coluna[i]);
    }

    fscanf(arq, "%d", &g.t.quant_manter);
    int l, c;
    for (int i = 0; i < g.t.quant_manter; i++) {
        fscanf(arq, "%d %d", &l, &c);
        g.t.resposta[l - 1][c - 1] = 1;
    }
    
    fscanf(arq, "%d", &g.t.quant_remover);
    for (int i = 0; i < g.t.quant_remover; i++) {
        fscanf(arq, "%d %d", &l, &c);
        g.t.resposta[l - 1][c - 1] = 2;
    }

    fgets(g.j.nome, M, arq);
    fgets(g.j.nome, M, arq);
    g.j.nome[strlen(g.j.nome)-1]='\0';

    fscanf(arq, " %d", &g.j.tempoT);

    fclose(arq);
    limparBuffer();
    
    g.t.gabarito = resolveGabarito(g.t, g.s); //Pega os valores da matriz com os valores do jogo, a soma das linhas e colunas e tenta criar a matriz gabarito
    g.parametro = 1;
    // Verifica se o jogo possui uma matriz gabarito completo, se não possuir, não a usará 
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g.t.gabarito[i][j] == 0) {
                g.parametro = 3;
                break;
            }
        }
    }
    return g;
}

int salvaArquivo(int l, Tabela t, Soma s, Jogador j){ //Essa função cria o arquivo com o nome desejado pelo jogador

    char nome[M];
    l++;//Pulando o espaço do comando 'salvar texto.txt'
    int n=0;
    
    while(t.opcao[l]!='\n'){ //Esse while vai pegar o nome escolhido pelo jogador para salvar
        nome[n]=t.opcao[l];
        n++;
        l++;
    }
    nome[n]='\0';

    if(!verificaNomeArquivo(nome))
        return 0;
        
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
    return 1;
}

int ** resolveGabarito(Tabela t, Soma s){ // Essa função tenta criar a matriz gabarito do jogo
    int n=pow(2,t.tam), soma=0, quant,aux,val, q=0,x, cont=0;

    int *linhaF=criaVetor(t.tam);
    t.gabarito=criaMatriz(t.tam);
    int **linha=malloc(n*sizeof(int*));
    for(int i=0; i<n; i++)
        linha[i]=calloc(t.tam+1,sizeof(int));

    do{
        for(int i=0; i<t.tam; i++){
            q=0;
            for(int k=0; k<n; k++){
                soma=0;
                linha[k][t.tam]=0;
                for(int j=0; j<t.tam; j++){
                    if((k>>j)&1){ // Essa parte faz com que todas as possibilidades (2 elevado ao tamanho do tabuleiro) sejam testadas
                        soma+=t.mat[i][j];
                        linha[k][j]=1;
                    }
                    else
                        linha[k][j]=2;
                }
                if(soma==s.linha[i] ){ // Se a soma gerada é igual a soma da linha, ele irá guardar essa posição
                    q++;    
                    linha[k][t.tam]=1;
                }  
            }    
            for(int j=0; j<t.tam; j++){ // Essa parte observará se todas as possibilidades iguais a soma da linha possui algum número que sempre é mantido ou é sempre removido
                aux=0;
                val=0;
                x=0;
                for(int k=0; k<n; k++){
                    if(linha[k][t.tam]==1 && x==0){
                        val=linha[k][j];
                        x=1;
                    }
                    if(val==linha[k][j] && linha[k][t.tam]==1){
                        aux++;
                    }
                }
                if(aux==q)
                    t.gabarito[i][j]=val;    
            }          
        }
        for(int i=0; i<t.tam; i++){ // Essa parte o mesmo da função de cima, só que para as colunas
            q=0;
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
                if(soma==s.coluna[i] ){
                    q++;    
                    linha[k][t.tam]=1;
                } 
            }    
            for(int j=0; j<t.tam; j++){
                aux=0;
                val=0;
                x=0;
                for(int k=0; k<n; k++){
                    if(linha[k][t.tam]==1 && x==0){
                        val=linha[k][j];
                        x=1;
                    }
                    if(val==linha[k][j] && linha[k][t.tam]==1){
                        aux++;
                    }
                }
                if(aux==q)
                    t.gabarito[j][i]=val;  
            }
        }        
        
        for(int i=0; i<t.tam; i++){ // Aqui pegará todas as possibilidades iguais a soma da linha. Caso tenha uma única possibilidade, logo ela será incluída na matriz gabarito

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
                    if(t.gabarito[i][j]!=linha[k][j] && t.gabarito[i][j]!=0)
                            linha[k][t.tam]=2;
                    if(soma==s.linha[i] && linha[k][t.tam]!=2){
                        quant++;
                        linha[k][t.tam]=1;
                    }  
            }
            if(quant==1)
                for(int k=0; k<n; k++)
                    if(linha[k][t.tam]==1)
                        for(int j=0; j<t.tam; j++)
                            linhaF[j]=linha[k][j];
        
            else              
                for(int j=0; j<t.tam; j++)
                    linhaF[j]=0;  

            for(int j=0; j<t.tam; j++)
                if(t.gabarito[i][j]==0)
                    t.gabarito[i][j]=linhaF[j];
                
        }

        for(int i=0; i<t.tam; i++){ // Aqui fará o mesmo que a função de cima, mas agora com a soma das colunas como referência
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
                    if(t.gabarito[j][i]!=linha[k][j] && t.gabarito[j][i]!=0)
                        linha[k][t.tam]=2;
                    if(soma==s.coluna[i] && linha[k][t.tam]!=2){
                        quant++;
                        linha[k][t.tam]=1;
                    }  
                }
                if(quant==1)
                    for(int k=0; k<n; k++)
                        if(linha[k][t.tam]==1)
                            for(int j=0; j<t.tam; j++)                           
                                linhaF[j]=linha[k][j];     
                else
                    for(int j=0; j<t.tam; j++)
                        linhaF[j]=0; 
                
                for(int j=0; j<t.tam; j++)
                    if(t.gabarito[j][i]==0)
                        t.gabarito[j][i]=linhaF[j];
        }
        aux=0;
        for(int i=0; i<t.tam; i++)
            for(int j=0; j<t.tam; j++)
                if(t.gabarito[i][j]!=0)
                    aux++;
            
        cont++;
    }while(aux!=(t.tam*t.tam) && cont<10); // Para evitar de ter um loop infinito que nunca será resolvido, ele será feito pelo menos 10 vezes
        
    for(int i=0; i<n; i++){
        free(linha[i]);
    }
    free(linha);
    free(linhaF);
    return t.gabarito;
}

void limparBuffer() { //Essa função limpa o buffer

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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
