#ifndef BARBEARIA_H
#define BARBEARIA_H

#define quantidade 50 //Quantidade de clientes para testar
#define BUFFERSIZE 20 /*Tamanho da fila de clientes*/
//Estruturas usadas
typedef struct{
    /*Struct com os dados do cliente*/
    int tipo;
    int tempo; //exec
    int c;
    int status;
}Cliente;

typedef struct{
    double pct_of, pct_cb, pct_sgt, pct_livre;
    float tme_of, tme_cb, tme_sgt;
    float tma_of, tma_cb, tma_sgt;
    int na_of, na_cb, na_sgt;
    float cmf_o, cmf_s, cmf_c;
    int nt_of, nt_cb, nt_sgt, nt_livre;
    int pausa;
    int auxte_o, auxte_s, auxte_c;
}Relatorio;

typedef struct{
    int qnt_o, qnt_s, qnt_c, qnt_l;
    int t_o, t_s, t_c, t_l;
    int te_o, te_s, te_c, te_l;
}Info;

//variaveis
Cliente buffer[BUFFERSIZE];
Cliente *clienteA, *clienteB, *clienteC;

Relatorio r, ra, rb, rc;
Info info, infoa, infob, infoc;

/*Mutex controlador do espaço cŕitico e semáforos de controle da quantidade da clientes na fila.
O Semáforo controla a entrada e saída de clientes.*/
pthread_mutex_t mutex; 
sem_t p;

/* Quantidade de threads usadas, tanto para o produtor quanto para o consumidor*/
//Tainha
//Caso A
pthread_t recrutaZero_atende, dentinho_atende, otto_atende;
pthread_t recrutaZero_libera, dentinho_libera, otto_libera;

/*Indices que percorrerão o vetor*/
int ccons=0, ct=0, t=0, qs=0, ta, tb, tc, z=0, vagas=0;
int x=0, m=0, op;
int t_Tainha, in=0;

//Funções de produção e consumo
void *prodC(void *arg);
void *consC(void *arg);
void *consA(void *arg);
void *consB(void *arg);
void *prodB(void *arg);
void *prodA(void *arg);

//Funções intermediarias
void iniciaInfo(Info* i){
    i->qnt_c = 0;
    i->qnt_o = 0;
    i->qnt_l = 0;
    i->qnt_s = 0;
    i->t_o = 0;
    i->t_s = 0;
    i->t_c = 0;
    i->t_l = 0;
    i->te_s = 0;
    i->te_o =0;
    i->te_l =0;
    i->te_c = 0;
}

void iniciarRelatorio(Relatorio* r){
    r->pct_of = 0;
    r->pct_cb = 0;
    r->pct_sgt = 0;
    r->auxte_o = 0;
    r->auxte_s = 0;
    r->auxte_c = 0;
    r->pct_livre = 0;
    r->tme_cb = 0;
    r->tme_of = 0;
    r->pausa = 0;
    r->tme_sgt = 0;
    r->tma_of = 0;
    r->tma_cb = 0;
    r->tma_sgt = 0;
    r->na_cb = 0;
    r->na_of = 0;
    r->na_sgt = 0;
    r->cmf_o = 0;
    r->cmf_s = 0;
    r->cmf_c = 0;
    r->nt_of = 0;
    r->nt_livre = 0;
    r->nt_cb = 0;
    r->nt_sgt = 0;
}

void exibe();
int busca_maior();
int busca(int i);
int temCliente();
int temClienteO();
int temClienteS();
int temClienteC();
int verificaQuantidade(int i);
void comprimentoFila();
void ocupacaoCadeiras();
void tempoAtendimento();
void tempoEspera();
void numeoAtendimentos();
void atualizaPct();
int semaforoDisponivel(int i);
void atender_cliente();
void liberar_cliente();
void tntEscovinha();
int gerar_valor(int i);
void sgtTainha(int aux);
void gerenciaOperadorA(int v);
void gerenciaOperadorB(int v);
void layoutRelatorio(int i);

//Funções dos casos A B C
void casoA();
void casoB();
void casoC();
#endif