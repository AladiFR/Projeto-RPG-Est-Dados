#include <iostream>
#include <string>
using namespace std;

const int MAX_VERTICES = 5;
int op;

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
    static const int MAX = 10;

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
      int sum = 0;

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
    
    void showReasoningLine(string clueKey){ //Exibindo linha de raciocínio visualmente com grafo
      for(int i = 0; i < numVertices; i++){
          if(vertices[i]->idClue == clueKey){
              cout << "\nLinha de raciocinio:\n\n";

              cout << vertices[i]->idClue;

              No* current = vertices[i];

              while(current->numConnections > 0){
                  current = current->connection[0];
                  cout << " -> " << current->idClue;
              }

              cout << endl;
              return;
          }
      }
    }

};
void showDeduction(Grafo &graph, string clueKey);

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

  cout << "\n\"Nao. Algo nao encaixa.\"\n";
  return false;
}
void investigateClue(TabelaHash &table, Grafo &graph,string key, Clue inventory[], int &totalClues, int &realCluesSolved){
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

  bool correct = checkAnswer(clue);
  if(correct && clue.realClue){
    realCluesSolved++;
    cout << "\n[Holmes liga essa pista ao assassinato]\n";
    
    showDeduction(graph, key);
  }

  cout << "\nProgresso da investigacao: " << realCluesSolved << "/3 pistas principais.\n";
  table.input(key, clue);
}
void showDeduction(Grafo &graph, string clueKey){
    
    cout << "\n[Holmes analisa as conexoes mentais]\n";
    graph.showReasoningLine(clueKey);


    if(clueKey == "P1"){
        cout << "A faca foi parcialmente limpa.\n";
        cout << "Isso sugere tentativa de esconder o assassinato.\n";
    }

    else if(clueKey == "P2"){
        cout << "O golpe veio de baixo para cima.\n";
        cout << "O assassino provavelmente era menor.\n";
    }

    else if(clueKey == "P3"){
        cout << "A violencia foi emocional.\n";
        cout << "Nao foi um assassinato frio.\n";
    }
}

//---------------[História]---------------
void introStory(){
  cout << "===== INVESTIGACAO POLICIAL =====" << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Londres -- 15 de Janeiro de 2010, 22:20" << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "A chuva fina castigava as ruas de Londres, a névoa envolvia os postes de luz, transformando o brilho amarelado em manchas fantasmagóricas no asfalto molhado." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Dentro de um táxi escuro, Sherlock Holmes observava silenciosamente o lado de fora enquanto John Watson terminava de vestir suas luvas." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "O veiculo desacelera." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "À frente, uma enorme residência vitoriana cercada por grades negras emerge da neblina." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"Luzes policiais iluminam a fachada.\nDuas viaturas.\nUma ambulância."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"Assim que descem do carro, um policial os aguarda próximo à entrada principal."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"Ele estende a mão."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Policial ==="<<endl;
  cout<<"— Boa noite, Detetive Holmes e Doutor Watson, sou o inspetor William Carter."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Inspetor Carter ==="<<endl;
  cout<<"— O patriarca da família foi brutalmente assassinado há cerca de uma hora."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Inspetor Carter ==="<<endl;
  cout<<"— Não encontramos sinais de arrombamento. Nenhuma testemunha. Nenhuma pista concreta."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Inspetor Carter ==="<<endl;
  cout<<"— Mas quem fez isso… sabia exatamente o que estava fazendo."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout<<"=== Inspetor Carter ==="<<endl;
  cout<<"— Os familiares estão reunidos na sala de estar. Ninguém saiu da casa desde nossa chegada. O corpo está na cozinha."<<endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

}
void crimeScene(){
  cout << endl << "=== CENA DO CRIME -- COZINHA ===" << endl;

  cout << "Ao entrarem, o cheiro metálico do sangue domina imediatamente o ambiente." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "A cozinha luxuosa parece congelada no tempo." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "No centro da cozinha…" << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "o corpo." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Um homem de meia idade, elegante, caído ao lado da ilha central." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "A camisa social branca está completamente tingida de vermelho." << endl;
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
  cout << "— Meu Deus…" << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Sherlock permanece em silêncio. Observando. Calculando." << endl;
  cout << "Para prosseguir apere Enter" << endl;
  next();

  while(op!=0){
    cout << "Pistas:" << endl;
    cout << " 1. Pista A" << endl;
    cout << " 2. Pista B" << endl;
    cout << " 3. Pista C" << endl;
    cout << " 4. Pista D" << endl;
    cout << " 5. Pista E" << endl;
    cout << " 6. Pista F" << endl;
    cout << " 0. Se retirar" << endl;
    cin<<op;

    switch(op){
      case 1:
        
    }
  }
  
  
}

//---------------[Interrogatórios]---------------
void eleanorInterrogation(){
  cout << endl << "=== A ESPOSA -- ELEANOR WHITMORE ===" << endl;
  
  cout<<"=== Características ==="<<endl;
  cout << "Elegante." << endl;
  cout << "Olhos cansados." << endl;
  cout << "Maos tremulas." << endl;
  
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  op = -1;
  while(op!=0){
    cout << "Perguntas:" << endl;
    cout<<" 1. Como era sua relacao com seu marido?" << endl;
    cout <<" 2. A senhora suspeitava de traicao?" << endl;
    cout <<" 3. Onde estava durante o assassinato?" << endl;
    cout<<" 0. Se retirar"<<endl;
    cin>>op;

    switch(op){
      case 1:
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
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "Ela evita olhar diretamente para Holmes." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
      break;
    
      case 2:
        cout<<"=== ELEANOR WHITMORE ==="<<endl;    
        cout << "Ela demora alguns segundos para responder." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "-- Um pouco." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ELEANOR WHITMORE ==="<<endl;
        cout << "Ela projeta um olhar arrependido." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;
      
      case 3:
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
        break;

      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;

      default:
        cout<<"Opção inválida"<<endl;
        cout<<"Tente novamente"<<endl;
        break;
    }
  }
}
void arthurInterrogation(){
  cout << endl << "=== FILHO MAIS VELHO -- ARTHUR WHITMORE ===" << endl;

  cout<<"=== Características ==="<<endl;
  cout << "Controlado." << endl;
  cout << "Ambicioso." << endl;
  cout << "Frio." << endl;

  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  op = -1;
  while(op!=0){
    cout << "Perguntas:" << endl;
    cout<<" 1. Voce queria assumir a empresa?" << endl;
    cout<<" 2. Seu pai pretendia mudar o testamento?" << endl;
    cout<<" 3. Voces discutiram hoje?" << endl;
    cout<<" 0. Se retirar"<<endl;
    cin>>op;

    switch(op){
      case 1:
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
      
      case 2:
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

      case 3:
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

      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;
        break;

      default:
        cout<<"Opção inválida"<<endl;
        cout<<"Tente novamente"<<endl;
        break;
    }
  }
}
void edwardInterrogation(){
  cout << endl << "=== FILHO DO MEIO -- EDWARD WHITMORE ===" << endl;

  cout<<"=== Características ==="<<endl;
  cout << "Nervoso." << endl;
  cout << "Ansioso." << endl;
  cout << "Instavel." << endl;

  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  op = -1;
  while(op!=0){
    cout << "Perguntas:" << endl;
    cout<<" 1. Voce tinha problemas com seu pai?" << endl;
    cout<<" 2. Onde estava durante o assassinato?" << endl;
    cout<<" 3. Fazendo o que?" << endl;
    cout<<" 4. Voce ouviu alguma coisa suspeita?" << endl;
    cout<<" 0. Se retirar"<<endl;
    cin>>op;

    switch(op){
      case 1:
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Ele nunca me respeitou." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- Sempre comparava tudo o que eu fazia com Arthur." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "Os olhos dele demonstram ressentimento." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;
      
      case 2:
        cout<<"=== EDWARD WHITMORE ==="<<endl;
        cout << "-- No jardim." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 3:
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

      case 4:
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
        cout<<"Opção inválida"<<endl;
        cout<<"Tente novamente"<<endl;
        break;
    }
  }
}
void violetInterrogation(){
  cout << endl << "=== FILHA CACULA -- VIOLET WHITMORE ===" << endl;

  cout<<"=== Características ==="<<endl;
  cout << "Calma demais." << endl;

  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  op = -1;
  while(op!=0){
    cout << "Perguntas:" << endl;
    cout<<" 1. Voce gostava do seu pai?" << endl;
    cout<<" 2. Esta triste?" << endl;
    cout<<" 3. O que acha que aconteceu?" << endl;
    cout<<" 0. Se retirar"<<endl;
    cin>>op;

    switch(op){
      case 1:
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "Ela sorri discretamente." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Muito." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Nossa relacao era maravilhosa." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 2:
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "Ela encara Holmes em silencio." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- E como nao estaria?" << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "Sherlock observa atentamente." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "Ela fala sobre tristeza." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "Mas nao demonstra nenhuma." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 3:
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "-- Acho que alguem finalmente perdeu o medo dele." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== VIOLET WHITMORE ==="<<endl;
        cout << "A sala inteira permanece em silencio por alguns segundos." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;
        break;
      
      default:
        cout<<"Opção inválida"<<endl;
        cout<<"Tente novamente"<<endl;
        break;
    }
  }
}
void alfredInterrogation(){
  cout << endl << "=== MORDOMO -- ALFRED ===" << endl;

  cout<<"=== Características ==="<<endl;
  cout << "Velho." << endl;
  cout << "Exausto." << endl;
  cout << "Amargurado." << endl;

  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  op = -1;
  while(op!=0){
    cout << "Perguntas:" << endl;
    cout<<" 1. Como Raymond o tratava?" << endl;
    cout<<" 2. O senhor ouviu algo?" << endl;
    cout<<" 3. Reconheceu os passos?" << endl;
    cout<<" 0. Se retirar"<<endl;
    cin>>op;

    switch(op){
      case 1:
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Como mobilia." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Eu servia a esta familia ha mais de vinte anos." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 2:
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Passos rapidos no corredor." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 3:
        cout<<"=== ALFRED ==="<<endl;
        cout << "Ele hesita." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ALFRED ==="<<endl;
        cout << "-- Leves demais para serem de um homem." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        cout<<"=== ALFRED ==="<<endl;
        cout << "Holmes permanece em silencio." << endl;
        cout << "\nPara prosseguir aperte Enter..." << endl;
        next();
        break;

      case 0:
        cout<<"=== Sherlock Holmes ==="<<endl;
        cout<<"-- Tudo bem."<<endl;
        break;
      
      default:
        cout<<"Opção inválida"<<endl;
        cout<<"Tente novamente"<<endl;
        break;
    }
  }
}
void interrogationScene(){
  cout << endl << "=== INTERROGATORIO ===" << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();
  
  op = -1;
  while(op != 0){
    cout<<"Suspeitos: "<<endl;
    cout<<" 1. Eleanor - Esposa"<<endl;
    cout<<" 2. Arthur - Filho mais velho"<<endl;
    cout<<" 3. Edward - Filho do meio"<<endl;
    cout<<" 4. Violet - Filha"<<endl;
    cout<<" 5. Alfred - Mordomo"<<endl;
    cout<<"Opção: ";
    cin>> op;

    switch(op){
      case 1:
        eleanorInterrogation();
        break;
      case 2:
        arthurInterrogation();
        break;
      case 3:
        edwardInterrogation();
        break;
      case 4:
        violetInterrogation();
        break;
      case 5:
        alfredInterrogation();
        break;
      case 0:
        cout<<"Saindo"<<endl;
        break;
      default:
          cout<<"Opção inválida"<<endl;
          cout<<"Tente novamente"<<endl;
          break;
    }
  }
}


//---------------[Final]---------------
void bestEnding(){
  cout << "Sherlock reunia todos." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "E explica:" << endl;
  cout << "- as facadas eram exageradas demais;" << endl;
  cout << "- a cena foi manipulada;" << endl;
  cout << "- o assassino era menor;" << endl;
  cout << "- alguém lavou as maos;" << endl;
  cout << "- passos leves;" << endl;
  cout << "- ausencia de emocao genuina." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Entao Holmes encara Violet." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "E ela..." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "sorri." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Não porque foi pega." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << "Mas porque:" << endl;
  cout << "finalmente encontrou alguém capaz de entendê-la." << endl;
  cout << "\nPara prosseguir aperte Enter..." << endl;
  next();

  cout << endl << "===== FIM =====" << endl;
};

//---------------[Menus Acusações]---------------
void accusationMenu(){ //Cena de acusação
  int opt = 0;

  cout << "\nQuem e o assassino?\n";
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
void kitchenMenu(TabelaHash &table, Grafo &graph,Clue inventory[], int &totalClues, int &realCluesSolved){ //Menu da cena da cozinha
  int opt = 0;
  
  do{
    cout<<"[COZINHA]\n"<< endl;
    
    cout << "1 - Examinar faca\n2 - Examinar corte\n3 - Examinar cabo da faca\n4 - Examinar corpo da vitma\n5 - Examinar vestigios na porta da cozinha\n6 - Falar com empregados\n7 - Examinar lixeira\n8 - Ver pistas\n9 - Acusacao\n0 - Encerrar Investigacao" << endl;
    cin >> opt;
    cin.ignore();
      
    switch(opt){
      case 1:
        investigateClue(table, graph, "P1",inventory, totalClues, realCluesSolved);
        break;
          
      case 2:
        investigateClue(table, graph, "P2",inventory, totalClues, realCluesSolved);
        break;
          
      case 3:
        investigateClue(table, graph, "P3",inventory, totalClues, realCluesSolved);
        break;
          
      case 4:
        investigateClue(table, graph, "P4",inventory, totalClues, realCluesSolved);
        break;

      case 5:
        investigateClue(table, graph, "P5",inventory, totalClues, realCluesSolved);
        break;
          
      case 6:
        investigateClue(table, graph, "P6",inventory, totalClues, realCluesSolved);
        break;
          
      case 7:
        investigateClue(table, graph, "P7",inventory, totalClues, realCluesSolved);
        break;
      
      case 8:
        showInventory(inventory, totalClues);
        break;
          
      case 9:
        if(realCluesSolved >= 3){
          accusationMenu();
        }else{
          cout << "\nHolmes ainda nao possui evidencias suficientes.\n";
        }
        break;
          
      default:
        cout << "Investigação encerrada." << endl;
    }
  } while(opt != 0);
}

//---------------[Criações]---------------
void creatClues(TabelaHash &table){
  //----------[Pista 1]----------
  Clue knife;
  knife.idClue = 1;
  knife.title = "Faca";
  knife.description = "A lâmina ainda possui sangue. Porém, quase não existem impressões digitais. Pequenas marcas de tecido podem ser vistas próximas ao cabo.";
  
  knife.question = "O que a ausencia parcial de digitais indica?";
  knife.options[0] = "A arma foi trocada";
  knife.options[1] = "O assassino tentou limpar a faca";
  knife.options[2] = "A vitima segurou a faca";
  knife.options[3] = "O sangue foi plantado";

  knife.correctReponse = 2;
  knife.realClue = true;
  table.input("P1", knife);
  
  
  
  //----------[Pista 2]----------
  Clue positionKnife;
  positionKnife.idClue = 2;
  positionKnife.title = "Posicao da Faca";
  positionKnife.description = "O golpe entrou em um ângulo incomum...a perfuração veio de baixo para cima. A vítima aparentemente estava em pé.";
  positionKnife.question = "O que voce conclui sobre quem realizou esse golpe?";
  
  positionKnife.options[0] = "O assassino era mais alto";
  positionKnife.options[1] = "O assassino atacou a distancia";
  positionKnife.options[2] = "O assassino era mais baixo";
  positionKnife.options[3] = "Existiam dois assassinos";

  positionKnife.correctReponse = 3;
  positionKnife.realClue = true;
  table.input("P2", positionKnife);
  
  
 
  //----------[Pista 3]----------
  Clue knifeHandle;
  knifeHandle.idClue = 3;
  knifeHandle.title = "Cabo com microfissuras";
  knifeHandle.description = "A forca aplicada na faca indica uma adrenalina alta, a pessoa estava em extrema tensao.";
  knifeHandle.question = "Que tipo de estado emocional o assassino demonstrava?";
  knifeHandle.options[0] = "Raiva";
  knifeHandle.options[1] = "Ansiedade";
  knifeHandle.options[2] = "Nervosismo";
  knifeHandle.options[3] = "Medo";

  knifeHandle.correctReponse = 3;
  knifeHandle.realClue = true;
  table.input("P3", knifeHandle);
  
  
  
  //----------[Pista Falsa 1]----------
  Clue tissue;
  tissue.idClue = 4;
  tissue.title = "Lenço da esposa";
  tissue.description = "Uma espécie de lenço feminino, manchado de sangue, encontrado proximo ao corpo. Voce sente um cheiro de perfume quando aproxima ele em seu rosto.";
  tissue.question = "A quem esse lenço pertence?";
  
  tissue.options[0] = "Violet";
  tissue.options[1] = "Eleanor";
  tissue.options[2] = "A empregada";
  tissue.options[3] = "Uma visitante";

  tissue.correctReponse = 2;
  table.input("P4", tissue);
  
  
  
  //----------[Pista Falsa 2]----------
  Clue cigarette;
  cigarette.idClue = 5;
  cigarette.title = "Cigarros do Mordomo";
  cigarette.description = "Cinzas de cigarro foram encontradas próximas à entrada da cozinha. A bituca de cigarro ainda esta morna";
  cigarette.question = "Quem possui o hábito de fumar?";
  
  cigarette.options[0] = "Arthur";
  cigarette.options[1] = "Edward";
  cigarette.options[2] = "Alfred";
  cigarette.options[3] = "William Carter";

  cigarette.correctReponse = 3;
  table.input("P5", cigarette);
 
 
 
  //----------[Pista Falsa 3]----------
  Clue discussion;
  discussion.idClue = 6;
  discussion.title = "Discussão recente do filho mais velho";
  discussion.description = "Empregados relataram uma discussão intensa horas antes do assassinato. O assunto envolvia herança e controle da empresa da família.";
  discussion.question = "Quem discutiu com a vítima?";
  
  discussion.options[0] = "Eleanor";
  discussion.options[1] = "Alfred";
  discussion.options[2] = "Edward";
  discussion.options[3] = "Arthur";

  discussion.correctReponse = 4;
  table.input("P6", discussion);
 
 
 
  //----------[Pista Falsa 4]----------
  Clue medication;
  medication.idClue = 7;
  medication.title = "Remédio antdepressivo";
  medication.description = "Um remedio antidepressivo foi encontrado no lixo. Mas o nome do paciente estava parcialmente rasgado";
  medication.question = "Qual suspeito demonstra uma instabilidade emocional?";
  
  medication.options[0] = "Arthur";
  medication.options[1] = "Violet";
  medication.options[2] = "Edward";
  medication.options[3] = "Alfred";

  medication.correctReponse = 3;
  table.input("P7", medication);
}
void createConnectionsClue(Grafo &graph){
  graph.addNo("P1", 1);
  graph.addNo("P2", 1);
  graph.addNo("P3", 1);

  graph.addNo("P4", 2);
  graph.addNo("P5", 2);
  graph.addNo("P6", 2);
  graph.addNo("P7", 2);

  //Conexões entre pistas que existem
  graph.addConnections(0,1);
  graph.addConnections(1,2);

  //Conexões falsas
  graph.addConnections(4,6);
}

//---------------[Main]---------------
int main() {
  TabelaHash table;

  Grafo gInvestigation;

  Clue inventory[20];
  int totalClues = 0;

  int realCluesSolved = 0;

  creatClues(table);
  createConnectionsClue(gInvestigation);

  introStory();
  crimeScene();
  interrogationScene();

  kitchenMenu(table, gInvestigation, inventory, totalClues, realCluesSolved);

  return 0;
}
