#include <bits/stdc++.h>
#include "write_bit.h"
using namespace std;

void codificador(ifstream& arq, FILE *output)
{
    //* tabela = dicionario inicial
    int tam_dic = TAM;
	unordered_map<string, int> tabela(tam_dic);

    //populando a tabela com os simbolos iniciais
	for (int i = 0; i <= 255; i++) 
    {
		string ch = "";
		ch += char(i); //char de indice i tem todos os itens da tabela ascii - populando todo o dicionario inicial
		tabela[ch] = i;
	}

    //* p = simbolo inicial
	string p = "";

    //* c = proximo simbolo
    string c = "";

    //pega primeiro caractere do arquivo
    p += (char)arq.get();

    int contador = 256;
    int indice = 256;

    //* codigo_saida = indices da tabela
	//vector<int> codigo_saida;

    //*ch = caractere atual
    char ch;
    //!-------
    int z = 1;
    int h = 1;
    double q = 0;
    double b = 0;
    double g = 0;
    double t = 0;
    //!--------
	while (!arq.fail()) {
        //*define ch como o caractere atual
        ch = arq.get();

        //define o c como o caractere seguinte na string
		if (!arq.eof())
			c += ch;

        //Procura dentro da tabela de strings o simbolo p+c, caso nao ache adiciona ele ao dicionario
		if (tabela.find(p + c) != tabela.end()) 
        {
            //caso tenha na tabela define o termo atual como (p+c) anteriores
			p = p + c;
		}
		else {
            addbit(tabela[p],(int)ceil(log2(indice+1)));

			//codigo_saida.push_back(tabela[p]);
            contador++;
            if(indice < TAM){
			    tabela[p + c] = indice;
			    indice++;
            }
			p = c;
		}
        if (t > 1000)
        {
            tabela.clear();
            for (int i = 0; i <= 255; i++) 
            {
                string ch = "";
                ch += char(i);
                tabela[ch] = i;
            }
        indice = 256;
        p = "";
        c = "";
        p += (char)arq.get();
        contador--;
        }
        
        b += p.size();
        q += (double)ceil(log2(indice+1));
        if(q/b >= g){
            t++;
        }else{
            t = 0;
        }
            g = q/b;

        //* zera o caractere c
		c = "";
	}
    addbit(tabela[p],(int)ceil(log2(indice+1)));
    // ! sempre sobra um indice que nao foi computado, que é adicionado agora
	//!std::cout << p << "- \t" << tabela[p] << endl;
	//codigo_saida.push_back(tabela[p]);
    
    int num_bits = (int)ceil(log2(indice));
    //!cout << "num_bits: " << num_bits << "\n";

    //salva dentro de um vetor a string com os binarios
    ////tamanho_comprimido = cnt * num_bits / 8;

    //adiciona um cabecalho com quantos simbolos foram comprimidos para a leitura
    fseek(output, 0, SEEK_SET);
    fprintf(output, "%d\n",contador-255);

    //salva o buffer no arquivo
    salvabytes(output);
}

void decodificador(ofstream& arq_out, string arq_nome)
{
    pos = 0;
    const char *nome_arq = arq_nome.c_str();
    arquivo = fopen(nome_arq, "rb");
    
    fseek(arquivo, 0, SEEK_END);
    //*tam = tamanho do arquivo em bytes
    int tam = ftell(arquivo);
    fseek(arquivo, 0, SEEK_SET);

    //reseta a posicao de insercao no buffer
    //!pos = 0;

    //caso haja algo no buffer zera ele
    if (buffer)
        delete []buffer;
    
    //cria um buffer exatamente do tamanho que é preciso
    buffer = new uint8_t[tam];

    //le o cabecalho para poder escrever decodificar somente o arquivo necessário
    int num_linhas = 0;
    char linhas_string[1000];
    fgets(linhas_string, 1000,arquivo);
    sscanf(linhas_string,"%d", &num_linhas);

    //le 8 bits (tam) vezes, para poder pegar todos os bytes do arquivo
    fread(buffer, 1, tam, arquivo);
    //---------------------------------------
    int tam_dic = TAM;
    unordered_map<int, string> tabela(tam_dic);
    for (int i = 0; i <= 255; i++) {
        string ch = "";
        ch += char(i);
        tabela[i] = ch;
    }
    
    int count = 256;
    int antigo = getBits(ceil(log2(count+1)), arquivo, tam);
    //!cout << ceil(log2(count+1)) << endl;
    int n;
    string s = tabela[antigo];
    string c = "";
    c += s[0];
    arq_out << s;

    //!-------
    int z = 1;
    int h = 1;
    double q = 0;
    double b = 0;
    double g = 0;
    double t = 0;
    //!--------

    for (int i = 0; i < num_linhas - 1; i++)
    {
        n = getBits(ceil(log2(count+2)), arquivo, tam);
        if (tabela.find(n) == tabela.end()) {
            s = tabela[antigo];
            s = s + c;
        }
        else {
            s = tabela[n];
        }

        arq_out << s;
        c = "";
        c += s[0];

            if(count < TAM){
        tabela[count] = tabela[antigo] + c;
        count++;
        }

        antigo = n;

        if (t > 1000)
        {
            tabela.clear();
                for (int i = 0; i <= 255; i++) {
                    string ch = "";
                    ch += char(i);
                    tabela[i] = ch;
                }
            count = 256;
            antigo = getBits(ceil(log2(count+1)), arquivo, tam);
            s = tabela[antigo];
            c = "";
            c += s[0];
            arq_out << s;
        }

        b += s.size();
        q += (double)ceil(log2(count+1));
        if(q/b >= g){
            t++;
        }else{
            t = 0;
        }
        g = q/b;
    }
}

int main(int argc, char** argv)
{
    
    if (argc < 2 || argc > 4) {
        cerr << "Estao faltando/sobrando informacoes" << endl;
        return 1;
    }
    if(string(argv[1]) == "-c"){
        ifstream arq (argv[2], ios::out | ios:: binary);
        if (arq.fail()) {
            cerr << "falha ao abrir o arquivo" << endl;
            return 1;
        }
        //executa a compressao
        
        //int tam_arq = tamanho_arq(argv[1]);
        string arquivo_saida = argc == 4 ? argv[3] : "saida_padrao.leozw";
        
        FILE * arq_out;
        arq_out = fopen(arquivo_saida.c_str(), "wb");
        if (arq_out == NULL) {
            printf("Erro abrindo o arquivo %s\n", arquivo_saida.c_str());
            return 0;
        }

        codificador(arq,arq_out);
        
    }

    if(string(argv[1]) == "-d"){

        string arq_saida = argc == 4 ? argv[3]: "saida_padrao.txt";
        ofstream arq_out(arq_saida);
        if (!arq_out) {
            cerr << "Erro ao abrir o arquivo de saida." << endl;
        }
        decodificador(arq_out, argv[2]);
        arq_out.close();
        fclose(arquivo);
    }

    return 0;
}