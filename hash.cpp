#include "hash.h"

using namespace std;

class Simbolo {
    public:
        bool isFunction;
        string escopo;
        string tipo;
        char* tipo_char;
        vector<string> tipoParametros;
        vector<string> idsParametros;
    
        Simbolo(string e, char* t) {
            escopo = e;
            tipo = t;
            tipo_char = (char*)calloc(strlen(t)+1,sizeof(char));
            strcpy(tipo_char,t);
        }

        void addParametro(string parametro){

            tipoParametros.push_back(parametro);
        }

        void addID(string id){

            idsParametros.push_back(id);
        }

        string getEscopo() { return escopo; }
        char* getTipo() { return tipo_char; }
};

boost::unordered_map<string, int> idade;

boost::unordered_map<string, Simbolo*> tabela;  

void imprimirString(char* str){

    string s;
    s = str;
    cout << s << endl;

}


int usaHash() {
    // Uso simples com valor escalar
    char tipo[] = "inteiro";
    char tipo2[] = "real";
    idade["Rodrigo"] = 20;
    cout << "Eu tenho " << idade["Rodrigo"] << " anos.\n";

    // Uso com um valor do tipo objeto Simbolo
    tabela["var1"] = new Simbolo("global", tipo);
    tabela["var2"] = new Simbolo("func1", tipo2);
    cout << "O tipo de var1 é " << tabela["var1"]->getTipo() << endl;
    cout << "O escopo de var2 é " << tabela["var2"]->getEscopo() << endl;


    if(tabela["testando"] == NULL) cout << "Testando" << endl;

    string a = "oi";
    string b = "oi";


    if(a == b) cout << "funciona" << endl;
    
    return 10;
}


int containsKey(char *key, char* scope){

    string k = key;
    string s = scope;

    if(tabela[k + "&" + s] == NULL)
        return 0;
    

    //if(tabela[k]->escopo != s)
        //return 0;

    return 1;
}

void putVariable(char *variable, char *type, char *scope){

    string key = variable;

    tabela[key + "&" + scope] = new Simbolo(scope,type);

    tabela[key + "&" + scope]->isFunction = false;
}

void putFunction(char *variable, char *type){

    string key = variable;

    key += "&&";

    tabela[key] = new Simbolo("&",type);

    tabela[key]->isFunction = true;
}

int isFunction(char* id){

    string identifier = id;

    if(tabela[identifier + "&&"]->isFunction)
        return 1;
    return 0;
}

void adicionaParametro(char* idFuncao, char* idParametro){ // Grava o tipo do parâmetro

    string id = idFuncao;

    tabela[id + "&&"]->addParametro(idParametro);
}

void adicionaId(char* identificadorFuncao, char* id){ // Grava o id do parâmetro
    string idFuncao = identificadorFuncao;

    tabela[idFuncao + "&&"]->addID(id);
}

char* getType(char* variable, char* scope){

    string v = variable;
    string s = scope;

    return tabela[v + "&" + s]->getTipo();
}
