#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>    // SetConsoleOutputCP
#endif

using namespace std;

const int MAX_VERTICES = 100;

int op;

// ---------------[Utilidades]---------------
void limparTela(){
    #ifdef _WIN32
        system("cls");
    #else 
        system("clear");
    #endif
}


//---------------[Struct]---------------
struct Clue { //Estrutura da Pista
  int idClue;
  
  string title;
  string description;
  string question;
  string options[4];
  int correctReponse;
  
  bool resolvida = false;
  bool descoberta = false;
  bool realClue = false;
};

//---------------[Auxiliar]---------------
void next(){ //Função pra pausa

    string continuar;
    getline(cin, continuar);
}

//---------------[Classes]---------------
class TabelaHash{
  private:
    static const int MAX = 999;

    struct No{ //Estrutura de entrada
      string key;
      Clue value;
      No* next;
    };

    No* tableHash[MAX];
    struct Suspects{
      string name;
      No* relatedClues[MAX];
      bool guilty;
    };

    int funcaoHash(string key) { //Pega o valor em ASCII e cria uma chave a partir do número feito, ou seja, pega o ascii de "P1"
      unsigned int sum = 0;

      for (int i = 0; i < key.size(); i++) {
        sum = (sum * 31) + key[i];
      }

      return sum % MAX;
    }

  public:
    TabelaHash() {
      for (int i = 0; i < MAX; i++) {
        tableHash[i] = NULL;
      }
    }

    void input(string key, Clue value) {
      int position = funcaoHash(key);

      No* temp = tableHash[position];
      while(temp != NULL){
        if(temp -> key == key){
          temp -> value = value;
          return;
        }
        temp = temp -> next;
      }

      No* element = new No;
      element -> key = key;
      element -> value = value;
      element -> next = tableHash[position];
      tableHash[position] = element;
    }

    Clue search(string key) {
      int position = funcaoHash(key);
      No* temp = tableHash[position];
      while (temp != NULL) {
        if(temp -> key == key){
          return temp -> value;
        }
        temp = temp -> next;
      }
      Clue null;
      null.idClue = -1;
      return null;
    }

    Clue searchId(int id) {
      for(int i = 0; i < MAX; i++){
        No* atual = tableHash[i];

        while(atual != NULL){
          if(atual->value.idClue == 0){
            return atual->value;
          }

          atual = atual->next;
        }
      }

      Clue clueEmpty;
      clueEmpty.idClue = -1;
      clueEmpty.title = "Pensamento Obscuro";
      clueEmpty.description = "";
      clueEmpty.descoberta = false;
      clueEmpty.resolvida = false;
    
      return clueEmpty;
    }

    void remover(string key) {
      int position = funcaoHash(key);
      No* temp = tableHash[position];
      No* ant = NULL;
      while (temp != NULL) {
        if (temp -> key == key) {
          if (ant == NULL) {
            tableHash[position] = temp -> next;
          } else {
            ant->next = temp->next;
          }

          delete temp;
          return;
        }
        ant = temp;
        temp = temp->next;
      }
    }
};

class Grafo{
  public:
      Grafo(){
          numVertices = 0;                          
          for(int i = 0; i < MAX_VERTICES; i++){
              vertices[i] = NULL;
          }
      }
  private:           
    struct No{
      string idClue;
      int nivel;
      No* connection[MAX_VERTICES];
      int numConnections = 0;
    };

    No* vertices[MAX_VERTICES];
    int numVertices;
    
    void dfsRaciocinio(No* atual, bool visitados[], TabelaHash &table){ //Busca em Profundidade
      if(atual == NULL) return;

      int index = -1;//Achando index do nó e marcar como visitado.
      for(int i = 0; i < numVertices; i++){
        if(vertices[i] == atual){
          index = i;
          return;
        }
      }

      //Não encontrou ou já foi visitado.
      if(index == -1 || visitados[index]) return;

      visitados[index] = true;

      //Busca estado atual da pista TabelaHash a pista.
      Clue clueGame = table.search(atual->idClue);

      if(clueGame.descoberta){
        cout << " -> " << atual->idClue;

        if(clueGame.resolvida){
          cout << "(Concluída)";
        } else{ //Fim raciocínio.
          cout << " (Pendente de analise)";
          return;
        }

        for(int i = 0; i < atual->numConnections; i++){
          dfsRaciocinio(atual->connection[i], visitados, table);
        }
      }
    };
  public:
    No* addNo(string idClue, int nivel){ //Criação do nó(Níveis)

      No* newNo = new No;
      newNo->idClue = idClue;
      newNo->nivel = nivel;
      newNo->numConnections = 0; 

      for(int i = 0; i < MAX_VERTICES; i++){ //Colocando conexões nulas
        newNo->connection[i] = NULL;
      }

      if(numVertices < MAX_VERTICES){ //Adicionando Nó criado no vetor de nível
        vertices[numVertices] = newNo; 
        numVertices++;
      }

      return newNo; //Retornando Nó criado
    }

    void addConnections(int v1, int v2) { //Adicionando conexões
      
      if(v1 < MAX_VERTICES && v2 < MAX_VERTICES){
        No* original = vertices[v1];
        
        if(original->numConnections < MAX_VERTICES){
          original->connection[original->numConnections] = vertices[v2];
          original->numConnections++;
        }
      }
    }
    
    void showConnections(int v){ //Exibindo coneções 
        No* present = vertices[v];
        
        cout << "Ligações entre as pistas:"<< present->idClue << endl;
        
        for(int i = 0; i < present-> numConnections; i++){
            cout<< present->connection[i]->idClue << endl;
        }
    }
    
    //AVALIAR:
    //A description das pistas precisam ser exatamente a dedução que Sherlock fará 
    //(ex: "A faca foi parcialmente limpa, sugerindo tentativa de ocultar o crime.").
    void showReasoningLine(string clueKey, TabelaHash &table){
      bool visitados[MAX_VERTICES] = {false};

      for(int i = 0; i < numVertices; i++){
        if(vertices[i]->idClue == clueKey){
          cout << "\n=== PALACIO MENTAL: LINHA DE RACIOCINIO ===" << endl;
          cout << "Origem do pensamento:";
          
          dfsRaciocinio(vertices[i], visitados, table);
          
          cout << "\n===========================================" << endl;
          return;
        }
      }
      cout << "\n[Sherlock nao possui conexoes sobre essa pista ainda.]" << endl;
    }

};

void showDeduction(Grafo &graph, TabelaHash& table, string clueKey);

//---------------[Inventário]---------------
void addInventory(Clue inventory[],int &totalClues,Clue clue){
  inventory[totalClues] = clue;
  totalClues++;

  cout << "\n[Pista adicionada ao inventario]\n";
}
void showInventory(Clue inventory[],int totalClues){
  cout << "\n===== INVENTARIO DE PISTAS =====\n";

  if(totalClues == 0){
    cout << "Nenhuma pista encontrada.\n";
    return;
  }

  for(int i = 0; i < totalClues; i++){

    cout << "\n[" << inventory[i].title << "]\n";

    cout << inventory[i].description << endl;
  }

  cout << endl;
}


//---------------[Sistemas]---------------
bool checkAnswer(Clue &pista){ //Verifica Pista
  limparTela();

  int response;
  
  cout << "\n" << pista.question << endl;

  for(int i = 0; i < 4; i++){
    cout << i + 1 << " - " << pista.options[i] << endl;
  }

  cin >> response;
  cin.ignore();

  if(response == pista.correctReponse){
    cout << "\nHolmes observa a cena em silencio.\n";
    cout << "\"Interessante... muito interessante.\"\n";

    pista.resolvida = true;

    return true;
  }

  return false;
}
void showDeduction(Grafo &graph, TabelaHash &table, string clueKey){   
  cout << "\n[Holmes analisa as conexoes mentais]\n";
  graph.showReasoningLine(clueKey, table);
}
void investigateClue(TabelaHash &table, Grafo &graph, string key, Clue inventory[], int &totalClues, int &realCluesSolved){
  limparTela();
  Clue clue = table.search(key);

  if(clue.idClue == -1){
    return;
  }

  if(clue.descoberta){
    cout << "\nEssa pista ja foi investigada.\n";
    return;
  }
  clue.descoberta = true;

  cout << "\n------[" << clue.title << "]------\n";
  cout << clue.description << endl;

  addInventory(inventory, totalClues, clue);

  if(clue.correctReponse != -1){
    bool correct = checkAnswer(clue);
    if(correct && clue.realClue){
      realCluesSolved++;
      showDeduction(graph, table, key); 
    }

    cout << "\n[Holmes liga essa pista ao assassinato]\n";

    //cout << "\nProgresso da investigacao: " << realCluesSolved << "/3 pistas principais.\n";
    table.input(key, clue);
  }
}

//---------------[História]---------------
void introStory(){
  cout << "===== INVESTIGACAO POLICIAL =====" << endl;
  next();

  cout << "Londres -- 15 de Janeiro de 2010, 22:20" << endl; //Nenhuma alteração necessária
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "A chuva fina castigava as ruas de Londres, a nevoa envolvia os postes de luz, transformando o brilho amarelado em manchas fantasmagoricas no asfalto molhado." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Dentro de um táxi escuro, Sherlock Holmes observava silenciosamente o lado de fora enquanto John Watson terminava de vestir suas luvas." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "O veiculo desacelera." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "A frente, uma enorme residencia vitoriana cercada por grades negras emerge da neblina." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"Luzes policiais iluminam a fachada.\nDuas viaturas.\nUma ambulancia."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"Assim que descem do carro, um policial os aguarda proximo a entrada principal."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"Ele estende a mão."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Policial ==="<<endl;
  cout<<"-- Boa noite, Detetive Holmes e Doutor Watson, sou o inspetor William Carter."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Inspetor Carter ==="<<endl;
  cout<<"-- O patriarca da familia foi brutalmente assassinado ha cerca de uma hora."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Inspetor Carter ==="<<endl;
  cout<<"-- Nao encontramos sinais de arrombamento. Nenhuma testemunha. Nenhuma pista concreta."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Inspetor Carter ==="<<endl;
  cout<<"-- Mas quem fez isso... sabia exatamente o que estava fazendo."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Inspetor Carter ==="<<endl;
  cout<<"-- Os familiares estao reunidos na sala de estar. Ninguem saiu da casa desde nossa chegada. O corpo esta na cozinha."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  limparTela();

}
void crimeScene(){
  cout << endl << "=== CENA DO CRIME -- COZINHA ===" << endl;

  cout << "Ao entrarem, o cheiro metalico do sangue domina imediatamente o ambiente." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "A cozinha luxuosa parece congelada no tempo." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "No centro da cozinha..." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "o corpo." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Um homem de meia idade, elegante, caido ao lado da ilha central." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "A camisa social branca esta completamente tingida de vermelho." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Uma faca de cozinha permanece cravada em seu peito." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Ao redor do corpo:" << endl;
  cout << "sangue." << endl;
  cout << "Pegadas borradas." << endl;
  cout << "Marcas de luta." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Watson se aproxima lentamente." << endl;
  
  cout<<"=== Watson ==="<<endl;
  cout << "-- Meu Deus..." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Sherlock permanece em silencio. Observando. Calculando." << endl;
  cout << "Para prosseguir apere Enter" << endl;
  next();
  
  cout<<"=== Sherlock ==="<<endl;
  cout << "-- Vamos conversar um pouco com nossos suspeitos..." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  limparTela();
  
}

//---------------[Interrogatórios]---------------
// ===============[Interrogatório Eleanor]==============
void eleanorInterrogation(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  Clue suspeita_traicao = table.search("suspeita_traicao");
  Clue lenco_ensanguentado = table.search("lenco_ensanguentado");
  Clue frasco_antidepressivo = table.search("frasco_antidepressivo");
  Clue sintomas_eleanor = table.search("sintomas_eleanor");

  int op = -1;
  do{
    // Adicionar as análise de comportamento e características físicas
    cout << endl << "=== A ESPOSA -- ELEANOR WHITMORE ===" << endl;
    cout <<"=== Características ==="<<endl;
    cout << "Elegante." << endl;
    cout << "Olhos cansados." << endl;
    cout << "Maos tremulas.\n" << endl;
    
    cout<< "Ela apresenta uma agitacao extrema nos pes; Espasmos musculares e uma forte sonolência incapacitante. Isso pode indicar uso de remédios antidepressivos." <<endl;
    investigateClue(table, graph, "sintomas_eleanor", inventory, totalClues, realCluesSolved); 

    
    cout << "\nPara prosseguir aperte Enter..." << endl;
    next();

    // Perguntas
    cout << "Perguntas:" << endl;
    cout<<" 1. Como era sua relacao com seu marido?" << endl; 
    cout <<" 2. Onde estava durante o assassinato?" << endl;
    if(suspeita_traicao.descoberta){
      cout <<" 3. A senhora suspeitava de traicao?" << endl;
    }
    if(lenco_ensanguentado.descoberta){
      cout <<" 4. Por que seu lenço estava ensanguentado e com marcas de ferrugem pesada na cozinha?" << endl;
    }
    if(frasco_antidepressivo.descoberta && sintomas_eleanor.descoberta){
      cout <<" 5. Encontramos o frasco de manipulação do seu filho Edward no lixo da cozinha. Estava vazio, alguém andou consumindo ele, e não era o seu filho. \nVocê sabe de algo sobre o frasco?" << endl;
    }
    cout<<" 0. Se retirar"<<endl;
    cin>>op;
    cin.ignore();

    switch(op){
      case 1:
        limparTela();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Distante... ultimamente muito distante." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Ele nao ficava em casa." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Vivia voltando tarde do trabalho." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "Ela evita olhar diretamente para Holmes, projetando uma postura defensiva que esconde um ressentimento profundo." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();

        investigateClue(table, graph, "suspeita_traicao", inventory, totalClues, realCluesSolved);
      break;
    
      case 2:
        limparTela();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- No meu quarto." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Estou enfrentando insonia recentemente." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Tenho tomado remedios para dormir." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        investigateClue(table, graph, "alibi_eleanor", inventory, totalClues, realCluesSolved); 
      break;
      
      case 3:
        limparTela();
        cout << "Ela demora alguns segundos para responder." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Um pouco." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();

        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Uma intuição que me corroía há meses, detetive." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();

        cout << "Ela projeta um olhar arrependido." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
      break;

      case 4:
        // Adicionar conversa
      break;

      case 5:
        // Adicionar conversa
      break;
      
      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;
      break;

      default:
        cout<<"Opcao invalida"<<endl;
        cout<<"Tente novamente"<<endl;
      break;
    }
  }while(op!=0);
}

// ===============[Interrogatório Arthur]==============
void arthurInterrogation(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  Clue discussao_arthur = table.search("discussao_arthur");
  Clue angulo_facada = table.search("angulo_facada");
  Clue cadeira_caida = table.search("cadeira_caida");
  Clue testamento_sucessao = table.search("testamento_sucessao");

  // Criar uma pista para o Alibi dele
  
  int op = -1;
  int continuacao = 0;
  do{
    // Adicionar as análise de comportamento e características físicas
    cout << endl << "=== FILHO MAIS VELHO -- ARTHUR WHITMORE ===" << endl;
    cout<<"=== Características ==="<<endl;
    cout << "Controlado." << endl;
    cout << "Ambicioso." << endl;
    cout << "Frio.\n" << endl;

    cout << "Perguntas:" << endl;
    cout<<" 1. Como era sua relacao com seu pai?" << endl; 
    cout <<" 2. Onde estava durante o assassinato?" << endl;
    if(discussao_arthur.descoberta){
      cout<<" 3. Voces discutiram hoje?" << endl;
      
      if(continuacao == 0){continuacao = 1;}
    }
    if(continuacao == 1){
      cout<<" 4. Voce queria assumir a empresa??" << endl;
    }
    if(angulo_facada.descoberta && cadeira_caida.descoberta){
      // Adicionar pergunta sobre a cena do crime, como o ângulo da facada e a cadeira caída
      cout<<" 5. " << endl;
    }
    if(testamento_sucessao.descoberta){
      cout<<" 6. Seu pai pretendia mudar o testamento?" << endl;
    }
    cout<<" 0. Se retirar"<<endl;
    cin>>op;
    cin.ignore();

    switch(op){
      case 1:
        // Adicionar conversa sobre a relação com o pai
         limparTela();
      break;

      case 2:
        // Adicionar conversa sobre onde estava
      break;

      case 3:
        limparTela();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Sim." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Mas nao foi nada fora do normal." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "Arthur fecha os punhos." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Se acha que eu mataria meu proprio pai..." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- entao esta completamente maluco." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
      break;
      
      case 4:
        limparTela();
        cout << "Ele responde rapidamente." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Alguem precisava assumir eventualmente!" << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Meu pai nao poderia continuar para sempre." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
      break;

      case 5:
        // Adicionar pergunta sobre a cena do crime, como o ângulo da facada e a cadeira caída
      break;
      
      case 6:
        limparTela();
        cout << "Arthur hesita por um breve instante." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Nao." << endl; 
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "-- Nao que eu saiba." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "Arthur encara Eleanor por alguns segundos." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ARTHUR WHITMORE ==="<<endl;
        cout << "Ela desvia o olhar." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
      break;
      
      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;
      break;

      default:
        cout<<"Opcao invalida"<<endl;
        cout<<"Tente novamente"<<endl;
      break;
    }
  }while(op!=0);
}

// ===============[Interrogatório Edward]==============
void edwardInterrogation(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  Clue cigarros_turcos = table.search("cigarros_turcos");
  Clue dedos_amarelados = table.search("dedos_amarelados");
  Clue cinzeiro_jardim = table.search("cinzeiro_jardim");
  Clue alibi_edward = table.search("alibi_edward");
  Clue portao_ferro = table.search("portao_ferro");
  Clue frasco_antidepressivo = table.search("frasco_antidepressivo");
  Clue cachimbo_alfredo = table.search("cachimbo_alfredo");
  
  int op = -1;
  
  do{
    cout << endl << "=== FILHO DO MEIO -- EDWARD WHITMORE ===" << endl;
    // Adicionar as análise de comportamento e características físicas
    cout<<"=== Características ==="<<endl;
    cout << "Nervoso." << endl;
    cout << "Ansioso." << endl;
    cout << "Instavel.\n" << endl;

    cout << "Perguntas:" << endl;
    cout<<" 1. Como era sua relacao com seu pai?" << endl; 
    cout <<" 2. Onde estava durante o assassinato?" << endl;
    if(cachimbo_alfredo.descoberta && cigarros_turcos.descoberta){
      cout<<" 4. Voce possui marcas em seus dedos de fumo compulsivo. explique-se." << endl;
    }
    // Adicionar  os outros casos de pistas
    cout<<" 3. Voce ouviu alguma coisa suspeita?" << endl;
    cout<<" 0. Se retirar"<<endl;
    cin>>op;
    cin.ignore();

    switch(op){
      case 1:
        limparTela();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Ele nunca me respeitou." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Sempre comparava tudo o que eu fazia com Arthur." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "Os olhos dele demonstram ressentimento." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;
      
      case 2:
        limparTela();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- No jardim." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Pensando." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Eu ouvi a discussao..." << endl; 
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Nao queria participar daquilo." << endl; 
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "Holmes percebe um cheiro de cigarro em sua roupa." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 3:
        limparTela();
        // Adicionar conversa sobre o que ele ouviu
        break;

      case 4:
        limparTela();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Passos." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Alguem andando rapido pelo corredor." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Mas nao consegui ver quem era." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;
        break;
      
      default:
        cout<<"Opcao invalida"<<endl;
        cout<<"Tente novamente"<<endl;
        break;
    }
  }while(op!=0);
}

// ===============[Interrogatório Violet]==============
void violetInterrogation(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  Clue tirano_domestico = table.search("tirano_domestico");
  Clue perdeu_medo = table.search("perdeu_medo");
  Clue alibi_violet = table.search("alibi_violet");
  Clue faca_crime = table.search("faca_crime");
  Clue sangue_pia = table.search("sangue_pia");
  Clue vinho_unhas = table.search("vinho_unhas");

  cout << endl << "=== FILHA CACULA -- VIOLET WHITMORE ===" << endl;

  // Adicionar as análise de comportamento e características físicas
  cout<<"=== Características ==="<<endl;
  cout << "Calma demais.\n" << endl;
  
  
  int op = -1;
  do{
    cout << "Perguntas:" << endl;
    cout<<" 1. Como era sua relacao com seu pai?" << endl; 
    cout <<" 2. Onde estava durante o assassinato?" << endl;
    
    if(perdeu_medo.descoberta){
      cout<<" 3. Esta triste?" << endl;
      cout<<" 4. O que acha que aconteceu?" << endl;
    }
    if(alibi_violet.descoberta){
      cout<<" 5. Voce escutou algo?" << endl;
    }
    if(tirano_domestico.descoberta){
      cout<<" 6. Voce disse que ele so se importava com voce?" << endl;
    }
    if(faca_crime.descoberta && sangue_pia.descoberta && vinho_unhas.descoberta){
      cout<<" 7. Vejo resquícios de vinho tinto sob suas unhas e na barra de seu vestido. Além disso, a pia onde o assassino limpou o sangue exala um perfume de lavanda idêntico ao que sinto vindo de você agora. Como explica essa proximidade com a cena, senhorita Violet?" << endl;
    }
    cout<<" 0. Se retirar"<<endl;
    cin>>op;
    cin.ignore();

    switch(op){
      case 1:
        limparTela();
        cout << "Ela sorri discretamente." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Ah, detetive... eu era a caçula, a protegida dele." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Ele me dava tudo o que eu pedia e tentava me manter longe das disputas corporativas que destruíam meus irmãos." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Nossa relacao era maravilhosa. Acredito que era a unica que ele realmente se importava." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();

        investigateClue(table, graph, "perdeu_medo", inventory, totalClues, realCluesSolved);
      break;

      case 2:
        limparTela();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- No meu quarto de costura no fim do corredor." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Eu estava terminando o bordado de um vestido novo." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        investigateClue(table, graph, "alibi_violet", inventory, totalClues, realCluesSolved);
      break;
      
      case 3:
        limparTela();
        cout << "Ela encara Holmes em silencio." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- E como nao estaria?" << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "Sherlock observa atentamente." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "Ela fala sobre tristeza." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "Mas nao demonstra nenhuma." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
      break;

      case 4:
        limparTela();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Acho que alguem finalmente perdeu o medo dele." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout << "A sala inteira permanece em silencio por alguns segundos." << endl; //"silêncio" não precisa de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        investigateClue(table, graph, "tirano_domestico", inventory, totalClues, realCluesSolved);
      break;

      case 5:
        limparTela();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Eu estava com os meus fones de ouvido e não escutei absolutamente nada até os gritos dos policiais." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        investigateClue(table, graph, "fones_ouvido", inventory, totalClues, realCluesSolved);
      break;
      
      case 6:
        limparTela();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Meu pai era um tirano doméstico, detetive Holmes." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Ele esmagava a individualidade de todos nesta casa." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Arthur virou um ganancioso previsivel. \nEdward um feixe de nervos ansioso \nE minha mae uma dependente de remedios." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- E uma dor imensa que sinto, mas ele nao era uma pessoa boa, acho que isso foi apenas o karma dele." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Alguem finalmente perdeu o medo dele." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        investigateClue(table, graph, "karma_violet", inventory, totalClues, realCluesSolved);
      break;

      case 7:
        // Adicionar conversa sobre a proximidade dela com a cena do crime, como o vinho tinto sob as unhas e o perfume de lavanda
      break;

      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;
      break;
      
      default:
        cout<<"Opcao invalida"<<endl;
        cout<<"Tente novamente"<<endl;
      break;
    }
  }while(op!=0);
}

// ===============[Interrogatório Alfred]==============
void alfredInterrogation(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  Clue cigarros_turcos = table.search("cigarros_turcos");

  int op = -1;
  int continuacao = 0;
  do{
    cout << endl << "=== MORDOMO -- ALFRED ===" << endl;
    cout<<"=== Características ==="<<endl;
    cout << "Velho." << endl;
    cout << "Exausto." << endl;
    cout << "Amargurado.\n" << endl;

    cout << "Perguntas:" << endl;
    cout<<" 1. Como era sua relacao com seu pai?" << endl; 
    cout<<" 2. Onde estava durante o assassinato?" << endl;
    if(continuacao == 0){
      cout<<" 3. O senhor ouviu algo?" << endl;
    }
    if(continuacao == 2){
      cout<<" 4. Reconheceu os passos?" << endl;
    }
    if(cigarros_turcos.descoberta){
      cout<<" 5. Encontramos um maço de cigarros turcos, que estava na cozinha é sua?" << endl;
    }
    cout<<" 0. Se retirar"<<endl;
    cin>>op;

    switch(op){
      case 1:
        limparTela();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Ele me tratava como mobilia." << endl; //"mobília" não precisa de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Eu servia a esta familia ha mais de vinte anos." << endl; //"família" e "há" não precisam de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Mas para o Sr. Raymond, homens da minha classe são como mobília. Invisíveis." << endl; //"família" e "há" não precisam de ajuste aqui, mas vou ajustar para consistência
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        investigateClue(table, graph, "mordomo_invisivel", inventory, totalClues, realCluesSolved);
      break;

      case 2:
        limparTela();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Eu estava nos aposentos dos fundos da criadagem, organizando a prataria do jantar de amanhã. " << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Não saí de lá até ouvir a correria dos policiais." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        if(continuacao == 0){
          continuacao = 1;
        }
      break;

      case 3:
        limparTela();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Passos rapidos no corredor." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        continuacao = 2;
      break;

      case 4:
        limparTela();
        cout<<"=== ALFRED ==="<<endl;
        cout << "Ele hesita." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Leves demais para serem de um homem." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        investigateClue(table, graph, "passos_leves", inventory, totalClues, realCluesSolved);
      break;

      case 5:
        limparTela();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Não, senhor!" << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Eu fumo meu cachimbo velho no jardim, senhores. Não tenho dinheiro para os luxos que os jovens compram na cidade." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Talvez, o jovem Edward que compra esses maços importados da cidade." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        investigateClue(table, graph, "cachimbo_alfred", inventory, totalClues, realCluesSolved);
      break;

      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;
        break;
      
      default:
        cout<<"Opcao invalida"<<endl;
        cout<<"Tente novamente"<<endl; //Não precisa de ajuste
        break;
    }
  }while(op!=0);
}

void interrogationScene(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  cout << endl << "=== INTERROGATORIO ===" << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  
  int op = -1;
  while(op != 0){
    //limparTela();
    cout<<"Suspeitos: "<<endl;
    cout<<" 1. Eleanor - Esposa"<<endl;
    cout<<" 2. Arthur - Filho mais velho"<<endl;
    cout<<" 3. Edward - Filho do meio"<<endl;
    cout<<" 4. Violet - Filha"<<endl;
    cout<<" 5. Alfred - Mordomo"<<endl;
    cout<<"Opcao: ";
    cin>> op;

    switch(op){
      case 1:
        eleanorInterrogation(table, graph, inventory, totalClues, realCluesSolved);
        break;
      case 2:
        arthurInterrogation(table, graph, inventory, totalClues, realCluesSolved);
        break;
      case 3:
        edwardInterrogation(table, graph, inventory, totalClues, realCluesSolved);
        break;
      case 4:
        violetInterrogation(table, graph, inventory, totalClues, realCluesSolved);
        break;
      case 5:
        alfredInterrogation(table, graph, inventory, totalClues, realCluesSolved);
        break;
      case 0:
        cout<<"Saindo"<<endl;
        break;
      default:
          cout<<"Opcao invalida"<<endl;
          cout<<"Tente novamente"<<endl; //Não precisa de ajuste
          break;
    }
  }
}


//---------------[Final]---------------
void bestEnding(){
  cout << "Sherlock reunia todos." << endl; //"reunia" não precisa de ajuste aqui, mas vou ajustar para consistência
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "E explica:" << endl;
  cout << "- as facadas eram exageradas demais;" << endl;
  cout << "- a cena foi manipulada;" << endl;
  cout << "- o assassino era menor;" << endl;
  cout << "- alguem lavou as maos;" << endl;
  cout << "- passos leves;" << endl;
  cout << "- ausencia de emocao genuina." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Entao Holmes encara Violet." << endl; //"Então" não precisa de ajuste aqui, mas vou ajustar para consistência
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "E ela..." << endl; //"..." não precisa de ajuste aqui, mas vou ajustar para consistência
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "sorri." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Nao porque foi pega." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Mas porque:" << endl;
  cout << "finalmente encontrou alguem capaz de entende-la." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << endl << "===== FIM =====" << endl;
};

//---------------[Menus Acusações]---------------
void accusationMenu(){ //Cena de acusação
  int opt = 0;
  limparTela();

  cout << "\nQuem e o assassino?\n"; //"é" não precisa de ajuste aqui, mas vou ajustar para consistência
  cout << "1 - Eleanor\n2 - Arthur\n3 - Edward\n4 - Violet\n5 - Alfred\n";

  cin >> opt;

  if(opt == 4){
    cout << "\nSherlock observa Violet em silencio...\n";
    cout << "Ela sorri.\n";
    
    bestEnding();
  }
  else{
    cout << "\nHolmes fecha os olhos.\n";
    cout << "A acusacao nao se sustenta.\n";
  }
}

//---------------[Menu Cozinha]---------------
void kitchenMenu(TabelaHash &table, Grafo &graph,Clue inventory[], int &totalClues, int &realCluesSolved){ //Menu da cena da cozinha
  int op = -1;
  int sub_op;
  do{
    limparTela();
    cout<<"\n[COZINHA]\n"<< endl;
    
    cout << "O que deseja investigar?" << endl;
    cout << " 1. A Faca na Vitima" << endl;
    cout << " 2. Cadeira Caida" << endl;
    cout << " 3. Taca de Vinho Quebrada" << endl;
    cout << " 4. Sangue na Pia" << endl;
    cout << " 5. Lenco no Chao" << endl;
    cout << " 6. Bituca de Cigarro" << endl;
    cout << " 7. Frasco de Remedio no lixo" << endl;
    cout << " 8. Conversar com alguns empregados" << endl;
    cout << " 9. Ver Inventario" << endl;
    cout << " 0. Se retirar" << endl;

    cin >> op;
    cin.ignore();
      
    switch(op){
      case 1: { // Pistas da Faca
        cout << "\nAnalisando a faca..." << endl;
        cout << "1. Examinar limpeza da lamina" << endl;
        cout << "2. Examinar angulo do golpe" << endl;
        cout << "3. Examinar cabo da faca" << endl;
        cout << "0. Voltar" << endl;
        cin >> sub_op;
        cin.ignore();

        switch(sub_op) {
            case 1:
                investigateClue(table, graph, "P1", inventory, totalClues, realCluesSolved); 
                break;
            case 2:
                investigateClue(table, graph, "angulo_facada", inventory, totalClues, realCluesSolved);
                break;
            case 3:
                investigateClue(table, graph, "P3", inventory, totalClues, realCluesSolved);
                break;
            case 0:
                break; // Volta ao menu da cozinha
            default:
              cout << "Opcao invalida." << endl;
        }
        break;
      }
      case 2:
        investigateClue(table, graph, "cadeira_caida", inventory, totalClues, realCluesSolved);
        break;
      case 3:
        investigateClue(table, graph, "P10", inventory, totalClues, realCluesSolved);
        break;
      case 4:
        investigateClue(table, graph, "P11", inventory, totalClues, realCluesSolved);
        break;
      case 5:
        investigateClue(table, graph, "lenco_ensanguentado", inventory, totalClues, realCluesSolved);
        break;
      case 6:
        investigateClue(table, graph, "P5", inventory, totalClues, realCluesSolved);
        break;
      case 7:
        investigateClue(table, graph, "frasco_antidepressivo", inventory, totalClues, realCluesSolved);
        break;
      case 8:
        investigateClue(table, graph, "discussao_arthur", inventory, totalClues, realCluesSolved);
        break;
      case 9:
        showInventory(inventory, totalClues);
        break;
      case 0:
        cout << "Saindo da cozinha..." << endl;
        break;
      default:
        if (op != 0) cout << "Opcao invalida." << endl;
    }
  } while(op != 0);
}

//---------------[Menu INVESTIGACAO]---------------
void menuManager(TabelaHash &table, Grafo &graph, Clue inventory[], int &totalClues, int &realCluesSolved){
  int op = -1;
  //TabelaHash t;
  //Grafo gInvestigation;
  
  do{
    limparTela();
    cout << "\n=== MENU DE INVESTIGACAO ===" << endl; 
    cout << "1. Investigar Cena do Crime" << endl;
    cout << "2. Interrogar Suspeitos" << endl;
    cout << "3. Fazer uma acusacao" << endl;
    cout << "0. Desistir" << endl;
    cout << "Opcao: ";
    cin >> op;

    switch(op){
      case 1:
        kitchenMenu(table, graph , inventory, totalClues, realCluesSolved);
        break;
      case 2:
        interrogationScene(table, graph, inventory, totalClues, realCluesSolved);
        break;
      case 3:
        if(realCluesSolved >= 3){
          accusationMenu();
        }else{
          cout << "\nHolmes ainda nao possui evidencias suficientes.\n"; //"evidências" não precisa de ajuste aqui, mas vou ajustar para consistência
        }
        break;
      case 0:
        cout << "Voce foi incapaz de resolver o caso." << endl;
        break;
      default:
        cout << "Opcao invalida." << endl;
        break;
    }
  }while(op != 0);
  
}

//---------------[Criações]---------------
void createClues(TabelaHash &table){
    // ============================================
    // PISTAS PRINCIPAIS (realClue = true)
    // ============================================
    
    // --- Faca do Crime ---
    {
        Clue faca_crime;
        faca_crime.idClue = 1;
        faca_crime.title = "Faca do Crime";
        faca_crime.description = "Uma faca de cozinha cravada no peito da vitima. "
                                 "A lamina esta parcialmente limpa: ha sangue visivel, "
                                 "mas quase nenhuma impressao digital no corpo metalico. "
                                 "Pequenas ranhuras proximas ao cabo ainda contem residuos de sangue, "
                                 "indicando que o assassino tentou limpar a arma, mas esqueceu os detalhes.";
        faca_crime.question = "O que a limpeza parcial da faca sugere sobre o assassino?";
        faca_crime.options[0] = "Foi um crime premeditado com tecnicos profissionais";
        faca_crime.options[1] = "O assassino estava com pressa ou nao tinha experiencia em limpar cenas de crime";
        faca_crime.options[2] = "A faca foi lavada acidentalmente durante o ataque";
        faca_crime.options[3] = "Nao ha como deduzir nada a partir da limpeza da faca";
        faca_crime.correctReponse = 1;
        faca_crime.resolvida = false;
        faca_crime.descoberta = false;
        faca_crime.realClue = true;
        table.input("faca_crime", faca_crime);
    }
    
    // --- Angulo da Facada ---
    {
        Clue angulo_facada;
        angulo_facada.idClue = 2;
        angulo_facada.title = "Angulo da Facada";
        angulo_facada.description = "A lamina entrou em um angulo descendente incomum. "
                                    "Isso significa que o assassino era significativamente mais baixo "
                                    "que o patriarca OU que ele golpeou a vitima por tras enquanto "
                                    "ela estava sentada.";
        angulo_facada.question = "O que o angulo descendente da facada indica?";
        angulo_facada.options[0] = "O assassino era canhoto";
        angulo_facada.options[1] = "A vitima foi atacada deitada no chao";
        angulo_facada.options[2] = "O assassino era mais baixo que a vitima OU a vitima estava sentada";
        angulo_facada.options[3] = "Foi um golpe dado por duas pessoas simultaneamente";
        angulo_facada.correctReponse = 2;
        angulo_facada.resolvida = false;
        angulo_facada.descoberta = false;
        angulo_facada.realClue = true;
        table.input("angulo_facada", angulo_facada);
    }
    
    // --- Microfissuras no Cabo ---
    {
        Clue microfissuras_cabo;
        microfissuras_cabo.idClue = 3;
        microfissuras_cabo.title = "Microfissuras no Cabo";
        microfissuras_cabo.description = "Existem microfissuras na estrutura de polimero do cabo da faca. "
                                          "Isso indica que quem a segurou aplicou uma forca excessiva "
                                          "induzida por tensao extrema, adrenalina e forte obsessao emocional. "
                                          "Nao e um golpe frio e calculado — e um crime passional.";
        microfissuras_cabo.question = "O que as microfissuras no cabo da faca revelam sobre o estado emocional do assassino?";
        microfissuras_cabo.options[0] = "O assassino estava calmo e calculista";
        microfissuras_cabo.options[1] = "A faca era de baixa qualidade e quebrou sozinha";
        microfissuras_cabo.options[2] = "O assassino agiu com tensao extrema, adrenalina e obsessao emocional";
        microfissuras_cabo.options[3] = "As microfissuras foram causadas pelo armazenamento inadequado da faca";
        microfissuras_cabo.correctReponse = 2;
        microfissuras_cabo.resolvida = false;
        microfissuras_cabo.descoberta = false;
        microfissuras_cabo.realClue = true;
        table.input("microfissuras_cabo", microfissuras_cabo);
    }
    
    // ============================================
    // PISTAS DE AMBIENTE (realClue = true)
    // ============================================
    
    // --- Cadeira Caida ---
    {
        Clue cadeira_caida;
        cadeira_caida.idClue = 4;
        cadeira_caida.title = "Cadeira Caida";
        cadeira_caida.description = "Uma cadeira caida distante do centro da cozinha. "
                                     "No chao, ha marcas de arrasto rapido. A disposicao faz parecer "
                                     "que foi colocada ou derrubada propositalmente para simular "
                                     "uma briga corporal que nao aconteceu.";
        cadeira_caida.question = "Por que a posicao da cadeira caida e suspeita?";
        cadeira_caida.options[0] = "Ela prova que houve uma luta corpo a corpo intensa";
        cadeira_caida.options[1] = "A cadeira foi arrastada para o centro durante o ataque";
        cadeira_caida.options[2] = "Ela parece ter sido colocada propositalmente para forjar uma briga";
        cadeira_caida.options[3] = "A cadeira caiu acidentalmente antes do crime";
        cadeira_caida.correctReponse = 2;
        cadeira_caida.resolvida = false;
        cadeira_caida.descoberta = false;
        cadeira_caida.realClue = true;
        table.input("cadeira_caida", cadeira_caida);
    }
    
    // --- Taca de Vinho Quebrada ---
    {
        Clue taca_vinho;
        taca_vinho.idClue = 5;
        taca_vinho.title = "Taca de Vinho Quebrada";
        taca_vinho.description = "Uma unica taca de vinho quebrada no chao. "
                                  "O liquido derramado indica que o homem estava sozinho "
                                  "apreciando a bebida antes do ataque. "
                                  "Nao ha segunda taca — ele nao estava acompanhado.";
        taca_vinho.question = "O que a presenca de uma unica taca de vinho indica?";
        taca_vinho.options[0] = "O assassino bebeu com a vitima antes do crime";
        taca_vinho.options[1] = "A vitima estava sozinha bebendo vinho antes do ataque";
        taca_vinho.options[2] = "Havia duas tacas, mas a segunda foi escondida";
        taca_vinho.options[3] = "A taca foi deixada la propositalmente como pista falsa";
        taca_vinho.correctReponse = 1;
        taca_vinho.resolvida = false;
        taca_vinho.descoberta = false;
        taca_vinho.realClue = true;
        table.input("taca_vinho", taca_vinho);
    }
    
    // --- Sangue na Pia ---
    {
        Clue sangue_pia;
        sangue_pia.idClue = 6;
        sangue_pia.title = "Sangue na Pia";
        sangue_pia.description = "Pequenos pingos de sangue perto da pia. "
                                  "O padrao de diluicao mostra que alguem lavou rapidamente "
                                  "as maos ali logo apos o ataque. Um cheiro do detergente "
                                  "permanece no local — um cheiro agradavel de lavanda.";
        sangue_pia.question = "O que o sangue diluido na pia revela sobre o assassino?";
        sangue_pia.options[0] = "A vitima tentou se limpar antes de morrer";
        sangue_pia.options[1] = "O assassino lavou as maos na pia apos o crime";
        sangue_pia.options[2] = "Foi uma tentativa falha de limpar a cena inteira";
        sangue_pia.options[3] = "O sangue caiu ali durante o ataque inicial";
        sangue_pia.correctReponse = 1;
        sangue_pia.resolvida = false;
        sangue_pia.descoberta = false;
        sangue_pia.realClue = true;
        table.input("sangue_pia", sangue_pia);
    }
    
    // --- Cheiro de Lavanda ---
    {
        Clue cheiro_lavanda;
        cheiro_lavanda.idClue = 7;
        cheiro_lavanda.title = "Cheiro de Lavanda na Pia";
        cheiro_lavanda.description = "O detergente usado na pia exala um perfume marcante "
                                      "de lavanda. E um produto de limpeza caro, importado, "
                                      "comumente associado a habitos femininos da alta sociedade. "
                                      "O cheiro ainda esta forte, indicando uso muito recente.";
        cheiro_lavanda.question = "O que o cheiro de lavanda na pia pode indicar sobre o assassino?";
        cheiro_lavanda.options[0] = "Era um empregado que limpava a cozinha";
        cheiro_lavanda.options[1] = "Provavelmente e uma mulher ou alguem que usa produtos femininos de luxo";
        cheiro_lavanda.options[2] = "O cheiro nao tem relacao com o crime";
        cheiro_lavanda.options[3] = "E um perfume masculino caro";
        cheiro_lavanda.correctReponse = 1;
        cheiro_lavanda.resolvida = false;
        cheiro_lavanda.descoberta = false;
        cheiro_lavanda.realClue = true;
        table.input("cheiro_lavanda", cheiro_lavanda);
    }
    
    // ============================================
    // PISTAS FALSAS / RED HERRINGS (realClue = false)
    // ============================================
    
    // --- Lenco Ensanguentado ---
    {
        Clue lenco_ensanguentado;
        lenco_ensanguentado.idClue = 8;
        lenco_ensanguentado.title = "Lenco Ensanguentado com Ferrugem";
        lenco_ensanguentado.description = "Um lenco de seda fina com o monograma 'E.W.', "
                                           "encontrado muito proximo a faca e parcialmente ensanguentado. "
                                           "Ao inspecionar a fundo, nota-se uma mancha escura de ferrugem "
                                           "de metal velho. O monograma 'E.W.' corresponde a Eleanor Whitmore. "
                                           "Porem, a ferrugem sugere que o lenco foi usado em algo alem do crime.";
        lenco_ensanguentado.question = "A quem pertence o lenco encontrado na cena do crime?";
        lenco_ensanguentado.options[0] = "A Violet Whitmore";
        lenco_ensanguentado.options[1] = "A Eleanor Whitmore (monograma E.W.)";
        lenco_ensanguentado.options[2] = "Ao mordomo Alfred";
        lenco_ensanguentado.options[3] = "A Edward Whitmore";
        lenco_ensanguentado.correctReponse = 1;
        lenco_ensanguentado.resolvida = false;
        lenco_ensanguentado.descoberta = false;
        lenco_ensanguentado.realClue = false;
        table.input("lenco_ensanguentado", lenco_ensanguentado);
    }
    
    // --- Cigarros Turcos ---
    {
        Clue cigarros_turcos;
        cigarros_turcos.idClue = 9;
        cigarros_turcos.title = "Cigarros Turcos na Cozinha";
        cigarros_turcos.description = "Uma bituca de cigarro encontrada perto da porta da cozinha. "
                                       "E de uma marca cara de tabaco turco com aroma adocicado "
                                       "de baunilha e filtro dourado. Ninguem na familia admite fumar "
                                       "este tipo de cigarro. Apenas o mordomo Alfred tem cheiro de fumo, "
                                       "mas ele fuma cachimbo barato, nao cigarros de luxo.";
        cigarros_turcos.question = "O que a bituca de cigarro turco na cozinha sugere?";
        cigarros_turcos.options[0] = "O mordomo Alfred e o assassino";
        cigarros_turcos.options[1] = "A vitima estava fumando antes de morrer";
        cigarros_turcos.options[2] = "Alguem que fuma cigarros turcos esteve na cozinha — ou a bituca foi plantada la";
        cigarros_turcos.options[3] = "E uma pista irrelevante";
        cigarros_turcos.correctReponse = 2;
        cigarros_turcos.resolvida = false;
        cigarros_turcos.descoberta = false;
        cigarros_turcos.realClue = false;
        table.input("cigarros_turcos", cigarros_turcos);
    }
    
    // --- Discussao Arthur ---
    {
        Clue discussao_arthur;
        discussao_arthur.idClue = 10;
        discussao_arthur.title = "Discussao Recente com o Filho Mais Velho";
        discussao_arthur.description = "Os empregados relataram uma briga violenta entre Arthur Whitmore "
                                        "e seu pai na manha do crime. Arthur confrontou Raymond sobre "
                                        "sua aposentadoria e a exclusao de seu nome da linha de sucessao "
                                        "das empresas da familia. Arthur queria assumir o controle dos negocios.";
        discussao_arthur.question = "O que motivou a discussao entre Arthur e seu pai?";
        discussao_arthur.options[0] = "Arthur descobriu a traicao da esposa do pai";
        discussao_arthur.options[1] = "Arthur foi excluido da linha de sucessao das empresas";
        discussao_arthur.options[2] = "Arthur queria vender a mansao da familia";
        discussao_arthur.options[3] = "Arthur se recusou a trabalhar na empresa";
        discussao_arthur.correctReponse = 1;
        discussao_arthur.resolvida = false;
        discussao_arthur.descoberta = false;
        discussao_arthur.realClue = false;
        table.input("discussao_arthur", discussao_arthur);
    }
    
    // --- Frasco Antidepressivo ---
    {
        Clue frasco_antidepressivo;
        frasco_antidepressivo.idClue = 11;
        frasco_antidepressivo.title = "Frasco de Antidepressivo no Lixo";
        frasco_antidepressivo.description = "Um frasco de vidro ambar de farmacia de manipulacao "
                                             "encontrado no lixo da cozinha. O rotulo esta rasgado, "
                                             "mas exibe o nome do paciente: 'Edwa...' e o medicamento "
                                             "e um forte antidepressivo. O frasco esta vazio. "
                                             "Edward Whitmore e o filho do meio, conhecido por crises "
                                             "de ansiedade e instabilidade emocional.";
        frasco_antidepressivo.question = "O que o frasco de antidepressivo no lixo da cozinha indica?";
        frasco_antidepressivo.options[0] = "Edward Whitmore estava na cozinha e matou o pai";
        frasco_antidepressivo.options[1] = "A vitima tomava antidepressivos escondido";
        frasco_antidepressivo.options[2] = "Alguem consumiu o remedio de Edward e descartou o frasco la";
        frasco_antidepressivo.options[3] = "O frasco foi jogado la dias antes do crime";
        frasco_antidepressivo.correctReponse = 2;
        frasco_antidepressivo.resolvida = false;
        frasco_antidepressivo.descoberta = false;
        frasco_antidepressivo.realClue = false;
        table.input("frasco_antidepressivo", frasco_antidepressivo);
    }
    
    // --- Suspeita de Traicao ---
    {
        Clue suspeita_traicao;
        suspeita_traicao.idClue = 12;
        suspeita_traicao.title = "Suspeita de Traicao";
        suspeita_traicao.description = "Eleanor Whitmore revelou a Sherlock que suspeitava "
                                        "que o marido Raymond estava tendo um caso extraconjugal. "
                                        "Ela mencionou que essa intuicao a corroia ha meses "
                                        "e que Raymond voltava tarde do trabalho constantemente, "
                                        "cada vez mais distante. Esse ciume pode ser um poderoso "
                                        "motivo passional para o crime.";
        suspeita_traicao.question = "Qual o possivel motivo de Eleanor para cometer o crime?";
        suspeita_traicao.options[0] = "Ela queria a heranca das empresas";
        suspeita_traicao.options[1] = "Ela suspeitava que Raymond a traia — motivo passional";
        suspeita_traicao.options[2] = "Ela foi chantageada por Arthur";
        suspeita_traicao.options[3] = "Ela nao tinha motivo algum";
        suspeita_traicao.correctReponse = 1;
        suspeita_traicao.resolvida = false;
        suspeita_traicao.descoberta = false;
        suspeita_traicao.realClue = false;
        table.input("suspeita_traicao", suspeita_traicao);
    }
    
    // ============================================
    // PISTAS TESTEMUNHAIS E DE CONEXAO
    // ============================================
    
    // --- Passos Leves ---
    {
        Clue passos_leves;
        passos_leves.idClue = 13;
        passos_leves.title = "Passos Leves no Corredor";
        passos_leves.description = "Tanto Edward quanto Alfred relataram ter ouvido passos "
                                    "no corredor proximo a cozinha. Alfred descreveu os passos "
                                    "como 'rapidos e leves demais para serem de um homem'. "
                                    "Edward tambem os ouviu e fugiu de volta para o jardim. "
                                    "Isso indica que uma mulher estava no corredor na hora do crime.";
        passos_leves.question = "O que os passos leves no corredor indicam sobre o assassino?";
        passos_leves.options[0] = "Era um homem pesando mais de 90 quilos";
        passos_leves.options[1] = "Duas pessoas estavam no corredor";
        passos_leves.options[2] = "Provavelmente era uma mulher — passos leves demais para homem";
        passos_leves.options[3] = "Nao e possivel deduzir nada a partir de passos";
        passos_leves.correctReponse = 2;
        passos_leves.resolvida = false;
        passos_leves.descoberta = false;
        passos_leves.realClue = true;
        table.input("passos_leves", passos_leves);
    }
    
    // --- Dedos Amarelados ---
    {
        Clue dedos_amarelados;
        dedos_amarelados.idClue = 14;
        dedos_amarelados.title = "Dedos Amarelados de Fumante";
        dedos_amarelados.description = "Watson observou que os dedos indicador e medio de Edward "
                                        "estao levemente amarelados — um sinal classico de fumante "
                                        "compulsivo. Isso contradiz a afirmacao inicial de Edward "
                                        "de que 'ninguem nesta familia fuma'. Edward mais tarde "
                                        "admitiu que fuma cigarros turcos escondido da mae.";
        dedos_amarelados.question = "O que os dedos amarelados de Edward revelam?";
        dedos_amarelados.options[0] = "Ele trabalha com produtos quimicos";
        dedos_amarelados.options[1] = "Ele e fumante compulsivo, apesar de negar inicialmente";
        dedos_amarelados.options[2] = "Ele tem uma doenca de figado";
        dedos_amarelados.options[3] = "E uma caracteristica genetica da familia";
        dedos_amarelados.correctReponse = 1;
        dedos_amarelados.resolvida = false;
        dedos_amarelados.descoberta = false;
        dedos_amarelados.realClue = false;
        table.input("dedos_amarelados", dedos_amarelados);
    }
    
    // --- Portao de Ferro ---
    {
        Clue portao_ferro;
        portao_ferro.idClue = 15;
        portao_ferro.title = "Portao de Ferro Emperrado";
        portao_ferro.description = "Edward confessou que entrou na cozinha pelas portas dos fundos. "
                                    "O portao de ferro do jardim estava emperrado, e ele usou "
                                    "um lenco que encontrou no jardim para forca-lo e conseguir abrir. "
                                    "Isso explica a mancha de ferrugem de metal velho encontrada no lenco "
                                    "com monograma 'E.W.' que apareceu na cena do crime.";
        portao_ferro.question = "Como a ferrugem foi parar no lenco de Eleanor?";
        portao_ferro.options[0] = "Eleanor tocou em metal velho antes do crime";
        portao_ferro.options[1] = "Edward usou o lenco para forcar um portao de ferro emperrado";
        portao_ferro.options[2] = "A ferrugem veio da faca do crime";
        portao_ferro.options[3] = "O lenco ja estava enferrujado quando foi comprado";
        portao_ferro.correctReponse = 1;
        portao_ferro.resolvida = false;
        portao_ferro.descoberta = false;
        portao_ferro.realClue = true;
        table.input("portao_ferro", portao_ferro);
    }
    
    // --- Cinzeiro no Jardim ---
    {
        Clue cinzeiro_jardim;
        cinzeiro_jardim.idClue = 16;
        cinzeiro_jardim.title = "Cinzeiro no Jardim";
        cinzeiro_jardim.description = "Edward mencionou que fuma no jardim e que ha um cinzeiro "
                                       "do lado de fora da casa. Ele acredita que alguem pegou "
                                       "uma de suas bitucas de cigarro turco do cinzeiro e a colocou "
                                       "na cozinha para incrimina-lo. Se for verdade, o assassino "
                                       "conhece os habitos de Edward e planejou incrimina-lo.";
        cinzeiro_jardim.question = "O que a bituca de cigarro na cozinha provavelmente significa?";
        cinzeiro_jardim.options[0] = "Edward fumou na cozinha durante o crime";
        cinzeiro_jardim.options[1] = "A vitima fumava cigarros turcos";
        cinzeiro_jardim.options[2] = "Alguem pegou a bituca do cinzeiro e plantou na cozinha para incriminar Edward";
        cinzeiro_jardim.options[3] = "O mordomo comprou os mesmos cigarros";
        cinzeiro_jardim.correctReponse = 2;
        cinzeiro_jardim.resolvida = false;
        cinzeiro_jardim.descoberta = false;
        cinzeiro_jardim.realClue = true;
        table.input("cinzeiro_jardim", cinzeiro_jardim);
    }
    
    // --- Sintomas Eleanor ---
    {
        Clue sintomas_eleanor;
        sintomas_eleanor.idClue = 17;
        sintomas_eleanor.title = "Sintomas Suspeitos de Eleanor";
        sintomas_eleanor.description = "Sherlock observou que Eleanor apresenta agitacao extrema nos pes, "
                                        "espasmos musculares e uma forte sonolencia incapacitante. "
                                        "Ela afirma tomar sedativos para insonia, mas esses sintomas "
                                        "sao classicos de abuso de antidepressivos sem prescricao — "
                                        "exatamente o tipo de medicamento encontrado no frasco "
                                        "de Edward descartado no lixo da cozinha.";
        sintomas_eleanor.question = "O que os sintomas de Eleanor realmente indicam?";
        sintomas_eleanor.options[0] = "Ela esta sob efeito de sedativos como afirma";
        sintomas_eleanor.options[1] = "Ela apresenta sintomas de abuso de antidepressivos, nao de sedativos";
        sintomas_eleanor.options[2] = "Ela sofre de uma doenca neurologica";
        sintomas_eleanor.options[3] = "Os sintomas sao irrelevantes para o caso";
        sintomas_eleanor.correctReponse = 1;
        sintomas_eleanor.resolvida = false;
        sintomas_eleanor.descoberta = false;
        sintomas_eleanor.realClue = false;
        table.input("sintomas_eleanor", sintomas_eleanor);
    }
    
    // --- Testamento e Sucessao ---
    {
        Clue testamento_sucessao;
        testamento_sucessao.idClue = 18;
        testamento_sucessao.title = "Testamento e Linha de Sucessao";
        testamento_sucessao.description = "Durante o interrogatorio de Arthur, quando perguntado "
                                           "sobre uma possivel mudanca no testamento, ele travou "
                                           "e encarou a mae, Eleanor, com um olhar de indagacao. "
                                           "Ela nao respondeu. Isso sugere que Eleanor sabe algo "
                                           "sobre o testamento que Arthur desconhece — ou que "
                                           "os dois tem um segredo compartilhado sobre a heranca.";
        testamento_sucessao.question = "O que a reacao de Arthur ao falar do testamento sugere?";
        testamento_sucessao.options[0] = "Ele ja sabia que foi deserdado";
        testamento_sucessao.options[1] = "Ele e Eleanor compartilham um segredo sobre a heranca";
        testamento_sucessao.options[2] = "O testamento nunca foi alterado";
        testamento_sucessao.options[3] = "Arthur estava blefando o tempo todo";
        testamento_sucessao.correctReponse = 1;
        testamento_sucessao.resolvida = false;
        testamento_sucessao.descoberta = false;
        testamento_sucessao.realClue = false;
        table.input("testamento_sucessao", testamento_sucessao);
    }
    
    // --- Mordomo Invisivel ---
    {
        Clue mordomo_invisivel;
        mordomo_invisivel.idClue = 19;
        mordomo_invisivel.title = "Mordomo Invisivel";
        mordomo_invisivel.description = "Alfred serviu Raymond Whitmore por quarenta anos. "
                                         "Quando perguntado sobre sua relacao com o patrao, "
                                         "respondeu: 'Para o Sr. Raymond, homens da minha classe "
                                         "sao como mobilia. Invisiveis.' Isso indica um profundo "
                                         "ressentimento acumulado por decadas de tratamento indigno. "
                                         "Por ser 'invisivel', Alfred poderia circular pela casa "
                                         "sem ser notado — a testemunha perfeita... ou o assassino perfeito.";
        mordomo_invisivel.question = "Por que a 'invisibilidade' de Alfred e relevante?";
        mordomo_invisivel.options[0] = "Significa que ele nao pode ser testemunha de nada";
        mordomo_invisivel.options[1] = "Ele poderia circular sem ser notado — testemunha ou suspeito ideal";
        mordomo_invisivel.options[2] = "Prova que ele e o assassino";
        mordomo_invisivel.options[3] = "Indica que ele nao trabalha mais na casa";
        mordomo_invisivel.correctReponse = 1;
        mordomo_invisivel.resolvida = false;
        mordomo_invisivel.descoberta = false;
        mordomo_invisivel.realClue = false;
        table.input("mordomo_invisivel", mordomo_invisivel);
    }
    
    // --- Cachimbo Alfred ---
    {
        Clue cachimbo_alfred;
        cachimbo_alfred.idClue = 20;
        cachimbo_alfred.title = "Cachimbo Velho do Mordomo";
        cachimbo_alfred.description = "Alfred possui um forte cheiro de fumo, mas ele explica "
                                       "que fuma seu cachimbo velho no jardim. Ele afirma nao ter "
                                       "dinheiro para 'os luxos que os jovens compram na cidade', "
                                       "referindo-se aos cigarros turcos importados. O cheiro de "
                                       "cachimbo e diferente do aroma adocicado de baunilha "
                                       "dos cigarros encontrados na cozinha.";
        cachimbo_alfred.question = "O cheiro de fumo em Alfred e compativel com os cigarros da cena do crime?";
        cachimbo_alfred.options[0] = "Sim, e exatamente o mesmo tabaco turco";
        cachimbo_alfred.options[1] = "Nao — Alfred fuma cachimbo barato, nao cigarros turcos de luxo";
        cachimbo_alfred.options[2] = "Alfred nao fuma, o cheiro e de outra pessoa";
        cachimbo_alfred.options[3] = "O cheiro nao tem como ser identificado";
        cachimbo_alfred.correctReponse = 1;
        cachimbo_alfred.resolvida = false;
        cachimbo_alfred.descoberta = false;
        cachimbo_alfred.realClue = false;
        table.input("cachimbo_alfred", cachimbo_alfred);
    }
    
    // ============================================
    // PISTAS SOBRE VIOLET (COMPORTAMENTAIS E FISICAS)
    // ============================================
    
    // --- Fones de Ouvido ---
    {
        Clue fones_ouvido;
        fones_ouvido.idClue = 21;
        fones_ouvido.title = "Fones de Ouvido de Violet";
        fones_ouvido.description = "Violet afirma que estava em seu quarto de costura "
                                    "com fones de ouvido, terminando o bordado de um vestido novo. "
                                    "Ela diz que nao escutou absolutamente nada ate os gritos "
                                    "dos policiais. No entanto, Edward relatou ter ouvido passos "
                                    "leves de mulher no corredor. Se Violet estava com fones "
                                    "no quarto, quem estava no corredor?";
        fones_ouvido.question = "Qual a contradicao no alibi de Violet?";
        fones_ouvido.options[0] = "Ela nao sabe costurar";
        fones_ouvido.options[1] = "Ela diz que usava fones no quarto, mas passos de mulher foram ouvidos no corredor";
        fones_ouvido.options[2] = "Os fones de ouvido estavam quebrados";
        fones_ouvido.options[3] = "Nao ha contradicao nenhuma";
        fones_ouvido.correctReponse = 1;
        fones_ouvido.resolvida = false;
        fones_ouvido.descoberta = false;
        fones_ouvido.realClue = true;
        table.input("fones_ouvido", fones_ouvido);
    }
    
    // --- Frieza Violet ---
    {
        Clue frieza_violet;
        frieza_violet.idClue = 22;
        frieza_violet.title = "Frieza Emocional de Violet";
        frieza_violet.description = "Sherlock observou que Violet responde com rapidez e um sorriso "
                                     "excessivamente simetrico. Suas palavras pintam um retrato "
                                     "de 'filha perfeita', mas seus olhos permanecem frios e distantes, "
                                     "com pouco vestigio real de luto ou calor afetivo. "
                                     "Quando perguntada se esta triste, ela diz: 'E como nao estar mais triste?' "
                                     "— mas Sherlock nota que ela DIZ que sente, mas nao demonstra.";
        frieza_violet.question = "O que o comportamento de Violet revela?";
        frieza_violet.options[0] = "Ela esta profundamente abalada e chora escondido";
        frieza_violet.options[1] = "Ela tem uma personalidade naturalmente fria, sem relacao com o crime";
        frieza_violet.options[2] = "Ela afirma sentir tristeza mas nao demonstra — ausencia de luto genuino";
        frieza_violet.options[3] = "Ela esta em estado de choque e por isso age assim";
        frieza_violet.correctReponse = 2;
        frieza_violet.resolvida = false;
        frieza_violet.descoberta = false;
        frieza_violet.realClue = true;
        table.input("frieza_violet", frieza_violet);
    }
    
    // --- Frase Perdeu o Medo ---
    {
        Clue perdeu_medo;
        perdeu_medo.idClue = 23;
        perdeu_medo.title = "'Alguem Finalmente Perdeu o Medo Dele'";
        perdeu_medo.description = "Quando Sherlock perguntou a Violet o que ela achava "
                                   "que tinha acontecido, ela respondeu com um sorriso discreto: "
                                   "'Acho que alguem finalmente perdeu o medo dele.' "
                                   "Esta frase e extremamente reveladora. Primeiro, reconhece "
                                   "que o pai era temido. Segundo, sugere alivio, nao choque. "
                                   "Terceiro, a palavra 'finalmente' indica que era algo esperado. "
                                   "Para Sherlock, isso soa quase como uma confissao velada.";
        perdeu_medo.question = "Por que a frase 'alguem finalmente perdeu o medo dele' e tao importante?";
        perdeu_medo.options[0] = "E uma frase comum sem significado especial";
        perdeu_medo.options[1] = "Revela que o pai era amado por todos";
        perdeu_medo.options[2] = "Soa como confissao velada — indica alivio, nao choque, e conhecimento intimo";
        perdeu_medo.options[3] = "Mostra que Violet acredita em teorias da conspIracao";
        perdeu_medo.correctReponse = 2;
        perdeu_medo.resolvida = false;
        perdeu_medo.descoberta = false;
        perdeu_medo.realClue = true;
        table.input("perdeu_medo", perdeu_medo);
    }
    
    // --- Adrenalina Violet ---
    {
        Clue adrenalina_violet;
        adrenalina_violet.idClue = 24;
        adrenalina_violet.title = "Tensao e Adrenalina em Violet";
        adrenalina_violet.description = "Sherlock observou que as maos de Violet estao tensas, "
                                         "como se estivesse com uma certa adrenalina no corpo. "
                                         "Ela diz estar triste e abalada, mas seu corpo conta "
                                         "uma historia diferente. A tensao muscular e compativel "
                                         "com alguem que passou por um esforco fisico extremo "
                                         "recentemente — como esfaquear alguem com forca obsessiva.";
        adrenalina_violet.question = "O que as maos tensas de Violet sugerem?";
        adrenalina_violet.options[0] = "Ela esta apenas nervosa pelo interrogatorio";
        adrenalina_violet.options[1] = "A tensao pode indicar esforco fisico recente, compativel com o crime";
        adrenalina_violet.options[2] = "Ela tem um problema de saude cronico";
        adrenalina_violet.options[3] = "E uma reacao alergica ao ambiente";
        adrenalina_violet.correctReponse = 1;
        adrenalina_violet.resolvida = false;
        adrenalina_violet.descoberta = false;
        adrenalina_violet.realClue = true;
        table.input("adrenalina_violet", adrenalina_violet);
    }
    
    // --- Vinho nas Unhas ---
    {
        Clue vinho_unhas;
        vinho_unhas.idClue = 25;
        vinho_unhas.title = "Vinho e Sangue nas Unhas de Violet";
        vinho_unhas.description = "Watson notou pequenas crostas arroxeadas sob as unhas "
                                   "polidas de Violet, alem de uma sutil mancha escura "
                                   "na barra interna de sua manga de seda. O cheiro de "
                                   "lavanda em suas maos nao consegue mascarar o odor "
                                   "denso e fermentado de uva. Isso a conecta diretamente "
                                   "a cena do crime, onde vinho e sangue se misturaram.";
        vinho_unhas.question = "O que as crostas arroxeadas nas unhas de Violet indicam?";
        vinho_unhas.options[0] = "Ela estava pintando as unhas antes do interrogatorio";
        vinho_unhas.options[1] = "Sao residuos de tinta do bordado";
        vinho_unhas.options[2] = "Sao crostas de vinho tinto seco, conectando-a a cena do crime";
        vinho_unhas.options[3] = "Ela sofre de uma condicao medica nas unhas";
        vinho_unhas.correctReponse = 2;
        vinho_unhas.resolvida = false;
        vinho_unhas.descoberta = false;
        vinho_unhas.realClue = true;
        table.input("vinho_unhas", vinho_unhas);
    }
    
    // --- Alibi Eleanor ---
    {
        Clue alibi_eleanor;
        alibi_eleanor.idClue = 26;
        alibi_eleanor.title = "Alibi de Eleanor";
        alibi_eleanor.description = "Eleanor afirma que estava em seu quarto tomando sedativos "
                                     "para insonia. No entanto, mais tarde admitiu ter ido a cozinha "
                                     "para roubar os antidepressivos de Edward. Ela estava na cena "
                                     "do crime antes do assassinato. Porem, ela afirma que Raymond "
                                     "ainda estava vivo quando ela passou por la.";
        alibi_eleanor.question = "Qual e a inconsistencia no alibi de Eleanor?";
        alibi_eleanor.options[0] = "Ela nunca esteve na cozinha";
        alibi_eleanor.options[1] = "Ela dizia estar no quarto, mas admitiu ter ido a cozinha";
        alibi_eleanor.options[2] = "Ela confessou o crime";
        alibi_eleanor.options[3] = "Seu alibi e perfeito e comprovado";
        alibi_eleanor.correctReponse = 1;
        alibi_eleanor.resolvida = false;
        alibi_eleanor.descoberta = false;
        alibi_eleanor.realClue = false;
        table.input("alibi_eleanor", alibi_eleanor);
    }
    
    // --- Alibi Edward ---
    {
        Clue alibi_edward;
        alibi_edward.idClue = 27;
        alibi_edward.title = "Alibi de Edward";
        alibi_edward.description = "Edward afirma que estava no jardim observando o ceu. "
                                    "Mais tarde admitiu ter entrado na cozinha para pegar gelo. "
                                    "Ele diz que viu que o pai nao estava la, ouviu passos "
                                    "de mulher no corredor e fugiu de volta para o jardim. "
                                    "Seu depoimento ajuda a estabelecer a linha do tempo.";
        alibi_edward.question = "O que o depoimento de Edward ajuda a provar?";
        alibi_edward.options[0] = "Que ele e o assassino";
        alibi_edward.options[1] = "Que uma mulher estava no corredor na hora proxima ao crime";
        alibi_edward.options[2] = "Que o crime aconteceu no jardim";
        alibi_edward.options[3] = "Que ele nao tem ligacao nenhuma com o caso";
        alibi_edward.correctReponse = 1;
        alibi_edward.resolvida = false;
        alibi_edward.descoberta = false;
        alibi_edward.realClue = true;
        table.input("alibi_edward", alibi_edward);
    }
    
    // --- Alibi Violet ---
    {
        Clue alibi_violet;
        alibi_violet.idClue = 28;
        alibi_violet.title = "Alibi de Violet";
        alibi_violet.description = "Violet afirma estar no quarto de costura no fim do corredor "
                                    "com fones de ouvido. Este alibi e contradito por: "
                                    "(1) Edward e Alfred ouviram passos de mulher no corredor; "
                                    "(2) Violet tem vinho nas unhas e cheiro de lavanda, "
                                    "conectando-a a cena do crime; "
                                    "(3) Sua calma e ausencia de luto genuino.";
        alibi_violet.question = "O alibi de Violet e confiavel?";
        alibi_violet.options[0] = "Sim, ela tem uma testemunha ocular";
        alibi_violet.options[1] = "Nao — multiplas evidencias a contradizem";
        alibi_violet.options[2] = "Sim, pois ela e a filha cacula";
        alibi_violet.options[3] = "E impossivel determinar";
        alibi_violet.correctReponse = 1;
        alibi_violet.resolvida = false;
        alibi_violet.descoberta = false;
        alibi_violet.realClue = true;
        table.input("alibi_violet", alibi_violet);
    }
    
    // --- Tirano Domestico ---
    {
        Clue tirano_domestico;
        tirano_domestico.idClue = 29;
        tirano_domestico.title = "'Tirano Domestico'";
        tirano_domestico.description = "Quando confrontada sobre as microfissuras na faca, Violet "
                                        "revelou sua verdadeira opiniao sobre o pai: 'Meu pai era um "
                                        "tirano domestico, detetive Holmes. Ele esmagava a individualidade "
                                        "de todos nesta casa. Arthur virou um ganancioso previsivel, "
                                        "Edward um feixe de nervos ansioso e minha mae uma dependente "
                                        "de remedios.' Isso contradiz sua afirmacao anterior de que "
                                        "a relacao com o pai era 'maravilhosa'.";
        tirano_domestico.question = "Qual e a contradicao nas declaracoes de Violet sobre o pai?";
        tirano_domestico.options[0] = "Nao ha contradicao nenhuma";
        tirano_domestico.options[1] = "Ela disse que a relacao era maravilhosa, mas depois chamou o pai de tirano domestico";
        tirano_domestico.options[2] = "Ela nunca falou sobre o pai";
        tirano_domestico.options[3] = "Ela sempre odiou o pai abertamente";
        tirano_domestico.correctReponse = 1;
        tirano_domestico.resolvida = false;
        tirano_domestico.descoberta = false;
        tirano_domestico.realClue = true;
        table.input("tirano_domestico", tirano_domestico);
    }
    
    // --- Karma ---
    {
        Clue karma_violet;
        karma_violet.idClue = 30;
        karma_violet.title = "'Foi Apenas o Karma Dele'";
        karma_violet.description = "Violet disse a Sherlock: 'E uma dor imensa que sinto, "
                                    "mas ele nao era uma pessoa boa, acho que isso foi apenas "
                                    "o karma dele.' Esta frase revela que, apesar de afirmar "
                                    "estar triste, ela racionaliza o assassinato como algo "
                                    "merecido. Nao ha indignacao contra o assassino — "
                                    "ha aceitacao do destino do pai.";
        karma_violet.question = "O que a frase 'foi apenas o karma dele' revela sobre Violet?";
        karma_violet.options[0] = "Ela acredita em religioes orientais";
        karma_violet.options[1] = "Ela sente raiva do assassino e quer justica";
        karma_violet.options[2] = "Ela racionaliza o crime como merecido — falta de indignacao moral";
        karma_violet.options[3] = "Ela nao entendeu a pergunta";
        karma_violet.correctReponse = 2;
        karma_violet.resolvida = false;
        karma_violet.descoberta = false;
        karma_violet.realClue = true;
        table.input("karma_violet", karma_violet);
    }

}
  
void createConnectionsClue(Grafo &graph) {
    
  // Nivel 1: Pistas basicas da cena do crime
  graph.addNo("faca_crime", 1);
  graph.addNo("cadeira_caida", 1);
  graph.addNo("taca_vinho", 1);
  graph.addNo("sangue_pia", 1);
  
  // Nivel 2: Analises das pistas basicas
  graph.addNo("angulo_facada", 2);
  graph.addNo("microfissuras_cabo", 2);
  graph.addNo("cheiro_lavanda", 2);
  graph.addNo("passos_leves", 2);
  graph.addNo("portao_ferro", 2);
  graph.addNo("vinho_unhas", 2);
  
  // Nivel 3: Pistas falsas e conexoes
  graph.addNo("lenco_ensanguentado", 3);
  graph.addNo("cigarros_turcos", 3);
  graph.addNo("discussao_arthur", 3);
  graph.addNo("frasco_antidepressivo", 3);
  graph.addNo("suspeita_traicao", 3);
  graph.addNo("dedos_amarelados", 3);
  graph.addNo("cinzeiro_jardim", 3);
  graph.addNo("sintomas_eleanor", 3);
  graph.addNo("testamento_sucessao", 3);
  graph.addNo("mordomo_invisivel", 3);
  graph.addNo("cachimbo_alfred", 3);
  
  // Nivel 4: Comportamento e contradicoes
  graph.addNo("fones_ouvido", 4);
  graph.addNo("frieza_violet", 4);
  graph.addNo("adrenalina_violet", 4);
  graph.addNo("alibi_eleanor", 4);
  graph.addNo("alibi_edward", 4);
  graph.addNo("alibi_violet", 4);
  
  // Nivel 5: Pistas psicologicas e revelacoes
  graph.addNo("perdeu_medo", 5);
  graph.addNo("tirano_domestico", 5);
  graph.addNo("karma_violet", 5);
  
  // Conexoes logicas
  graph.addConnections(0, 4);   // faca_crime -> angulo_facada
  graph.addConnections(0, 5);   // faca_crime -> microfissuras_cabo
  graph.addConnections(3, 6);   // sangue_pia -> cheiro_lavanda
  graph.addConnections(1, 4);   // cadeira_caida -> angulo_facada
  graph.addConnections(2, 9);   // taca_vinho -> vinho_unhas
  graph.addConnections(6, 9);   // cheiro_lavanda -> vinho_unhas
  graph.addConnections(7, 23);  // passos_leves -> alibi_violet
  graph.addConnections(5, 20);  // microfissuras_cabo -> adrenalina_violet
  graph.addConnections(20, 22); // adrenalina_violet -> perdeu_medo
  graph.addConnections(19, 22); // frieza_violet -> perdeu_medo
  graph.addConnections(22, 24); // perdeu_medo -> tirano_domestico
  graph.addConnections(24, 25); // tirano_domestico -> karma_violet
  graph.addConnections(8, 10);  // portao_ferro -> lenco_ensanguentado
  graph.addConnections(11, 12); // cinzeiro_jardim -> cigarros_turcos
  graph.addConnections(23, 7);  // alibi_violet -> passos_leves
  graph.addConnections(9, 6);   // vinho_unhas -> cheiro_lavanda
  graph.addConnections(9, 7);   // vinho_unhas -> passos_leves
};

//---------------[Main]---------------
int main() {
  TabelaHash table;

  Grafo gInvestigation;

  Clue inventory[20];
  int totalClues = 0;

  int realCluesSolved = 0;

  createClues(table);
  createConnectionsClue(gInvestigation);
  
  introStory();
  crimeScene();
  interrogationScene(table, gInvestigation, inventory, totalClues, realCluesSolved);
  menuManager(table, gInvestigation, inventory, totalClues, realCluesSolved);

  cout<< "\n=== FIM DE JOGO ===\n";

  return 0;
}