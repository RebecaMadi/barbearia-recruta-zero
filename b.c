#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include "barbearia.h"

int t=0, ta=0, tb=0, delta=0, q=-1;
Cliente buffer[BUFFERSIZE];

Relatorio r, ra, rb, rc;
Info info, infoa, infob;

/*Mutex controlador do espaço cŕitico e semáforos de controle da quantidade da clientes na fila.
O Semáforo controla a entrada e saída de clientes.*/
pthread_mutex_t mutex, maux; 
sem_t p, c;

/* Quantidade de threads usadas, tanto para o produtor quanto para o consumidor*/
//Tainha
//Caso A
pthread_t atender, a;
pthread_t atendidos;
//Caso B
pthread_t atenderB[2];
pthread_t atendidosB[2];
//Caso C

/*Indices que percorrerão o vetor*/
int cprod=0, ccons=0, ct=0, x=0, z=0, m =0;
int t_Tainha;
int q1=0, q2=0;

void main(){
    printf("Informe o tempo de soneca do Sgt Tainha (1s a 5s): ");
    scanf("%d", &t_Tainha);
    casoA();
    printf("Tempo total: %d\n", t);
    printf("x: %d l: %.2f q1: %d q2: %d\n", x, r.pct_livre, q1, q2);
    casoB();
    printf("Tempo total: %d\n", t);
    printf("x: %d l: %.2f q1: %d q2: %d\n", x, r.pct_livre, q1, q2);
}

void casoA(){
    /*Inicialização CASO A*/
    t = 0;
    ccons = 0;
    ct = 0;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&p, 0, 1);
    iniciarRelatorio(&r);
    iniciaInfo(&info);
    iniciarRelatorio(&rb);
    iniciaInfo(&infob);
    iniciarRelatorio(&ra);
    iniciaInfo(&infoa);
    m=0;
    int k=0;
    while (ccons<quantidade)
    {
        //exibe();
        //printf("t: %d\n", t);
        int s=0;
        if(ct==0){
            r.pausa++;
            s=1;
        }
        //t = ta;
        int taux = t;
        if((t%3)==0){
            tntEscovinha(t);
            if(z==1) z=-1;
        }else{
            if(s==1) q2++;
        }

        sgtTainha(t); /*Verificação do sargento*/

        if(temCliente()==0) ta++;
        /*Uma thread para quando só tem um funcionário
        Uma para o produtor e outra para o consumidor*/
        pthread_create(&atender, NULL, prodA, NULL);
        //pthread_create(&atendidos, NULL, con, NULL);

        //ta = t;
        pthread_join(atender, NULL); /*Atende*/
        //pthread_join(atendidos, NULL); /*registra*/
        t = ta;
        //if(z==1) z=-1;
        if((taux+1)<t){ //Atualiza as inserções do Tainha no tempo
            for(int i=taux+1; i<t; i++){
                if((i%3)==0){ /*Verifica se t é divisivel por 3*/
                    tntEscovinha(i);
                    if(z==1) z=-1;
                    //if(x==1000) break;
                }
                if((i%t_Tainha)==0){
                    sgtTainha(i);
                }
            }
        }
        k++;
    }
    while (ct>0)
    {
        if((t%3)==0){ //Verifica se t é divisivel por 3
            tntEscovinha(t);
        }
        //Uma thread para quando só tem um funcionário
        //Uma para o produtor e outra para o consumidor
        pthread_create(&atender, NULL, prodA, NULL);

        
        pthread_join(atender, NULL); 
        t = ta;;
        k++;
    }
    if(infoa.te_l!=0){
        tntEscovinha(t);
    }
    atualizaPct();
    pthread_mutex_destroy(&mutex);
    layoutRelatorio(1);
}

void casoB(){
    /*Inicialização CASO B*/
    t = 0;
    z=0;
    q = -1;
    ccons = 0;
    q1=0;
    ct = 0;
    m=0;
    x = 0;
    pthread_mutex_init(&mutex, NULL);
    sem_init(&p, 0, 2);
    iniciarRelatorio(&r);
    iniciaInfo(&info);
    iniciarRelatorio(&rb);
    iniciaInfo(&infob);
    iniciarRelatorio(&ra);
    iniciaInfo(&infoa);
    //m=0;
    ta = 0;
    tb = 0;
    q2=0;
    int k=0;
    while (ccons<quantidade)
    {
            if(ta==tb){
                if(ct==0) r.pausa++;
                if((t%3)==0 && t>0){ 
                    tntEscovinha(t);
                    if(z==1) z=-1;
                }
                sgtTainha(t);
                
                if(temCliente()==0){
                    ta++;
                    tb++;
                    if(ta>tb) t = ta;
                    else t = tb; 
                }else{
                    pthread_create(&atender, NULL, prodB, NULL);
                    pthread_join(atender, NULL);
                }
                
            }else if(ta>tb){
                gerenciaOperadorB(0);
            }else if(tb>ta){
                gerenciaOperadorA(0);
            }
        //printf("ta: %d tb: %d t: %d\n", ta, tb, t);
      // printf("ta: %d tb: %d\n", ta, tb);
        if(ta>tb) t = ta;
        else t = tb; 
        //if(k==20) break;
        k++;
    }
    while (ct>0)
    {
        if(ta==tb){
                if((t%3)==0 && t>0){ 
                    tntEscovinha(t);
                    if(z==1) z=-1;
                }
                if(temCliente()==0){
                    ta++;
                    tb++;
                    if(ta>tb) t = ta;
                    else t = tb; 
                }else{
                    pthread_create(&atender, NULL, prodB, NULL);
                    pthread_join(atender, NULL);
                }
            }else if(ta>tb){
                gerenciaOperadorB(1);
            }else if(tb>ta){
                gerenciaOperadorA(1);
            }
      // printf("ta: %d tb: %d\n", ta, tb);
        if(ta>tb) t = ta;
        else t = tb; 
        k++;
    }
    if(infoa.te_l!=0 || infob.te_l!=0){
        tntEscovinha(t);
    }
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&maux);
    atualizaPct();
    tempoEspera();
    layoutRelatorio(2);
}

void *prod(void * arg){
    int n;
        /* Solicita atendimento*/
        sem_wait(&p);
        /*Solicita acesso a seção crítica e bloqueia se estiver ocupada*/
        pthread_mutex_lock(&mutex);
        
        if(ct>0){
            n = busca_maior();
            cprod = n;
            //printf("cp: %d ct: %d\n", cprod, ct);
            info.te_l++;
            buffer[n].status = 1;
           //printf("ss: %d %d %d\n", buffer[n].tipo, buffer[n].tempo, buffer[n].status);

            int espera = t - buffer[n].c; //tempo atual - tempo de chegada
            //printf("e: %d\n", espera);
                if(buffer[n].tipo==1){
                    //info.te_o += espera;
                    ra.auxte_o += espera;
                }
                if(buffer[n].tipo==2){
                    //info.te_s += espera;
                    ra.auxte_s += espera;
                }
                if(buffer[n].tipo==3){
                    ra.auxte_c += espera;
                }
            t += buffer[n].tempo;
            
        }else{
            t++;
        }
        m++;
        /*Libera acesso a seção crítica*/
        pthread_mutex_unlock(&mutex);
        /* Finaliza atendimento*/
        sem_post(&c);
}

void *con(void * arg){
    int n;
        /* Solicita atendimento*/
        sem_wait(&c); 
        
        /*Solicita acesso a seção crítica e bloqueia se estiver ocupada*/
        pthread_mutex_lock(&mutex);
        
        if(ct>0){
            int aux = cprod;
            for(int i=0; i<ct; i++){
                if(buffer[i].status==1){
                    aux = i; 
                    break;
                }
            }
            if(buffer[aux].tipo==1){
                infoa.qnt_o++;
                //r.nt_of++;
                infoa.t_o += buffer[aux].tempo;
            }
            if(buffer[aux].tipo==2){
                infoa.qnt_s++;
                //r.nt_sgt++;
                infoa.t_s += buffer[aux].tempo;
            }
            if(buffer[aux].tipo==3){
                infoa.qnt_c++;
                //r.nt_cb++;
                infoa.t_c += buffer[aux].tempo;
            }
            for(int i=aux; i<ct; i++){
                if(i!=ct){
                    buffer[i] = buffer[i+1];
                }
            }
            ct--;
            ccons++;
        }
        /*Libera acesso a seção crítica*/
        pthread_mutex_unlock(&mutex);
        /* Finaliza atendimento*/
        sem_post(&p);
}

void atender_cliente(){
    sem_wait(&p);
}

void liberar_cliente(){
    sem_post(&p);
}

void *prodA(void * arg){
    int n;
        atender_cliente();
        /*Solicita acesso a seção crítica e bloqueia se estiver ocupada*/
        pthread_mutex_lock(&mutex);
        if(temCliente()==1){
            n = busca_maior();
            cprod = n;
            //printf("cp: %d ct: %d\n", cprod, ct);
            infoa.te_l++;
            buffer[n].status = 1;
           //printf("ss: %d %d %d\n", buffer[n].tipo, buffer[n].tempo, buffer[n].status);
            if(buffer[n].c > ta) ta += buffer[n].c - ta;;
            int espera = ta - buffer[n].c; //tempo atual - tempo de chegada
            if(espera<0){
                //printf("a\n");
                ta += espera*(-1);
                espera = 0;
            }
            //printf("e: %d\n", espera);
                if(buffer[n].tipo==1){
                    ra.auxte_o += espera;
                    infoa.qnt_o++;
                    infoa.t_o += buffer[n].tempo;
                }
                if(buffer[n].tipo==2){
                    ra.auxte_s += espera;
                    infoa.qnt_s++;
                    infoa.t_s += buffer[n].tempo;
                }
                if(buffer[n].tipo==3){
                    ra.auxte_c += espera;
                    infoa.qnt_c++;
                    infoa.t_c += buffer[n].tempo;
                }
            ta += buffer[n].tempo;
            for(int i=n; i<ct; i++){
                if(i!=ct){
                    buffer[i] = buffer[i+1];
                }
            }
            ct--;
            ccons++;
            //if(temCliente()==0) tb++;
        }else{
            //ta++;
            //tb++;
        }
        /*Libera acesso a seção crítica*/
        pthread_mutex_unlock(&mutex);
        /* Finaliza atendimento*/
        liberar_cliente();
}

void *prodB(void * arg){
    int n;
    //printf("a\n");
        /* Solicita atendimento*/
        atender_cliente();
        /*Solicita acesso a seção crítica e bloqueia se estiver ocupada*/
        pthread_mutex_lock(&mutex);
        
        if(temCliente()==1){
            n = busca_maior();
            cprod = n;
            //printf("cp: %d ct: %d\n", cprod, ct);
            infob.te_l++;
            buffer[n].status = 1;
           //printf("ss: %d %d %d\n", buffer[n].tipo, buffer[n].tempo, buffer[n].status);
            if(buffer[n].c > tb) tb += buffer[n].c - tb;
            int espera = tb - buffer[n].c; //tempo atual - tempo de chegada
            if(espera<0){
                //printf("b\n");
                tb += espera*(-1);
                espera = 0;
            }
            //printf("e: %d\n", espera);
                if(buffer[n].tipo==1){
                    rb.auxte_o += espera;
                    infob.qnt_o++;
                    infob.t_o += buffer[n].tempo;
                }
                if(buffer[n].tipo==2){
                    rb.auxte_s += espera;
                    infob.qnt_s++;
                    infob.t_s += buffer[n].tempo;
                }
                if(buffer[n].tipo==3){
                    rb.auxte_c += espera;
                    infob.qnt_c++;
                    infob.t_c += buffer[n].tempo;
                }
            tb += buffer[n].tempo;
            for(int i=n; i<ct; i++){
                if(i!=ct){
                    buffer[i] = buffer[i+1];
                }
            }
            ct--;
            ccons++;
            //if(temCliente()==0) ta++;
        }else{
           //tb++;
           //ta++;
        }
        /*Libera acesso a seção crítica*/
        pthread_mutex_unlock(&mutex);
        /* Finaliza atendimento*/
        liberar_cliente();
}

void gerenciaOperadorB(int v){
    if(ta>tb){
        int i = tb;
        while(i<ta){
            //printf("b: %d tb: %d\n", i, tb);
            
            //printf("i: %d ta: %d cons: %d\n", i, ta, ccons);
            if(tb<=i){
                if(temCliente()==1){
                    if(tb<=ta){
                        pthread_create(&atender, NULL, prodB, NULL);
                        pthread_join(atender, NULL);
                    }
                }else{
                    tb++;
                }
            }
            if(v==0){
                sgtTainha(i);
            }
            //if(ct==0) r.pausa++;
            if((i%3)==0){
                tntEscovinha(i);
                if(z==1) z=-1;
            }
            
            i++;
        }
    }
}

void gerenciaOperadorA(int v){
    if(tb>ta){
        int i = ta;
        while(i<tb){
            //printf("a: %d ta: %d tb: %d t: %d\n", i, ta, tb, t);
           
            //printf("i: %d tb: %d cons: %d\n", i, tb, ccons);
            if(ta<=i){
                if(temCliente()==1){
                    if(ta<=tb){
                        pthread_create(&a, NULL, prodA, NULL);
                        pthread_join(a, NULL);
                    }
                }else{
                    ta++;
                }
            }
            if(v==0){
                sgtTainha(i);
            }
            if((i%3)==0){
                tntEscovinha(i);
                if(z==1) z=-1;
            }
            
            i++;
        }
    }
    //printf("*\n");

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
    for(int j=0; j<ct; j++){
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
    /*if(t==3){
        r.cmf_o += a;
        r.cmf_s += b;
        r.cmf_c += c;
    }else if(ct>0){
        if(a!=0){
            r.cmf_o += a;
            r.cmf_o = (float) r.cmf_o/2.0;
        }
        if(b!=0){
            r.cmf_s += b;
            r.cmf_s = (float) r.cmf_s/2.0;
        }
        if(c!=0){
            r.cmf_c += c;
            r.cmf_c = (float) r.cmf_c/2.0;
        }
    }*/
}

void ocupacaoCadeiras(){
    int o, c, s, l;
    o = verificaQuantidade(1);
    s = verificaQuantidade(2);
    c = verificaQuantidade(3);
    l = BUFFERSIZE - (o + s + c);
    //printf("l: %d o: %d s: %d c: %d\n", l,  o, s, c);
    r.nt_livre += l;

        r.pct_of += (double) o/BUFFERSIZE;
        r.pct_sgt += (double) s/BUFFERSIZE;
        r.pct_cb += (double) c/BUFFERSIZE;
        double pp = (double) l/BUFFERSIZE;
        r.pct_livre += pp;//(float) l/BUFFERSIZE;
        if(pp==1.0) q1++;
       // else printf("pctl: %.4lf \n", pp);
        //else printf("pctl: %.4lf \n", r.pct_livre);
        /*if(t==3){
            r.pct_of = (float) o/BUFFERSIZE;
            r.pct_sgt = (float) s/BUFFERSIZE;
            r.pct_cb = (float) c/BUFFERSIZE;
            r.pct_livre = (float) l/BUFFERSIZE;
        }else{
            r.pct_of = (r.pct_of + ((float) o/BUFFERSIZE))/2.0;
            r.pct_sgt = (r.pct_sgt + ((float) s/BUFFERSIZE))/2.0;
            r.pct_cb = (r.pct_cb + ((float) c/BUFFERSIZE))/2.0;
            r.pct_livre = (r.pct_livre + ((float) l/BUFFERSIZE))/2.0;
        
            printf("  %.2f              %.2f            %.2f            %.2f\n", r.pct_of, r.pct_sgt, r.pct_cb, r.pct_livre);
        }*/
    
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
        //printf("z==1\n");
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
    
    //r.auxte_s += info.te_o;
    //r.auxte_c += info.te_c;
    float a, b, c;
    /*if(info.qnt_o!=0) a = (float) r.auxte_o/info.qnt_o;
    else a=-1;
    if(info.qnt_s!=0) b = (float) r.auxte_s/info.qnt_s;
    else b=-1;
    if(info.qnt_c!=0) c = (float) r.auxte_c/info.qnt_c;
    else c=-1;*/
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
    
        //printf("a: %.4f b: %.4f c: %.4f || q1: %d q2: %d q3: %d || teo: %d\n", a, b, c, info.qnt_o, info.qnt_s, info.qnt_c, info.te_s);
        //printf("o: %d s: %d c: %d\n", r.auxte_o, r.auxte_s, r.auxte_c);
        if(a!=-1) r.tme_of = (r.tme_of+a)/2.0;
        if(b!=-1) r.tme_sgt = (r.tme_sgt+b)/2.0;
        if(c!=-1) r.tme_cb = (r.tme_cb+c)/2.0;
        //printf("%.2f %.2f %.2f %d\n", r.tme_of, r.tme_sgt, r.tme_cb, x);
    }
}

void numeoAtendimentos(){
    r.auxte_o = ra.auxte_o + rb.auxte_o;
    r.auxte_s = ra.auxte_s + rb.auxte_s;
    r.auxte_c = ra.auxte_c + rb.auxte_c;
    info.t_o = infoa.t_o + infob.t_o;
    info.t_s = infoa.t_s + infob.t_s;
    info.t_c = infoa.t_c + infob.t_c;
    info.qnt_o = infoa.qnt_o + infob.qnt_o;
    info.qnt_s = infoa.qnt_s + infob.qnt_s;
    info.qnt_c = infoa.qnt_c + infob.qnt_c;
    info.te_l = infoa.te_l + infob.te_l;
    r.nt_of += infoa.qnt_o + infob.qnt_o;
    r.nt_sgt += infoa.qnt_s + infob.qnt_s;
    r.nt_cb += infoa.qnt_c + infob.qnt_c;
}

void atualizaPct(){
    printf("x: %d l: %.2f\n", x, r.pct_livre);
    r.pct_of = r.pct_of/x;
    r.pct_sgt = r.pct_sgt/x;
    r.pct_cb = r.pct_cb/x;
    r.pct_livre = (float) r.pct_livre/x;

    r.cmf_o = r.cmf_o/x;
    r.cmf_s = r.cmf_s/x;
    r.cmf_c = r.cmf_c/x;

    //printf("x: %d l: %.2f\n", x, r.pct_livre);
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
        m = t2;
    }
    //r.auxte_c =0;
    //r.auxte_o =0;
    //r.auxte_s =0;
}

void sgtTainha(int aux){
    /*Verifica se o ta no tempo do Tainha*/
    //printf("q: %d aux: %d\n", q, aux); 
        if((aux%t_Tainha)==0 && aux>q){
            q = aux;
            if(z==0) z=1;
            
            /*Tainha tenta inserir*/
            int a, b;
            a = gerar_valor(4);
            b = gerar_valor(a);
            if(ct<BUFFERSIZE){
                /*Insere*/
                buffer[ct].tipo = a;
                buffer[ct].tempo = b;
                buffer[ct].status = 0;
                buffer[ct].c = aux;
                ct++;
            }else{
                if(a==1) r.na_of++;
                if(a==2) r.na_sgt++;
                if(a==3) r.na_cb++;
            }
        }
}

void exibe(){
    for(int i=0; i<ct; i++){
        printf("%d %d %d %d\n", buffer[i].tipo, buffer[i].tempo, buffer[i].status, buffer[i].c);
    }
}

int busca_maior(){
    if(buffer[0].tipo==0){
        return 0;
    }
    for(int i=0; i<ct; i++){
        if(buffer[i].tipo==0) break;;
        if(buffer[i].tipo==1){
            return i;
        }
    }
    for(int i=0; i<ct; i++){
        if(buffer[i].tipo==0) break;;
        if(buffer[i].tipo==2){
            return i;
        }
    }
    for(int i=0; i<ct; i++){
        if(buffer[i].tipo==0) break;;
        if(buffer[i].tipo==3){
            return i;
        }
    }
    return -1;
}

int temCliente(){
    if(ct>0) return 1;
    else return 0;
}

void layoutRelatorio(int i){
    printf("-----------------------------------------------\n");
    if(i==1) printf("                    CASO A                      \n");
    if(i==2) printf("                    CASO B                      \n");
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