# Projeto 1 SOPE

No âmbito da unidade curricular, apresentamos uma simplificação do comando "du" - simpledu.

## Usage

```bash
simpledu -l <path> <-a> <-b> <-B size> <-L> <-S> <--max-depth=N>
```

Se a variável ambiente LOG_FILENAME não estiver definida, o ficheiro de texto para onde é escrito o "log", é o ficheiro log.txt.

Tal como no du, caso o caminho para onde o simpledu deve correr não for especificado, o caminho por omissão é o ".", ou seja, a pasta onde o utilizador se encontra naquele momento. 

## Implentações

Na opção -B (ou --block-size=SIZE), no caso do tamanho especificado não for potência de 2, ocorrem algumas falhas no tamanho apresentado.

Não detetámos outras falhas na nossa fase de testes.

## Collaborators

Flávia Carvalhido - up201806857@fe.up.pt\
João Gonçalves - up201806796@fe.up.pt\
Nuno Resende - up201806825@fe.up.pt


Turma 7 - MIEIC 2019/20
