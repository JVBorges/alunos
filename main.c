/* Integrantes: Felipe Aguiar       S.I *
 *              Joao Vitor Borges   C.C *
 *              Vinicius de Freitas C.C */

#include <stdio.h>
#include <string.h>

// Declaracao das structs
typedef struct {
    int cod_aluno;
    char nome[41];
    int estagio;
} aluno;

typedef struct {
    int dia;
    int mes;
    int ano;
} tdata;

typedef struct {
    int cod_aluno;
    tdata data;
    float nota;
} avaliacao;

// Prototipos das funcoes
void ordena_alunos(int, aluno*);
void cadastrar_aluno();
void cadastrar_avaliacoes();
void listar_alunos();
void listar_avaliacoes();
void consultar_aluno();
void alterar_aluno();
int compara_datas(tdata, tdata);
void lista_av(int, int, aluno*, avaliacao*);

int main()
{
    int op;
    do {
        printf("\t   ============================ \n");
        printf("\t  |  <<1>> Cadastrar aluno     |\n");        //x
        printf("\t  |  <<2>> Listar alunos       |\n");        //x
        printf("\t  |  <<3>> Alterar aluno       |\n");        //x
        printf("\t  |  <<4>> Consultar aluno     |\n");        //x
        printf("\t  |  <<5>> Cadastrar avaliacao |\n");        //x
        printf("\t  |  <<6>> Listar avaliacao    |\n");        //x
        printf("\t  |  <<0>> Sair                |\n");        //x
        printf("\t   ============================ \n");
        scanf("%d", &op);
        switch(op) {
        case 0:
            break;
        case 1:
            cadastrar_aluno();
            break;
        case 2:
            listar_alunos();
            break;
        case 3:
            alterar_aluno();
            break;
        case 4:
            consultar_aluno();
            break;
        case 5:
            cadastrar_avaliacoes();
            break;
        case 6:
            listar_avaliacoes();
            break;
        default:
            printf("Opcao invalida! Tente novamente\n");
            break;
        }
    }
    while(op != 0);
    return 0;
}

void cadastrar_aluno()
{
    FILE * arq;
    aluno a;
    int op;
    do {
    // Abre o arquivo em modo de preenchimento binario
    if ((arq = fopen("alunos.bin", "ab")) == NULL) {
        fprintf(stderr, "Nao foi possivel criar o arquivo alunos.bin\n");
        return;
    }
    // fseek para posicionar o acesso no final do arquivo
    fseek(arq, 0, SEEK_END);
    // a funçao ftell retorna o valor de bytes dentro do arquivo
    // a funçao sizeof retorna o valor de bytes da strcut aluno
    a.cod_aluno = ftell(arq) / sizeof(aluno) + 1;
    printf("\nCodigo do aluno: %d\n", a.cod_aluno);
    printf("Nome do aluno  : ");
    scanf(" %40[^\n]", a.nome);
    // Ciro: deveria informar o que o usuário tem que digitar no estágio (Interface: -0.25)
    printf("Estagio        : ");
    scanf("%d", &a.estagio);
    // Ciro: não valida o estágio (Interface: -0.25)
    fwrite(&a, sizeof(aluno), 1, arq);
    fclose(arq);
    printf("\n\nDeseja cadastrar um novo aluno?\n<1> Sim\n<0> Nao\n");
    scanf("%d", &op);
    } while(op != 0);
    printf("\n\n");
}

void listar_alunos()
{
    // Abre o arquivo na funcao de leitura binaria
    FILE * arq;
    if ((arq = fopen("alunos.bin", "rb")) == NULL) {
        fprintf(stderr, "Nenhum aluno cadastrado!\n\n");
        return;
    }
    fseek(arq, 0, SEEK_END);
    // n recebe o valor da quantidade de estruturas(-1) contida no arquivo
    // para servir de indice ao vetor de struct aluno
    int n = ftell(arq) / sizeof(aluno), op, i, j;
    aluno a[n];
    rewind(arq);
    // leitura de todas estruturas aluno salvo no arquivo
    fread(a, sizeof(aluno), n, arq);
    fclose(arq);
    // chamada a funcao de ordenacao
    ordena_alunos(n, a);
    printf("\n\n");
    printf("  ================================================================ \n");
    printf(" |                        Alunos cadastrados                      |\n");
    printf("  ================================================================ \n");
    printf(" |Codigo  |Nome                                    |Estagio       |\n");
    printf("  ================================================================\n");
    for (i = 0; i < n; i++) {
        printf(" |%07d |%-40s|", a[i].cod_aluno, a[i].nome);
        // Ciro: poderia ter usado um if-else ou switch-case (Legibilidade: -0.25)
        if (a[i].estagio == 1) printf("Iniciante     |\n");
        if (a[i].estagio == 2) printf("Intermediario |\n");
        if (a[i].estagio == 3) printf("Avancado      |\n");
    }
    printf("  ================================================================ \n");
    printf("\n\nDeseja criar um arquivo de texto com a lista de alunos?\n<1> Sim\n<0> Nao\n");
    scanf("%d", &op);
    if (op == 1) {
        // Abre o arquivo no tipo de escritura de texto
        // Preenche o arquivo de texto com os dados da estrutura aluno
        FILE * arq;
        if ((arq = fopen("alunos.txt", "w")) == NULL) {
            fprintf(stderr, "Erro ao abrir o arquivo\n\n");
            return;
        }
        fprintf(arq, "\n\n");
        fprintf(arq, "  ================================================================ \n");
        fprintf(arq, " |                        Alunos cadastrados                      |\n");
        fprintf(arq, "  ================================================================ \n");
        fprintf(arq, " |Codigo  |Nome                                    |Estagio       |\n");
        fprintf(arq, "  ================================================================ \n");
        for (i = 0; i < n; i++) {
            fprintf(arq, " |%07d |%-40s|", a[i].cod_aluno, a[i].nome);
	        // Ciro: poderia ter usado um if-else ou switch-case (Legibilidade: -0.25)
            if (a[i].estagio == 1) fprintf(arq, "Iniciante     |\n");
            if (a[i].estagio == 2) fprintf(arq, "Intermediario |\n");
            if (a[i].estagio == 3) fprintf(arq, "Avancado      |\n");
        }
        fprintf(arq, "  ================================================================ \n\n");
        printf("Arquivo criado com sucesso!\n");
        fclose(arq);
    }
}

void ordena_alunos(int n, aluno a[])
{
    int i, j;
    aluno aux;
    // Algoritmo de ordenacao do tipo de insercao por ordem alfabetica
    // strcmp(str1, str2) devolve 0 caso as duas strings sejam iguais
    //                    devolve > 0 caso a str1 venha depois de str2
    //                    devolve < 0 caso contrario

   /* Ciro: não era necessário ordenar o arquivo 2 vezes, desde que a condição do for fosse a seguinte:
	* j > 0 && ((aux.estagio < a[j - 1].estagio) || (aux.estagio == a[j - 1].estagio && strcasecmp(aux.nome, a[j - 1].nome) < 0)) */

    for (i = 1; i < n; i++) {
        aux = a[i];
        for (j = i; (j > 0) && (strcmp(aux.nome, a[j - 1].nome) < 0); j--) {
            a[j] = a[j - 1];
        }
        a[j] = aux;
    }
    // Algoritmo de ordenacao do tipo de insercao por ordem do menor para o maior estagio
    for (i = 1; i < n; i++) {
        aux = a[i];
        for (j = i; (j > 0) && (aux.estagio < a[j - 1].estagio); j--) {
            a[j] = a[j - 1];
        }
        a[j] = aux;
    }
}

void cadastrar_avaliacoes()
{
    FILE * arq;
    avaliacao av;
    int cod;
    if ((arq = fopen("alunos.bin", "rb")) == NULL) {
        fprintf(stderr, "Nenhum aluno cadastrado!\n\n");
        return;
    }
    printf("Codigo do aluno : ");
    scanf("%d", &cod);
    // Funcao fseek para posicionar o acesso a partir da posicao do cod do aluno solicitado
    fseek(arq, (cod - 1) * sizeof(aluno), SEEK_SET);
    aluno a;
    // Funcao fread para ler os dados da struct aluno do aluno solicitado
    fread(&a, sizeof(aluno), 1, arq);
    fclose(arq);
    if (a.cod_aluno != cod) {
        printf("Nao existe aluno registrado com esse codigo (%d)\n\n", cod);
        return;
    }
    printf("Nome do aluno   : %s\n", a.nome);
    // Ciro: poderia ter usado um if-else ou switch-case (Legibilidade: -0.25)
    if (a.estagio == 1) printf("Estagio do Aluno: Iniciante\n");
    if (a.estagio == 2) printf("Estagio do Aluno: Intermediario\n");
    if (a.estagio == 3) printf("Estagio do Aluno: Avancado\n");
    // Abre o arquivo em modo de preenchimento binario para o cadastro das avaliacoes
    if ((arq = fopen("avaliacoes.bin", "ab")) == NULL) {
        printf("Erro ao criar o arquivo avaliacoes.bin\n\n");
        return;
    }
    av.cod_aluno = cod;
    printf("Data da avaliacao (dd/mm/aaaa): ");  // Ciro: pede a data separada por / mas exige espaço (Interface: -0.5)
    scanf("%d %d %d", &av.data.dia, &av.data.mes, &av.data.ano);
    printf("Nota: ");
    scanf("%f", &av.nota); // Ciro: não valida a nota (Interface: -0.25)
    fwrite(&av, sizeof(avaliacao), 1, arq);
    fclose(arq);
    printf("\n\n");
}

void listar_avaliacoes()
{
    FILE * arq;
    tdata dt1, dt2;
    if ((arq = fopen("alunos.bin", "rb")) == NULL) {
        fprintf(stderr, "Nenhum aluno cadastrado!\n\n");
        return;
    }
    fseek(arq, 0, SEEK_END);
    // n_a recebe o valor da quantidade de estruturas(-1) contida no arquivo
    // para servir de indice ao vetor de struct aluno
    int n_a = ftell(arq) / sizeof(aluno), i;
    aluno a[n_a];
    rewind(arq);
    fread(a, sizeof(aluno), n_a, arq);
    fclose(arq);

    if ((arq = fopen("avaliacoes.bin", "rb")) == NULL) {
        fprintf(stderr, "Nenhuma avaliacao cadastrada!\n\n");
        return;
    }
    fseek(arq, 0, SEEK_END);
    // n_av recebe o valor da quantidade de estruturas(-1) contida no arquivo
    // para servir de indice ao vetor de struct avaliacao
    int n_av = ftell(arq) / sizeof(avaliacao);
    avaliacao av[n_av];
    rewind(arq);
    fread(av, sizeof(avaliacao), n_av, arq);
    fclose(arq);
    printf("\n\nInforme as datas no formato dd mm aaaa\n");
    printf("Data Inicial: ");
    scanf("%d %d %d", &dt1.dia, &dt1.mes, &dt1.ano);
    printf("Data Final  : ");
    scanf("%d %d %d", &dt2.dia, &dt2.mes, &dt2.ano);
    printf("\n\n");
    int cont = 0;
    printf(" ========================================================== \n");
    printf("|Nome                                    |Data       |Nota |\n");
    printf(" ========================================================== \n");
    for (i = 0; i < n_av; i++) {
        tdata aux = av[i].data;
        // Chamada da comparacao das datas
        if (compara_datas(aux, dt2) < 0) { // Ciro: <= 0  (Interface: -0.25)
            if (compara_datas(aux, dt1) >= 0) {
                lista_av(n_a, i, a, av);
                cont++;
            }
        }
    }
    if (cont == 0) {
        printf("|    Nao ha avaliacoes cadastradas dentro deste periodo    |\n");
        printf("|          ou data invalida! (maior para menor)            |\n");
        printf(" ========================================================== \n\n");
    }
    else {
        printf(" ========================================================== \n\n");
    }
}

int compara_datas(tdata dt1, tdata dt2)
{
    // Funcao para comprar as datas
    // Caso data1 seja maior que data2 a funcao retorna 1
    // Caso contrario retorna -1
    // Caso sejam iguais retorna 0
    int data1, data2;
    data1 = dt1.ano * 10000 + dt1.mes * 100 + dt1.dia;
    data2 = dt2.ano * 10000 + dt2.mes * 100 + dt2.dia;
    if (data1 > data2) {
        return 1;
    }
    else {
        if (data1 < data2) {
            return -1;
        }
        else {
            return 0;
        }
    }
}

void lista_av(int n_a, int i, aluno a[], avaliacao av[])
{
    int j;
    for (j = 0; j < n_a; j++) {
        if (a[j].cod_aluno == av[i].cod_aluno) {
            printf("|%-40s", a[j].nome);
            break;
                }
            }
    printf("|%02d/%02d/%d |", av[i].data.dia, av[i].data.mes, av[i].data.ano);
    printf("%05.2f|\n", av[i].nota);
}

void consultar_aluno()
{
    FILE * arq;
    int cod;

    if ((arq = fopen("alunos.bin", "rb")) == NULL) {
        fprintf(stderr, "Nenhum aluno cadastrado!\n\n");
        return;
    }
    printf("\n\nCod do aluno    : ");
    scanf("%d", &cod);
    // Funcao fseek para posicionar o acesso a partir da posicao do cod do aluno solicitado
    fseek(arq, (cod - 1) * sizeof(aluno), SEEK_SET);
    aluno a;
    // Funcao fread para ler os dados da struct aluno do aluno solicitado
    fread(&a, sizeof(aluno), 1, arq);
    fclose(arq);
    if (a.cod_aluno != cod) {
        printf("Nao existe aluno registrado com esse codigo (%d)\n\n", cod);
        return;
    }
    printf("Nome do Aluno   : %s\n", a.nome);

    // Ciro: poderia ter usado um if-else ou switch-case (Legibilidade: -0.25)
    if (a.estagio == 1) printf("Estagio do Aluno: Iniciante\n");
    if (a.estagio == 2) printf("Estagio do Aluno: Intermediario\n");
    if (a.estagio == 3) printf("Estagio do Aluno: Avancado\n");

    if ((arq = fopen("avaliacoes.bin", "rb")) == NULL) {
        fprintf(stderr, "Nenhuma avaliacao cadastrada!\n\n");
        return;
    }
    fseek(arq, 0, SEEK_END);
    // n recebe o valor da quantidade de estruturas(-1) contida no arquivo
    // para servir de indice ao vetor de struct avaliacao
    int n = ftell(arq) / sizeof(avaliacao), i;
    avaliacao av[n];
    rewind(arq);
    fread(av, sizeof(avaliacao), n, arq);
    fclose(arq);
    printf ("\n\n");
    printf("\t\t ================= \n");
    printf("\t\t|    Avaliacoes   |\n");
    printf("\t\t ================= \n");
    printf("\t\t|Data       |Nota |\n");
    printf("\t\t =================\n");
    for (i = 0; i < n; i++) {
        if (av[i].cod_aluno == cod) {
            printf("\t\t|%02d/%02d/%d |%05.2f|\n", av[i].data.dia, av[i].data.mes, av[i].data.ano, av[i].nota);
        }
    }
    printf("\t\t =================\n\n\n");
}

void alterar_aluno()
{
    FILE * arq;
    int cod;
    aluno a;
    // Abre o arquivo no modo leitura/gravacao binaria
    if ((arq = fopen("alunos.bin", "r+b")) == NULL) {
        fprintf(stderr, "Nenhum aluno cadastrado!\n\n");
        return;
    }
    printf("Cod do aluno: ");
    scanf("%d", &cod);
    // Funcao fseek para posicionar o acesso a partir da posicao do cod do aluno solicitado
    fseek(arq, (cod - 1) * sizeof(aluno), SEEK_SET);
    // Funcao fread para ler os dados da struct aluno do aluno solicitado
    fread(&a, sizeof(aluno), 1, arq);
    if (a.cod_aluno != cod) {
        printf("Nao existe aluno cadastrado com esse codigo (%d)!\n\n", cod);
        return;
    }
    printf("\n\nAluno           : %s\n", a.nome);

    // Ciro: poderia ter usado um if-else ou switch-case (Legibilidade: -0.25)
    if (a.estagio == 1) printf("Estagio do Aluno: Iniciante\n");
    if (a.estagio == 2) printf("Estagio do Aluno: Intermediario\n");
    if (a.estagio == 3) printf("Estagio do Aluno: Avancado\n");
    printf("\nNovo nome   : ");
    scanf(" %40[^\n]", a.nome);

    // Ciro: deveria informar o que o usuário tem que digitar no estágio (Interface: -0.25)
    printf("Novo estagio: ");
    scanf("%d", &a.estagio);
    // Ciro: não valida o estágio (Interface: -0.25)
    // Reeutilizacao do fseek para retornar a posicao do aluno solicitado para assim poder gravar os novos dados
    fseek(arq, (cod - 1) * sizeof(aluno), SEEK_SET);
    fwrite(&a, sizeof(aluno), 1, arq);
    fclose(arq);
    printf("Aluno alterado com sucesso!\n\n");
}


/*---------------------------
          AVALIAÇÃO
  ---------------------------
        APRESENTAÇÃO
  ---------------------------
  - Felipe Aguiar:  6.00
  - João Vitor...: 10.00
  - Vinicius.....:  7.00
   ---------------------------
        CÓDIGO FONTE
  ---------------------------
  - Corretude...: 10.00
  - Interface...:  8.00
  - Legibilidade:  8.75
  - Nota........:  9.50
  ---------------------------
  	   NOTA DO TRABALHO
  ---------------------------
  - Felipe Aguiar:  7.75
  - João Vitor...:  9.75
  - Vinicius.....:  8.25
  ---------------------------*/
