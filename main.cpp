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





int main() {
  TabelaHash table;
  Clue c1;
  c1.idClue = 1;
  c1.description = "Uma faca ensanguentada na cozinha...";
  c1.question = "A faca pertence a quem?";
  c1.correctReponse = "Cozinheiro";
  
  
  cout << "===== INVESTIGACAO POLICIAL =====" << endl;
  cin >> continuar;

  cout << "Londres -- 15 de Janeiro de 2010, 22:20" << endl;
  cin >> continuar;

  cout << "A chuva fina castigava as ruas de Londres, a nevoa envolvia os postes de luz, transformando o brilho amarelado em manchas fantasmagoricas no asfalto molhado." << endl;
  cin >> continuar;

  cout << "Dentro de um taxi escuro, Sherlock Holmes observava silenciosamente o lado de fora enquanto John Watson terminava de vestir suas luvas." << endl;
  cin >> continuar;

  cout << "O veiculo desacelera." << endl;
  cin >> continuar;

  cout << "A frente, uma enorme residencia vitoriana cercada por grades negras emerge da neblina." << endl;
  cin >> continuar;

  cout << "Luzes policiais iluminam a fachada." << endl;
  cin >> continuar;

  cout << "Duas viaturas." << endl;
  cout << "Uma ambulancia." << endl;
  cin >> continuar;

  cout << "Assim que descem do carro, dois policias os aguardam proximos a entrada principal." << endl;
  cin >> continuar;

  cout << "Um deles, alto, cansado e claramente pressionado, aproxima-se rapidamente." << endl;
  cin >> continuar;

  cout<<"== Policial 1 =="<<endl;
  cout << "-- Detetive Holmes... graças a Deus chegou." << endl;
  cin >> continuar;

  cout << "Ele estende a mao." << endl;
  cin >> continuar;

  cout<<"== Inspetor Carter =="<<endl;
  cout << "-- Inspetor William Carter." << endl;
  cin >> continuar;

  cout << "Ao lado dele, um homem mais jovem mantem expressao seria." << endl;
  cin >> continuar;

  cout<<"== Oficial Miller =="<<endl;
  cout << "-- Oficial David Miller." << endl;
  cin >> continuar;

  cout << "William respira fundo antes de continuar." << endl;
  cin >> continuar;

  cout<<"== Inspetor Carter =="<<endl;
  cout << "-- O patriarca da familia, Raymond Whitmore, foi brutalmente assassinado ha cerca de uma hora." << endl;
  cin >> continuar;

  cout<<"== Inspetor Carter =="<<endl;
  cout << "-- Nao encontramos sinais de arrombamento. Nenhuma testemunha. Nenhuma pista concreta." << endl;
  cin >> continuar;

  cout << "William encara Holmes." << endl;
  cin >> continuar;

  cout<<"== Inspetor Carter =="<<endl;
  cout << "-- Mas quem fez isso... sabia exatamente o que estava fazendo." << endl;
  cin >> continuar;

  cout << "David abre a porta da residencia." << endl;
  cin >> continuar;

  cout<<"== Oficial Miller =="<<endl;
  cout << "-- Os familiares estao reunionados na sala de estar. Ninguem saiu da casa desde nossa chegada." << endl;
  cin >> continuar;

  cout<<"== Oficial Miller =="<<endl;
  cout << "-- O corpo esta na cozinha." << endl;
  cin >> continuar;

  cout << "Sherlock apenas ajeita o casaco." << endl;
  cin >> continuar;

  cout<<"== Sherlock =="<<endl;
  cout << "-- Naturalmente esta." << endl;
  cin >> continuar;

  cout << endl << "=== CENA DO CRIME -- COZINHA ===" << endl;
  cin >> continuar;

  cout << "Ao entrarem, o cheiro-metalico do sangue domina imediatamente o ambiente." << endl;
  cin >> continuar;

  cout << "A cozinha luxuosa parece congelada no tempo." << endl;
  cin >> continuar;

  cout << "Panelas ainda estao sobre o fogao." << endl;
  cout << "Uma taça de vinho cahida lentamente pinga sobre o marmore branco." << endl;
  cout << "O relogio na parede continua funcionando." << endl;
  cin >> continuar;

  cout << "Tick." << endl;
  cout << "Tick." << endl;
  cout << "Tick." << endl;
  cin >> continuar;

  cout << "No centro da cozinha..." << endl;
  cin >> continuar;

  cout << "o corpo." << endl;
  cin >> continuar;

  cout << "Um homem de meia idade, elegante, cahido ao lado da ilha central." << endl;
  cin >> continuar;

  cout << "A camisa social branca esta completamente tingida de vermelho." << endl;
  cin >> continuar;

  cout << "Uma faca de cozinha permanece cravada em seu peito." << endl;
  cin >> continuar;

  cout << "Ao redor do corpo:" << endl;
  cout << "sangue." << endl;
  cout << "Pegadas borradas." << endl;
  cout << "Marcas de luta." << endl;
  cin >> continuar;

  cout << "Watson se aproxima lentamente." << endl;
  cin >> continuar;

  cout << "-- Meu Deus..." << endl;
  cin >> continuar;

  cout << "Sherlock permanece em silencio. Observando. Calculando." << endl;
  cin >> continuar;

  cout << endl << "=== PISTAS REAIS -- FACA DO CRIME ===" << endl;
  cin >> continuar;

  cout << "1. A faca foi limpa parcialmente" << endl;
  cout << "Há sangue na lamina... mas quase nenhuma impressao digital." << endl;
  cout << "Isso indica:" << endl;
  cout << "- o assassino tentou limpar;" << endl;
  cout << "- porem esqueceu partes pequenas proximas ao cabo." << endl;
  cin >> continuar;

  cout << "2. A posicao da faca e estranha" << endl;
  cout << "A faca entrou em um angulo incomum." << endl;
  cout << "Isso sugere:" << endl;
  cout << "- o assassino era mais baixo;" << endl;
  cout << "OU" << endl;
  cout << "- golpeou enquanto a vtima estava sentada." << endl;
  cin >> continuar;

  cout << "3. Existem microfissuras no cabo" << endl;
  cout << "Como se alguém tivesse apertado a faca com forca excessiva." << endl;
  cout << "Indica:" << endl;
  cout << "- tensao extrema;" << endl;
  cout << "- adrenalina;" << endl;
  cout << "- obsessao emocional." << endl;
  cin >> continuar;

  cout << endl << "=== DETALHES IMPORTANTES DO AMBIENTE ===" << endl;
  cin >> continuar;

  cout << "1. Uma cadeira cahida" << endl;
  cout << "Mas distante demais da luta." << endl;
  cout << "Parece placementa propositalmente." << endl;
  cin >> continuar;

  cout << "2. Taca de vinho quebrada" << endl;
  cout << "Existe apenas UMA taca." << endl;
  cout << "O Homem estava sozinho antes do ataque." << endl;
  cin >> continuar;

  cout << "3. Pequenos pingos de sangue perto da pia" << endl;
  cout << "Como se alguém tivesse lavado rapidamente as maos." << endl;
  cin >> continuar;

  cout << endl << "=== 4 PISTAS FALSAS (RED HERRINGS) ===" << endl;
  cin >> continuar;

  cout << "1. Lenco da esposa" << endl;
  cout << "Encontrado proximo ao corpo." << endl;
  cin >> continuar;

  cout << "2. Discussao recente do filho mais velho" << endl;
  cout << "Empregados ouviram os dois brigando sobre a aposentadoria do homem, e o seu filho mais velho criticando a decisao de nao por seu nome como o proximo dono das empresas da familia." << endl;
  cin >> continuar;

  cout << "3. Marca de cigarro do mordomo" << endl;
  cout << "Bitucas encontradas perto da cozinha." << endl;
  cin >> continuar;

  cout << "4. Um remedies antidepressant do filho do meio" << endl;
  cout << "Encontrado no lixo." << endl;
  cin >> continuar;

  cout << endl << "=== SALA DE INTERROGATORIO ===" << endl;
  cin >> continuar;

  cout << "A Esposa -- Eleanor Whitmore" << endl;
  cout << "Elegante." << endl;
  cout << "Olhos cansados." << endl;
  cout << "Maos tremulas." << endl;
  cin >> continuar;

  cout << "Pergunta: Como era sua relacao com seu marido?" << endl;
  cout << "-- Distante... ultimamente muito distante. Ele nao ficava em casa. Vivia voltando tarde do trabalho." << endl;
  cin >> continuar;

  cout << "Ela evita olhar diretamente." << endl;
  cin >> continuar;

  cout << "Pergunta: A senhora suspeitava de traicao?" << endl;
  cout << "Ela demora para responder." << endl;
  cout << "-- Um pouco." << endl;
  cin >> continuar;

  cout << "Ela projeta um olhar um pouco arrependido." << endl;
  cin >> continuar;

  cout << "Pergunta: Onde estava durante o assassinato?" << endl;
  cout << "-- No meu quarto. Estou com um problema de insonia recentemente, entao estou tomando remedios para auxiliar a dormir." << endl;
  cin >> continuar;

  cout << "Mas Watson percebe:" << endl;
  cout << "ela nao parece estar sobre um efeito de sedativos." << endl;
  cin >> continuar;

  cout << endl << "Filho Mais Velho -- Arthur Whitmore" << endl;
  cout << "Controlado." << endl;
  cout << "Ambicioso." << endl;
  cout << "Frio." << endl;
  cin >> continuar;

  cout << "Pergunta: Voce queria assumir a empresa?" << endl;
  cout << "Ele fala em um tom arisco." << endl;
  cout << "-- Alguem precisava assumir eventualmente!" << endl;
  cout << "-- Eleiria ter que se aposentar e nao ha pessoa mais adequada para gerir a empresa do que eu." << endl;
  cin >> continuar;

  cout << "Pergunta: Seu pai pretendia mudar o testamento?" << endl;
  cout << "Ele trava por meio segundo." << endl;
  cout << "-- Nao!" << endl;
  cout << "-- Nao que eu saiba." << endl;
  cin >> continuar;

  cout << "Ele olha para todos ao seu redor. Mas em especifico, encara a mae com um olhar de indagem." << endl;
  cin >> continuar;

  cout << "A mae, nao responde." << endl;
  cin >> continuar;

  cout << "Pergunta: Vocês discutiram hoje?" << endl;
  cout << "-- Sim... Mas nao foi algo tao intenso!" << endl;
  cout << "-- Se acha que eu sou capaz de matar o MEU PAI, voce esta maluco!" << endl;
  cin >> continuar;

  cout << endl << "Filho do Meio -- Edward Whitmore" << endl;
  cout << "Nervoso." << endl;
  cout << "Ansioso." << endl;
  cout << "Instavel." << endl;
  cin >> continuar;

  cout << "Pergunta: Voce tinha problemas com seu pai?" << endl;
  cout << "-- Ele... nunca me respeitou." << endl;
  cout << "-- Vivia se gabando do Art, mas nunca falou nada de mim." << endl;
  cin >> continuar;

  cout << "Os olhos dele indicam um leve sentimento de vazio." << endl;
  cin >> continuar;

  cout << "Pergunta: Onde estava?" << endl;
  cout << "-- No jardim..." << endl;
  cin >> continuar;

  cout << "E oque estava fazendo?" << endl;
  cout << "-- Observando o ceu. Depois que eu comecei a ouvir a briga. Nao gosto disso." << endl;
  cin >> continuar;

  cout << "Mentira parcial." << endl;
  cout << "(Ele estava fumando)" << endl;
  cin >> continuar;

  cout << "Pergunta: Voce ouviu algo?" << endl;
  cout << "-- Enquanto eu subia para o meu quarto, escutei um leve barulho de passos. Mas nada alem disso" << endl;
  cin >> continuar;

  cout << "Os passos da irma." << endl;
  cin >> continuar;

  cout << endl << "Filha Cacula -- Violet Whitmore" << endl;
  cout << "Calma demais." << endl;
  cout << "Esse e o problema." << endl;
  cin >> continuar;

  cout << "Pergunta: Voce gostava do seu pai?" << endl;
  cout << "Ela sorri discretamente." << endl;
  cout << "-- Muito. Nossa relacao era maravilhosa." << endl;
  cin >> continuar;

  cout << "Pergunta: Esta triste?" << endl;
  cout << "Ela encara Holmes." << endl;
  cout << "-- E como nao estar mais triste?" << endl;
  cin >> continuar;

  cout << "Sherlock em seus pensamentos:" << endl;
  cout << "Ela diz que sente, mas nao apresenta." << endl;
  cin >> continuar;

  cout << "Pergunta: O que acha que aconteceu?" << endl;
  cout << "-- Acho que alguém finalmente perdeu o medo dele." << endl;
  cin >> continuar;

  cout << "Essa frase e MUITO importante." << endl;
  cin >> continuar;

  cout << endl << "Mordomo -- Alfred" << endl;
  cout << "Velho." << endl;
  cout << "Exausto." << endl;
  cout << "Amargurado." << endl;
  cin >> continuar;

  cout << "Pergunta: Como Raymond o tratava?" << endl;
  cout << "-- Como mobilia." << endl;
  cin >> continuar;

  cout << "Pergunta: Ouviu algo?" << endl;
  cout << "-- Passos rapidos no corredor." << endl;
  cin >> continuar;

  cout << "Pergunta: Reconheceu os passos?" << endl;
  cout << "Ele hesita." << endl;
  cout << "-- ...leves demais para serem de um homem." << endl;
  cin >> continuar;

  cout << "Pista da filha." << endl;
  cin >> continuar;

  cout << endl << "=== MELHOR TIPO DE FINAL ===" << endl;
  cin >> continuar;

  cout << "Sherlock reunia todos." << endl;
  cin >> continuar;

  cout << "E explica:" << endl;
  cout << "- as facadas eram exageradas demais;" << endl;
  cout << "- a cena foi manipulada;" << endl;
  cout << "- o assassino era menor;" << endl;
  cout << "- alguém lavou as maos;" << endl;
  cout << "- passos leves;" << endl;
  cout << "- ausencia de emocao genuina." << endl;
  cin >> continuar;

  cout << "Entao Holmes encara Violet." << endl;
  cin >> continuar;

  cout << "E ela..." << endl;
  cin >> continuar;

  cout << "sorri." << endl;
  cin >> continuar;

  cout << "Não porque foi pega." << endl;
  cin >> continuar;

  cout << "Mas porque:" << endl;
  cout << "finalmente encontrou alguém capaz de entendê-la." << endl;
  cin >> continuar;

  cout << endl << "===== FIM =====" << endl;
}