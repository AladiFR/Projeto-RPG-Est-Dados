# Documentação do Projeto RPG - Investigação Policial

## Visão Geral

Este projeto é um jogo de investigação policial em C++ que utiliza estruturas de dados como **Tabela Hash** e **Grafo** para gerenciar pistas e suas conexões.

---

# Estruturas de Dados (Structs)

## Clue

Estrutura responsável por representar uma pista investigativa.

| Campo          | Tipo      | Descrição                                   |
| -------------- | --------- | ------------------------------------------- |
| idClue         | int       | Identificador único da pista                |
| title          | string    | Nome/título da pista                        |
| description    | string    | Descrição da evidência                      |
| question       | string    | Pergunta investigativa relacionada à pista  |
| options        | string[4] | Alternativas de resposta                    |
| correctReponse | int       | Índice da resposta correta                  |
| resolvida      | bool      | Indica se a pista foi resolvida             |
| descoberta     | bool      | Indica se a pista já foi descoberta         |
| realClue       | bool      | Define se a pista é uma evidência principal |

---

## No (TabelaHash)

Estrutura utilizada internamente pela tabela hash.

| Campo | Tipo   | Descrição                       |
| ----- | ------ | ------------------------------- |
| key   | string | Chave de identificação da pista |
| value | Clue   | Objeto da pista armazenada      |
| next  | No*    | Ponteiro para o próximo nó      |

---

## Suspects

Estrutura utilizada para representar suspeitos.

| Campo        | Tipo   | Descrição                       |
| ------------ | ------ | ------------------------------- |
| name         | string | Nome do suspeito                |
| relatedClues | No*[]  | Pistas relacionadas ao suspeito |
| guilty       | bool   | Indica se o suspeito é culpado  |

---

## No (Grafo)

Estrutura responsável pelos nós do grafo investigativo.

| Campo          | Tipo   | Descrição                  |
| -------------- | ------ | -------------------------- |
| idClue         | string | Identificador da pista     |
| nivel          | int    | Nível hierárquico da pista |
| connection     | No*[]  | Conexões com outras pistas |
| numConnections | int    | Quantidade de conexões     |

---

# Classes

## TabelaHash

Classe responsável pelo armazenamento e gerenciamento das pistas.


---

### Métodos Privados

#### funcaoHash(string key)

* **Descrição:** Calcula o índice da tabela hash utilizando os valores ASCII da chave.
* **Parâmetros:** `key` — chave textual da pista.
* **Retorno:** inteiro representando a posição na tabela.

---

### Métodos Públicos

#### TabelaHash()

* **Descrição:** Inicializa todas as posições da tabela como nulas.
* **Parâmetros:** Nenhum.
* **Retorno:** Nenhum.

---

#### input(string key, Clue value)

* **Descrição:** Insere ou atualiza uma pista na tabela hash.
* **Parâmetros:**

  * `key` — chave textual da pista.
  * `value` — objeto Clue.
* **Retorno:** Nenhum.

---

#### search(string key)

* **Descrição:** Busca uma pista na tabela hash.
* **Parâmetros:** `key` — chave da pista.
* **Retorno:** Objeto Clue.
  Caso não exista, retorna uma pista com `idClue = -1`.

---

#### remover(string key)

* **Descrição:** Remove uma pista da tabela hash.
* **Parâmetros:** `key` — chave da pista.
* **Retorno:** Nenhum.

---

## Grafo

Classe responsável pela representação lógica das conexões entre pistas.

O grafo é utilizado para exibir a linha de raciocínio investigativa de Sherlock Holmes.

---

### Métodos Públicos

#### Grafo()

* **Descrição:** Inicializa o grafo e define os vértices como nulos.
* **Parâmetros:** Nenhum.
* **Retorno:** Nenhum.

---

#### addNo(string idClue, int nivel)

* **Descrição:** Cria e adiciona um novo nó ao grafo.
* **Parâmetros:**

  * `idClue` — identificador da pista.
  * `nivel` — nível investigativo da pista.
* **Retorno:** Ponteiro para o nó criado.

---

#### addConnections(int v1, int v2)

* **Descrição:** Cria uma conexão entre duas pistas.
* **Parâmetros:**

  * `v1` — índice do vértice de origem.
  * `v2` — índice do vértice de destino.
* **Retorno:** Nenhum.

---

#### showConnections(int v)

* **Descrição:** Exibe todas as conexões de uma pista.
* **Parâmetros:** `v` — índice do vértice.
* **Retorno:** Nenhum.

---

#### showReasoningLine(string clueKey)

* **Descrição:** Exibe visualmente a linha de raciocínio das pistas conectadas.
* **Parâmetros:** `clueKey` — identificador da pista.
* **Retorno:** Nenhum.

---

# Sistemas do Projeto

## Sistema de Investigação

Responsável por:

* Buscar pistas na tabela hash
* Registrar pistas descobertas
* Validar respostas do jogador
* Atualizar progresso da investigação
* Mostrar deduções de Sherlock Holmes

---

## Sistema de Inventário

O jogador possui um inventário de pistas encontradas durante a investigação.

### Funções

#### addInventory()

* Adiciona pistas ao inventário.

#### showInventory()

* Exibe todas as pistas coletadas.

---

## Sistema de Perguntas

Cada pista possui:

* descrição
* pergunta investigativa
* quatro alternativas
* uma resposta correta

A validação é feita pela função:

### checkAnswer(Clue &pista)

* **Descrição:** Verifica se o jogador respondeu corretamente.
* **Retorno:** `true` ou `false`.

---

## Sistema de Dedução

Após resolver pistas principais (`realClue == true`), o jogo:

* incrementa o progresso investigativo
* exibe conexões do grafo
* apresenta deduções narrativas de Holmes

---

# História e Narrativa

O jogo contém:

* introdução cinematográfica
* cena do crime
* interrogatórios individuais
* sistema de acusação final

---

## Personagens Interrogados

| Personagem       | Papel            |
| ---------------- | ---------------- |
| Eleanor Whitmore | Esposa           |
| Arthur Whitmore  | Filho mais velho |
| Edward Whitmore  | Filho do meio    |
| Violet Whitmore  | Filha caçula     |
| Alfred           | Mordomo          |

---

# Sistema de Acusação

O jogador só pode realizar uma acusação após resolver pelo menos:

* **3 pistas principais**

Caso a acusação correta seja feita:

* o final verdadeiro é exibido.

---

# Pistas Implementadas

## Pistas Principais

| Código | Título                 |
| ------ | ---------------------- |
| P1     | Faca                   |
| P2     | Posição da Faca        |
| P3     | Cabo com microfissuras |

---

## Pistas Secundárias/Falsas

| Código | Título                                |
| ------ | ------------------------------------- |
| P4     | Lenço da esposa                       |
| P5     | Cigarros do Mordomo                   |
| P6     | Discussão recente do filho mais velho |
| P7     | Remédio antidepressivo                |

---

# Fluxo do Jogo

```text
Início
  │
  ├─► Introdução da história
  │
  ├─► Cena do crime
  │
  ├─► Interrogatórios
  │
  ├─► Exploração da cozinha
  │       │
  │       ├─► Investigar pistas
  │       ├─► Responder perguntas
  │       ├─► Adicionar ao inventário
  │       └─► Resolver pistas principais
  │
  ├─► Sistema de dedução
  │
  └─► Acusação final
          │
          ├─► Acerto → Final verdadeiro
          │
          └─► Erro → Acusação inválida
```

---

# Constantes

| Constante    | Valor | Descrição                              |
| ------------ | ----- | -------------------------------------- |
| MAX_VERTICES | 5     | Quantidade máxima de vértices do grafo |
| MAX          | 10    | Tamanho da tabela hash                 |

---

# Funções Auxiliares

## next()

* **Descrição:** Cria pausas entre diálogos e cenas.

---

# Funções Narrativas

## IntroStory()

Responsável pela introdução narrativa do jogo.

---

## crimeScene()

Exibe a cena inicial do crime.

---

## interrogationScene()

Executa todos os interrogatórios.

---

# Final do Jogo

## bestEnding()

Exibe o final verdadeiro quando o jogador acusa corretamente Violet.

---

# Observações

* O projeto utiliza programação estruturada e orientação a objetos.
* O sistema de pistas utiliza tabela hash com encadeamento.
* O grafo representa relações investigativas entre evidências.
* O inventário armazena todas as pistas encontradas.
* Existem pistas reais e pistas falsas para aumentar a complexidade investigativa.
* O jogo possui narrativa interativa baseada em escolhas.
* A investigação é linear, porém permite exploração livre das pistas no menu principal.
