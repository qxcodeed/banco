#include <random>
#include "Libs/pintor.h"

#include <vector>
#include "Libs/pintor.h"
#include <algorithm>
#include <time.h>//funcao time
#include <stdlib.h> //funcao srand

using namespace std;

//Cada dia possui 6 horas de trabalho
//60 minutos x 6 horas = 360 minutos
//cada minuto eh um turno.
const int TURNOS_DIA = 360;

//A quantidade máxima de caixas não pode ser alterada.
const int MAX_CAIXAS = 10;

//Esse valor determina o quanto o banco por caixa por dia de trabalho
const int CUSTO_CAIXA_DIA = 100;

//numero de simulacoes de teste para cada caixa
const int N_EXEC_TESTE = 100;

//gera um numero entre min e max, [min, max]
int rand_mm(int min, int max){
    static int init = 1;
    if(init){
        init = 0;
        srand(time(NULL));
    }
    return rand() % (max - min + 1) + min;
}

//Essa simulação é apenas uma mostragem de como
//funciona a visualização.
void simulacao(){
    Pintor pintor;
    int min_pac = 1, max_pac = 10;
    int min_doc = 1, max_doc = 5;

    // Tempo da simulação
    const unsigned int tempo = TURNOS_DIA;
    const unsigned int numCaixas = 5;//5 caixas no banco

    // Estruturas para representar os clientes
    // no caixa e na fila
    std::vector<Cliente*> caixa(numCaixas);
    std::list<Cliente*> fila;

    int qtd = 5;
    for(int i = 0; i < qtd; i++)
        fila.push_back(new Cliente(rand_mm(min_pac, max_pac),
                                   rand_mm(min_doc, max_doc)));

    caixa[0] = new Cliente(MAX_PACIENCIA, 5);//tem cliente no primeiro caixa
    caixa[2] = new Cliente(0, 5);//tem cliente no terceiro caixa

    for (unsigned int i = 0; i < tempo; i++)
    {
        caixa[2]->documentos = i;//aumentando os documentos desse cliente
        pintor.desenhar(caixa, fila, 1000);

        //removendo um cliente da fila aleatoriamente
        if(!fila.empty()){
            Cliente * cliente = fila.front();
            fila.pop_front();
            delete cliente;
        }

        //inserindo clientes aleatoriamente
        if(rand_mm(0, 9) < 7){
            fila.push_back(new Cliente(rand_mm(15, 20), rand_mm(10, 20)));
        }
    }


    delete caixa[1];
    caixa[1] = nullptr;

    for(int i = 1; i < qtd; i++){
        delete fila.front();
        fila.pop_front();
    }

}


int main ()
{

    simulacao();
    //executar_testes();
    return 0;
}
