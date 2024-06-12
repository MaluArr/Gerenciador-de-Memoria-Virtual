# Gerenciador de Memória Virtual
Esta implementação foi realizada para a disciplina de Infraestrutura de Software e visa tornar possível o gerenciador de memória virtual conforme apresentado na página P-51 do livro "Operating System Concepts" de Silberschatz, A. et al, 10ª edição. O instrutor da disciplina sugeriu algumas modificações, detalhadas a seguir:

## Modificações Sugeridas:
### Memória Física:
A implementação deve considerar uma memória física com apenas 128 frames, conforme descrito na página P-51 do livro mencionado.
### Linguagem e Sistema Operacional:
O programa deve ser implementado em C e ser executável em sistemas Linux.
### Compilação:
A compilação deve ser feita via Makefile, através do comando make no terminal, gerando um arquivo executável denominado vm.
### Validação: 
A implementação será validada com o compilador gcc 13.2.0, e o arquivo de saída deve seguir rigorosamente a formatação e o texto especificados.
### Preenchimento de Frames: 
Os frames na memória física devem ser preenchidos de 0 a 127. Quando a memória estiver cheia, deve-se aplicar um algoritmo de substituição para determinar qual frame será atualizado.
### Algoritmos de Substituição de Página:
Devem ser implementados dois algoritmos de substituição de página: FIFO (First In, First Out) e LRU (Least Recently Used). Na TLB (Translation Lookaside Buffer), apenas o algoritmo FIFO será aplicado.
### Argumentos de Linha de Comando: 
O primeiro argumento deve ser um arquivo de endereços lógicos (similar ao address.txt anexado ao Classroom) e o segundo argumento deve ser o tipo de algoritmo de substituição de página a ser utilizado (fifo ou lru). Por exemplo, a chamada:
./vm address.txt lru
indica que o algoritmo de substituição de página será o LRU.
### Arquivo de Saída:
O arquivo de saída será denominado correct.txt, seguindo rigorosamente a mesma formatação do arquivo anexado na atividade do Classroom, que utilizou o algoritmo FIFO.

## Como Executar
### Compile o programa utilizando o comando make no terminal:
make

### Execute o programa com os argumentos necessários:
./vm address.txt lru

## Licença
Este projeto está licenciado sob a licença MIT.
