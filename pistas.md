## TÓPICO 1: O INVENTÁRIO DE PISTAS (Mapeamento de Evidências)

Ao usar o comando `inspecionar`, o terminal deve exibir estas descrições detalhadas. Cada uma possui uma **assinatura única** que destrava opções secretas de diálogo.

A. Evidências da Faca

- `pista_faca_limpa`: A faca foi limpa parcialmente. Há sangue na lâmina, mas quase nenhuma impressão digital no corpo metálico; o assassino tentou limpar, mas esqueceu ranhuras pequenas próximas ao cabo.
    
- `pista_faca_microfissuras`: Existem microfissuras na estrutura de polímero do cabo. Isso indica que quem a segurou aplicou uma força excessiva induzida por tensão extrema, adrenalina e forte obsessão emocional.
    
- `pista_faca_angulo`: A lâmina entrou em um ângulo descendente incomum. Isso significa que o assassino era significativamente mais baixo que o patriarca **OU** que ele golpeou a vítima por trás enquanto ela estava sentada.
    

B. Evidências do Ambiente

- `pista_cadeira_caida`: Uma cadeira caída distante do centro da cozinha. No chão, há marcas de arrasto rápido. A disposição faz parecer que foi colocada ou derrubada propositalmente para simular uma briga corporal.
    
- `pista_taca_vinho`: Uma única taça de vinho quebrada no chão. O líquido derramado indica que o homem estava sozinho apreciando a bebida antes do ataque.
    
- `pista_sangue_pia`: Pequenos pingos de sangue perto da pia. O padrão de diluição mostra que alguém lavou rapidamente as mãos ali logo após o ataque. Um cheiro do detergente permanece no local, um cheiro agradável de Lavanda.
    
- `pista_lenco_esposa`: Um lenço de seda fina com o monograma "E.W.", encontrado muito próximo à faca e parcialmente ensanguentado. Ao inspecionar a fundo, nota-se uma mancha escura de ferrugem de metal velho.


### C. Evidências Ocultas / Red Herrings (Pistas Falsas)

- `pista_cigarro_turco`: Uma bituca de cigarro encontrada perto da porta da cozinha. É de uma marca cara de tabaco turco com aroma adocicado de baunilha e filtro dourado.

- `pista_antidepressivo`: Um frasco de vidro âmbar de farmácia de manipulação jogado no lixo. O rótulo está rasgado, mas exibe o nome do paciente: _"Edwa..."_ e o medicamento é um forte antidepressivo.


---

## TÓPICO 2: ÁRVORE DE DIÁLOGOS E CONTRADIÇÕES (SQL Style no Terminal)

Todos os personagens possuem os dois nós obrigatórios: `perguntar_relacao` e `perguntar_corte` (onde estava). As opções de confrontamento só abrem se o jogador possuir a pista no inventário.

1. Eleanor Whitmore (A Esposa)

- **Pergunta Universal ("Onde estava?"):** "No meu quarto. Estou com problemas de insônia e tomo meus sedativos fortes."

- **Pergunta Universal ("Como era sua relação com seu marido?"):** 
	- *Eleanor*: "Distante... ultimamente muito distante. Ele mal parava em casa e vivia voltando tarde da noite do trabalho." 
	
	- *Análise de Sherlock*: "Ela evita olhar diretamente nos meus olhos, projetando uma postura defensiva que esconde um ressentimento profundo." 
	
	- *Sherlock*: "A senhora suspeitava de algo? Algum segredo profissional?"
	
	- *Eleanor* (Baixando o tom de voz): "Eu suspeitava... de traição. Uma intuição que me corroía há meses, detetive."

- **Nó de Confronto (`pista_lenco_esposa`):**
    
    - _Sherlock:_ "Seu lenço estava ensanguentado na cozinha. Mas por que ele tem marcas de ferrugem pesada?"
    
    - _Eleanor:_ "Ferrugem? Eu nunca toco em metal velho. Eu perdi esse lenço no jardim esta tarde... alguém deve ter pegado com as mãos sujas!"
    
- **Nó de Confronto (`pista_antidepressivo` e sabendo sobre o remédio do filho):**
	- *Análise de Sherlock:* "Ela apresenta uma agitação extrema nos pés; Espasmos musculares e uma forte sonolência incapacitante. Isso pode indicar uso de remédios antidepressivos."
    
	- _Sherlock:_ "Encontramos o frasco de manipulação do seu filho Edward no lixo da cozinha. Estava vazio, alguém andou consumindo ele, e não era o seu filho."
	
    - _Eleanor (Chorando):_ "Meus sedativos comuns não fazem mais efeito... eu fui até a cozinha roubar as pílulas do Edward mais cedo para conseguir dormir! Eu estava lá, mas Raymond estava vivo!"
	

- Conclusão de Sherlock (Notas de Dedução no Terminal):
	- Ponto de Conexão 1: Eleanor afirma que estava em seu quarto sob o efeito de sedativos fortes, mas a análise visual revelou sintomas clássicos de abuso de antidepressivos sem prescrição (agitação e espasmos). Ela admite ter ido à cozinha sorrateiramente para pegar o remédio de Edward. Pessoas que utilizam de remédios desse tipo sem prescrição podem sofrer de pensamentos intrusivos, impulsividade perigosa e irritabilidade extrema.
	- Ponto de Conexão 2: O lenço com o monograma dela ("E.W.") foi encontrado na cena do crime com sangue e ferrugem. Ela alega que o lenço sumiu à tarde no jardim, o que coincide com a linha do tempo de Edward, que admite ter usado um lenço achado no jardim para forçar o portão de ferro.
	- Ponto de Conexão 3: Ela afirma que suspeitava de uma traição vinda do marido, o que gera um forte componente emocional e motivo passional oculto para o crime.

---
2. Edward Whitmore (Filho do Meio)

- **Pergunta Universal ("Onde estava?"):** "No jardim, apenas observando o céu longe das brigas da casa."

- **Pergunta Universal ("Como era sua relação com seu pai?"):
	- Edward: "Ele... ele era um homem difícil. Nada do que eu fazia era bom o suficiente. Ele achava que minhas crises de ansiedade eram frescura ou falta de caráter."
	
	- Análise de Sherlock: "Edward morde o lábio inferior e evita o contato visual. O tremor leve em suas mãos ao tocar no nome do pai evidencia uma relação pautada pelo medo crônico, estresse psicológico e submissão forçada."

- **Nó de Confronto (`pista_antidepressivo`):**
	- *Sherlock:* "O senhor utiliza de remédios antidepressivo, senhor Edward?"
	
	- *Edward:* "Sim... Mas o frasco sumiu do meu quarto há dois dias. Achei que os empregados tivessem jogado fora."

- **Nó de Confronto (`pista_cigarro_turco`):**
    
    - _Sherlock:_ "Você diz que não entrou na casa, mas este raro cigarro turco de baunilha foi achado na cozinha. Você sabe de algo? Por acaso seu pai fumava?"
	
    - _Edward:_ "Não não! Meu pai nunca encostou um dedo em nada que envolvesse fumo! Ninguém daqui fuma!" -- Ele fala em um tom de leve desconforto.
      
    - _Edward:_ "Eu entrei rápido pelas portas dos fundos para pegar gelo! O portão de ferro do jardim estava emperrado, então acabei forçando ele com um lenço que estava ali perto para poder abrir. Mas meu pai não estava na cozinha! Eu ouvi passos leves de mulher no corredor e fugi de volta para o jardim." _(Valida a ferrugem do lenço e os passos da irmã)_.

- **Nó de Confronto (`pista_cigarro_turco` e diálogo com Alfred):**
	- *Análise de Sherlock:* "Watson nota que os dedos indicador e médio de Edward estão levemente amarelados (sinal de fumante compulsivo)."
	
	- _Sherlock:_ "Você diz que ninguém fuma nesta família, no entanto as marcas em seus dedos dizem o contrário, explique-se."
	
	-  *Edward (Engolindo em seco, escondendo as mãos nos bolsos do paletó):* "Eu... eu admito que fumo, mas faço isso escondido da minha mãe! Meu pai descobriu há algumas semanas e jurou que me cortaria da mesada se me pegasse com um maço de novo. É por isso que menti! Eu compro os cigarros turcos na cidade, mas na noite de hoje eu fumei apenas no jardim. Alguém deve ter pego uma das minhas bitucas no cinzeiro do lado de fora e levado para a cozinha para me incriminar!"

- *Conclusão de Sherlock (Notas de Dedução no Terminal):*
	- Ponto de Conexão 1: Edward afirma categoricamente que "ninguém fuma nesta família" e que passou a noite isolado no jardim. No entanto, a análise física revelou dedos amarelados de fumante compulsivo, e o mordomo Alfred confirmou que Edward é o único que compra os raros cigarros turcos importados.
	- Ponto de Conexão 2: Confrontado com a bituca de cigarro turco na cozinha, ele recua e admite ter mentido. Ele confessa ter entrado pelas portas dos fundos e usado o lenço de Eleanor para abrir o portão de ferro emperrado, o que explica a ferrugem encontrada no tecido na cena do crime.
	- Ponto de Conexão 3: Edward alega ter fugido antes do crime ao ouvir "passos leves de mulher" no corredor.

---
3. Alfred (O Mordomo)
- **Pergunta Universal ("Onde estava?"):** "Eu estava nos aposentos dos fundos da criadagem, organizando a prataria do jantar de amanhã. Não saí de lá até ouvir a correria dos policiais."

- **Pergunta Universal ("Como era sua relação com seu marido/patrão?"):** 
	- *Alfred:* "Eu o servia há quarenta anos, detetive. Mas para o Sr. Raymond, homens da minha classe são como mobília. Invisíveis." 
	
- **Diálogo Inicial (O jogador percebe o cheiro de fumo):** 
	- *Sherlock:* "O senhor possui um forte cheiro de fumo, fumou recentemente?" 
	
	- *Alfred:* "Eu fumo meu cachimbo velho no jardim, senhores. Não tenho dinheiro para os luxos que os jovens compram na cidade."

- **Nó de Confronto (`pista_cigarro_turco`):**      
    - _Sherlock:_ "Esta marca de cigarro, que estava na cozinha é sua?"
    
    - _Alfred:_ "Não, senhor! Nesta família ninguém fuma cigarros."
      
    - _Alfred:_ "Na verdade, somente o jovem Edward que compra esses maços importados da cidade."

- *Conclusão de Sherlock (Notas de Dedução no Terminal):*
	- Ponto de Conexão 1: Alfred não possui nenhuma pista física direta que o ligue à cena do crime na cozinha (como sangue ou impressões digitais), e o cheiro de fumo em suas roupas foi justificado pelo uso de seu cachimbo barato no jardim, e não pelos cigarros de luxo encontrados.
	- Ponto de Conexão 2: Ele demonstra um profundo ressentimento pelo patriarca ("me tratava como mobília") e foi a testemunha que ouviu: "passos rápidos e leves demais para um homem" no corredor, apontando indiretamente para uma das mulheres da casa.
	- Ponto de Conexão 3: Sendo o funcionário que cuida da manutenção da casa, ele conhece a rotina de todos e os segredos da família (como o vício de Edward e o roubo de remédios da mãe).

---
4. Arthur Whitmore (Filho Mais Velho)
- **Pergunta Universal ("Onde estava?"):** "Eu estava trancado no escritório do andar superior. Estava revisando os relatórios fiscais e os contratos das empresas da família, tentando ignorar a presença do meu pai."

- **Pergunta Universal ("Como era sua relação com seu pai?"):**
	- Arthur: "Era uma relação estritamente profissional, detetive. Meu pai construiu um império, mas nos últimos anos sua mente estava obsoleta. Ele se recusava a aceitar que a empresa precisava da minha visão moderna."
	- Análise de Sherlock: "Arthur mantém os braços cruzados, uma barreira física defensiva. Seu tom exala um desdém intelectual profundo, típico de quem considerava o pai um obstáculo para sua própria ambição."

- **Nó de Confronto (A pista da Discussão Recente sobre a empresa):**
    
    - _Análise de Sherlock:_ "Arthur aperta os punhos firmemente contra os lados do corpo; a veia em sua têmpora pulsa com uma raiva fria e controlada."
        
    - _Sherlock:_ "Os empregados relataram uma briga violenta entre você e seu pai hoje cedo. Você o confrontou sobre a aposentadoria dele e o fato de seu nome ter sido excluído da linha de sucessão da empresa, correto?"
        
    - _Arthur:_ "Sim, nós brigamos! Alguém precisa gerir este império e eu sou o único qualificado. Mas eu sou um homem de negócios, detetive, não um carniceiro. Eu pretendia tirá-lo do controle legalmente, forçando uma votação no conselho de administração, não com uma faca de cozinha!"
        
- **Nó de Confronto (`pista_faca_angulo` e `pista_cadeira_caida`):**
    
    - _Sherlock:_ "A cadeira da cozinha foi derrubada para simular uma luta física vigorosa de pé, mas o ângulo descendente da faca prova que a vítima foi golpeada pelas costas enquanto estava indefesa e sentada. Por que forjar um combate corporal, Arthur?"
        
    - _Arthur (Ficando pálido, mas firme):_ "Eu sou um homem alto e forte, detetive Holmes! Se eu quisesse matá-lo, não precisaria de encenações covardes ou de pegadas borradas no chão. Eu o enfrentaria de frente! Quem fez essa bagunça na cozinha claramente tinha medo do tamanho dele, ou precisava forjar uma luta para esconder a própria fraqueza física!"
    

- *Conclusão de Sherlock (Notas de Dedução no Terminal):*
	- Ponto de Conexão 1: Arthur admite ter tido uma briga severa com o pai sobre a linha de sucessão da empresa horas antes do crime. A análise visual revelou uma raiva fria e controlada, indicando que ele possuía o motivo financeiro mais forte entre todos os presentes na casa.
	- Ponto de Conexão 2: A cena do crime na cozinha foi montada de forma patética com uma cadeira caída distante do corpo para simular um combate corporal vigoroso. No entanto, Arthur diz que não teria a necessidade de forjar uma luta para mascarar fraqueza física.
	- Ponto de Conexão 3: O ângulo descendente da faca prova que a vítima foi golpeada pelas costas enquanto estava indefesa e sentada na cadeira. Arthur pode ter usado sua inteligência de negócios para planejar uma execução covarde pelas costas (encenando a briga depois).

---
5. Violet Whitmore (Filha Caçula)
- **Pergunta Universal ("Onde estava?"):** "No meu quarto de costura no fim do corredor, terminando o bordado de um vestido novo. Eu estava com os meus fones de ouvido e não escutei absolutamente nada até os gritos dos policiais."

- **Pergunta Universal ("Como era sua relação com seu pai?")**:
	- Violet: "Ah, detetive... eu era a caçula, a protegida dele. Ele me dava tudo o que eu pedia e tentava me manter longe das disputas corporativas que destruíam meus irmãos."
	  
	- Análise de Sherlock: "Violet responde com uma rapidez e um sorriso excessivamente simétrico. Suas palavras pintam um retrato de 'filha perfeita', mas seus olhos permanecem frios e distantes, com pouco vestígio real de luto ou calor afetivo."

- **Nó de Confronto (`pista_faca_microfissuras`):**
    
    - _Análise de Sherlock:_ "Violet mantém uma certa calmaria. Ela não demonstra muitos sinais de luto ou do desespero esperado para uma filha querida. Mas suas mãos estão tensas como se estivesse com uma certa adrenalina, talvez por medo ou talvez por ansiedade."
        
    - _Sherlock:_ "O cabo da faca possui microfissuras causadas por uma pressão manual absurda, fruto de uma descarga massiva de adrenalina. Você mencionou que 'alguém finalmente perdeu o medo dele'. Foi você quem perdeu o medo, Violet?"
        
    - _Violet (Com um sorriso discreto no canto dos lábios):_ "Meu pai era um tirano doméstico, detetive Holmes. Ele esmagava a individualidade de todos nesta casa. Arthur virou um ganancioso previsível, Edward um feixe de nervos ansioso e minha mãe uma dependente de remédios. É uma dor imensa que sinto, mas ele não era uma pessoa boa, acho que isso foi apenas o karma dele."
        
- **Nó de Confronto (`pista_taca_vinho`, `pista_faca_limpa` e `pista_sangue_pia`)**
	
	- _Análise de Sherlock_: "A postura de Violet permanece impecável, uma calmaria gélida que desafia a tragédia ao seu redor. No entanto, ao me aproximar, o cheiro de lavanda em suas mãos não consegue mascarar por completo o odor denso e fermentado de uva. Watson, com seu olhar cirúrgico, nota pequenas crostas arroxeadas sob as unhas polidas da jovem, além de uma sutil mancha escura na barra interna de sua manga de seda."
	    
	- *Sherlock:* "O cenário na cozinha nos diz que seu pai desfrutava de uma única taça de vinho, completamente sozinho, momentos antes do ataque. Curiosamente, vejo resquícios de vinho tinto sob suas unhas e na barra de seu vestido. Além disso, a pia onde o assassino limpou o sangue exala um perfume de lavanda idêntico ao que sinto vindo de você agora. Como explica essa proximidade com a cena, senhorita Violet?"
	    
	- *Violet (Sustentando o olhar de Holmes, com uma voz perfeitamente calma):* "O olhar de seu companheiro é astuto, detetive, mas a explicação é enfadonha. Eu desci à cozinha no início da noite, muito antes do ocorrido, para me servir de uma taça enquanto meu pai ainda estava em seu escritório. Sendo desajeitada, acabei derrubando o cristal no mármore. Usei um lenço que encontrei ali perto para tentar estancar a mancha no chão antes que os empregados vissem, e foi assim que me sujei. Uma tolice doméstica, nada mais."
	

- *Conclusão de Sherlock (Notas de Dedução no Terminal)*: 
		- Ponto de Conexão 1: Violet afirma ter derrubado a taça e limpado o local muito antes do crime. Contudo, a taça foi encontrada caída no centro da cena, vertendo sangue e vinho ainda frescos sobre o mármore, o que contradiz a linha do tempo dela. 
		- Ponto de Conexão 2: Ela alega ter usado o lenço da mãe apenas para limpar vinho. No entanto, o lenço foi encontrado ensanguentado e com marcas de ferrugem pesada — ferrugem esta que Edward afirma ter deixado no lenço ao forçar o portão de ferro do jardim.
		- Ponto de Conexão 3: O assassino usou o lenço para manipular a faca ou o portão sem deixar digitais, e o cheiro de lavanda nas mãos de Violet coincide perfeitamente com o odor residual encontrado na pia onde o sangue foi lavado. 