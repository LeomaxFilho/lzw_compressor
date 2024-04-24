#include <bits/stdc++.h>
using namespace std;

//!SIZE funciona apenas para a escrita
//!Caso o o buffer seja menor VAI DAR MERDA
#define SIZE 800000000
#define TAM 2097152

uint8_t *buffer = new uint8_t[SIZE];
//*pos = posicao do [] dentro do array de bytes
int pos = 0;
//*falta = numero de bits que faltam para completar um byte
int falta = 0;
//*base = onde sera armazenado os bits que estao prontos para a retirada
long int base = 0;
//*indice = posicao onde inicia o local do proximo simbolo, dentro da base
int indice = 0;
//* arquivo = arquivo usado pelo getBits em uma tentativa de agilizar o processor
FILE * arquivo;

long int getBits(int qBits, FILE *arq, int tam)
{
    //*num_saida =  numero que sera a saida o retorno da funcao
    long int num_saida = 0;
    //*indice = quantos bytes serao lidos de dentro do buffer
    int indice = ceil((float)(qBits-falta)/8);
    ////cout <<"indice: " << indice << endl;

    //*byte = onde sera armazenado temporariamente os bits que irao para a base
    uint8_t byte;

    //*agora todos os bytes do arquivo estao armazenados dentro do array de uint8_t "buffer"
    /*
    //for (int i = 0; i < tam; i++)
    //{
        //bitset<8> w(buffer[i]);
        //cout << i << ": " << w << endl;
    //}
    */


    //preenche o buffer com a quantidade de bytes minima que e necessario para
    // que ler a quantidade de bites desejado
    // mas apenas quando e necessario que haja incersao de novos bytes
    if(qBits > falta)
        for (int i = pos; i < pos+indice; i++)
        {
            byte = buffer[i];
            //bitset<8> g(byte);
            //cout << "byte: " << i << ": "<< g << endl;

            for (int j = 0; j < 8; j++){
                base = base | ((byte & ((long long)1 << j)) << falta);
            }
            falta += 8;
        }

    //coloca na no num_saida os bits que serao enviados para fora
    for (int i = 0; i < qBits; i++){
        num_saida |= (base & ((long long)1 << i));
    }
    //deleta da base os bits que foram transferidos para a saida
    for (int i = 0; i < qBits; i++)
        base &= (base & (~((long long)1 << i)));
    base = base >> qBits;
    falta -= qBits;

    //redefine a posicao que sera retirado o prox byte do "buffer"
    pos += indice;

    return num_saida;
}

void addbit(long int num, int qtdbits){
    //*qtdbits = quantidade de bits com que o int sera armazenado
    //*num = numero a ser salvo no arquivo

    //olha se o proximo indice a ser adicionado no buffer
    //esta dentro do limite de memoria
    if(pos > SIZE){
        cout << "limite no tamanho do buffer";
        //lanca o erro que ja quebra o codigo de uma vez por todas que e melhor
        throw overflow_error("buffer overflow");
    }

    falta += qtdbits;

    //*byte = a cada 8 bits sera compilado aqui
    uint8_t byte;
    //posteriormente ele sera adicionado ao array buffer

    //!nao pode ser maior que 32
    if(qtdbits+indice > 32)
        std::cout << "Deu merda";

    //coloca o int NUM, com qtdbits de bits dentro da base
    for (int i = 0; i < qtdbits; i++)
        base |= ((num & (1<<i)) << indice);
    
    //coloca em formato 0011001 
    bitset<32> t(base);
    //cout<< "base:" << t << endl;

    //colocar os primeiros 8 bits dentro de um byte
    ////cout << "falta: "<< falta << endl;
    while (falta >= 8)
    {
        //zera novamente o byte
        byte = 0;
        for (int i = 0; i < 8; i++)
        {
            byte |= ((base & (1 << i)));
        }
        for (int i = 0; i < 8; i++)
            base &= (base & (~(1 << i)));
        base = base >> 8;

        //salva o byte na posicao pos
        buffer[pos] = byte;
        pos++;

        //retira da quantidade de bits os bits salvos, 
        //restando somente os que ainda faltam ser salvos
        //e estao dentro da base ainda
        falta -= 8;
    }
    //agora o indice vai ser os bits que faltam serem impressos ainda,
    //assim adicionando bits somente depois dos que ainda nao foram
    //impressos, evitando que se apague algo
    indice = falta;
}

void salvabytes(FILE *f){
    //adiciona a quantidade de bits para completar um byte e salvar
    if(falta > 0)
        addbit(0,8);
    fwrite((char *)buffer,1,pos,f);
}