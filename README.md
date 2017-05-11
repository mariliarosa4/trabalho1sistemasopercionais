# algoritmo dinâmico para um sistema eletrocardiograma #

**Objetivo**:desenvolver os conceitos adquiridos na disciplina de Sistemas Operacionais,
tais como fluxo de execução, estados e descritores de processos e algoritmos de
escalonamento. Além disso, estimular a desenvoltura e apresentação oral do aluno.
O aluno está sendo convidado a desenvolver um algoritmo dinâmico para um
sistema eletrocardiograma. O sistema deve ser composto por três módulos distintos:
(i) Coletor, (ii) Monitor e (iii) Visualizador.

**Módulo Coletor:** para cada coleta é necessário utilizar 2 sensores
simultâneamente, por exemplo, (i) o ritmo cardíaco, (ii) suprimento sanguíneo, (iii)
suprimento de oxigênio, (iv) análise da onda P (despolarização atrial), (v) T
(repolarização ventricular), (vi) intervalo da repolarização ventricular, etc. O sistema
deve possuir 5 conjuntos de análises, sendo que os sensores podem ser utilizados em
mais de uma análise, desde de que sua utilização seja atômica.

**Módulo Monitor:** as informações coletadas devem ser enviadas para um
módulo de monitoramento que será responsável por armazenar as informações das
análises de forma sincronizada. Por exemplo, criando uma estrutura de dados
compatilhada para receber as informações coletadas.

**Módulo de visualizador:** após as informações serem monitoradas, estas
devem ser apresentadas para o usuário final de forma sumarizada. Por exemplo,
quantidade de batimentos, aritmias, etc.