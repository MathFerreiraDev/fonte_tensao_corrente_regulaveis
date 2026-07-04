# Fonte de Bancada Ajustável 0–15V / 1,5A com Monitoramento Digital

> Projeto de fonte linear regulável, com ajuste independente de tensão (0–15V) e corrente (0–1,5A), operando de forma automática nos modos CV e CC, e com sistema de monitoramento digital de tensão e corrente via INA219 + Arduino Nano + display LCD 16x2.
>
> **Disciplina:** Eletrônica Para Computação — [SSC0180] &nbsp;|&nbsp; **Docente:** Eduardo do Valle Simões

---

## Objetivos do Projeto

Este trabalho visa projetar, simular, calcular e montar uma **fonte linear regulável**, capaz de converter a tensão CA da rede (**220V / 60Hz**) em tensão CC ajustável de **0 a 15V**, com corrente máxima de **1,5A** e limite de corrente ajustável, além de exibir tensão e corrente de saída em tempo real através de um sistema de monitoramento digital independente.

## Descrição Geral do Circuito

O circuito foi projetado e simulado no **Falstad Circuit Simulator** para checar seu comportamento em tempo real, depois montado fisicamente e, por fim, migrado para uma versão em PCB.

### Topologia do circuito

1. **Transformador rebaixador U1** (220V CA → 18V CA) com isolamento galvânico, conectado externamente à placa através do conector **JP1**
2. **Ponte retificadora de onda completa** — módulo integrado (rotulado `PONTE_RET` no esquemático), eletricamente equivalente a 4 diodos de silício em ponte
3. **Capacitor de filtro principal C1** (**4700µF** — eletrolítico) para supressão de ripple
4. **Capacitor bypass C2** (100nF — cerâmico) para filtro de alta frequência
5. **Referência de tensão:** Zener D5 (15V) polarizado por R1 (**2kΩ**)
6. **Transistor driver Q1** (**BD139** — NPN) para amplificação do sinal de controle
7. **Transistor de passagem Q2** (**TIP36C** — PNP) como elemento série do regulador
8. **Potenciômetro R3** (10kΩ linear) para controle contínuo da tensão de saída (0–15V)
9. **Potenciômetro R2** (5kΩ linear) para limitação ajustável de corrente (0–1,5A)
10. **Resistor de bias R4** (470Ω) para estabilidade do ponto de operação de Q1
11. **Resistor bleeder R5** (**4,4kΩ**) como carga mínima permanente
12. **Diodo de proteção D6** (1N5408 — 3A / 1000V) contra picos de back-EMF
13. **LED indicador + R6** (**2,2kΩ**) sinalizando que a fonte está energizada
14. **Conversor buck LM2596** (módulo), configurado para **+5V fixo**, derivado do barramento CC bruto — alimenta exclusivamente o sistema de monitoramento (e uma saída USB auxiliar)
15. **Sistema de monitoramento:** sensor **INA219** (tensão/corrente via I2C) + **Arduino Nano** + **display LCD 16x2**
16. **Conector de saída** (bornes `PIN_VAR+`/`PIN_VAR-` e/ou jack USB para a saída ajustável)

> ⚠️ **Nota sobre a leitura dos esquemáticos:** os valores acima foram extraídos diretamente da simulação no Falstad e do esquemático real fornecidos. Dois pontos ficaram como interpretação razoável e vale a pena confirmar contra a placa física: (1) a tensão/potência nominal do Zener D5 foi mantida em 15V/1W, já que a imagem só confirma os 15V; (2) o resistor de 4,4kΩ foi identificado como o bleeder R5 pela sua posição no circuito (ligado à saída), substituindo o antigo 1kΩ/3W.

---

### Imagens do Circuito

**I) Simulação no Falstad:**
- 🔗 [Simulação interativa no Falstad](https://tinyurl.com/23xghob4)

<img width="922" height="358" alt="image" src="https://github.com/MathFerreiraDev/fonte_tensao_corrente_regulaveis/blob/main/assets/fonte.png?raw=true" />

---

**II) Circuito físico implementado:**

| Vista 1 | Vista 2 |
|---|---|
| ![Circuito montado — vista 1](imagens/circuito_montado_1.jpg) | ![Circuito montado — vista 2](imagens/circuito_montado_2.jpg) |

---

**III) Esquemático e PCB:**
| 📸 Demonstração do Projeto |
| :---: |
| **Esquematico: Fonte com sensor de tensão e corrente** |
| <img src="https://github.com/MathFerreiraDev/fonte_tensao_corrente_regulaveis/blob/main/assets/esquematico_v2.png?raw=true" width="922" /> |
| **PCB: Fonte com sensor de tensão e corrente** |
| <img src="https://github.com/MathFerreiraDev/fonte_tensao_corrente_regulaveis/blob/main/assets/pcb_v2.jpg?raw=true" width="922" /> |

> **Detalhe:** Tanto o esquemático quanto a PCB não incluem o **Arduino Nano** + **Display LCD 16x2** usados para monitorar os valores de tensão e corrente — apenas o módulo **INA219** aparece no design. Arduino e display são conectados externamente por fios/headers.

---

## 🔢 Cálculo de Valores Pertinentes

> Todos os cálculos assumem rede elétrica de **220V CA / 60Hz** e os valores de componentes reais extraídos do esquemático e da simulação no Falstad.

---

### 1. Tensão de Pico Pós-Retificação

A partir da tensão eficaz (RMS) do secundário do transformador (18V, mesma especificação do projeto original — não alterada no esquemático):

$$V_{RMS} = 18\text{ V}$$

**Tensão de pico no secundário do transformador:**

$$V_{pico} = V_{RMS} \times \sqrt{2} = 18 \times 1{,}414 \approx 25{,}46\text{ V}$$

**Tensão de pico no barramento CC** — descontando a queda da ponte retificadora ($\approx 1{,}4\text{ V}$, equivalente a dois diodos em condução):

$$V_{CC} = V_{pico} - 1{,}40 \approx \boxed{24{,}05\text{ V}}$$

> Essa é a tensão bruta do barramento CC, antes da regulação, usada como $V_{CC}$ nos cálculos seguintes. Na simulação real do Falstad, com os potenciômetros próximos do ajuste máximo, a tensão de saída medida foi de **14,812V** — validando na prática o teto teórico de 15V estabelecido pelo Zener.

---

### 2. Ripple e Dimensionamento do Capacitor C1

Num retificador de onda completa (ripple = $2 \times f_{rede} = 120\text{ Hz}$), o ripple de tensão é dado por:

$$\Delta V_{ripple} = \frac{I_{carga}}{2 \cdot f_{rede} \cdot C}$$

**Capacitância mínima para ripple de 10% com carga máxima (1,5A)** — esse cálculo independe do capacitor instalado, é o requisito teórico:

$$\Delta V_{alvo} = 0{,}10 \times 24{,}05 \approx 2{,}4\text{ V}$$

$$C_{min} = \frac{I_{carga}}{2 \cdot f \cdot \Delta V_{alvo}} = \frac{1{,}5}{2 \times 60 \times 2{,}4} = \frac{1{,}5}{288} \approx \boxed{5{,}2\text{ mF}}$$

> 💡 **O valor real instalado (C1 = 4700µF) está muito próximo desse mínimo recomendado** — uma melhoria expressiva em relação a uma versão com apenas 220µF, que só seria suficiente para cargas muito leves.

**Ripple real com C1 = 4700µF instalado:**

| $I_{carga}$ | $\Delta V_{ripple}$ | % do pico |
|---|---|---|
| 100 mA | ≈ 0,18 V | ≈ 0,7% |
| 500 mA | ≈ 0,89 V | ≈ 3,7% |
| 1,0 A | ≈ 1,77 V | ≈ 7,4% |
| 1,5 A | ≈ 2,66 V | ≈ 11% |

**Headroom do regulador com C1 = 4700µF:**

O regulador funciona bem enquanto a tensão mínima do barramento atender:

$$V_{CC,min} = V_{CC} - \Delta V_{ripple} \geq V_{saida} + V_{dropout}$$

Com $V_{dropout} \approx 3\text{ V}$, a corrente máxima que C1 suporta sem prejudicar a regulação é:

$$I_{max} = \Delta V_{max} \times 2 \times f \times C, \quad \Delta V_{max} = V_{CC} - V_{saida} - V_{dropout}$$

| $V_{saida}$ | $\Delta V_{max}$ disponível | $I_{max}$ com C1 = 4700µF |
|---|---|---|
| 15 V | 6,05 V | ≈ **3,41 A** |
| 12 V | 9,05 V | ≈ **5,10 A** |
| 5 V | 16,05 V | ≈ **9,05 A** |
| 0 V | 21,05 V | ≈ **11,87 A** |

> ✅ Em todos os pontos da faixa de saída, o $I_{max}$ suportado por C1 é **muito superior aos 1,5A** de especificação da fonte — ou seja, ao contrário de uma versão com capacitor subdimensionado, o C1 real (4700µF) garante boa regulação em **toda** a faixa de tensão e corrente de projeto, sem necessidade de upgrade adicional.

---

### 3. Corrente e Potência de Polarização do Zener D5

O resistor R1 (**2kΩ**) polariza o Zener D5 (15V) diretamente do barramento CC:

$$I_{R1} = \frac{V_{CC} - V_Z}{R1} = \frac{24{,}05 - 15}{2000} = \frac{9{,}05}{2000} \approx \boxed{4{,}5\text{ mA}}$$

**Verificações de segurança:**

$$I_{z,min,tipica} \approx 1\text{ mA} \quad \Rightarrow \quad 4{,}5\text{ mA} > 1\text{ mA} \quad ✅ \text{ (polarização garantida)}$$

$$P_{Zener} = V_Z \times I_{R1} = 15 \times 0{,}0045 \approx 68\text{ mW} \ll 1\text{ W (especificação assumida)} \quad ✅$$

---

### 4. Dissipação de Potência no TIP36C (Q2)

O transistor de passagem dissipa toda a diferença entre a tensão bruta e a tensão de saída regulada. Esse cálculo depende apenas de $V_{CC}$, $V_{saida}$ e $I_{carga}$ — não do modelo do transistor:

$$P_{Q2} = (V_{CC} - V_{saida}) \times I_{carga}$$

| $V_{saida}$ | $I_{carga}$ | $P_{Q2}$ |
|---|---|---|
| **0 V** | **1,5 A** | **≈ 36,1 W** ← pior caso absoluto |
| 5 V | 1,5 A | ≈ 28,6 W |
| 7,5 V | 1,5 A | ≈ 24,8 W |
| 12 V | 1,5 A | ≈ 18,1 W |
| 15 V | 1,5 A | ≈ 13,6 W |

> ⚠️ A dissipação máxima acontece com **saída em 0V** — não na saída máxima — pois toda a tensão bruta cai sobre Q2. O **TIP36C** utilizado no circuito real (25A / 100V / $P_{tot}$ = 125W em $T_{case}=25°C$) tem uma margem de segurança bem maior que o TIP42C original de referência (6A / 100W), tornando essa dissipação de pior caso bastante confortável para o componente em si — o dissipador continua sendo o fator limitante.

---

### 5. Dimensionamento do Dissipador Térmico de Q2

**Dados do TIP36C** (datasheet, encapsulamento TO-218/TO-3P):
- $R_{\theta jc} = 1\text{ °C/W}$ (resistência térmica junção–cápsula)
- $T_{j,max} = 150\text{ °C}$ (temperatura máxima da junção)
- $P_{tot} = 125\text{ W}$ @ $T_{case}=25°C$

Adotando pasta térmica entre Q2 e o dissipador: $R_{\theta cs} \approx 0{,}5\text{ °C/W}$

Isolando a resistência térmica máxima do dissipador na equação:

$$T_j = T_a + P_{Q2} \times (R_{\theta jc} + R_{\theta cs} + R_{\theta sa})$$

$$R_{\theta sa} \leq \frac{T_{j,max} - T_a}{P_{Q2}} - R_{\theta jc} - R_{\theta cs}$$

**Pior caso** ($P_{Q2} = 36{,}1\text{ W}$, $T_a = 25\text{ °C}$):

$$R_{\theta sa} \leq \frac{150 - 25}{36{,}1} - 1 - 0{,}5 = 3{,}46 - 1{,}5 \approx \boxed{1{,}96\text{ °C/W}}$$

**Operação típica** ($V_{saida} = 5\text{ V}$, $P_{Q2} = 28{,}6\text{ W}$):

$$R_{\theta sa} \leq \frac{125}{28{,}6} - 1{,}5 \approx 4{,}37 - 1{,}5 \approx \boxed{2{,}87\text{ °C/W}}$$

> **Recomendação:** usar dissipador de alumínio com $R_{\theta sa} \leq 1{,}5\text{ °C/W}$ + pasta térmica. Graças ao $R_{\theta jc}$ mais baixo e ao $P_{tot}$ bem maior do TIP36C em relação ao TIP42C de referência, o dissipador necessário é um pouco menos exigente (1,96°C/W vs. 1,04°C/W no projeto original), mantendo boa margem de segurança em toda a faixa de tensão e corrente.

---

### 6. Resistor Bleeder R5 — Verificação de Carga Mínima

$$I_{bleeder} = \frac{V_{saida,max}}{R5} = \frac{15\text{ V}}{4400\text{ Ω}} \approx \boxed{3{,}4\text{ mA}}$$

$$P_{R5,max} = \frac{V_{saida,max}^2}{R5} = \frac{15^2}{4400} \approx \boxed{51\text{ mW}}$$

Com R5 = 4,4kΩ, o bleeder consome uma fração ainda menor da corrente máxima de saída (≈0,23% de 1,5A) do que o resistor de 1kΩ/3W usado como referência inicial (que consumia ~15mA). A potência dissipada é tão baixa que um resistor comum de 1/4W já atende com folga — dispensando o resistor de potência de 3W do projeto original, com pequeno ganho de eficiência em vazio.

---

## 🔬 Explicação Técnica (Geral)

### Transformador U1 e Rede CA (220V / 60Hz)

A entrada do circuito é a tensão da rede (**220V CA / 60Hz**), aplicada ao primário do transformador **U1** (18V / ≥ 1,5A), que permanece **externo à placa**, conectando-se ao circuito através do conector **JP1** visível no esquemático. O transformador cumpre duas funções essenciais:

1. **Redução de tensão:** converte os 220V da rede para os 18V CA que o circuito precisa.
2. **Isolamento galvânico:** separa eletricamente a rede do circuito e do operador. Uma falha após o transformador não energiza a estrutura metálica nem o operador, eliminando o risco de choque direto pela rede.

A razão de espiras do transformador determina a tensão de saída: $V_{sec} = V_{prim} \times (N_2 / N_1)$.

O **LED** com **R6 (2,2kΩ)** fica ligado na saída da ponte retificadora — antes do regulador — e funciona de forma independente da tensão ajustada. Ele fica aceso sempre que a fonte estiver ligada, mesmo com a saída em 0V. O valor de R6 real (2,2kΩ) é maior que o de uma referência inicial de 1kΩ, resultando em um LED um pouco menos brilhante, porém mais eficiente.

---

### Ponte Retificadora de Onda Completa (módulo integrado)

No circuito real, essa função é implementada por um **módulo de ponte retificadora integrada** (rotulado `PONTE_RET` no esquemático), eletricamente equivalente a quatro diodos de silício em configuração de ponte. Por aproveitar os dois semiciclos da CA, essa topologia resulta em:

- **Frequência de ripple = 2 × f_rede = 120 Hz** (o dobro de um retificador de meia-onda)
- Maior continuidade de carga, menor stress no capacitor de filtro
- Tensão média mais alta em comparação ao retificador de meia-onda

Em cada semiciclo, dois diodos conduzem em série, com queda combinada estimada em ~1,4V (valor pode variar ligeiramente conforme a especificação exata do módulo utilizado).

---

### Capacitores de Filtro C1 e C2

**C1 (4700µF — eletrolítico polarizado):**
Funciona como o reservatório de energia principal: carrega nos picos da tensão retificada e entrega corrente nos vales, suavizando o ripple de 120Hz. Com o valor real instalado, C1 fica muito próximo do mínimo recomendado para 10% de ripple em carga máxima — diferente de um projeto inicial com apenas 220µF, que só seria adequado para cargas leves. A polaridade (+/−) deve ser respeitada na instalação — inverter destrói o componente.

**C2 (100nF — cerâmico):**
Trabalha em paralelo com C1 filtrando ruídos e transientes de alta frequência. O eletrolítico tem indutância série (ESL) considerável e responde mal acima de alguns kHz. O cerâmico, com ESL quase nula, cobre essa limitação e elimina os ruídos que o eletrolítico deixa passar.

---

### Referência de Tensão — Zener D5 e R1

O **Zener D5 (15V)**, polarizado em reverso por **R1 (2kΩ)**, gera a referência estável que define o **teto absoluto da saída**. O regulador não consegue ultrapassar esse valor, independentemente do ajuste de R3.

O Zener funciona pelo efeito avalanche reverso: ao atingir sua tensão de ruptura (15V), ele conduz e mantém a tensão em seus terminais quase constante, mesmo com variações de corrente. R1 limita essa corrente a ~4,5mA, deixando o Zener bem dentro do seu limite de potência.

---

### Par de Transistores Q1/Q2 — Núcleo da Regulação de Tensão

O coração do regulador é o par complementar **Q1 (BD139 — NPN)** e **Q2 (TIP36C — PNP)**, em malha fechada de realimentação negativa.

**Q2 (TIP36C)** é o **transistor de passagem** (*series-pass element*). Fica em série entre o barramento bruto (~24V) e a saída, agindo como uma válvula variável: quanto mais corrente de base Q1 fornece, mais Q2 conduz e menor sua queda de tensão, elevando a saída. Q2 dissipa em calor toda a diferença entre entrada e saída — por isso precisa de dissipador. Comparado ao TIP42C usado como referência inicial, o TIP36C oferece bem mais margem: 25A contínuos e 125W de dissipação total contra 6A e ~65W do TIP42C, dando mais folga térmica e elétrica ao projeto.

**Q1 (BD139)** é o **transistor driver**: amplifica o sinal de erro do laço de controle e aciona a base de Q2. É um NPN de 80V/1,5A — versão de maior tensão de ruptura dentro da mesma família (BD135/137/139), o que dá mais margem de segurança frente aos ~24V do barramento bruto. Juntos, Q1+Q2 formam um par de altíssimo ganho de corrente — parecido com um Darlington, mas com polaridades complementares (NPN × PNP) — o que permite que variações de microampères no laço controlem correntes de saída da ordem de ampères.

Esse mecanismo garante a **rigidez de tensão** da fonte — qualquer perturbação de carga ou de rede é detectada e corrigida automaticamente pelo laço, em microssegundos.

---

### Potenciômetro R3 (10kΩ) — Controle da Tensão de Saída

R3 funciona como um divisor de tensão ajustável no laço de realimentação. Variando sua resistência, o usuário controla a fração de tensão realimentada para a base de Q1, definindo o ponto de equilíbrio do laço e a tensão de saída. O ajuste é suave e contínuo, de **0 a 15V**.

---

### Potenciômetro R2 (5kΩ) — Limitação Ajustável de Corrente

R2 define o limite máximo de corrente de saída. Quando a carga exige uma corrente igual ao valor ajustado em R2, o circuito de limitação age: reduz aos poucos a condução de Q1 e, em cascata, de Q2, mantendo a corrente travada no valor configurado. A tensão de saída cai livremente conforme a resistência da carga, e a fonte passa do modo CV para o CC.

---

### Resistor R4 (470Ω) — Bias do Estágio Driver

R4 define o ponto de operação (bias) de Q1, garantindo que ele opere na região ativa linear correta em toda a faixa de ajuste de R2 e R3. Sem R4, Q1 poderia saturar ou cortar em certas combinações de ajuste, deixando a regulação instável ou inoperante.

---

### Resistor Bleeder R5 (4,4kΩ) — Carga Mínima Permanente

R5, sempre conectado à saída, garante ~3,4mA de corrente mínima circulando pelo circuito, mesmo sem carga externa. Isso cumpre duas funções importantes:

1. **Estabilidade do laço em circuito aberto:** mantém os transistores no ponto de operação correto sem carga conectada. Sem R5, o laço de realimentação, sem corrente suficiente, pode perder a referência e a tensão de saída flutuar ou subir indevidamente.
2. **Descarga segura dos capacitores:** ao desligar a fonte, R5 drena a energia guardada em C1 de forma controlada, evitando choque ao tocar nos terminais logo após desligar.

Com valor mais alto que a referência inicial de 1kΩ, R5 agora dissipa menos potência (~51mW contra ~225mW), reduzindo levemente o consumo em vazio da fonte.

---

### Diodo D6 (1N5408) — Proteção contra Back-EMF

D6 protege Q2 contra picos de tensão reversa gerados por cargas indutivas (motores DC, relés, solenoides, bobinas). Ao desconectar essa carga de repente, ela gera uma força contra-eletromotriz (*back-EMF*) de polaridade oposta, que pode gerar picos de tensão e destruir o transistor de passagem na hora. D6 (1000V e 3A) oferece um caminho de baixa impedância para absorver e dissipar esses picos com segurança.

---

## 🖥️ Sistema de Monitoramento (INA219 + Arduino Nano + Display LCD 16x2)

Além do regulador linear, o projeto conta com um sistema de monitoramento digital que exibe a tensão e a corrente reais de saída em tempo real, sem interferir na malha de regulação analógica.

### Arquitetura e alimentação

O sistema de monitoramento é **eletricamente independente** do laço de regulação principal:

- Um módulo conversor buck **LM2596** deriva do **barramento CC bruto** (~24V, o mesmo nó que alimenta Q2) uma tensão fixa de **+5V**, representada no esquemático como a saída `OUT+`/`OUT-` do módulo LM2596.
- Essa saída de **5V fixo** é a única fonte de alimentação do **INA219**, do **Arduino Nano** e do **display LCD 16x2** — nenhum desses componentes é alimentado pela saída ajustável (0–15V).
- O mesmo rail de 5V também alimenta um conector USB auxiliar (`USB_OUT`) no esquemático, disponibilizando uma saída fixa de 5V para uso geral, independente da saída variável da fonte.
- Por ser um domínio de alimentação separado, o consumo do Arduino/INA219/LCD não onera R5 nem os potenciômetros R2/R3, e uma eventual instabilidade na malha de regulação (0–15V) não afeta a leitura ou o funcionamento do display.

### Medição (INA219)

O **INA219** é um sensor de tensão/corrente de barramento com interface I2C, capaz de medir até 26V no barramento e ±3,2A através de um shunt resistivo interno/anexo. No circuito:

- Seus pinos **VIN+/VIN-** são inseridos em série com a linha de saída ajustável (`PIN_VAR+`), de forma que toda a corrente entregue à carga externa passa pelo shunt do INA219.
- Isso permite medir simultaneamente a **tensão** de saída e a **corrente** de carga em tempo real.
- A comunicação com o microcontrolador é feita via barramento **I2C** (pinos **SDA/SCL**), com alimentação em **VCC/GND** vinda do rail de +5V do LM2596.

### Processamento e exibição (Arduino Nano + LCD 16x2)

- O **Arduino Nano** (ATmega328P) lê periodicamente os registradores de tensão e corrente do INA219 via I2C, calcula a potência instantânea e formata os valores.
- Esses valores são exibidos em tempo real no **display LCD 16x2**, permitindo acompanhar a tensão e a corrente ajustadas nos potenciômetros R3/R2 sem a necessidade de multímetro externo.
- Arduino e display são atualizados continuamente conforme a carga ou os ajustes dos potenciômetros mudam, oferecendo leitura dinâmica equivalente à de fontes de bancada comerciais com display digital.

> ⚠️ **Importante:** apenas o módulo **INA219** está representado no esquemático e na PCB. O **Arduino Nano** e o **display LCD 16x2** são módulos externos, conectados à placa por fios/headers, e não fazem parte do desenho da placa.

### Componentes do sistema de monitoramento

| Componente | Modelo | Função |
|---|---|---|
| Conversor buck | LM2596 (módulo) | Gera +5V fixo a partir do barramento CC bruto (~24V); alimenta toda a eletrônica de monitoramento e a saída USB auxiliar |
| Sensor de tensão/corrente | INA219 | Mede tensão e corrente na saída ajustável (em série via VIN+/VIN-); comunica-se por I2C |
| Microcontrolador | Arduino Nano (ATmega328P) | Lê o INA219 via I2C, calcula e formata os valores de tensão/corrente/potência |
| Display | LCD 16x2 | Exibe tensão e corrente de saída em tempo real |

---

## ⚡ Modos de Operação e Comportamentos Importantes

### 1. Modo CV (Tensão Constante) e Modo CC (Corrente Constante)

A fonte opera automaticamente em dois regimes, com transição contínua entre eles:

| Modo | Condição de ativação | O que é regulado | O que varia |
|---|---|---|---|
| **CV** — Tensão Constante | $I_{carga} < I_{limite}$ ajustado em R2 | Tensão de saída (pelo R3) | Corrente (conforme a resistência da carga) |
| **CC** — Corrente Constante | $I_{carga} \geq I_{limite}$ ajustado em R2 | Corrente de saída (pelo R2) | Tensão (cai livremente com a carga) |

A transição não usa chaves, relés ou interrupções — é uma consequência suave do comportamento dos transistores no laço de controle.

---

### 2. Queda de Tensão ao Reduzir o Limite de Corrente

Ao reduzir o limite de corrente (R2) com uma carga conectada, a tensão de saída cai automaticamente — uma consequência inevitável da Lei de Ohm:

$$V_{saida} = I_{limitada} \times R_{carga}$$

Com a corrente limitada e a carga fixa, a tensão cai necessariamente. Na prática, com limites de corrente muito baixos, a tensão pode chegar a **~5V**, independentemente do ajuste de R3, pois a limitação de corrente sobrepõe o controle de tensão em modo CC.

Esse piso de ~5V reflete a saturação do par Q1/Q2 — abaixo dela, o laço perde a precisão da regulação.


---

### 3. Comportamento com Cargas de Corrente Controlada (ex.: Smartphones)

Ao conectar um smartphone ou qualquer dispositivo com controlador de carga interno (PMIC — *Power Management IC*):

1. **O dispositivo gerencia sua própria corrente.** Seu circuito interno negocia e limita o consumo conforme o protocolo de carga, o estado da bateria e a temperatura — não absorve toda a corrente disponível.
2. **Se o limite da fonte (R2) estiver acima do consumo do dispositivo** → a fonte opera em modo **CV**. O PMIC do aparelho define a corrente real, e R2 não interfere.
3. **A fonte não empurra corrente na carga** — apenas estabelece um teto máximo. A corrente real é sempre a que a carga escolhe consumir, dentro desse teto.
4. **Se o limite da fonte (R2) estiver abaixo do consumo desejado pelo dispositivo** → a fonte entra em modo **CC**, a tensão cai e o carregamento fica lento ou para.

---

### 4. Dissipação de Calor e Eficiência do Regulador Linear

Por ser linear (não chaveado), toda a diferença entre entrada (~24V) e saída vira calor em Q2. A eficiência varia diretamente com a tensão de saída ajustada:

$$\eta = \frac{P_{saida}}{P_{entrada}} = \frac{V_{saida}}{V_{CC}} = \frac{V_{saida}}{24{,}05}$$

| $V_{saida}$ | Eficiência |
|---|---|
| 15 V | ≈ 62% |
| 12 V | ≈ 50% |
| 5 V | ≈ 21% |
| 0 V | 0% |

> Reguladores chaveados atingem 85–95% de eficiência, mas com mais complexidade, ruído de alta frequência e custo. Já os lineares dão saída bem limpa (ruído baixíssimo), resposta rápida a transitórios e circuito simples — ideal para bancadas eletrônicas de precisão. O conversor buck **LM2596** usado no sistema de monitoramento, por outro lado, é um estágio chaveado — sua eficiência (tipicamente 75–90%) importa pouco aqui, já que alimenta apenas eletrônica de baixo consumo (INA219 + Arduino Nano + LCD), não a saída de potência da fonte.

---

### 5. Ripple Residual e Qualidade da Regulação

O ripple do barramento CC é bastante reduzido pelo laço de regulação ativo — Q1 e Q2 respondem às variações de tensão na entrada e as compensam continuamente. O ripple residual na saída regulada é bem menor que o ripple bruto do retificador.

| Parâmetro | Característica |
|---|---|
| Frequência do ripple | 120 Hz (rede 60Hz, retificação onda completa) |
| Regulação de linha | Boa — variações de tensão da rede são compensadas pelo laço |
| Regulação de carga | Boa — tensão estável com variações de corrente dentro do limite |
| Ripple residual na saída | Muito baixo — a regulação ativa rejeita ativamente as variações de entrada |

---

### 6. Tensão de Dropout e Teto Máximo de Saída

O par Q1+Q2 precisa de uma diferença mínima entre entrada e saída para operar na região ativa — a chamada **tensão de dropout**. Neste circuito:

$$V_{dropout} \approx 2\text{ a }3\text{ V}$$

Como a tensão bruta do barramento é ~24V e a referência Zener é 15V, a saída máxima estável coincide com a referência de D5: **15V** (na simulação real, a leitura de 14,812V confirma esse comportamento). Acima disso, o Zener não sustenta mais o laço de controle e a regulação fica instável.

---

### 7. Comportamento em Circuito Aberto (Sem Carga)

Sem carga na saída, **R5 (4,4kΩ)** garante ~3,4mA de corrente mínima, mantendo os transistores no ponto de operação correto e evitando que a saída flutue ou suba além do ajustado. Sem R5, o laço de realimentação, sem corrente suficiente, perde a referência de controle e a saída poderia subir sem controle.

---

## 📋 Componentes e Funções

| Componente | Modelo / Valor | Função |
|---|---|---|
| Transformador U1 | 18V CA / ≥ 1,5A (externo, via JP1) | Reduz 220V CA para 18V CA; isolamento galvânico |
| Ponte retificadora | Módulo integrado (`PONTE_RET`) | Converte CA em CC pulsante de onda completa |
| Capacitor C1 | 4700 µF (eletrolítico) | Filtro principal de ripple de 120 Hz |
| Capacitor C2 | 100 nF (cerâmico) | Bypass de ruídos de alta frequência |
| Diodo Zener D5 | 15V | Referência de tensão; define teto absoluto da saída |
| Resistor R1 | 2 kΩ | Polariza D5; limita corrente no Zener |
| Transistor Q1 | BD139 (NPN — 1,5A / 80V) | Driver do regulador; amplifica sinal de controle |
| Transistor Q2 | TIP36C (PNP — 25A / 100V) | Pass transistor; elemento série do regulador |
| Potenciômetro R3 | 10 kΩ linear | Controle contínuo da tensão de saída (0–15V) |
| Potenciômetro R2 | 5 kΩ linear | Limitação ajustável de corrente (0–1,5A) |
| Resistor R4 | 470 Ω | Bias do driver Q1; estabiliza ponto de operação |
| Resistor R5 | 4,4 kΩ | Bleeder: carga mínima, estabilidade e descarga de C1 |
| Diodo D6 | 1N5408 (3A / 1000V) | Proteção de Q2 contra picos de back-EMF indutivo |
| LED + R6 | LED 5mm + 2,2 kΩ | Indicador visual de energização da fonte |
| Conversor buck | LM2596 (módulo, +5V fixo) | Alimenta o sistema de monitoramento e saída USB auxiliar |
| Sensor de monitoramento | INA219 | Mede tensão/corrente de saída via I2C |
| Microcontrolador | Arduino Nano | Processa os dados do INA219 (externo à placa) |
| Display | LCD 16x2 | Exibe tensão/corrente em tempo real (externo à placa) |
| Dissipador térmico | $R_{\theta sa} \leq 1{,}5\text{ °C/W}$ | Resfriamento do TIP36C (Q2) |
| Conector de saída | Bornes / `PIN_VAR+`/`PIN_VAR-` | Saída da tensão regulada |

---

## 🔄 Funcionamento Resumido

1. A tensão da rede (**220V CA / 60Hz**) é reduzida para **18V CA** pelo transformador U1 (externo, via JP1), com isolamento galvânico total.
2. O **módulo de ponte retificadora** converte os 18V CA em CC pulsante (~24V de pico), aproveitando os dois semiciclos da onda.
3. **C1 (4700µF)** suaviza o ripple de 120Hz; **C2 (100nF)** elimina ruídos de alta frequência.
4. O **Zener D5** (polarizado por R1 = 2kΩ) estabelece a referência estável de **15V**, que define o teto absoluto da tensão de saída.
5. O **potenciômetro R3** ajusta a fração de tensão realimentada para Q1, definindo o ponto de equilíbrio do regulador e a **tensão de saída (0 a 15V)**.
6. **Q1 (BD139)** amplifica o sinal de controle; **Q2 (TIP36C)** conduz proporcionalmente, regulando a tensão de saída em malha fechada de realimentação negativa.
7. O **potenciômetro R2** define o limite de corrente. Ao atingi-lo, a fonte transita automaticamente de modo **CV → CC**, mantendo a corrente constante e deixando a tensão cair livremente.
8. **R5 (4,4kΩ)** garante corrente mínima permanente para estabilidade do laço de controle e descarga segura de C1 ao desligar a fonte.
9. **D6 (1N5408)** protege Q2 contra picos de back-EMF gerados por cargas indutivas desconectadas abruptamente.
10. O **LED + R6** sinalizam que a fonte está energizada, independentemente da tensão de saída configurada.
11. Em paralelo, um **conversor buck LM2596** deriva do barramento bruto uma tensão fixa de **+5V**, dedicada exclusivamente à eletrônica de monitoramento e a uma saída USB auxiliar independente da saída ajustável.
12. O sensor **INA219**, inserido em série com a saída ajustável, mede tensão e corrente reais de saída e as reporta via **I2C** ao **Arduino Nano**.
13. O **Arduino Nano** processa os dados do INA219 e exibe tensão, corrente e potência em tempo real no **display LCD 16x2** — ambos conectados externamente à placa, sem fazer parte do esquemático/PCB.
