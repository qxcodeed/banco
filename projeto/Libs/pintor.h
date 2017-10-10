#ifndef BANCO_H
#define BANCO_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include "Libs/assetmanager.h"
#include <cstdlib>

//A paciencia maxima pode ser alterada.
//Ela é usada para criar o efeito de cores
//na visualização da simulação.
const int MAX_PACIENCIA = 20;

struct Cliente
{
    int paciencia;
    int documentos;
    std::string nome;

    Cliente(int paciencia, int documentos)
    {
        gerarNomeAleatorio();
        this->paciencia = paciencia;
        this->documentos = documentos;
    }

    void gerarNomeAleatorio(){
        std::string consoantes = "bcdfghjklmnpqrstvxyz";
        std::string vogais = "aeiou";
        this->nome = std::string(1, consoantes[std::rand() % consoantes.size()]);
        this->nome += std::string(1, vogais[std::rand() % vogais.size()]);
        this->nome += std::string(1, consoantes[std::rand() % consoantes.size()]);
    }
};

class Pintor
{
public:

    Pintor()
    {
        this->unidade = 64;
        this->largura = 1024;
        this->altura = 640;

        // Definindo chao
        chao.setTexture(&(AssetManager::GetTexture("../projeto/Libs/TexChao.jpg")));
        chao.setSize(sf::Vector2f(largura, altura));
        chao.setFillColor(sf::Color(128,128,128));

        // Definindo texto de clientes excedentes
        excedenteFila.setFont(AssetManager::GetFont("../projeto/Libs/inconsolata.otf"));
        excedenteFila.scale(0.65,0.65);
        excedenteFila.setPosition(largura-4*unidade, unidade/8);

        janela.create(sf::VideoMode(largura, altura), "Fila do Banco");
        preencherCaixa();
        preencherFila();
        criarCenario();
        criarGrade();
    }
    ~Pintor()
    {

    }

    //sleep_time: o tempo em milissegundos no qual o desenho eh mostrado.
    void desenhar(std::vector<Cliente*> &caixa, std::list<Cliente*> &fila, int sleep_time = 300)
    {

        if(!janela.isOpen())
            return;

        //processar o evento de fechar a tela
        sf::Event event;
        while (janela.pollEvent(event))
        {
            // Request for closing the window
            if (event.type == sf::Event::Closed)
                janela.close();
        }

        janela.clear(sf::Color(32,32,32));
        janela.draw(chao);

        // Desenha grade
        //    for (auto i : grade)
        //        janela.draw(i);

        for (auto i : cenario)
            janela.draw(i);

        // Desenhando pessoas na fila
        int i = 0;
        for (auto &cliente : fila)
        {
            if (i >= int(clientesFila.size()))
                break;
            desenharCliente(cliente, clientesFila[i++]);
        }

        // Verificando excedente da fila
        std::string exceStr = "\tExcedente: +";
        int qtdExced =  (clientesFila.size() < fila.size() ? int(fila.size() - clientesFila.size()) : 0);
        excedenteFila.setString(exceStr + std::to_string(qtdExced));
        janela.draw(excedenteFila);

        sf::RectangleShape placa;
        placa.setSize(sf::Vector2f(900,390));
        placa.setTexture(&AssetManager::GetTexture("../projeto/Libs/placas.jpg"));
        placa.setTextureRect(sf::IntRect(0,450,900,390));
        placa.scale(0.05,0.05);
        placa.rotate(-90);
        // Desenhando pessoas no caixa
        for (int i = 0; i < int(clientesCaixa.size()); i++)
        {

            if (i < int(caixa.size()))
            {
                if (caixa[i] != 0)
                {
                    desenharCliente(caixa[i], clientesCaixa[i]);
                }
            }
            else
            {
                placa.setPosition(unidade+4, (i+1)*unidade-9);
                janela.draw(placa);
            }
        }

        janela.display();
        sf::sleep(sf::milliseconds(sleep_time));
    }

private:
    // Janela
    int unidade;
    unsigned int altura;
    unsigned int largura;
    sf::RenderWindow janela;
    sf::Text excedenteFila;

    // Cenario
    sf::RectangleShape chao;
    std::vector<sf::CircleShape> clientesCaixa;
    std::vector<sf::CircleShape> clientesFila;
    std::vector<sf::RectangleShape> cenario;
    std::vector<sf::RectangleShape> grade;

    // Gerenciador de arquivos
    AssetManager manager;

    // Desenha cliente
    void desenharCliente(Cliente const* clienteInfo, sf::CircleShape &clienteShape)
    {
        sf::Text paciencia;
        paciencia.setFont(AssetManager::GetFont("../projeto/Libs/inconsolata.otf"));
        paciencia.setCharacterSize(24);
        paciencia.setString(clienteInfo->nome + "\n" +
                            std::to_string(clienteInfo->documentos));
        auto raio = clienteShape.getRadius();
        paciencia.setPosition(clienteShape.getPosition() - sf::Vector2f(raio, raio));

        int cor = ((255/MAX_PACIENCIA)*clienteInfo->paciencia);
        clienteShape.setFillColor(sf::Color(255-cor, 0, cor));

        janela.draw(clienteShape);
        janela.draw(paciencia);
    }

    // Metodos de inicializacao
    void preencherCaixa()
    {
        float fator = 0.3;
        for (int i = 0; i < int(altura); i+=unidade)
        {
            clientesCaixa.push_back(sf::CircleShape(unidade*fator));
            clientesCaixa.back().setOrigin(unidade*fator,unidade*fator);
            clientesCaixa.back().setPosition(unidade/2,i+unidade/2);
        }
    }
    void preencherFila()
    {
        float fator = 0.3;
        for (int i = 2*unidade; i < int(largura-unidade); i+=unidade)
        {
            if ((i/unidade)%2 == 0)
            {
                for (int j = altura-2*unidade; j > 0; j-=unidade)
                {
                    clientesFila.push_back(sf::CircleShape(unidade*fator));
                    clientesFila.back().setOrigin(unidade*fator,unidade*fator);
                    clientesFila.back().setPosition(i+unidade/2,j+unidade/2);
                }
            }
            else
            {
                for (int j = unidade; j < int(altura-unidade); j+=unidade)
                {
                    clientesFila.push_back(sf::CircleShape(unidade*fator));
                    clientesFila.back().setOrigin(unidade*fator,unidade*fator);
                    clientesFila.back().setPosition(i+unidade/2,j+unidade/2);
                }
            }
        }
    }
    void criarCenario()
    {
        // Prisicoes no caixa
        for (int i = unidade; i < int(altura); i += unidade)
        {
            cenario.push_back(novaLinha(unidade));
            cenario[int(cenario.size())-1].setPosition(0, i);
            cenario[int(cenario.size())-1].setFillColor(sf::Color::Yellow);
        }
        // Marcacao da fila
        cenario.push_back(novaLinha(largura-3*unidade));
        cenario[int(cenario.size())-1].setPosition(2*unidade, 1*unidade);
        cenario[int(cenario.size())-1].setFillColor(sf::Color::Yellow);
        cenario.push_back(novaLinha(largura-3*unidade));
        cenario[int(cenario.size())-1].setPosition(2*unidade, altura-unidade);
        cenario[int(cenario.size())-1].setFillColor(sf::Color::Yellow);
        for (int i = 2*unidade; i < int(largura); i += unidade)
        {
            cenario.push_back(novaLinha(altura-3*unidade));
            cenario[int(cenario.size())-1].rotate(90);
            cenario[int(cenario.size())-1].setFillColor(sf::Color::Yellow);
            if ((i/unidade)%2 == 0)
                cenario[int(cenario.size())-1].setPosition(i, unidade);
            else
                cenario[int(cenario.size())-1].setPosition(i, 2*unidade);
        }

        sf::RectangleShape cabecalho(sf::Vector2f(unidade, unidade));
        cabecalho.setPosition(largura-4*unidade, 0);
        cabecalho.setFillColor(sf::Color(0,0,0,128));
        cabecalho.scale(4,0.7);
        cenario.push_back(cabecalho);
    }
    void criarGrade()
    {
        for (unsigned int i = unidade; i < largura; i += unidade)
        {
            sf::RectangleShape aux = novaLinha(altura, 1);
            aux.setPosition(sf::Vector2f(float(i), 0));
            aux.rotate(90);
            aux.setFillColor(sf::Color::Black);
            grade.push_back(aux);
        }
        for (unsigned int i = unidade; i < altura; i += unidade)
        {
            sf::RectangleShape aux = novaLinha(largura, 1);
            aux.setPosition(sf::Vector2f(0, float(i)));
            aux.setFillColor(sf::Color::Black);
            grade.push_back(aux);
        }
    }
    sf::RectangleShape novaLinha(float comprimento, float largura = 2.0f)
    {
        sf::RectangleShape linha(sf::Vector2f(comprimento, largura));
        linha.setOrigin(sf::Vector2f(0.0f, largura/2.0f));
        return linha;
    }

};

#endif // BANCO_H
