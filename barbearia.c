/*
Nome: Rebeca Madi Oliveira
RA: 22153168
*/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include "barbearia.h"

void main(){
    printf("Informe o tempo de soneca do Sgt Tainha (1s a 5s): ");
    scanf("%d", &t_Tainha);
    casoA();
    printf("Tempo total: %d segundos\n", t);
    printf("quantiadde de vezes que o tenente apareceu: %d\n", x);
    casoB();
    printf("Tempo total: %d segundos\n", t);
    printf("quantiadde de vezes que o tenente apareceu: %d\n", x);
    casoC();
    printf("Tempo total: %d segundos\n", t);
    printf("quantiadde de vezes que o tenente apareceu: %d\n", x);
}

void casoA(){
    pthread_mutex_init(&mutex, NULL);
    sem_init(&p, 0, 1);
    iniciarRelatorio(&r);
    iniciaInfo(&info);
    iniciarRelatorio(&rb);
    iniciaInfo(&infob);
    iniciarRelatorio(&ra);
    iniciaInfo(&infoa);
    op=0;
    t=0;
    ta=0;
    tb=0;
    x=0;
    qs=0;
    m=0;
    z=0;
    in=0;
    vagas=0;
    clienteA = NULL;
    
    ccons=0;
    while (ccons<quantidade || vagas>0 || clienteA!=NULL ){
        if(vagas==0 && clienteA==NULL){
            r.pausa++;
        }
        if((t%3)==0){
            tntEscovinha(t);
            if(z==0) z=-1;
        }
        if(in<quantidade) sgtTainha(t);
        if(semaforoDisponivel(1)==1 && temCliente()==1 && clienteA==NULL){
                atender_cliente();
                pthread_create(&recrutaZero_atende, NULL, prodA, NULL);
                pthread_join(recrutaZero_atende, NULL);
        }
        if(clienteA!=NULL){
            if(t == (ta + clienteA->tempo -1)){
                pthread_create(&recrutaZero_libera, NULL, consA, NULL);
                pthread_join(recrutaZero_libera, NULL);
                liberar_cliente();
            }
        }
        t++;
    }
    tntEscovinha(t);
    pthread_mutex_destroy(&mutex);
    atualizaPct();
    layoutRelatorio(1);
}

void casoB(){
    pthread_mutex_init(&mutex, NULL);
    sem_init(&p, 0, 2);
    iniciarRelatorio(&r);
    iniciaInfo(&info);
    iniciarRelatorio(&rb);
    iniciaInfo(&infob);
    iniciarRelatorio(&ra);
    iniciaInfo(&infoa);
    op=0;
    t=0;
    ta=0;
    tb=0;
    x=0;
    z=0;
    m=0;
    in=0;
    qs=0;
    vagas=0;
    clienteA = NULL;
    clienteB = NULL;

    ccons=0;
    while (ccons<quantidade || vagas>0 || clienteA!=NULL || clienteB!=NULL){
        if(vagas==0 && clienteA==NULL && clienteB==NULL){
             r.pausa++;
        }
        
        if((t%3)==0){
            tntEscovinha(t);
            if(z==0) z=-1;
        }
        if(ccons<quantidade) sgtTainha(t);
        if(semaforoDisponivel(2)==1 && temCliente()==1 && clienteA==NULL){
                atender_cliente();
                pthread_create(&recrutaZero_atende, NULL, prodA, NULL);
                pthread_join(recrutaZero_atende, NULL);
        }
        if(semaforoDisponivel(2)==1 && temCliente()==1 && clienteB==NULL){
                atender_cliente();
                pthread_create(&dentinho_atende, NULL, prodB, NULL);
                pthread_join(dentinho_atende, NULL);
        }
        if(clienteA!=NULL){
            if(t == (ta + clienteA->tempo -1)){
                pthread_create(&recrutaZero_libera, NULL, consA, NULL);
                pthread_join(recrutaZero_libera, NULL);
                liberar_cliente();
            }
        }
        if(clienteB!=NULL){
            if(t == (tb + clienteB->tempo -1)){
                pthread_create(&dentinho_libera, NULL, consB, NULL);
                pthread_join(dentinho_libera, NULL);
                liberar_cliente();
            }
        }
        
        t++;
    }
    tntEscovinha(t);
    pthread_mutex_destroy(&mutex);
    atualizaPct();
    layoutRelatorio(2);
}

void casoC(){
    pthread_mutex_init(&mutex, NULL);
    sem_init(&p, 0, 3);
    iniciarRelatorio(&r);
    iniciaInfo(&info);
    iniciarRelatorio(&rb);
    iniciaInfo(&infob);
    iniciarRelatorio(&ra);
    iniciaInfo(&infoa);
    iniciarRelatorio(&rc);
    iniciaInfo(&infoc);
    op=1;
    t=0;
    ta=0;
    tb=0;
    x=0;
    z=0;
    m=0;
    qs=0;
    in=0;
    vagas=0;
    clienteA = NULL;
    clienteB = NULL;
    clienteC = NULL;
    ccons=0;
    while (ccons<quantidade || vagas>0 || clienteA!=NULL || clienteB!=NULL || clienteC!=NULL){
        if(vagas==0 && clienteA==NULL && clienteB==NULL && clienteC==NULL){
            r.pausa++;
        }
        
        if((t%3)==0){
            tntEscovinha(t);
            if(z==0) z=-1;
        }
        if(ccons<quantidade) sgtTainha(t);
        
        if(semaforoDisponivel(3)==1 && temClienteO()==1 && clienteA==NULL){
                atender_cliente();
                pthread_create(&recrutaZero_atende, NULL, prodA, NULL);
                pthread_join(recrutaZero_atende, NULL);
        }else if(semaforoDisponivel(3)==1 && temCliente()==1 && clienteA==NULL){
                op = 0;
                atender_cliente();
                pthread_create(&recrutaZero_atende, NULL, prodA, NULL);
                pthread_join(recrutaZero_atende, NULL);
                op = 1;
        }
        if(semaforoDisponivel(3)==1 && temClienteS()==1 && clienteB==NULL){
                atender_cliente();
                pthread_create(&dentinho_atende, NULL, prodB, NULL);
                pthread_join(dentinho_atende, NULL);
        }else if(semaforoDisponivel(3)==1 && temCliente()==1 && clienteB==NULL){
                op=0;
                atender_cliente();
                pthread_create(&dentinho_atende, NULL, prodB, NULL);
                pthread_join(dentinho_atende, NULL);
                op=1;
        }
        if(semaforoDisponivel(3)==1 && temClienteC()==1 && clienteC==NULL){
                atender_cliente();
                pthread_create(&otto_atende, NULL, prodC, NULL);
                pthread_join(otto_atende, NULL);
        }else if(semaforoDisponivel(3)==1 && temCliente()==1 && clienteC==NULL){
            op=0;
            atender_cliente();
            pthread_create(&otto_atende, NULL, prodC, NULL);
            pthread_join(otto_atende, NULL);
            op=1;
        }

        if(clienteA!=NULL){
            if(t == (ta + clienteA->tempo -1)){
                pthread_create(&recrutaZero_libera, NULL, consA, NULL);
                pthread_join(recrutaZero_libera, NULL);
                liberar_cliente();
            }
        }
        if(clienteB!=NULL){
            if(t == (tb + clienteB->tempo -1)){
                pthread_create(&dentinho_libera, NULL, consB, NULL);
                pthread_join(dentinho_libera, NULL);
                liberar_cliente();
            }
        }
        if(clienteC!=NULL){
            if(t == (tc + clienteC->tempo -1)){
                pthread_create(&otto_libera, NULL, consC, NULL);
                pthread_join(otto_libera, NULL);
                liberar_cliente();
            }
        }
        t++;
    }
    tntEscovinha(t);
    pthread_mutex_destroy(&mutex);
    atualizaPct();
    layoutRelatorio(3);
}

void* prodA(void* arg){
    pthread_mutex_lock(&mutex);
    int n;
    if(op==0) n = busca_maior();
    else n = busca(1);

    clienteA = malloc(sizeof(Cliente));
    *clienteA = buffer[n];
    ta = t;
    for(int i=n; i<vagas; i++){
        if(i!=vagas){
            buffer[i] = buffer[i+1];
        }
    }
    vagas--;
    pthread_mutex_unlock(&mutex);
}

void* consA(void* arg){
    pthread_mutex_lock(&mutex);
     int espera = ta - clienteA->c;
    if(clienteA->tipo==1){
        ra.auxte_o += espera;
        infoa.qnt_o++;
        infoa.t_o += clienteA->tempo;
    }else if(clienteA->tipo==2){
        ra.auxte_s += espera;
        infoa.qnt_s++;
        infoa.t_s += clienteA->tempo;
    }else if(clienteA->tipo==3){
        ra.auxte_c += espera;
        infoa.qnt_c++;
        infoa.t_c += clienteA->tempo;
    }
    clienteA = NULL;
    ccons++;
    pthread_mutex_unlock(&mutex);
}

void* prodB(void* arg){
    pthread_mutex_lock(&mutex);
    int n;

    if(op==0) n = busca_maior();
    else n = busca(2);

    clienteB = malloc(sizeof(Cliente));
    *clienteB= buffer[n];
    tb = t;
    for(int i=n; i<vagas; i++){
        if(i!=vagas){
            buffer[i] = buffer[i+1];
        }
    }
    vagas--;
    pthread_mutex_unlock(&mutex);
}

void* consB(void* arg){
    pthread_mutex_lock(&mutex);
     int espera = tb - clienteB->c;
    if(clienteB->tipo==1){
        rb.auxte_o += espera;
        infob.qnt_o++;
        infob.t_o += clienteB->tempo;
    }else if(clienteB->tipo==2){
        rb.auxte_s += espera;
        infob.qnt_s++;
        infob.t_s += clienteB->tempo;
    }else if(clienteB->tipo==3){
        rb.auxte_c += espera;
        infob.qnt_c++;
        infob.t_c += clienteB->tempo;
    }
    clienteB = NULL;
    ccons++;
    pthread_mutex_unlock(&mutex);
}

void* prodC(void* arg){
    pthread_mutex_lock(&mutex);
    int n;

    if(op==0) n = busca_maior();
    else n = busca(3);

    clienteC = malloc(sizeof(Cliente));
    *clienteC = buffer[n];
    tc = t;
    for(int i=n; i<vagas; i++){
        if(i!=vagas){
            buffer[i] = buffer[i+1];
        }
    }
    vagas--;
    pthread_mutex_unlock(&mutex);
}

void* consC(void* arg){
    pthread_mutex_lock(&mutex);
     int espera = tc - clienteC->c;
    if(clienteC->tipo==1){
        rc.auxte_o += espera;
        infoc.qnt_o++;
        infoc.t_o += clienteC->tempo;
    }else if(clienteC->tipo==2){
        rc.auxte_s += espera;
        infoc.qnt_s++;
        infoc.t_s += clienteC->tempo;
    }else if(clienteC->tipo==3){
        rc.auxte_c += espera;
        infoc.qnt_c++;
        infoc.t_c += clienteC->tempo;
    }
    clienteC = NULL;
    ccons++;
    pthread_mutex_unlock(&mutex);
}

int busca(int j){
    if(j==1){
        for(int i=0; i<vagas; i++){
            if(buffer[i].tipo==1) return i;
        }
        return -1;
    }else if(j==2){
        for(int i=0; i<vagas; i++){
             if(buffer[i].tipo==2) return i;
        }
        return -1;
    }else if(j==3){
        for(int i=0; i<vagas; i++){
             if(buffer[i].tipo==3) return i;
        }
        return -1;
    }
}

int busca_maior(){
    if(buffer[0].tipo==0){
        return 0;
    }
    for(int i=0; i<vagas; i++){
        if(buffer[i].tipo==0) break;;
        if(buffer[i].tipo==1 && buffer[i].status!=1){
            return i;
        }
    }
    for(int i=0; i<vagas; i++){
        if(buffer[i].tipo==0) break;;
        if(buffer[i].tipo==2 && buffer[i].status!=1){
            return i;
        }
    }
    for(int i=0; i<vagas; i++){
        if(buffer[i].tipo==0) break;;
        if(buffer[i].tipo==3 && buffer[i].status!=1){
            return i;
        }
    }
    return -1;
}

void atender_cliente(){
    qs++;
    sem_wait(&p);
}

void liberar_cliente(){
    qs--;
    sem_post(&p);
}

int temCliente(){
    if(vagas>0) return 1;
    else return 0;
}

int temClienteO(){
    if(vagas==0) return 0;
    else{
        for(int i=0; i<vagas; i++){
            if(buffer[i].tipo==1) return 1;
        }
        return 0;
    }
}

int temClienteS(){
    if(vagas==0) return 0;
    else{
        for(int i=0; i<vagas; i++){
            if(buffer[i].tipo==2) return 1;
        }
        return 0;
    }
}

int temClienteC(){
    if(vagas==0) return 0;
    else{
        for(int i=0; i<vagas; i++){
            if(buffer[i].tipo==3) return 1;
        }
        return 0;
    }
}

int semaforoDisponivel(int i){
    if(i==1){
        if(qs<1) return 1;
        else return 0;
    }else if(i==2){
        if(qs<2) return 1;
        else return 0;
    }else if(i==3){
        if(qs<3) return 1;
        else return 0;
    }
}

void sgtTainha(int aux){
        if((aux%t_Tainha)==0 && in<quantidade){
            if(z==0) z=1;
            /*Tainha tenta inserir*/
            int a, b;
            a = gerar_valor(4);
            b = gerar_valor(a);
            if(vagas<BUFFERSIZE){
                /*Insere*/
                in++;
                buffer[vagas].tipo = a;
                buffer[vagas].tempo = b;
                buffer[vagas].status = 0;
                buffer[vagas].c = aux;
                vagas++;
            }else{
                if(a==1) r.na_of++;
                if(a==2) r.na_sgt++;
                if(a==3) r.na_cb++;
            }
        }
}

void layoutRelatorio(int i){
    printf("-----------------------------------------------\n");
    if(i==1) printf("                    CASO A                      \n");
    if(i==2) printf("                    CASO B                      \n");
    if(i==3) printf("                    CASO C                      \n");
    printf("-----------------------------------------------\n");
    printf("Estado de ocupação da(s) cadeira(s) (%% por categoria e livre):\n");
    printf("Oficiais         Sargentos         Cabos          Livre\n");
    printf("%.4f%%            %.4f%%            %.4f%%         %.4f%%\n", r.pct_of*100, r.pct_sgt*100, r.pct_cb*100, r.pct_livre*100);
    printf("Comprimento médio das filas:\n");
    printf("Oficiais         Sargentos         Cabos\n");
    printf("  %.4f              %.4f            %.4f\n", r.cmf_o, r.cmf_s, r.cmf_c);
    printf("Tempo médio de atendimento por categoria:\n");
    printf("Oficiais         Sargentos         Cabos\n");
    printf("  %.4f              %.4f            %.4f\n", r.tma_of, r.tma_sgt, r.tma_cb);
    printf("Tempo médio de espera por categoria:\n");
    printf("Oficiais         Sargentos         Cabos\n");
    printf("  %.4f              %.4f            %.4f\n", r.tme_of, r.tme_sgt, r.tme_cb);
    printf("Número de atendimentos por categoria: \n");
    printf("Oficiais         Sargentos         Cabos\n");
    printf("  %d              %d            %d\n", r.nt_of, r.nt_sgt, r.nt_cb);
    printf("Número de clientes não atendidos categoria: \n");
    printf("Oficiais         Sargentos         Cabos\n");
    printf("  %d              %d            %d\n", r.na_of, r.na_sgt, r.na_cb);
    printf("Número total de clientes por categoria (oficiais, sargentos, cabos e pausa): \n");
    printf("Oficiais         Sargentos         Cabos          Pausas\n");
    printf("  %d              %d            %d            %d\n", r.nt_of + r.na_of, r.nt_sgt + r.na_sgt, r.nt_cb + r.na_cb, r.pausa);
    printf("total atendidos: %d\n", r.nt_of + r.nt_sgt + r.nt_cb);
}

void atualizaPct(){
    r.pct_of = r.pct_of/x;
    r.pct_sgt = r.pct_sgt/x;
    r.pct_cb = r.pct_cb/x;
    r.pct_livre = (float) r.pct_livre/x;

    r.cmf_o = r.cmf_o/x;
    r.cmf_s = r.cmf_s/x;
    r.cmf_c = r.cmf_c/x;
}

void tntEscovinha(int t2){
    if(t2>m){
       // if(t<500) printf("t: %d m: %d\n", t2, m);
        x++;
        numeoAtendimentos();
        comprimentoFila();
        ocupacaoCadeiras();
        tempoAtendimento();
        
        tempoEspera();
        iniciaInfo(&info);
        iniciaInfo(&infoa);
        iniciaInfo(&infob);
        iniciaInfo(&infoc);
        m = t2;
    }
}

int gerar_valor(int i){
    if(i==4){
        return 1 + (rand() % 3);
    }
    if(i==1){
        return 4 + (rand() % 3);
    }
    if(i==2){
        return 2 + (rand() % 3);
    }
    if(i==3){
        return 1 + (rand() % 3);
    }
    if(i==0){
        return 1;
    }
}

int verificaQuantidade(int i){
    /*Verifica a quantidade de clientes do tipo i na fila*/
    int cont = 0;
    for(int j=0; j<vagas; j++){
        if(buffer[j].tipo == i) cont++;
    }
    return cont;
}

void comprimentoFila(){
    int a, b, c;
    a = verificaQuantidade(1);
    b = verificaQuantidade(2);
    c = verificaQuantidade(3);
    r.cmf_o += a;
    r.cmf_s += b;
    r.cmf_c += c;
}

void ocupacaoCadeiras(){
    int o, c, s, l;
    o = verificaQuantidade(1);
    s = verificaQuantidade(2);
    c = verificaQuantidade(3);
    l = BUFFERSIZE - (o + s + c);
    r.nt_livre += l;

    r.pct_of += (double) o/BUFFERSIZE;
    r.pct_sgt += (double) s/BUFFERSIZE;
    r.pct_cb += (double) c/BUFFERSIZE;
     double pp = (double) l/BUFFERSIZE;
    r.pct_livre += pp;
    
}

void tempoAtendimento(){
    float a, b, c;
    if(info.qnt_o!=0) a = ((float) info.t_o/info.qnt_o);
    else a = -1;

    if(info.qnt_s!=0) b = ((float) info.t_s/info.qnt_s);
    else b = -1;

    if(info.qnt_c!=0) c = ((float) (info.t_c/info.qnt_c));
    else c = -1;
    
    if(z==1){
        r.tma_of = a;
        r.tma_sgt = b;
        r.tma_cb = c;
    }else{
        if(a!=-1) r.tma_of = (float) (r.tma_of + a)/2.0;
        if(b!=-1) r.tma_sgt = (float) (r.tma_sgt + b)/2.0;
        if(c!=-1) r.tma_cb = (float) (r.tma_cb + c)/2.0;
    }
}

void tempoEspera(){
    float a, b, c;
    if(r.nt_of!=0) a = (float) r.auxte_o/r.nt_of;
    else a = -1;
    if(r.nt_sgt!=0) b = (float) r.auxte_s/r.nt_sgt;
    else b=-1;
    if(r.nt_cb!=0) c = (float) r.auxte_c/r.nt_cb;
    else c=-1;

    if(z==1){
        if(a!=-1) r.tme_of = a;
        if(b!=-1) r.tme_sgt = b;
        if(c!=-1) r.tme_cb = c;
    }else{
        if(a!=-1) r.tme_of = (r.tme_of+a)/2.0;
        if(b!=-1) r.tme_sgt = (r.tme_sgt+b)/2.0;
        if(c!=-1) r.tme_cb = (r.tme_cb+c)/2.0;
    }
}

void numeoAtendimentos(){
    r.auxte_o = ra.auxte_o + rb.auxte_o + rc.auxte_o;
    r.auxte_s = ra.auxte_s + rb.auxte_s + rc.auxte_s;
    r.auxte_c = ra.auxte_c + rb.auxte_c + rc.auxte_c;
    info.t_o = infoa.t_o + infob.t_o + infoc.t_o;
    info.t_s = infoa.t_s + infob.t_s + infoc.t_s;
    info.t_c = infoa.t_c + infob.t_c + infoc.qnt_c;
    info.qnt_o = infoa.qnt_o + infob.qnt_o + infoc.qnt_o;
    info.qnt_s = infoa.qnt_s + infob.qnt_s + infoc.qnt_s;
    info.qnt_c = infoa.qnt_c + infob.qnt_c + infoc.qnt_c;
    info.te_l = infoa.te_l + infob.te_l;
    r.nt_of += infoa.qnt_o + infob.qnt_o + infoc.qnt_o;
    r.nt_sgt += infoa.qnt_s + infob.qnt_s + infoc.qnt_s;
    r.nt_cb += infoa.qnt_c + infob.qnt_c + infoc.qnt_c;
}