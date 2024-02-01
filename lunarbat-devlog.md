# Day 1
Hoje pela terceira vez começei a programar minha linguagem de programação
 do zero, mas dessa vez já tenho um pouco mais de experiência

Decidi fazer `C++` pelos motivos
- Não gosto de Rust
- `C` não é OOP
- `C++` é OOP

Hoje eu fiz a base do Tokenizer, acho que essa é a parte mais divertida de
 fazer uma linguagem de programação, sempre me divirto programando o tokenizer

Amanhã acho que vou fazer talvez algo pra fazer track da posição, erro handler
 e talvez começar o interpreter



# Day 2
Ontem eu comecei depois de escrever a entry
 eu comecei a fazer o track de posição e o error handler

Hoje só terminei de programar esses dois

Com isso eu percebi uma coisa, `C++` não é difícil, só tem um monte de features
 desnecessárias que ninguém pediu, por isso decidi fazer o que todo mundo faz
 que é: Usar `C++` do meu jeito invés de seguir um padrão

Acho que vou tentar usar `C++` literalmente como se fosse `C` com classes, porque
 é uma das únicas coisas que não gosto em `C`

Também o gerenciamento de memória dinâmica do `C++` acho que é beeem melhor
 só me confunde quando devo ou não usar, mas acho que entendi:
- **[1.]** Quando vou criar um Array/Lista/Vector etc de um tamanho que não da pra saber
 no tempo de compilamento
- **[2.]** Quando vou criar um objeto ou variável que vai ser usada fora do scope e que
 depois de alguma hora não vai ser mais necessária (Tempo de vida não determinado)
<!--
- **[3.]** Quando preciso armazenar uma variável ou elemento que vou precisar alterar o valor
 original alguma hora (Ex.: Passar *nodes* como referência, assim posso alterar o valor
 delas se eu precisar)
-->

A única coisa que me confunde é quando que eu devo usar um Objeto com `new` e
 quando não devo, ou se sempre devo, mas por enquanto só estou usando como no caso **2.**

E é bem chato toda vez que vou criar uma função eu ter que pensar se ela precisar
 ser `const` se eu preciso marcar os parâmetros como `const` e se ela precisa
 retornar uma `const` e depois pensar qual parâmetro pode ser passado como referência

Para solucionar isso eu estou marcando tudo como `const` e depois tirando conforme precisa
 (é bem mais fácil e eficaz), sobre os parâmetros como referência, eu acho que só precisa
 marcar se for uma lista, ou uma string (e óbvio, se eu pretendo mudar o valor)

Uma feature legal também é o **unique_ptr**, mas ainda não estou com muita segurança
 de usar essa feature, eu poderia usar para armazenar os *Tokens*, mas seria bem
 mais chato de manusear

Por enquanto eu sei exatamente quando vou deletar cada memória alocada, mas quando
 eu não sou ber eu vou usar **unique_ptr**

Então as princípais features de `C++` que vou usar são:
- Objetos
- Alocação dinâmica do `C++` (`new` e `delete`)
- **unique_ptr** (não inclui `shared pointers` por enquanto)


No mais, o que mais me preocupa é a eficácia, velocidade e memory management do meu código,
 principalmente porque memory management não é meu forte, então fico com muita
 paranóia se estou fazendo algo errado, se eu deveria usar uma feature em um lugar
 que não usei, se ficaria mais rápido se fizesse de outro jeito e etc

Mas de qualquer forma isso vai me dar mais experiência, se precisar eu re-escrevo
 pela quarta vez

O que acho que vai ser o caso, porque minha experiência em programar linguagens
 não vai tão longe assim

### Same day
Eu acabei terminando de fazer o Position, Error Handler e o **Lexer**

Amanhã vou começar a programar o **Parser**


# Day 3
Eu não fiz muito hoje, só comecei a fazer o **Parser** e fiz poucas mudanças

Eu estou em dúvida se crio uma Struct pra cada tipo de **node** ou se crio uma
 Class de Node, para padronizar

O que me deixa com essa dúvida é que a Node de NumericLiteral, armazena o valor
 como *double* e isso não seria possível usando Class (seria usando generics, mas
 acho que seria desnecessário pra um caso só)
 sendo assim eu teria que em algum momento quando fosse usar o valor das nodes
 ter que verificar se é do tipo NumericLiteral e converter para double


# Day 4
Hoje eu programei um pouco mais do **Parser**, uma coisa que deixei pra fazer hoje,
 mas hoje eu estava pensando um pouco sobre e decidi que vou re-fazer o código do zero

Eu não vou re-programar tudo de novo, só vou programar do jeito que está
 até o ponto de agora, porque eu não upei no github e seria meio estranho upar
 agora, então vou fazer do zero e ir upando

E também porque descobri um programa que me ajuda a identificar Memory Leak
 então vou usar ele conforme vou programando a linguagem pra me ajudar a
 identificar Memory Leaks

### Same day
Agora são 22:40 do dia 31/12/2023, eu não fiz o que disse que ia fazer hoje
 porque é ano novo e eu quis aproveitar esse último dia

Também fiquei com bastaante estresse de coisas pessoais que aconteceram,
 então vou tentar fazer tudo amanhã


# Day 5
Terminei de re-escrever tudo, foi bom até, pois resolvi alguns probleminhas que
 não tinha percebido antes e fiz algumas alterações

Também resolvi o memory leak que estava acontecendo com o outro código, foi
 meio difícil de descobrir o que era mas foi divertido de fazer

### O que aconteceu?
Eu tenho alguns tipos de **Node**, vamos focar nesses três:
```cpp
struct Statement {
	NodeType kind;
	
	Statement(const NodeType type) : kind(type) {}
	inline NodeType get_kind() {
		return this->kind;
	}
};

// Program represents a block of statements
struct Program : public Statement {
	std::vector<const Statement*> body;

	Program() : Statement(NodeType::Program) {}
	~Program()  {
		for(const Statement* stmt : this->body) {
			delete stmt;
		}
	}

	inline void insert(const Statement* stmt) {
		body.emplace_back(stmt);
	}
};

struct Expr : public Statement {}

struct BinaryExpr : public Expr {
	const Expr* left;
	const Expr* right;
	const std::string& operatorStr;

	BinaryExpr(...) {}
	
	~BinaryExpr() {
		delete left;
		delete right;
	}
};
```

>Obviamente isso é só um exemplo e o código não está totalmente correto

No meu código é retornado um **Program** que contém o **body** com todos os **Statements**.
 Nesse exemplo, era retornado um **BinaryExpr** "convertido" para **Statement** *(elementos do **body**)*
 e eu deletava esse **Statement** depois do uso, como era na verdade um **BinaryExpr**,
 eu achei que ao usar **delete** keyword, iria executar o *destructor* do **BinaryExpr**,
 porém, continuava tendo uma Memory Leak e eu não sabia o porque


### Como o Memory Leak funcionava?
Eu descobri que ao deletar o **BinaryExpr** "convertido" para **Statement** o *destructor* do **BinaryExpr**
 não é executado e sim o *destructor* do **Statement** *(no caso é implicito um `~Statement() = default;`)*,
 já que **BinaryExpr** está sendo tratado como **Statement** nesse caso

### Minha solução
Então o que eu fiz foi mudar o *destructor* do **Statement** para *virtual* e no **BinaryExpr**
 *override* o *destructor*

```cpp
struct Statement {
	NodeType kind;
	
	Statement(const NodeType type) : kind(type) {}
	virtual ~Statement() = default; // VIRTUAL DESTRUCTOR

	inline NodeType get_kind() {
		return this->kind;
	}
};

// CODE //

struct BinaryExpr : public Expr {
	const Expr* left;
	const Expr* right;
	const std::string& operatorStr;

	BinaryExpr(...) {}
	
	// OVERRIDE DESTRUCTOR //
	~BinaryExpr() override {
		delete left;
		delete right;
	}
};
```


### Mas por que isso funciona?
A keyword **virtual** significa que esse método pode ser "substituído" e **override**
 serve para marcar *explicitamente* que aquele método está sendo substituído
 *(seu uso não é totalmente necessário, porém fica melhor para entender o código e achar erros)*

**Virtual** também é usado para métodos em classes que ao derivar de outra, o método PRECISA ser implementado,
 isso é feito da seguinte forma: 

```cpp
class Base {
public:
	// Função virtual pura
	virtual void metodoAbstrato() = 0; // "= 0" - Virtual pura
};

class Derivada : public Base {
public:
	// Implementação OBRIGATÓRIA da função virtual pura
	void metodoAbstrato() override {
		std::cout << "Implementação do método abstrato na classe derivada" << std::endl;
	}
};

```

E caso você não saiba **default** significa que aquele método tem o comportamento padrão,
 nesse caso o de um destructor padrão *(deletar a si mesmo)*, usar o **default** é um jeito
 de deixar esse comportamento *explícito*

Então o que aconteceria era:
```
delete Statement -> Rodar ~Statement() = default;
```

Agora o que acontece é:
```
delete Statement -> ~Statement() é substituído por ~BinaryExpr() -> Rodar ~Statement() com instruções do ~BinaryExpr()
```

E isso resolveu o problema de Memory Leak

### Conclusão
Sinceramente não sei se essa é a melhor opção, mas é a que está funcionando e a
 mais simples eu acho, eu gostei dessa solução e foi até que divertido achar e resolver esse Memory Leak

Eu acho que nesse caso eu poderia ter usado *smart pointers*, mas iria ficar um
 código muito confuso e eu pessoalmente ainda prefiro *new* e *delete*

Com certeza nem todos os Memory Leaks vão ser fáceis de encontrar e resolver, mas esse foi

Só um adendo porque não falei até agora: Eu estou usando `valgrind` para saber se está ocorrendo algum Memory Leak


# Day 6
Vou tentar terminar de implementar um **Parser** básico,
 não sei se vou conseguir pois estou com problemas pessoais

### Same day
Eu terminei de implementar o **Parser** e está dando *quase* tudo certo
 eu só estou com dois problemas

- 3 (1 + 1) é validado
- Quando algum erro ocorre acontece um Memory Leak

O primeiro problema eu posso resolver depois, mas o segundo é algo que só fui
 perceber agora, por causa do jeito que Error Handler funciona, acontece Memory
 Leak quando um erro acontece, porém eu não sei muito bem como resolver isso

# Day 7
Hoje eu resolvi o problema de Memory Leak, eu tive que criar uma *struct* de **Result**
 invés da estratégia que eu estava usando de `try catch`

Eu ainda preciso melhorar essa *struct* e pensar numa lógica melhor, mas por enquanto está
 funcionando

Sobre `x (...)` ser válido, eu ainda não cheguei a resolver, mas não parecer ser
 difícil de resolver, eu posso deixar isso pra depois

Por enqunato vou focar na otmização do **Result**

### Same day
Foquei um pouco mais em otimização, amanhã vou avançar mais um pouco e começar
 o *Interpreter* *(e talvez terminar ainda amanhã)*

Eu percebi que estou passando uns 2 dias implementando uma feature nova e uns 3 dias
 otimizando essa feature, resolvendo Memory Leaks, otimização aonde eu acho que da pra
 ter e etc

Não sei se essa é uma boa estratégia mas parece estar funcionando

# Day 8
Não fiz muita coisa hoje, só uma otimização que tive ideia e resolver uns bugs,
 não me senti com muita vontade hoje, mas amanhã vou começar a *Interpreter*

# Day 9
Hoje iniciei e terminei o básico do *Interpreter*, foi bem mais fácil do que eu
 imaginei e com isso eu percebi que sempre quando vou implementar algo
 novo eu sigo praticamente os mesmos passos:

- Implementa o básico e arruma até funcionar
- Checa Memory Leaks e resolve todos
- "Implementação verdadeira" no arquivo principal
- Otmização

Ainda hoje vou tentar fazer o suporte para operator *unário* (número negativo)
 e implementar suporte para variáveis

Não sei se vou conseguir por problemas pessoais, mas espero pelo menos conseguir
 implementar um desses hoje

### Same day
Não fiz muitos avanços no *Interpreter*, mas adicionei suporte para *unary* e
 corrigi alguns bugs

Esse projeto está indo melhor que eu imaginei e estou ficando feliz com
 como está indo, estou sentindo que estou programando bem em `C++` pela primeira
 vez :)

# Day 10
Hoje comecei a implementar as variáveis, deu um probleminha, mas no final deu
 tudo certo e está funcionando

Fiquei meio com preguiça hoje e talvez eu continue de implementar amanhã,
 tem algumas coisas que preciso fazer como dar handle no erro que pode dar no *environment*
 e resolver um bug estranho que está acontecendo e o compiler não me diz o que é *(valeu C++)*

Amanhã então eu acho que consigo implemtnar *let* e *const* junto com esse handle
 que já sei como vou fazer, só não estou com muita vontade de fazer agora

### Same day
Eu não sei como explicar isso

Depois que escrevi isso eu fiquei pensando em um erro que estava dando
 *(`terminate called after throwing an instance of 'std::out_of_range' what():  array::at: __n (which is 1431655834) >= _Nm (which is 3)`)*
 e fui tentar resolver, acho que comecei às 19:00 talvez e agora são 23:00

Esse erro normalmente da no `utils` quando eu tendo acessar um elemento que não existe no array
 que uso para mostrar os *Enums* como strings

Eu fiquei muito tempo tentando entender porque que quando o hash-map do *environment*
 chegava no `utils` o tipo dele mudava pra um número aleatório, eu tentei de tudo
 fiquei muito tempo nisso

Eu achei que era um erro a ver com *ownership*, achei que de algum jeito quando eu
 criava outro ponteiro para a variável, a *ownership* saia do *environment*,
 *(sem sentido, eu sei, mas eu estava enlouquecendo)*

Até que decidi tentar de novo implementar o `stack-allocation` para armazenar as variáveis
 invés de só um hash-map, porque vai que

Eu tinha tentado antes, mas desisti porque parece que os dois rodam em velocidades parecidas

Enfim, eu implementei e com isso veio outra mensagem de erro, dizendo que um valor
 deletado foi acessado e eu descobri o erro, que era na verdade no `interpreter`

Depois de fazer o cálculo, os números usados pro cálculo são deletados, porque não
 vão ser mais usados e isso estava acontecendo com variáveis também e no `utils`
 quando tentava pegar o *tipo* do elemento, ele retornava um número aleatório,
 porque ele foi deletado

Puta que pariu, eu odeio o `gcc` e o `clang` as vezes, eu fiquei umas 2 horas enlouquecendo
 tentando descobrir porque esse erro estava acontecendo

Por que eu quando eu deleto um pointer dentro de uma lista/array/map ele não é
 tirado automaticamente????? EU FIQUEI HORAS TENTANDO DESCOBRIR ISSO, VAI SE FODER `C++`

Agora eu vou lidar com esse erro, agora que finalmente sei o que é, ou amanhã
 não sei, estou exausta

# Day 11
Meu deus do céu, hoje eu fiquei mais 2 horas tentado resolver esse erro, era o
 erro certo, mas no lugar errado, não estava acontecendo no destructor do *environment*,
 ele era causado pois ao retornar um *Identifier* como *RuntimeVal* ele retornava o pointer
 original no *symbolTable* *(onde ficam armazenadas as variáveis)* e isso causava o *double free error*,
 na hora de deletar o valor retornado pelo interpreter e os itens no *environment*,
 por que não a cada item deletado setar o valor para *nullptr* e assim checar se o valor do *interpreter* é
 *nullptr* antes de deletar, caso seja uma *Identifier*? Porque simlesmente não da pelo visto,
 **não sei** o que impossibilita isso ou o que eu fiz de errado, mas ao setar um *pointer*
 para *nullptr* em um contexto, no outro ele permanece com o valor original, agora,
 POR QUE O COMPILER NÃO SETA O POINTER PARA *NULLPTR* POR PADRÃO DEPOIS DE DELETAR ELE??

Mas até eu descobrir isso e pensar em como resolver demorou horas, pode me chamar de idiota,
 mas eu culpo o compiler

Minha solução foi fazer um método virtual que clona a própria struct, assim, ao retornar
 um *Identifier* vai retornar um pointer clone e não o original que está no *environment*.
 Eu fiz isso invés de converter para a classe certa do *Identifier*
 *(Ex.: Se Identifier é um Number, retornar um clone de Number)* porque simplesmente não consigo fazer isso,
 não da *(sem usar generics, mas eu não quero para o código não ficar confuso)*

O que mais me preocupa com essa solução é que talvez eu tenha dificuldades no futuro,
 quando eu quiser retirar uma variável do *environment* e  ter que deletar o *clone*
 e o *pointer original*. Não sei se foi a melhor situação *(provavelmente não)*,
 mas foi a que está funcionando or enquanto, talvez no futuro eu pense em outra solução

Ah sim, hoje também fiz melhorias na struct do *Response*, está bem mais funcional e menos confuso,
 mas agora só amanhã, estou exausta de novo

Enfim, não sei deu pra entender o erro, sinceramente estou com muita preguiça, cansaço e raiva
 para explicar com mais detalhes

### Same day
Eu estava fazendo uns testes e estou meio preocupada de ter algum Memory Leak no meu
 código e eu não perceber, quando rodo o `Valgrind` ele não aponta nenhum Leak, porém
 quando rodo o *sanitizer* do `clang`, ele aponta que no código em algum ponto está
 tentando acessar uma memória deletada, o que não faz sentido, porque isso normalmente
 da um erro no compilador ou no `Valgrind` e não tive nenhum dos dois erros

Porém, isso pode ser pelo fato do sanitizer não estar instalado direito
 *(não esto afim de instalar isso agora)*, espero que seja isso

Enfim, vou tentar ignorar isso, não quero que esse último erro me derrube e amanhã
 vou continuar adicionando mais features


# Day 12
Não fiz nada hoje porque minha namorada ficou animada com a ideia de fazer mod pro
 minecraft e eu fiquei animada junto e quis começar a fazer

Vou começar a separar meu tempo pra cada projeto

# Day 13
Hoje só fiz ser possível declarar variáveis novas, não deu muito trabalho e foi bem fácil,
 ainda não vou fazer minha linguagem ser `statically-types`, vou deixar pra fazer isso
 quando eu tiver mais coisas estabelecidas

# Day 14
Hoje vou implementar re-assigment das variáveis, eu estava pensando em amanhã começar a fazer
 a implementar a parte **statically-typed**, porque talvez fique muito difícil fazer isso
 depois que muitas coisas estiverem prontas, ainda mais que depois disso vou começar
 a fazer os primeiros objetos, então acho melhor preparar tudo agora *(amanhã no caso)*


### Same day
Eu implementei isso, fiquei bem feliz com o resultado, depois vou fazer *Type Safe*,
 alguma hora, mesmo que seja muito simples de fazer isso eu não estou com tanta vontade agora

Hoje também decidi finalmente escolher um nome para a linguagem: **Mushroom**

Motivos:
- Cogumelo
- Eu gosto
- Melhor que qualquer outra coisa que eu consegui pensar

# Day 15
Hoje tentei implementar os objetos, eu estou quase conseguindo, mas tem um Memory Leak
 que não sei porque está acontecendo, eu fiquei quase a manhã e tarde toda implementando
 e tentando resolver esse memory leak, mas não sei porque ele está acontecendo e
 sinceramente não estou mais com cabeça pra isso, vou deixar isso pra amanhã

Como fiz com as variáveis, no primeiro dia vou implementar, no segundo vou programar ser
 possível acessar e no terceiro implementar a parte **statically-typed**,
 isso depois de eu resolver o memory leak

Depois disso eu vou tirar um tempo para fazer algumas coisas do `todo.md`,
 já que acumulei muitas

Com isso, eu tive que voltar o jeito antigo que estava *parse* os tokens e com isso
 o bug de *3 (1 + 1)* ser válido voltou. Já que o parser interpreta como sendo um
 número *(`3`)* e uma expressão *(`(1 + 1)`)*. Eu posso resolver isso fazendo `;`
 ser obrigatório no final de cada *Statement*, mas eu acho que não quero isso,
 então vai ficar desse jeito por enquanto até eu pensar em algo melhor

# Day 16
O problema do dia 10 e 11 voltaram, eu estou muito cansada pra explicar em detalhes,
 mas basicamente com a adição dos objetos, é possível criar uma "varíável" dentro de um objeto,
 `{ key: 123 }` porém também é possível usar uma váriavel já existente `x = 100 -> { x } -> { x: 100 }`

O segundo caso eu demorei pra perceber mas consegui resolver, caso a variável exista, é só
 clonar pra dentro do `ObjectVal` e pronto, não vai ter double delete ou *SIGSEGV*
 *(talvez eu mude isso no futuro, mas por enquanto é assim que funciona)*

Com isso estaria tudo certo, se não fosse o mesmo problema de double delete na classe principal

**ObjectVal** é tratado como um valor de variável, porém ele também é retornado como *RuntimeVal*
 e o valor é deletado na classe principal e dentro do *environment*, caso seja uma variável *(esse caso)*

Eu tinha resolvido isso com um método de clonagem, onde o valor era um clone da váriavel no *environment*,
 mas esse caso é diferente, porque o **ObjectVal** primeiro é tratado como um valor de variável
 então eu preciso retornar o pointer principal, então na hora de retornar o valor, ele retorna esse pointer principal
 e se eu retornar para a variável um clone, esse clone não vai ser deletado nunca

Então acontece o problema de antes de double delete na classe principal, onde quando a variavel é deletada
 ela delta o valor, que no caso é um **ObjectVal** o destructor do **ObjectVal** deleta todos os valores dentro dele, o que é o certo,
 porém ele também é retornado como um valor para a classe principal, mas os valores
 dentro dele já foram deletados, causando um *SIGSEGV* que EU NÃO FAÇO IDEIA DE
 COMO RESOLVER E ESTOU ENLOUQUECENDO

Eu até tentei usar *shared_ptr* e *unique_ptr* mas não funcionou, não faço ideia do porque

Um jeito que descobri de como resolver isso é deletando a variável criada dentro do **ObjectVal**,
 assim não preciso rodar o destructor do **ObjectVal**, mas essa solução é horrível

Outro jeito de fazer isso é checando se o valor retornado pelo interpreter é um Objeto,
 se for, não deletar ele, mas essa opção também é horrível porque ele pode não ser
 necessariamente uma variável, só um valor, o que vai causar um memory leak

Eu sei que clonar os valores não é a solução mais ideal, mas foi o que resolvi fazer
 pra solucionar esse problema, mas agora eu preciso pensar em outra coisa, mas não sei
 no que

EU ESTOU ENLOQUECENDO SOCORRO


Eu também tentei fazer o valor retornado pelo interpreter ser um smart pointer,
 mas acontece a mesma coisa, eu teria que fazer tudo ser um smart pointer, mas eu
 não quero isso


# Day 16
Por causa de um problema pessoal eu fiquei mais ou menos 10 dias sem mexer nesse projeto,
 eu não vou contar esses dias como já fiz algumas vezes, pois não fiquei sem fazer por causa
 de outra coisa, ou porque não quis, foi porque eu estava bem mal pra fazer isso

Bom, eu decidi reescrever tudo do zero de novo, pois eu aprendi uns conceitos novos
 sobre gerenciamento de memória e também acho que seria bom verificar algumas coisas
 e melhorar outras

Esse processo vai ser bem rápido, deve demorar uns 5 dias se eu continuar mexendo diariamente,
 5 dias porque eu quero voltar aos poucos e não a todo vapor como eu estava antes

Eu mudei `vector` dos tokens para `queue` o que faz mais sentido, vi alguns erros
 de lógica e decidi que vou tentar implementar um **Lazy Parser**, pois o consumo de memória
 é bastante elevado, já que eu estou usando o sistema de *"Response"* e *Position*, e
 talvez o **Lazy Parser** ajude nisso, sacrificando um pouco mais do processador

O **Lazy Parser** só vai tokenizar o que for usado, invés de primeiro tokenizar tudo
 e depois criar as nodes, **Lazy Parser** faz tudo ao mesmo tempo e não parece complicado
 de implementar


# Day 18
Ontem eu não fiz nada porque descobri algo chocante. Eu estava ajustando umas coisas
 e percebi que o programa estava usando muita memória, muito mais que deveria *(uns 80kb)*

Resumindo a história, eu descobri que isso era culpa das *standard libraries* do `C++`,
 eu nunca tinha parado pra pensar nisso e fiquei em choque, com isso eu fiquei refletindo
 se eu deveria começar a usar `C` invés de `C++` ou então usar as *libraries* de `C`,
 porém, eu decidi continuar usando `C++`, parece uma troca justa pra ter as comodides
 que `C++` oferece, então da pra se dizer que meu programar está com `80kb` a mais do
 que deveria ter


Hoje eu implementei o **Parser**, com algumas poucas mudanças e ajustei pra que funcione
 como um **Lazy Parser**, o que acho que melhorou bastante a perfomance

Amanhã vou re-implementar o interpreter e é essa parte que estou com medo, porque foi onde
 eu parei por causa do erro difícil de resolver


### Same day
Eu descobri que vou ter que fazer uma estratégia parecida com o **Lazy Parser** no *Interpreter*,
 não vai ser tão difícil de fazer, só vai demorar um tempinho, mas vai ser bem melhor

Eu também terminei de implementaar o *Interpreter*, mas estou tendo um erro bem parecido com o
 da última vez, dessa vez só o **Valgrind** que está encontrando esse erro e acontece por um
 motivo muito parecido:

Se dentro de um *Object* é usado uma variável já existente, essa variável
 vai ser deletada dentro do Objeto e vai ser deletada depois do uso, causando um double delete.
 Acontece que eu já sabia desse problema e tentei de todas as formas clonar a variável pra dentro do
 **ObjectVal**, porém mantendo o pointer, mas o **Valgrind** em todas as minhas soluções sempre retorna
 um erro *(acho que erro de ownership)*, então minha solução foi armazenar cópias dos objetos principais
 *(sem o pointer)*, depois eu penso melhor em como resolver isso, por enquanto vou focar em
 aprimorar o *Interpreter* mais ainda

Ah sim, esqueci de mencionar, mas agora no *Environment* é armazenado *Referencias* dos valores e não
 o pointer, o que eu deveria ter feito desde o início e que faz muito mais sentido, resolvendo
 o problema de clonar valores e variáveis

# Day 19
Uma variável comum segue **dois caminhos**
```
ENV
    \___ INTERPRETER
    /
RES
```

Então o pointer *(ou referencia)* no **Environment** *(ENV)* é deletada e seu *Resultado* *(RES)* do **Interpreter** também

Porém um objeto tem **três caminhos**
``` 
    ____________________
   /                    \
ENV                      OBJ
    \___ INTERPRETER ___/
    /                  /
RES __________________/
```

O problema não é com o **ObjectVal** em si *(OBJ)* e sim com os valores que ele armazena,
 no caso um *hash map* com todos os elementos do objeto

Em **Environment** é armazenado uma cópia do *Result*, uma variável com o valor igual ao **ObjectVal**

Quando o **ObjectVal** é deletado como *Result*, ele deleta todos os valores dentro do *hash map*,
 porém, depois ele é deletado do **Environment** e tenta de novo deletar todos os valores dentro do *hash map*,
 causando um erro

### Same day
Eu consegui resolver isso, toda variável dentro do ENV precisa ser uma cópia, pois mesmo que seja uma referência ou
 um pointer da variável original, ao criar uma nova, todas as outras vão ter o mesmo valor, pois no "visitor"
 da variáel é onde está o pointer da variável original, assim sempre tendo o valor da última
 criada

Agora, estou tentando lidar de novo com o problema do double delete do **ObjectVal**
 eu sinto que estou quase conseguindo resolver, estou quase e vou poder finalmente
 poder adicionar uma feature nova

### Same day
FINALMENTE!

FINALMENTE!!

FINALMENTE!!!
Eu consegui revolver o erro dos objetos, pela primeira vez estou usando `std::shared_ptr<T>` e
 dessa forma fuiz capaz de resolver isso, eu tentei usar *Referências* e *Cópias*, mas não iria
 funcionar para valores criados dentro do objeto e não ligo nem um pouco se tem uma solução que
 não usa *smart pointers* *(mentira ligo sim, mas não me importo agora)*

Desde o dia 10 *(praticamente)* que esse erro vem acontecendo e eu finalmente resolvi

mas...

Eu notei outro erro. É possível acontecer um erro no **Parser** dentro da função `advance`,
 mas em alguns casos não é possível **deletar** memória alocada quando o erro ocorre, como aqui

```cpp
const Expr* value = this->parse_expr();
this->advance();
```

Se um erro acontecer nesse *advance*, a memória alocada para *value*, não vai ser deletada,
isso ocorre por causa da forma que o **Interpreter** funciona

```cpp
while((stmt = this->parser->make_node()) != nullptr) {
	// ...
}
```

Uma *node* só é criada quando o **Interpreter** precisa, isso não acontece no **Parser** pois
 memória só alocada quando é retornada como **Token**, então acho que vou ser obrigada
 a voltar pro jeito antigo de como o **Interpreter** funcionava

```cpp
for(const Statement* stmt : program->body) {
	lastEval = this->evaluate(stmt);
}
```

Primeiro criar um array de *nodes* e depois interpretar elas

Acho que o processamento entre o *"Lazy Interpreter"* e desse jeito não vai ser tão significativo,
 já que o número de *nodes* é MUITO menor se comparado com o número de *tokens*

Mas vou fazer isso amanhã, fiquei exausta depois de tentar por literais DIAS finalmente
 ter objetos funcionando, agora só preciso dar uns toques finais e testar todos os erros
 para garantir que não está acontecendo nenhum *Memory Leak*


# Day 20
Ok, eu admito, fiquei uns dias procastinando e/ou programando outras coisas, mas ainda não desisti desse projeto

Eu descobri que não preciso abortar a criação das tokens assim que tiver um token inválido,
 eu posso criar um token do tipo inválido *(ou EOF)* e assim vai ter um erro dentro da própria
 criação das *nodes*, quando tiver um *expect*

Eu estou implementando isso e tentando também o *Lazy Interpreter*, mas ainda estou
 voltando aos poucos pra esse projeto, é um projeto que me demanda bastante mentalidade
 e cansa bastante, mas ainda quero terminar ele