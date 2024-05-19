#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <map>
using namespace std;
typedef pair<string, string> decUnario; // <simboloNormal, SimboloUnario>

// Verificadores
void verifyNumberStates(int numStates)
{
    if (numStates < 1 || numStates > 20)
    {
        cout << "Numero de estados invalido" << endl;
        exit(1);
    }
}

void verifyAlphabet(string alphabet)
{
    // devera ser constituido apenas de digitos (0 a 9) e letras minusculas (a ate z)
    for (int i = 0; i < alphabet.size(); i++)
    {
        if (!islower(alphabet[i]) && !isdigit(alphabet[i]) && alphabet[i] != ' ')
        {
            cout << "Alfabeto invalido" << endl;
            exit(1);
        }
    }
}

void verifyTape(string tape){
    // Apenas $ e letras maiusculas
    for (int i = 0; i < tape.size(); i++)
    {
        if (tape[i] != '$' && !isupper(tape[i]) && tape[i] != ' ')
        {
            cout << "Fita invalida" << endl;
            exit(1);
        }
    }
}

void verifyState(int state, int allStates){
    if(state < 1 || state > allStates){
        cout << "Estado invalido" << endl;
        exit(1);
    }
}

//Entradas do usuário
int userInputs(vector<string> &vet){
    string aux;
    int numTransicoes;
    getline(cin, aux); //Numero de estados
    verifyNumberStates(stoi(aux));
    vet.push_back(aux); 

    for(int i = 0; i < 4; i++){ //Respectivamente o alfabeto, a fita, o estado inicial e o estado final
        getline(cin, aux);
        switch (i)
        {
        case 0:
            verifyAlphabet(aux);
            break;
        case 1:
            verifyTape(aux);
            break;
        case 2:
            verifyState(stoi(aux), stoi(vet[0]));
            break;
        case 3:
            for(int i = 0; i < aux.size(); i++){
                if(aux[i] != ' ')
                    verifyState(stoi(string(1, aux[i])), stoi(vet[0]));
            }
            break;
        default:
            break;
        }
        vet.push_back(aux);
    }
    cin >> numTransicoes; 
    if(numTransicoes < 1){
        cout << "Numero de transições invalido" << endl;
        exit(1);
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');    //Limpa o buffer

    aux = static_cast<char>(numTransicoes + '0'); //Converte o numero de transições para string
    vet.push_back(aux);

    for(int i = 0; i < numTransicoes; i++){ 
        getline(cin, aux);
        vet.push_back(aux);
    }
    return numTransicoes;
}


string toUnary(int num) //Converte o numero para unário
{
    string unario = "";
    for (int i = 0; i < (num); i++)
    {
        unario += "1";
    }
    return unario;
}

string findSymbol(string symbol, vector<decUnario> vet){ //Retorna o simbolo unário correspondente ao simbolo normal salvo no par
    for(int i = 0; i < vet.size(); i++){
        if(vet[i].first == symbol)
            return vet[i].second;
    }
    return "";
}

int main()
{
    // Variaveis de entrada
    vector<string> entradas, transicoesUnarias;
    int numEstados, numTransicoes;
    string alfabeto, fita, estadoInicial, estadoFinal;
    vector <decUnario> direcoes = {make_pair("D", "1"), make_pair("E", "11")};
    vector<decUnario> estados,alfabetoFita;

    // Leitura dos dados de entrada
    numTransicoes = userInputs(entradas);
    numEstados = stoi(entradas[0]);
    alfabeto = entradas[1];
    fita = entradas[2];
    estadoInicial = entradas[3];
    estadoFinal = entradas[4];

    // Verificacao dos dados de entrada
    verifyNumberStates(numEstados);
    verifyAlphabet(alfabeto);
    
    // Inicializacao dos vetores de estados e alfabeto da fita
    //Converter estados para unário
    for (int i = 0; i < numEstados; i++)
    {
        estados.push_back(make_pair(to_string(i+1), toUnary(i+1)));
    }
    
    
    //Retirar espaços
    alfabetoFita.push_back(make_pair("$", toUnary(1)));
    int contador = 2;
    for(int i = 0; i < alfabeto.size(); i++){
        if(alfabeto[i] != ' '){
            alfabetoFita.push_back(make_pair(string(1, alfabeto[i]), toUnary(contador)));
            contador++;
        }
    }

    //fita sem espaço
    string fitaSemEspaco = "";
    for(int i = 0; i < fita.size(); i++){
        if(fita[i] != ' ' && fita[i] != '$'){
            alfabetoFita.push_back(make_pair(string(1, fita[i]), toUnary(contador)));
            contador++;
        }
            
    }

    //Converter transições para unário
    for (int i = 1; i < numTransicoes+1; i++)
    {
            string transicao = entradas[5 + i]; 

            string transicaoUnaria = "";
            string estadoAtual, simboloLido, proximoEstado, simboloEscrito, direcao;
            /*
            R⟨tk⟩ = R⟨qi⟩0R⟨x⟩0R⟨qj⟩0R⟨y⟩0R⟨Dir⟩
            Estados finais são representados por:
                R⟨F⟩ = R⟨qf1⟩0R⟨qf2⟩0...0R⟨qfn⟩
            1 zero (0) para separar os elementos da transição e dos estados finais;
            2 zeros (00) consecutivos para separar as transições
            R⟨M⟩ = R⟨F⟩00R⟨t1⟩00R⟨t2⟩00..00R⟨tn⟩
            */
            estadoAtual = findSymbol(string(1, transicao[0]), estados);
            simboloLido = findSymbol(string(1, transicao[2]), alfabetoFita);
            proximoEstado = findSymbol(string(1, transicao[4]), estados);
            simboloEscrito = findSymbol(string(1, transicao[6]), alfabetoFita);
            direcao = findSymbol(string(1, transicao[8]), direcoes);
            transicaoUnaria = estadoAtual + "0" + simboloLido + "0" + proximoEstado + "0" + simboloEscrito + "0" + direcao;
            transicoesUnarias.push_back(transicaoUnaria);
    }

    // Mostrar tudo
    for (int i = 0; i < estados.size(); i++)
    {
        cout << estados[i].second << endl;
    }

    for (int i = 0; i < alfabetoFita.size(); i++)
    {
        cout << alfabetoFita[i].second << endl;
    }

    //Direções
    for (int i = 0; i < direcoes.size(); i++)
    {
        cout << direcoes[i].second << endl;
    }
    //transições
    for(int i = 0; i < transicoesUnarias.size(); i++){
        cout << transicoesUnarias[i] << endl;
    }

    //Estados finais
    for(int i = 0; i < estadoFinal.size(); i++){
        if(estadoFinal[i] != ' ')
            cout << findSymbol(string(1, estadoFinal[i]), estados) << endl;
    }

    //R{MT}
    //Primeiro os estados finais
    for(int i = 0; i < estadoFinal.size(); i++){
        if(estadoFinal[i] != ' ')
            cout << findSymbol(string(1, estadoFinal[i]), estados) << (i+1 == estadoFinal.size() ? "00" : "0");
    }
    //Agora o restante
    for (int i = 0; i < transicoesUnarias.size(); i++)
    {
        cout << transicoesUnarias[i] << (i+1 == transicoesUnarias.size() ? "" : "00");
    }

    return 0;
}