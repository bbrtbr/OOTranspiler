# OOTranspilerC

Transpilador didático feito em C para converter um subconjunto de código orientado a objetos, inspirado em C++, para código Lab11.

O objetivo do projeto é mostrar, de forma simples, algumas etapas de um compilador/transpilador: análise léxica, análise sintática, análise semântica e geração de código.

## O que o projeto faz

A entrada é um arquivo `.cpp` com recursos básicos de orientação a objetos, como classes, campos, métodos, construtores e herança simples.

A saída é um arquivo `.lab11`, em um formato mais simples e procedural. Nessa saída, objetos são transformados em variáveis comuns e chamadas de métodos são expandidas no código gerado.

Exemplo de ideia geral:

```cpp
Pessoa p;
p.idade = 17;
p.aniversario();
```

Pode virar algo como:

```cpp
int pIdade;
pIdade = 17;
pIdade = pIdade + 1;
```

## Estrutura do projeto

```text
src/
  main.c              entrada do programa
  transpiler.c        fluxo principal da transpilação
  lexer.c             separação do código em tokens
  parser.c            montagem da AST
  semantic.c          validação semântica
  model.c             modelo de classes, objetos e herança
  codegen.c           início da geração de código
  lowering*.c         conversão para comandos Lab11

include/
  arquivos .h usados pelo projeto

examples/
  exemplos de entrada .cpp e saída esperada .lab11
```

## Como compilar

No Windows com MinGW:

```powershell
mingw32-make
```

Isso gera o executável:

```text
oo2lab11.exe
```

## Como executar

Exemplo:

```powershell
.\oo2lab11.exe examples\pessoa.cpp saida.lab11
```

Para ver a saída:

```powershell
Get-Content saida.lab11
```

Também é possível rodar os outros exemplos:

```powershell
.\oo2lab11.exe examples\pessoa_param.cpp saida_param.lab11
.\oo2lab11.exe examples\heranca_polimorfismo.cpp saida_heranca.lab11
```

## Exemplos disponíveis

- `pessoa.cpp`: classe simples, campos, métodos e `if`.
- `pessoa_param.cpp`: construtor e métodos com parâmetros.
- `heranca_polimorfismo.cpp`: herança simples, sobrescrita de método e polimorfismo didático.

## Recursos suportados

- classes simples;
- campos `int`, `float` e `bool`;
- métodos `void`;
- construtor simples;
- parâmetros primitivos;
- uso de `this`;
- criação de objetos locais;
- acesso a campos;
- chamada de métodos;
- herança simples;
- sobrescrita de métodos;
- `if`, `while` e `do while`.

## Limitações

O projeto não é um compilador C++ completo. Ele aceita apenas o subconjunto necessário para os testes propostos.

Algumas limitações:

- não suporta `new` e `delete`;
- não suporta ponteiros ou referências;
- não suporta métodos com retorno;
- não suporta sobrecarga de métodos;
- não suporta mais de um construtor por classe;
- não suporta parâmetros do tipo objeto;
- não suporta `else`;
- o polimorfismo só funciona quando o tipo real pode ser identificado durante a geração do código.
