/*
  Autor......:  Caua Hora de Barros 
  Data.......:  06/18/2024
  Objetivo...:  Sistema de  Controle de Estoque
*/

// NOTE: A parte de Moviemntação quando coloca o codigo do produto ela fecha o programa
// e eu nao estou conseguindo de jeito nenhum ajeitar isso , porem toda logica estaca correta

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>

// Cadastro de Produto
typedef struct {
    int cd_produto;
    char ds_produto[50];
    char de_unid_med[3];
    char de_validade[11];
    float qt_produto;
    float vl_CustoMedio;
    float vl_total;
} reg_produto;

typedef struct TipoItem *TipoApontador;

typedef struct TipoItem {
    reg_produto conteudo;
    TipoApontador proximo;
} TipoItem;

typedef struct {
    TipoApontador primeiro;
    TipoApontador ultimo;
} TipoLista;

// Movimentaçao de Estoque
typedef struct {
    char  dt_movim[11];
    int   cd_prod_movim;
    char  tp_movim;
    float qt_movim;
    float vl_produto;
    float vl_unit_movim;
    float vl_total_movim;
} reg_movim;

typedef struct TipoItem_movim *TipoApontador_movim;

typedef struct TipoItem_movim {
    reg_movim conte_movim;
    TipoApontador_movim proximo;
} TipoItem_movim;

typedef struct {
    TipoApontador_movim primeiro;
    TipoApontador_movim ultimo;
} TipoLista_movim;


void gotoxy(int x, int y);
void tela();

void inicializarListaProdutos(TipoLista *lista);
void inserirProdutoNaPosicao(TipoLista *lista, reg_produto produto, int posicao);
void inserirProdutoNoFinal(TipoLista *lista, reg_produto produto);
void inserirProdutoNoInicio(TipoLista *lista, reg_produto produto);
void listarProdutos(TipoLista *Fichario);
TipoApontador buscarProduto(TipoLista *lista, int codigo);
void alterarProduto(TipoLista *ListaBuscaProduto, int codigo);
void excluirProduto(TipoLista *lista, int codigo);

void inicializarListaMovim(TipoLista_movim *lista);
void inserirMovim(TipoLista_movim *lista, reg_movim movim);
void listarMovim(TipoLista_movim *lista);

int CadastrarMovmentacao();
int Consultar();
int Cadastrar();
int CadastrarMovmentacao();
void MenuMovEstoque();
void MenuProduto();
void RetornaMenu();
void MenuProdutos();
void MenuMovimentacoes();
void MenuListaProdutos();
int LerEntradaInteiro();

void LimparLinha(int linha)
{
    gotoxy(2, linha);
    printf("                                                                                "); // 80 espaços para limpar a linha
    gotoxy(1, 23);
    printf("|");
    gotoxy(80, 23);
    printf("|");
    gotoxy(2, linha); // Volta para o início da linha
}

void MostraMSG(char *MSG)
{
    LimparLinha(23);
    gotoxy(1, 22);
    printf("+------------------------------------------------------------------------------+\n");
    gotoxy(2, 23);
    printf("MSG.: %s", MSG);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = (short)x;
    coord.Y = (short)y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void tela() {
    int lin;
    system("color 1F");
    system("cls");

    for (lin = 1; lin <= 24; lin++) {
        gotoxy(01, lin);
        printf("|");
        gotoxy(80, lin);
        printf("|");
    }

    gotoxy(01, 01);
    printf("+------------------------------------------------------------------------------+\n");

    gotoxy(03, 02);
    printf("Caua Hora");
    gotoxy(38, 02);
    printf("UNICV");
    gotoxy(61, 02);
    printf("Estrutura de Dados");

    gotoxy(01, 04);
    printf("+------------------------------------------------------------------------------+\n");

    gotoxy(01, 22);
    printf("+------------------------------------------------------------------------------+\n");

    gotoxy(02, 23);
    printf("MSG.:");
    /*
    gotoxy(20, 3);
    printf("%s", mensagemSubmenu);
    */
    gotoxy(01, 24);
    printf("+------------------------------------------------------------------------------+\n");
}

void RetornaMenu()
{
    char ch;
    do
    {
        gotoxy(1, 22);
        printf("+------------------------------------------------------------------------------+\n");
        gotoxy(2, 23);
        printf("MSG.: Digite (R/r) para retornar ao menu: ");
        ch = getchar();
        ch = toupper(ch); // Converte para maiúsculo
    } while (ch != 'R');
}

//  FUNÇÃO PARA BUSCAR UM PRODUTO POR CÓDIGO
TipoApontador BuscaProdutoCodigo(TipoLista *ListaBuscaProduto, int codigo)
{
    TipoApontador aux = ListaBuscaProduto->primeiro->proximo;
    while (aux != NULL)
    {
        if (aux->conteudo.cd_produto == codigo)
        {
            return aux; // RETORNA O PONTEIRO DO PRODUTO ENCONTRADO
        }
        aux = aux->proximo;
    }
    return NULL; // PRODUTO NÃO ENCONTRADO NA LISTA
}

reg_produto LerDadosProduto(char *tipoInclusao)
{
    system("cls");
    tela(tipoInclusao);
    reg_produto NovoProduto;

    gotoxy(14, 8);
    printf("Codigo do produto........: "); // CODIGO
    NovoProduto.cd_produto = LerEntradaInteiro("Codigo do produto: ");
    gotoxy(41, 8);
    printf("%d", NovoProduto.cd_produto); // PRINTA CODIGO DO PRODUTO

    gotoxy(10, 10);
    printf("1 - Descricao do produto.....: "); //  DESCRIÇÃO
    MostraMSG(" Descricao do produto: ");
    scanf("%s", NovoProduto.ds_produto);
    LimparLinha(23);
    gotoxy(41, 10);
    printf("%s", NovoProduto.ds_produto); // PRINTA DESCRIÇÃO DO PRODUTO

    gotoxy(10, 12);
    printf("2 - Unidade de medida........: "); // UNIDADE DE MEDIDA
    MostraMSG(" Unidade de medida do produto: ");
    scanf("%s", NovoProduto.de_unid_med);
    LimparLinha(23);
    gotoxy(41, 12);
    printf("%s", NovoProduto.de_unid_med); // PRINTA UNIDADE DE MEDIDA

    gotoxy(10, 14);
    printf("3 - Data de Validade.........: ");
    MostraMSG(" Data de validade do produto: ");
    scanf("%s", NovoProduto.de_validade); // DATA DE VALIDADE
    LimparLinha(23);
    gotoxy(41, 14);
    printf("%s", NovoProduto.de_validade); // PRINTA UNIDADE DE MEDIDA

    NovoProduto.qt_produto = 0;
    NovoProduto.vl_CustoMedio = 0.0;
    NovoProduto.vl_total = 0.0;

    return NovoProduto;
}

// Inicializar lista de produtos
void inicializarListaProdutos(TipoLista *lista) {
    lista->primeiro = (TipoApontador)malloc(sizeof(TipoItem));
    lista->ultimo = lista->primeiro;
    lista->primeiro->proximo = NULL;
}

// Cadastrar Produtos
void CadastraProdutoInicio(TipoLista *ListaProdutoInicio)
{
    reg_produto ProdutoNovoInicio = LerDadosProduto("Cadastra no inicio");

    TipoApontador NovoProduto = (TipoApontador)malloc(sizeof(TipoItem));
    NovoProduto->conteudo = ProdutoNovoInicio;
    NovoProduto->proximo = ListaProdutoInicio->primeiro->proximo;
    ListaProdutoInicio->primeiro->proximo = NovoProduto;

    if (ListaProdutoInicio->ultimo == ListaProdutoInicio->primeiro)
    {
        ListaProdutoInicio->ultimo = NovoProduto;
    }
    RetornaMenu();
}

void CadastraProdutoFinal(TipoLista *ListaProdutoFinal)
{
    reg_produto ProdutoNovoFim = LerDadosProduto("Cadastra no Final");
    ListaProdutoFinal->ultimo->proximo = (TipoApontador)malloc(sizeof(TipoItem));
    ListaProdutoFinal->ultimo = ListaProdutoFinal->ultimo->proximo;
    ListaProdutoFinal->ultimo->conteudo = ProdutoNovoFim;
    ListaProdutoFinal->ultimo->proximo = NULL;
    
    RetornaMenu();
}

void CadastraProdutoPosicao(TipoLista *ListaProdutoPosicao)
{
    int posicao;
    int contador = 0;

     reg_produto ProdutoNovoPosicao = LerDadosProduto("Cadastra em uma posicao");

    posicao = LerEntradaInteiro("Digite a posicao para inserir o produto: ");
    TipoApontador NovoProduto = (TipoApontador)malloc(sizeof(TipoItem));
    NovoProduto->conteudo = ProdutoNovoPosicao;
    LimparLinha(23);

    TipoApontador Aux = ListaProdutoPosicao->primeiro;

    while (Aux != NULL && contador < posicao - 1)
    {
        Aux = Aux->proximo;
        contador++;
    }
    if (Aux == NULL)
    {
        printf("Posição inválida!\n");
        free(NovoProduto);
        return;
    }

    NovoProduto->proximo = Aux->proximo;
    Aux->proximo = NovoProduto;

    if (NovoProduto->proximo == NULL)
    {
        ListaProdutoPosicao->ultimo = NovoProduto;
    }
    
   RetornaMenu();
}

//Excluir Produto
void ExcluirProdutoInicio(TipoLista *ListaRemoveProdutoInicio)
{
    tela();
    if (ListaRemoveProdutoInicio == NULL)
    {
        MostraMSG("Produto nao existe");
        Sleep(2000);
        return;
    }

    TipoApontador aux = ListaRemoveProdutoInicio->primeiro;
    ListaRemoveProdutoInicio->primeiro = ListaRemoveProdutoInicio->primeiro->proximo;

    if (ListaRemoveProdutoInicio->primeiro == NULL)
    {
        ListaRemoveProdutoInicio->ultimo = NULL;
    }
    free(aux);
    MostraMSG("Produto removido com sucesso...");
    Sleep(2000);
}

void ExcluirProdutoFinal(TipoLista *ListaRemoveProdutoFinal)
{
    tela();
    if (ListaRemoveProdutoFinal == NULL)
    {
        MostraMSG("A lista tá vazia!");
        Sleep(2000);
        return;
    }

    TipoApontador atual = ListaRemoveProdutoFinal->primeiro;
    TipoApontador anterior = NULL;

    while (atual->proximo != NULL)
    {
        anterior = atual;
        atual = atual->proximo;
    }

    if (anterior == NULL)
    {
        ListaRemoveProdutoFinal->primeiro = NULL;
    }
    else
    {
        anterior->proximo = NULL;
    }

    ListaRemoveProdutoFinal->ultimo = anterior;
    free(atual);
    MostraMSG("Produto excluido com sucesso.\n");
    Sleep(2000);
}

void ExcluirProdutoPosicao(TipoLista *ListaRemovePosicao)
{
    int posicao;
    if (ListaRemovePosicao->primeiro == NULL)
    {
         MostraMSG("A lista esta vazia.\n");
         Sleep(2000);
        return;
    }

    posicao = LerEntradaInteiro("Digite a posicao para retirar o produto: ");

    if (posicao < 1)
    {
        MostraMSG("Posicao invalida.\n");
        Sleep(2000);
        return;
    }

    TipoApontador atual = ListaRemovePosicao->primeiro;
    TipoApontador anterior = NULL;

    for (int i = 1; atual != NULL && i < posicao; i++)
    {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL)
    {
         MostraMSG("Posicao invalida.\n");
         Sleep(2000);
        return;
    }

    if (anterior == NULL)
    {
        ListaRemovePosicao->primeiro = atual->proximo;
    }
    else
    {
        anterior->proximo = atual->proximo;
    }

    if (atual->proximo == NULL)
    {
        ListaRemovePosicao->ultimo = anterior;
    }

    free(atual);
    MostraMSG("Produto excluido com sucesso.\n");
    Sleep(2000);
}

void listarProdutos(TipoLista *Fichario) {


    TipoApontador aux = Fichario->primeiro->proximo;
    TipoApontador produtoAnterior = NULL;

    char comando;
    while (1)
    {
        system("cls");
        tela();

        if (aux != NULL)
        {
            // ExibirProduto(aux);
            gotoxy(2, 5);
            printf("COD");
            gotoxy(2, 6);
            printf("---");

            gotoxy(2, 8);
            printf("%d", aux->conteudo.cd_produto);

            gotoxy(8, 5);
            printf("Descricao do Produto");
            gotoxy(8, 6);
            printf("--------------------");
            gotoxy(8, 8);
            printf("%s", aux->conteudo.ds_produto);

            gotoxy(31, 5);
            printf("Unid");
            gotoxy(31, 6);
            printf("-----");
            gotoxy(31, 8);
            printf("%s", aux->conteudo.de_unid_med);

            gotoxy(38, 5);
            printf("Data Valid");
            gotoxy(38, 6);
            printf("----------");
            gotoxy(38, 8);
            printf("%s", aux->conteudo.de_validade);

            gotoxy(51, 5);
            printf("Qtde");
            gotoxy(51, 6);
            printf("----");
            gotoxy(51, 8);
            printf("%.2f", aux->conteudo.qt_produto);

            gotoxy(58, 5);
            printf("Vl. Unit");
            gotoxy(58, 6);
            printf("--------");
            gotoxy(58, 8);
            printf("%.2f", aux->conteudo.vl_CustoMedio);

            gotoxy(68, 5);
            printf("Vl. Total");
            gotoxy(68, 6);
            printf("---------");
            gotoxy(68, 8);
            printf("%.2f", aux->conteudo.vl_total);

            MostraMSG("Digite (p/P) - proximo, (a/A) - anterior, (r/R) - retornar: ");
            comando = _getch();

            if (comando == 'p' || comando == 'P')
            {
                if (aux->proximo != NULL)
                {
                    produtoAnterior = aux;
                    aux = aux->proximo;
                }
            }
            else if (comando == 'a' || comando == 'A')
            {
                if (produtoAnterior != NULL)
                {
                    aux = produtoAnterior;
                    produtoAnterior = NULL; // Reset para evitar ciclo
                }
            }
            else if (comando == 'r' || comando == 'R')
            {
                break;
            }
        }
        else
        {
            gotoxy(2, 21);
            printf("Nenhum produto encontrado.\n");
            MostraMSG("Pressione 'r'/'R' para retornar ao menu.");
            comando = _getch();
            if (comando == 'r' || comando == 'R')
            {
                break;
            }
        }
    }
}

// Buscar produto pelo código
TipoApontador buscarProduto(TipoLista *lista, int codigo) {
    TipoApontador atual = lista->primeiro; // Começamos do primeiro elemento da lista
    while (atual != NULL && atual->conteudo.cd_produto != codigo) {
        atual = atual->proximo;
    }
    return atual;
}

// Alterar dados do produto
void alterarProduto(TipoLista *ListaBuscaProduto, int codigo) {
    int tipoAlteracao;

    if (ListaBuscaProduto == NULL || ListaBuscaProduto->primeiro == NULL) {
        MostraMSG("Lista de produtos vazia.\n");
        return;
    }

    TipoApontador produto = BuscaProdutoCodigo(ListaBuscaProduto, codigo);

    if (produto == NULL) {
        MostraMSG("Produto com codigo nao encontrado.\n");
        return;
    }
    
    tela();
    tipoAlteracao = LerEntradaInteiro("1 para alterar campo especifico ou 2 para alterar todos os campos: ");

    if (tipoAlteracao == 1) {
        system("cls");
        tela();
         gotoxy(10, 10);
        printf("1 - Descricao do produto.....: %s\n", produto->conteudo.ds_produto); // Descrição
        gotoxy(10, 12);
        printf("2 - Unidade de medida........: %s\n", produto->conteudo.de_unid_med); // Unidade de Medida
        gotoxy(10, 14);
        printf("3 - Data de Validade.........: %s\n", produto->conteudo.de_validade); // Data de Validade
        int campo = LerEntradaInteiro("Digite o numero do campo que deseja alterar: ");

        switch (campo) {
        case 1:
            system("cls");
            tela();
            gotoxy(20, 10);
            printf("Nova Descricao: ");
            MostraMSG("Digite a descricao: ");
            scanf("%49s", produto->conteudo.ds_produto);
            gotoxy(36, 10);
            printf("%s", produto->conteudo.ds_produto);
            break;
        case 2:
            system("cls");
            tela();
            gotoxy(20, 10);
            printf("Nova Unidade de Medida: ");
            MostraMSG("Digite a unidade de medida: ");
            scanf("%19s", produto->conteudo.de_unid_med);
            gotoxy(43, 10);
            printf("%s", produto->conteudo.de_unid_med);
            break;
        case 3:
            system("cls");
            tela();
            gotoxy(20, 10);
            printf("Nova Data de Validade: ");
            MostraMSG("Digite a data de validade: ");
            scanf("%9s", produto->conteudo.de_validade);
            gotoxy(43, 10);
            printf("%s", produto->conteudo.de_validade);
            break;
        default:
            printf("Campo invalido.\n");
            break;
        }
    } else if (tipoAlteracao == 2) {
        system("cls");
        tela();

        // Exibe o produto
        gotoxy(5, 8);
        printf("+----------------------------------------------------------------------+\n");
        gotoxy(5, 9);
        printf("| COD | DESC. PROD | UN. MED | DT. VAL | VL. UNIT | VL. TOTAL |\n");
        gotoxy(5, 10);
        printf("+----------------------------------------------------------------------+\n");

        gotoxy(5, 11);
        printf("| %-3d | %-10s | %-7s | %-8s | %-8.2f | %-8.2f |\n",
            produto->conteudo.cd_produto,
            produto->conteudo.ds_produto,
            produto->conteudo.de_unid_med,
            produto->conteudo.de_validade,
            produto->conteudo.vl_CustoMedio,
            produto->conteudo.vl_total);

        gotoxy(5, 12);
        printf("+----------------------------------------------------------------------+\n");

        // Solicita ao usuario para alterar os dados
        MostraMSG("Descricao: ");
        scanf("%49s", produto->conteudo.ds_produto);

        MostraMSG("Unidade de Medida: ");
        scanf("%19s", produto->conteudo.de_unid_med);

        MostraMSG("Data de Validade: ");
         scanf("%9s", produto->conteudo.de_validade);
    } else {
        MostraMSG("Opcao invalida.\n");
    }

    RetornaMenu();
}

int LerEntradaInteiro(char *Mensagem)
{
    char entrada[100];
    int valido = 0, numero = 0;

    while (!valido)
    {
        MostraMSG(Mensagem);
        scanf("%s", entrada);

        valido = 1; // Assume que a entrada é válida até provar o contrário
        for (int i = 0; i < strlen(entrada); i++)
        {
            if (!isdigit(entrada[i]))
            {
                gotoxy(2, 21);
                printf("Entrada invalida. Por favor, digite um valor inteiro.\n");
                valido = 0;
                break;
            }
        }

        if (!valido)
        {
            MostraMSG("Entrada inválida. Por favor, digite um número inteiro.\n");
        }
        else
        {
            numero = atoi(entrada);
        }
    }

    return numero;
}


int LerEntradaFloat(char *Mensagem)
{
    char entrada[100];
    float valido = 0, numero = 0;

    while (!valido)
    {
        MostraMSG(Mensagem);
        scanf("%f", &entrada);

        valido = 1; // Assume que a entrada é válida até provar o contrário
        for (int i = 0; i < strlen(entrada); i++)
        {
            if (!isdigit(entrada[i]))
            {
                gotoxy(2, 21);
                printf("Entrada invalida. Por favor, digite um valor float.\n");
                valido = 0;
                break;
            }
        }

        if (!valido)
        {
            MostraMSG("Entrada inválida. Por favor, digite um número flaot.\n");
        }
        else
        {
            numero = atoi(entrada);
        }
    }

    return numero;
}


void IniciarMovimentacoes(TipoLista_movim *IniciaMovimentacao)
{
    IniciaMovimentacao->primeiro = NULL;
    IniciaMovimentacao->ultimo = NULL;
}

// FUNÇÃO PARA REGISTRAR UMA MOVIMENTAÇÃO
void RegistraMovimentacao(TipoLista* ListaProdutos, TipoLista_movim* ListaMovimentacoes, int codigo)
{
   system("cls");
    tela();

    gotoxy(10, 8);
    printf("Codigo do produto........: ");
    gotoxy(10, 10);
    printf("Tipo de movimentacao.....: ");
    gotoxy(10, 12);
    printf("Quantidade de produtos movimentados........: ");
    gotoxy(10, 14);
    printf("Valor unitario.........: ");
    gotoxy(10, 16);
    printf("Valor total.........: ");
    gotoxy(10, 18);
    printf("Data de validade.........: ");
    
        
        TipoApontador produto = buscarProduto(ListaProdutos, codigo);
        
        if (produto == NULL) {
            MostraMSG("Produto nao encontrado.");
            Sleep(2000);
            return;
        } else {
            gotoxy(41, 8);
            printf("%d - %s", produto->conteudo.cd_produto, produto->conteudo.ds_produto);
        }
       
    char tipoMov;
    do {
        MostraMSG("Digite o tipo de movimentacao (E/S): ");
        scanf("%c", &tipoMov);
        tipoMov = toupper(tipoMov);
        gotoxy(41, 10);
        printf("%c", tipoMov);
    } while (tipoMov != 'E' && tipoMov != 'S');

    float qt_movim = LerEntradaFloat("Digite a quantidade de movimentacao: ");
    gotoxy(55, 12);
    printf("%.2f", qt_movim);

    if (tipoMov == 'S' && produto->conteudo.qt_produto < qt_movim) {
        printf("Quantidade em estoque insuficiente para a saida. ");
        MostraMSG("Pressione qualquer tecla para continuar...");
        getch();
        return;
    }

    float vl_unit_movim = LerEntradaFloat("Digite o valor unitario: ");
    gotoxy(41, 14);
    printf("%.2f", vl_unit_movim);
    gotoxy(41, 16);
    printf("%.2f", qt_movim * vl_unit_movim);

    reg_movim novaMovimentacao;
    MostraMSG("Digite a data de validade: ");
    scanf("%10s", novaMovimentacao.dt_movim);
    gotoxy(41, 18);
    printf("%s", novaMovimentacao.dt_movim);

    novaMovimentacao.cd_prod_movim = codigo;
    novaMovimentacao.tp_movim = tipoMov;
    novaMovimentacao.qt_movim = qt_movim;
    novaMovimentacao.vl_unit_movim = vl_unit_movim;
    novaMovimentacao.vl_total_movim = qt_movim * vl_unit_movim;

    TipoApontador_movim novaMov = (TipoApontador_movim)malloc(sizeof(TipoItem_movim));
    novaMov->conte_movim = novaMovimentacao;
    novaMov->proximo = NULL;

    if (ListaMovimentacoes->ultimo != NULL) {
        ListaMovimentacoes->ultimo->proximo = novaMov;
    } else {
        ListaMovimentacoes->primeiro = novaMov;
    }
    ListaMovimentacoes->ultimo = novaMov;

    if (tipoMov == 'E') {
        produto->conteudo.qt_produto += qt_movim;
    } else if (tipoMov == 'S') {
        produto->conteudo.qt_produto -= qt_movim;
    }

    gotoxy(2, 21);
    MostraMSG("Movimentacao registrada com sucesso ");
    RetornaMenu();
}

// FUNÇÃO PARA EXIBIR AS MOVIMENTAÇÕES
void ExibirMovimentacoes(TipoLista_movim *listaMov, TipoLista *listaProd)
{
    
    if (listaMov->primeiro == NULL)
    {
        MostraMSG("Nenhuma movimentacao registrada.\n");
        RetornaMenu();
        return;
    }

    TipoApontador_movim atual = listaMov->primeiro;

    char comando;
    while (1)
    {
        system("cls");
        tela();

        if (atual != NULL)
        {
            // Buscar o produto associado à movimentação
            TipoApontador produto = buscarProduto(listaProd, atual->conte_movim.cd_prod_movim);

            if (produto != NULL)
            {
                gotoxy(2, 5);
                printf("COD");
                gotoxy(2, 6);
                printf("---");
                gotoxy(2, 8);
                printf("%d", atual->conte_movim.cd_prod_movim);

                gotoxy(8, 5);
                printf("Data");
                gotoxy(8, 6);
                printf("----");
                gotoxy(8, 8);
                printf("%s", atual->conte_movim.dt_movim);

                gotoxy(21, 5);
                printf("Tipo");
                gotoxy(21, 6);
                printf("----");
                gotoxy(21, 8);
                printf("%c", atual->conte_movim.tp_movim);

                gotoxy(28, 5);
                printf("Qtde");
                gotoxy(28, 6);
                printf("----");
                gotoxy(28, 8);
                printf("%.2f", atual->conte_movim.qt_movim);

                gotoxy(38, 5);
                printf("Vl. Unit");
                gotoxy(38, 6);
                printf("--------");
                gotoxy(38, 8);
                printf("%.2f", atual->conte_movim.vl_unit_movim);

                gotoxy(48, 5);
                printf("Vl. Total");
                gotoxy(48, 6);
                printf("---------");
                gotoxy(48, 8);
                printf("%.2f", atual->conte_movim.vl_total_movim);

                gotoxy(58, 5);
                printf("Qtde Est.");
                gotoxy(58, 6);
                printf("---------");
                gotoxy(58, 8);
                printf("%.2f", produto->conteudo.qt_produto);

                gotoxy(68, 5);
                printf("Custo Med.");
                gotoxy(68, 6);
                printf("---------");
                gotoxy(68, 8);
                printf("%.2f", produto->conteudo.vl_CustoMedio);

                gotoxy(78, 5);
                printf("Valor Total");
                gotoxy(78, 6);
                printf("-----------");
                gotoxy(78, 8);
                printf("%.2f", produto->conteudo.vl_total);
            }
            else
            {
                gotoxy(2, 8);
                printf("Produto nao encontrado.");
            }

            MostraMSG("Digite (p/P) - proximo, (a/A) - anterior, (r/R) - retornar: ");
            comando = _getch();

            if (comando == 'p' || comando == 'P')
            {
                if (atual->proximo != NULL)
                {
                    atual = atual->proximo;
                }
            }
            else if (comando == 'r' || comando == 'R')
            {
                break;
            }
        }
        else
        {
            gotoxy(2, 21);
            printf("Nenhuma movimentacao encontrada.\n");
            MostraMSG("Pressione 'r'/'R' para retornar ao menu.");
            comando = _getch();
            if (comando == 'r' || comando == 'R')
            {
                break;
            }
        }
    }

    RetornaMenu();
}


int MeuProduto(){

    TipoLista ListaSimplesProduto;
    inicializarListaProdutos(&ListaSimplesProduto);
    int opc;
    system("color 1F");
    setlocale(LC_ALL, "portuguese-brazilian");

    do {
        tela();
        gotoxy(30, 03);
        printf("MENU PRINCIPAL");
        gotoxy(20, 8);
        printf("1 - Cadastrar Cliente no final da Lista");
        gotoxy(20, 9);
        printf("2 - Cadastrar Cliente no Inicio da Lista");
        gotoxy(20, 10);
        printf("3 - Cadastrar Cliente em uma Possicao da Lista");
        gotoxy(20, 11);
        printf("4 - Remover Cliente no Final da Lista");
        gotoxy(20, 12);
        printf("5 - Remover Cliente na Posicao da Lista");
        gotoxy(20, 13);
        printf("6 - Remover Cliente no Inicio da Lista");
        gotoxy(20, 14);
        printf("7 - Consultar Todos Cliente");
        gotoxy(20, 15);
        printf("8 - Alterar dados do Cliente");
        gotoxy(20, 16);
        printf("9 - Retonar ao Menu Principal");
        gotoxy(8, 23);
        printf("Digite sua opcao.:");
        gotoxy(28, 23);
        scanf("%d", &opc);

        switch(opc) {
            case 1: 
                CadastraProdutoFinal(&ListaSimplesProduto);
                break;
            case 2:
                CadastraProdutoInicio(&ListaSimplesProduto);
                break;
            case 3:
                CadastraProdutoPosicao(&ListaSimplesProduto);
                break;
            case 4:
                ExcluirProdutoFinal(&ListaSimplesProduto);
                break;
            case 5:
                ExcluirProdutoPosicao(&ListaSimplesProduto);
                break;
            case 6:
                ExcluirProdutoInicio(&ListaSimplesProduto);
                break;
            case 7:
                listarProdutos(&ListaSimplesProduto);
                break;
            case 8:
                {
                    int codigo = LerEntradaInteiro("Digite o codigo do produto a ser alterado: ");
                    alterarProduto(&ListaSimplesProduto, codigo);
                }
                break;
            case 9:
                return 0;
                break;
            default:
                gotoxy(20, 23);
                printf("Opcao Invalida");
                break;
        }
    } while(opc != 9);

}

void MenuMovEstoque() {
    setlocale(LC_ALL, "Portuguese");
    TipoLista_movim listaMov;
    TipoLista listarProdutos;
    IniciarMovimentacoes(&listaMov);
    int opcao;
    system("color 1F");

    do {
        system("cls");
        tela();
        gotoxy(20, 8);
        printf("1 - Cadastrar Movimentacao de Estoque");
        gotoxy(20, 9);
        printf("2 - Lista Movimentacao de Estoque");
        gotoxy(20, 10);
        printf("3 - Retonar ao Menu Principal");
        gotoxy(8, 23);
        printf("Digite sua opcao.:");
        gotoxy(28, 23);
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            int codigo = LerEntradaInteiro("Digite o codigo do produto: ");
            RegistraMovimentacao( &listarProdutos, &listaMov, codigo);
            break;

        case 2:
            ExibirMovimentacoes(&listaMov, &listarProdutos);
            break;

        case 3:
            return;

        default:
            gotoxy(20, 23);
            printf("Opcao Invalida");
            Sleep(1500);
            break;
        }

    } while(opcao != 3);
}

reg_produto CadastrarProduto(){

    system("cls");
    system("color 1F");
    setlocale(LC_ALL, "portuguese-brazilian");
    // le_arquivo(&L);

    tela();
    reg_produto NovoProduto;
    
    gotoxy(14, 8);
    printf("Codigo do produto........: "); // CODIGO
    NovoProduto.cd_produto = LerEntradaInteiro("Codigo do produto: ");
    gotoxy(41, 8);
    printf("%d", NovoProduto.cd_produto); // PRINTA CODIGO DO PRODUTO

    gotoxy(10, 10);
    printf("1 - Descricao do produto.....: "); //  DESCRIÇÃO
    MostraMSG(" Descricao do produto: ");
    scanf("%s", NovoProduto.ds_produto);
    LimparLinha(23);
    gotoxy(41, 10);
    printf("%s", NovoProduto.ds_produto); // PRINTA DESCRIÇÃO DO PRODUTO

    gotoxy(10, 12);
    printf("2 - Unidade de medida........: "); // UNIDADE DE MEDIDA
    MostraMSG(" Unidade de medida do produto: ");
    scanf("%s", NovoProduto.de_unid_med);
    LimparLinha(23);
    gotoxy(41, 12);
    printf("%s", NovoProduto.de_unid_med); // PRINTA UNIDADE DE MEDIDA

    gotoxy(10, 14);
    printf("3 - Data de Validade.........: ");
    MostraMSG(" Data de validade do produto: ");
    scanf("%s", NovoProduto.de_validade); // DATA DE VALIDADE
    LimparLinha(23);
    gotoxy(41, 14);
    printf("%s", NovoProduto.de_validade); // PRINTA UNIDADE DE MEDIDA

    NovoProduto.qt_produto = 0;
    NovoProduto.vl_CustoMedio = 0.0;
    NovoProduto.vl_total = 0.0;

    return NovoProduto;
        
    

}

int Consultar(){

    TipoLista L;
    L.primeiro= NULL;
    L.ultimo= NULL;
    int opc;
    system("color 1F");
    setlocale(LC_ALL, "portuguese-brazilian");
    // le_arquivo(&L);

    do {
        tela();
        gotoxy(30, 03);
        printf("Consultar Produto");
        gotoxy(01, 04);
        printf("+------------------------------------------------------------------------------+\n");
        gotoxy(02, 05);
        printf("CD | Descricao do Produto   | Uind | Data Valid | Qtde  | VL.Unit | VL Total");
        gotoxy(01, 06);
        printf("+--- ------------------------ ------ ------------ ------- --------- -----------+\n");
        
       

        gotoxy(8, 23);
        printf("Digite sua opcao.:");
        gotoxy(28, 23);
        scanf("%d", &opc);

        switch(opc){
            case 1: {
                //Menu_produto(&L);
                break;
            }
            case 2:
                break;
            case 3:
                break;
            
           
            default:
                gotoxy(20, 23);
                printf("Opcao Invalida");
                break;
        }

        
    } while(opc = 9);

}

void main(){
    setlocale(LC_ALL, "Portuguese");
    int opc;

    do {
        tela();
        gotoxy(30, 03);
        printf("MENU PRINCIPAL");
        gotoxy(20, 8);
        printf("1 - Menu de Produtos");
        gotoxy(20, 9);
        printf("2 - Menu de Movimentacao de Estoque");
        gotoxy(20, 10);
        printf("3 - Sair");
        gotoxy(8, 23);
        printf("Digite sua opcao.:");
        gotoxy(28, 23);
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                MeuProduto();
                break;
            case 2:
                MenuMovEstoque();
                break;
            case 3:
                exit(0);
            default:
                gotoxy(20, 23);
                printf("Opcao Invalida");
                break;
        }
    } while (opc != 3);
}
