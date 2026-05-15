#include <iostream>
#include <string>
using namespace std;

const int MAX_VERTICES = 5;

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
  next();

  cout << "Londres -- 15 de Janeiro de 2010, 22:20" << endl;
  next();

  cout << "A chuva fina castigava as ruas de Londres..." << endl;
  next();

  cout << "Dentro de um taxi escuro..." << endl;
  next();

  cout << "O veiculo desacelera." << endl;
  next();

  cout << "A frente, uma enorme residencia vitoriana..." << endl;
  next();
}
void crimeScene(){
  cout << endl << "=== CENA DO CRIME -- COZINHA ===" << endl;
  next();

  cout << "Ao entrarem, o cheiro-metalico do sangue..." << endl;
  next();

  cout << "A cozinha luxuosa parece congelada no tempo." << endl;
  next();

  cout << "No centro da cozinha..." << endl;
  next();

  cout << "o corpo." << endl;
  next();
}

//---------------[Interrogatórios]---------------
void eleanorInterrogation(){
  cout << endl << "=== A ESPOSA -- ELEANOR WHITMORE ===" << endl;
  
  cout << "Elegante." << endl;
  cout << "Olhos cansados." << endl;
  cout << "Maos tremulas." << endl;
  
  next();
  
  cout << "Pergunta: Como era sua relacao com seu marido?" << endl;
  
  cout << "-- Distante... ultimamente muito distante." << endl;
  cout << "-- Ele nao ficava em casa." << endl;
  cout << "-- Vivia voltando tarde do trabalho." << endl;
  
  next();
  
  cout << "Ela evita olhar diretamente para Holmes." << endl;
  
  next();
  
  cout << "Pergunta: A senhora suspeitava de traicao?" << endl;
  
  cout << "Ela demora alguns segundos para responder." << endl;
  
  cout << "-- Um pouco." << endl;
  
  next();
  
  cout << "Ela projeta um olhar arrependido." << endl;
  
  next();
  
  cout << "Pergunta: Onde estava durante o assassinato?" << endl;
  
  cout << "-- No meu quarto." << endl;
  cout << "-- Estou enfrentando insonia recentemente." << endl;
  cout << "-- Tenho tomado remedios para dormir." << endl;
  
  next();
  
  cout << "Watson observa atentamente." << endl;
  
  next();
  
  cout << "Ela nao aparenta estar sob efeito de sedativos." << endl;
  
  next();
}
void arthurInterrogation(){
  cout << endl << "=== FILHO MAIS VELHO -- ARTHUR WHITMORE ===" << endl;

  cout << "Controlado." << endl;
  cout << "Ambicioso." << endl;
  cout << "Frio." << endl;

  next();

  cout << "Pergunta: Voce queria assumir a empresa?" << endl;

  cout << "Ele responde rapidamente." << endl;

  cout << "-- Alguem precisava assumir eventualmente!" << endl;
  cout << "-- Meu pai nao poderia continuar para sempre." << endl;

  next();

  cout << "Pergunta: Seu pai pretendia mudar o testamento?" << endl;

  cout << "Arthur hesita por um breve instante." << endl;

  cout << "-- Nao." << endl;
  cout << "-- Nao que eu saiba." << endl;

  next();

  cout << "Ele encara Eleanor por alguns segundos." << endl;

  next();

  cout << "Ela desvia o olhar." << endl;

  next();

  cout << "Pergunta: Voces discutiram hoje?" << endl;

  cout << "-- Sim." << endl;
  cout << "-- Mas nao foi nada fora do normal." << endl;

  next();

  cout << "Ele fecha os punhos." << endl;

  cout << "-- Se acha que eu mataria meu proprio pai..." << endl;
  cout << "-- entao esta completamente maluco." << endl;

  next();
}
void edwardInterrogation(){
  cout << endl << "=== FILHO DO MEIO -- EDWARD WHITMORE ===" << endl;

  cout << "Nervoso." << endl;
  cout << "Ansioso." << endl;
  cout << "Instavel." << endl;

  next();

  cout << "Pergunta: Voce tinha problemas com seu pai?" << endl;

  cout << "-- Ele nunca me respeitou." << endl;
  cout << "-- Sempre comparava tudo o que eu fazia com Arthur." << endl;

  next();

  cout << "Os olhos dele demonstram ressentimento." << endl;

  next();

  cout << "Pergunta: Onde estava durante o assassinato?" << endl;

  cout << "-- No jardim." << endl;

  next();

  cout << "Pergunta: Fazendo o que?" << endl;

  cout << "-- Pensando." << endl;
  cout << "-- Eu ouvi a discussao..." << endl;
  cout << "-- Nao queria participar daquilo." << endl;

  next();

  cout << "Holmes percebe cheiro de cigarro em sua roupa." << endl;

  next();

  cout << "Pergunta: Voce ouviu alguma coisa suspeita?" << endl;

  cout << "-- Passos." << endl;
  cout << "-- Alguem andando rapido pelo corredor." << endl;
  cout << "-- Mas nao consegui ver quem era." << endl;

  next();
}
void violetInterrogation(){
  cout << endl << "=== FILHA CACULA -- VIOLET WHITMORE ===" << endl;

  cout << "Calma demais." << endl;
  cout << "Esse e o problema." << endl;

  next();

  cout << "Pergunta: Voce gostava do seu pai?" << endl;

  cout << "Ela sorri discretamente." << endl;

  cout << "-- Muito." << endl;
  cout << "-- Nossa relacao era maravilhosa." << endl;

  next();

  cout << "Pergunta: Esta triste?" << endl;

  cout << "Ela encara Holmes em silencio." << endl;

  cout << "-- E como nao estaria?" << endl;

  next();

  cout << "Sherlock observa atentamente." << endl;

  next();

  cout << "Ela fala sobre tristeza." << endl;
  cout << "Mas nao demonstra nenhuma." << endl;

  next();

  cout << "Pergunta: O que acha que aconteceu?" << endl;

  cout << "-- Acho que alguem finalmente perdeu o medo dele." << endl;

  next();

  cout << "A sala inteira permanece em silencio por alguns segundos." << endl;

  next();
}
void alfredInterrogation(){
  cout << endl << "=== MORDOMO -- ALFRED ===" << endl;

  cout << "Velho." << endl;
  cout << "Exausto." << endl;
  cout << "Amargurado." << endl;

  next();

  cout << "Pergunta: Como Raymond o tratava?" << endl;

  cout << "-- Como mobilia." << endl;
  cout << "-- Eu servia a esta familia ha mais de vinte anos." << endl;

  next();

  cout << "Pergunta: O senhor ouviu algo?" << endl;

  cout << "-- Passos rapidos no corredor." << endl;

  next();

  cout << "Pergunta: Reconheceu os passos?" << endl;

  cout << "Ele hesita." << endl;

  cout << "-- Leves demais para serem de um homem." << endl;

  next();

  cout << "Holmes permanece em silencio." << endl;

  next();
}
void interrogationScene(){
  cout << endl << "=== SALA DE INTERROGATORIO ===" << endl;

  next();

  eleanorInterrogation();

  arthurInterrogation();

  edwardInterrogation();

  violetInterrogation();

  alfredInterrogation();
}


//---------------[Final]---------------
void bestEnding(){
  cout << "Sherlock reunia todos." << endl;
  next();

  cout << "E explica:" << endl;
  cout << "- as facadas eram exageradas demais;" << endl;
  cout << "- a cena foi manipulada;" << endl;
  cout << "- o assassino era menor;" << endl;
  cout << "- alguém lavou as maos;" << endl;
  cout << "- passos leves;" << endl;
  cout << "- ausencia de emocao genuina." << endl;
  next();

  cout << "Entao Holmes encara Violet." << endl;
  next();

  cout << "E ela..." << endl;
  next();

  cout << "sorri." << endl;
  next();

  cout << "Não porque foi pega." << endl;
  next();

  cout << "Mas porque:" << endl;
  cout << "finalmente encontrou alguém capaz de entendê-la." << endl;
  next();

  cout << endl << "===== FIM =====" << endl;
};

//---------------[Menus Acusações]---------------
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
