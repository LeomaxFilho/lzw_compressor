#include <bits/stdc++.h>
using namespace std;

//!SIZE funciona apenas para a escrita
//!Caso o o buffer seja menor VAI DAR MERDA
#define SIZE 800000000

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
    for (int i = 0; i < tam; i++)
    {
        bitset<8> w(buffer[i]);
        //cout << i << ": " << w << endl;
    }
    */

    ////cout << "pos:" << pos << " - indice: " << indice << endl;

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
                ////bitset<8> y((byte & (1 << j)) << falta);
                ////cout << j << ": "<< y << endl;
            }
            falta += 8;
        }
    //bitset<32> z(base);
    //cout <<"base: "<< z << " - antes" <<endl;

    //coloca na no num_saida os bits que serao enviados para fora
    for (int i = 0; i < qBits; i++){
        num_saida |= (base & ((long long)1 << i));
        ////bitset<50> p(((long long)1 << i));
        ////cout << i << ": "<<p << endl;
    }
    //deleta da base os bits que foram transferidos para a saida
    for (int i = 0; i < qBits; i++)
        base &= (base & (~((long long)1 << i)));
    base = base >> qBits;
    falta -= qBits;

    ////bitset<32> h(base);
    ////cout <<"base: " <<h << endl;
    ////bitset<32> l(num_saida);
    ////cout << "num_saida: "<<l << endl;
    ////cout <<"num_saida - numerico: "<< num_saida << endl;
    ////cout << "falta: " <<falta << endl;

    //redefine a posicao que sera retirado o prox byte do "buffer"
    pos += indice;
    //cout << "pos: " << pos << endl;

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
            ////bitset<8> q(byte);
            ////cout << i+1 << ": " << q << endl;
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
    ////cout << "indice final: " << indice << endl;
    /*
    //bitset<32> g(base);
    //std::cout << "antigo: \t\t\t\t novo:" << endl;
    //std::cout <<t <<"\t"<< g<<endl;
    //cout << t << endl;
    */
}

void salvabytes(FILE *f){
    //adiciona a quantidade de bits para completar um byte e salvar
    if(falta > 0)
        addbit(0,8);
    /*
    for (int i = pos-1; i >= 0; i--){
        bitset<8> a(buffer[i]);
        cout << i <<": " << a << endl;
    }
    */
    fwrite((char *)buffer,1,pos,f);
}

//todo FILE * arq
vector<int> codificador(ifstream& arq, FILE *output)
{
    //* tabela = dicionario inicial
	unordered_map<string, int> tabela;

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

    ////p += (char)arq.get();
    ////p += s[0]; *substitui pelo arquivo

    //pega primeiro caractere do arquivo
    p += (char)arq.get();

    ////ofstream cod ("codigo.txt", ios::out);
    ////cout << "num bits inidice 0:" << ceil(indice+1))<< "\n";

    int indice = 256;

    //* codigo_saida = indices da tabela
	vector<int> codigo_saida;

    ////cod << "String\tCodigo\tAdicao\n"; 
	////std::cout << "Simbolo\tCodigo_saida\tAdicionado\n";
    //*ch = caractere atual
    char ch;
    
    ////unsigned long long cnt = 0;
	while (!arq.fail()) {
        //*define ch como o caractere atual
        ch = arq.get();
        ////cout << ch << endl;

        //define o c como o caractere seguinte na string
		if (!arq.eof())
			c += ch;

        //Procura dentro da tabela de strings o simbolo p+c, caso nao ache adiciona ele ao dicionario
		if (tabela.find(p + c) != tabela.end()) 
        {
            //todo: caso tenha na tabela define o termo atual como (p+c) anteriores
			p = p + c;
		}
		else {
			/////cod << p << "\t" << tabela[p] << "\t\t" << p + c << "\t" << indice << " ; " << endl;
            addbit(tabela[p],(int)ceil(log2(indice+1)));
            ////cout << ceil(log2(indice+1)) << endl;
			codigo_saida.push_back(tabela[p]);

            ////cnt++;
			tabela[p + c] = indice;
			indice++;
			p = c;
		}
        //* zera o indice c
		c = "";
	}
    addbit(tabela[p],(int)ceil(log2(indice+1)));
    // ! sempre sobra um indice que nao foi computado, que é adicionado agora
	//!std::cout << p << "- \t" << tabela[p] << endl;
	codigo_saida.push_back(tabela[p]);
    
    int num_bits = (int)ceil(log2(indice));
    //!cout << "num_bits: " << num_bits << "\n";

    //salva dentro de um vetor a string com os binarios
    ////tamanho_comprimido = cnt * num_bits / 8;

    //adiciona um cabecalho com quantos simbolos foram comprimidos para a leitura
    fseek(output, 0, SEEK_SET);
    fprintf(output, "%d\n",indice-255);

    //salva o buffer no arquivo
    salvabytes(output);
    return codigo_saida;
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
    ////printf("tamanho do arquivo: %d\n", tam);

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

    unordered_map<int, string> tabela;
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
    bool cond = true;
    for (int i = 0; i < num_linhas - 1; i++)
    {
        n = getBits(ceil(log2(count+2)), arquivo, tam);
        //cout << ceil(log2(count+2)) << endl;
        //cout << "val="<< n << ", byte = "<<g++ << ", qtdBits=" << ceil(log2(count+1)) << endl;
        if (tabela.find(n) == tabela.end()) {
            s = tabela[antigo];
            s = s + c;
        }
        else {
            s = tabela[n];
        }

        arq_out << s;
        //cout << s;
        c = "";
        c += s[0];
        tabela[count] = tabela[antigo] + c;
        count++;
        antigo = n;
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

        std::vector<int> codigo_saida = codificador(arq,arq_out);
        /*
        ofstream tabela ("saida.txt", ios::out);
        for (auto a : codigo_saida)
        {
            tabela << a << endl;
        }
        */
        
    }

    if(string(argv[1]) == "-d"){

        FILE *arq_in;
        string arq_saida = argc == 4 ? argv[3]: "saida_padrao.txt";
        ofstream arq_out(arq_saida);
        //FILE *ot;
        //ot = fopen(arq_saida.c_str(),"w");
        if (!arq_out) {
            cerr << "Erro ao abrir o arquivo de saida." << endl;
        }
        decodificador(arq_out, argv[2]);
        arq_out.close();
        fclose(arq_in);
    }

    return 0;
}