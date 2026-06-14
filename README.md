# Fonte de Bancada Ajustável 0–15V / 1,5A

> Projeto de fonte de alimentação linear regulável com controle independente de tensão (0–15V) e limitação de corrente (0–1.5A), com operação automática nos modos CV e CC.
>
> **Disciplina:** Eletrônica Para Computação — [SSC0180] &nbsp;|&nbsp; **Docente:** Eduardo do Valle Simões

---

## Objetivos do Projeto

Este trabalho tem como objetivo projetar, simular, calcular e montar fisicamente uma **fonte de alimentação linear regulável**, capaz de converter a tensão alternada da rede elétrica (220V CA / 60Hz) em tensão contínua ajustável entre **0 e 15V**, com corrente máxima de **1,5A** e limitação de corrente ajustável.

## Descrição Geral do Circuito

O circuito foi primeiramente projetado e simulado no **Falstad Circuit Simulator** para verificar o comportamento dinâmico dos componentes em tempo real, sendo em seguida implementado fisicamente e, por fim, transposto para uma versão em PCB.

### Topologia do circuito

1. **Transformador rebaixador U1** (220V CA → 18V CA) com isolamento galvânico
2. **Ponte retificadora de onda completa** (4 × diodos 1N4007)
3. **Capacitor de filtro principal C1** (220µF / 35V — eletrolítico) para supressão de ripple
4. **Capacitor bypass C2** (100nF — cerâmico) para filtro de alta frequência
5. **Referência de tensão:** Zener D5 (15V / 1W) polarizado por R1 (2,2kΩ)
6. **Transistor driver Q1** (BD137 — NPN) para amplificação do sinal de controle
7. **Transistor de passagem Q2** (TIP42C — PNP) como elemento série do regulador
8. **Potenciômetro R3** (10kΩ linear) para controle contínuo da tensão de saída (0–15V)
9. **Potenciômetro R2** (5kΩ linear) para limitação ajustável de corrente (0–1,5A)
10. **Resistor de bias R4** (470Ω) para estabilidade do ponto de operação de Q1
11. **Resistor bleeder R5** (1kΩ / 3W) como carga mínima permanente
12. **Diodo de proteção D6** (IN5408 — 3A / 1000V) contra picos de back-EMF
13. **LED indicador + R6** (1kΩ) sinalizando que a fonte está energizada
14. **Conector de saída J1** (bornes ou jack banana)

---

### Imagens do Circuito

**I) Simulação no Falstad:**
- 🔗 [Simulação interativa no Falstad](https://tinyurl.com/23xghob4)
  
[![Simulação no Falstad](imagens/simulacao_falstad.png)](#)

---

**II) Circuito físico implementado:**

| Vista 1 | Vista 2 |
|---|---|
| ![Circuito montado — vista 1](imagens/circuito_montado_1.jpg) | ![Circuito montado — vista 2](imagens/circuito_montado_2.jpg) |

---

**III) Esquemático e PCB:**

![Esquemático do circuito](imagens/esquematico.png)

![PCB — Layout das trilhas](imagens/pcb_layout.jpg)

![PCB — Placa finalizada](imagens/pcb_montada.jpg)

---

## 🔢 Cálculo de Valores Pertinentes

> Todos os cálculos assumem rede elétrica de **220V CA / 60Hz** e os valores de componentes exibidos no esquemático.

---

### 1. Tensão de Pico Pós-Retificação

A partir da tensão eficaz (RMS) do secundário do transformador:

$$V_{RMS} = 18\text{ V}$$

**Tensão de pico no secundário do transformador:**

$$V_{pico} = V_{RMS} \times \sqrt{2} = 18 \times 1{,}414 \approx 25{,}46\text{ V}$$

**Tensão de pico no barramento CC** — com desconto de dois diodos conduzindo simultaneamente na ponte ($V_D \approx 0{,}7\text{ V}$ cada):

$$V_{CC} = V_{pico} - 2 \times V_D = 25{,}46 - 1{,}40 \approx \boxed{24{,}05\text{ V}}$$

> Esta é a tensão bruta no barramento CC, antes da regulação. Todos os cálculos subsequentes utilizam este valor como $V_{CC}$.

---

### 2. Ripple e Dimensionamento do Capacitor C1

Para um retificador de onda completa (frequência de ripple = $2 \times f_{rede} = 120\text{ Hz}$), a fórmula do ripple de tensão é:

$$\Delta V_{ripple} = \frac{I_{carga}}{2 \cdot f_{rede} \cdot C}$$

**Capacitância mínima para ripple de 10% com carga máxima (1,5A):**

$$\Delta V_{alvo} = 0{,}10 \times 24{,}05 \approx 2{,}4\text{ V}$$

$$C_{min} = \frac{I_{carga}}{2 \cdot f \cdot \Delta V_{alvo}} = \frac{1{,}5}{2 \times 60 \times 2{,}4} = \frac{1{,}5}{288} \approx \boxed{5{,}2\text{ mF} \approx 5600\text{ µF (valor comercial)}}$$

**Ripple real com C1 = 220µF instalado:**

| $I_{carga}$ | $\Delta V_{ripple}$ | % do pico |
|---|---|---|
| 63 mA | ≈ 2,4 V | ≈ 10% |
| 100 mA | ≈ 3,8 V | ≈ 16% |
| 500 mA | ≈ 18,9 V | ≈ 79% |
| 1,5 A | > $V_{CC}$ | fora da validade linear |

> ⚠️ **C1 = 220µF é adequado apenas para cargas leves (até ~63mA com ripple de 10%).** Para correntes maiores, o laço de regulação ativo (Q1/Q2) compensa o ripple residual na saída — desde que a **tensão mínima** no barramento não caia abaixo do limiar de operação do regulador.

**Headroom do regulador com C1 = 220µF:**

O regulador linear opera corretamente enquanto a tensão mínima do barramento satisfaz:

$$V_{CC,min} = V_{CC} - \Delta V_{ripple} \geq V_{saida} + V_{dropout}$$

Assumindo $V_{dropout} \approx 3\text{ V}$, a corrente máxima suportada por C1 sem degradar a regulação é:

$$I_{max} = \Delta V_{max} \times 2 \times f \times C$$

onde $\Delta V_{max} = V_{CC} - V_{saida} - V_{dropout}$:

| $V_{saida}$ | $\Delta V_{max}$ disponível | $I_{max}$ com C1 = 220µF |
|---|---|---|
| 15 V | 6,05 V | ≈ **158 mA** |
| 12 V | 9,05 V | ≈ **238 mA** |
| 5 V | 16,05 V | ≈ **423 mA** |
| 0 V | 21,05 V | ≈ **554 mA** |

> 💡 **Recomendação:** para extrair correntes próximas de 1,5A com boa regulação, substituir C1 por **2200µF / 35V**, que garante margem adequada em toda a faixa de tensão de saída.

---

### 3. Corrente e Potência de Polarização do Zener D5

O resistor R1 (2,2kΩ) polariza o Zener D5 (15V) diretamente do barramento CC:

$$I_{R1} = \frac{V_{CC} - V_Z}{R1} = \frac{24{,}05 - 15}{2200} = \frac{9{,}05}{2200} \approx \boxed{4{,}1\text{ mA}}$$

**Verificações de segurança:**

$$I_{z,min,tipica} \approx 1\text{ mA} \quad \Rightarrow \quad 4{,}1\text{ mA} > 1\text{ mA} \quad ✅ \text{ (polarização garantida)}$$

$$P_{Zener} = V_Z \times I_{R1} = 15 \times 0{,}0041 \approx 62\text{ mW} \ll 1\text{ W (especificação)} \quad ✅$$

---

### 4. Dissipação de Potência no TIP42C (Q2)

O transistor de passagem dissipa toda a diferença de potencial entre o barramento bruto e a tensão de saída regulada:

$$P_{Q2} = (V_{CC} - V_{saida}) \times I_{carga}$$

| $V_{saida}$ | $I_{carga}$ | $P_{Q2}$ |
|---|---|---|
| **0 V** | **1,5 A** | **≈ 36,1 W** ← pior caso absoluto |
| 5 V | 1,5 A | ≈ 28,6 W |
| 7,5 V | 1,5 A | ≈ 24,8 W |
| 12 V | 1,5 A | ≈ 18,1 W |
| 15 V | 1,5 A | ≈ 13,6 W |

> ⚠️ A máxima dissipação ocorre com **tensão de saída mínima (0V)** — não na saída máxima — pois toda a tensão bruta recai sobre Q2.

---

### 5. Dimensionamento do Dissipador Térmico de Q2

**Dados do TIP42C** (datasheet TO-220):
- $R_{\theta jc} = 1{,}92\text{ °C/W}$ (resistência térmica junção–cápsula)
- $T_{j,max} = 150\text{ °C}$ (temperatura máxima da junção)

Adotando pasta térmica entre Q2 e o dissipador: $R_{\theta cs} \approx 0{,}5\text{ °C/W}$

A resistência térmica máxima do dissipador é isolada de:

$$T_j = T_a + P_{Q2} \times (R_{\theta jc} + R_{\theta cs} + R_{\theta sa})$$

$$R_{\theta sa} \leq \frac{T_{j,max} - T_a}{P_{Q2}} - R_{\theta jc} - R_{\theta cs}$$

**Pior caso** ($P_{Q2} = 36{,}1\text{ W}$, $T_a = 25\text{ °C}$):

$$R_{\theta sa} \leq \frac{150 - 25}{36{,}1} - 1{,}92 - 0{,}5 = 3{,}46 - 2{,}42 \approx \boxed{1{,}04\text{ °C/W}}$$

**Operação típica** ($V_{saida} = 5\text{ V}$, $P_{Q2} = 28{,}6\text{ W}$):

$$R_{\theta sa} \leq \frac{125}{28{,}6} - 2{,}42 \approx 4{,}37 - 2{,}42 \approx \boxed{1{,}95\text{ °C/W}}$$

> **Recomendação:** dissipador de alumínio com $R_{\theta sa} \leq 1\text{ °C/W}$ (perfis de 80–100mm) + pasta térmica, garantindo operação segura em toda a faixa de tensão e corrente.

---

### 6. Resistor Bleeder R5 — Verificação de Carga Mínima

$$I_{bleeder} = \frac{V_{saida,max}}{R5} = \frac{15\text{ V}}{1000\text{ Ω}} = \boxed{15\text{ mA}}$$

$$P_{R5,max} = \frac{V_{saida,max}^2}{R5} = \frac{15^2}{1000} = \boxed{225\text{ mW}} \ll 3\text{ W (especificação)} \quad ✅$$

O bleeder representa apenas **1%** da corrente máxima de saída (15mA / 1500mA), com impacto mínimo na eficiência. A especificação de 3W provê ampla margem de segurança para transientes.

---

## 🔬 Explicação Técnica (Geral)

### Transformador U1 e Rede CA (220V / 60Hz)

A entrada do circuito é a tensão da rede elétrica de **220V CA / 60Hz**, aplicada ao primário do transformador rebaixador **U1** (18V / ≥ 1,5A). O transformador cumpre duas funções essenciais e insubstituíveis:

1. **Redução de tensão:** adequa os 220V da rede para os 18V CA necessários ao circuito.
2. **Isolamento galvânico:** separa eletricamente a rede de distribuição do circuito e do operador. Qualquer falha no circuito após o transformador não energiza a estrutura metálica ou o referencial do operador, eliminando o risco de choque direto pela rede.

A razão de espiras do transformador determina a tensão de saída: $V_{sec} = V_{prim} \times (N_2 / N_1)$.

O **LED** com **R6 (1kΩ)** está conectado à saída da ponte retificadora — antes do regulador — funcionando de forma completamente independente da tensão ajustada na saída. O LED permanece aceso sempre que a fonte estiver energizada, mesmo com a saída em 0V.

---

### Ponte Retificadora de Onda Completa (4 × 1N4007)

Quatro diodos de silício dispostos em configuração de ponte convertem a tensão alternada em tensão CC pulsante. A topologia de onda completa aproveita **ambos os semiciclos** da CA, resultando em:

- **Frequência de ripple = 2 × f_rede = 120 Hz** (o dobro de um retificador de meia-onda)
- Maior continuidade de carga, menor stress no capacitor de filtro
- Tensão média mais alta em comparação ao retificador de meia-onda

Em cada semiciclo, dois diodos conduzem em série, com queda combinada de ~1,4V.

---

### Capacitores de Filtro C1 e C2

**C1 (220µF / 35V — eletrolítico polarizado):**
Atua como o reservatório de energia principal. Carrega nos picos de tensão retificada e fornece corrente nos vales da onda pulsante, suavizando o ripple de 120Hz. A polaridade correta (+/−) deve ser rigorosamente respeitada na instalação — inversão destrói o componente.

**C2 (100nF — cerâmico):**
Trabalha em paralelo com C1 para filtrar ruídos e transientes de alta frequência. O capacitor eletrolítico possui indutância equivalente em série (ESL) significativa e responde mal a frequências acima de alguns kHz. O cerâmico, com ESL praticamente nula, complementa perfeitamente esta limitação, eliminando ruídos que o eletrolítico deixa passar.

---

### Referência de Tensão — Zener D5 e R1

O **diodo Zener D5 (15V / 1W)**, polarizado em reverso por **R1 (2,2kΩ)**, gera a referência de tensão estável que define o **teto absoluto da saída**. O regulador não pode fornecer tensão além do valor estabelecido pelo Zener — independentemente do ajuste do potenciômetro R3.

O princípio do Zener baseia-se no efeito avalanche reverso: ao atingir sua tensão de ruptura (15V), o diodo conduz mantendo a tensão em seus terminais praticamente constante, mesmo com variações de corrente. R1 limita esta corrente a ~4,1mA, mantendo o Zener bem dentro de sua potência nominal de 1W.

---

### Par de Transistores Q1/Q2 — Núcleo da Regulação de Tensão

O coração do regulador é o par complementar **Q1 (BD137 — NPN)** e **Q2 (TIP42C — PNP)**, operando em malha fechada de realimentação negativa.

**Q2 (TIP42C)** é o **transistor de passagem** (*series-pass element*). Posicionado em série entre o barramento bruto (~24V) e a saída, funciona como uma válvula de condução variável: quanto maior a corrente de base fornecida por Q1, mais Q2 conduz e menor a queda de tensão sobre ele, elevando a tensão de saída. Q2 dissipa toda a diferença de potencial entre entrada e saída na forma de calor — daí a necessidade do dissipador térmico.

**Q1 (BD137)** é o **transistor driver**, que amplifica o sinal de erro do laço de controle e aciona a base de Q2. O par Q1+Q2 forma um conjunto de altíssimo ganho de corrente composto — similar a um par Darlington, porém com polaridades complementares (NPN × PNP) — permitindo que variações de corrente da ordem de microampères no laço de controle governem correntes de saída da ordem de ampères.


Este mecanismo é responsável pela **rigidez de tensão** da fonte — qualquer perturbação (variação de carga ou de rede) é detectada e corrigida automaticamente pelo laço, em microssegundos.

---

### Potenciômetro R3 (10kΩ) — Controle da Tensão de Saída

R3 atua como um divisor de tensão ajustável no laço de realimentação. Ao variar sua resistência, o usuário controla a fração da tensão de saída realimentada para a base de Q1 — definindo o ponto de equilíbrio do laço e, portanto, a tensão de saída resultante. O ajuste é suave e contínuo de **0 a 15V**.

---

### Potenciômetro R2 (5kΩ) — Limitação Ajustável de Corrente

R2 define o limite máximo de corrente de saída. Quando a corrente demandada pela carga atinge o valor configurado em R2, o circuito de limitação entra em ação: ele reduz progressivamente a condução de Q1 e, em cascata, de Q2 — forçando a corrente a se manter no valor configurado. A tensão de saída cai livremente conforme a resistência da carga. A fonte transita do modo CV para o modo CC.

---

### Resistor R4 (470Ω) — Bias do Estágio Driver

R4 define o ponto de operação estático (bias) de Q1, garantindo que o transistor driver opere na região ativa linear correta em toda a faixa de ajuste de R2 e R3. Sem R4, Q1 poderia entrar em saturação ou corte em determinadas combinações de ajuste, tornando a regulação instável ou inoperante.

---

### Resistor Bleeder R5 (1kΩ / 3W) — Carga Mínima Permanente

R5, permanentemente conectado à saída, garante ~15mA de corrente mínima circulando pelo circuito, mesmo sem carga externa. Isso cumpre duas funções críticas:

1. **Estabilidade do laço de controle em circuito aberto:** mantém os transistores no ponto de operação correto quando nenhuma carga está conectada. Sem R5, o laço de realimentação sem corrente suficiente pode perder o referencial e a tensão de saída flutuaria ou subiria indevidamente.
2. **Descarga segura dos capacitores:** ao desligar a fonte, R5 drena a energia armazenada em C1 de forma controlada, evitando que o operador leve choque ao tocar nos terminais imediatamente após o desligamento.

---

### Diodo D6 (IN5408) — Proteção contra Back-EMF

D6 protege Q2 contra picos de tensão reversa gerados por cargas indutivas (motores DC, relés, solenoides, bobinas). Ao desconectar abruptamente esse tipo de carga, ela gera uma força contra-eletromotriz (*back-EMF*) de polaridade oposta, podendo gerar picos que excedem a tensão de ruptura coletor-emissor do TIP42C (100V) e destruí-lo instantaneamente. D6 (especificado para 1000V e 3A) oferece um caminho de baixa impedância para absorver e dissipá-los de forma segura.

---

## ⚡ Modos de Operação e Comportamentos Importantes

### 1. Modo CV (Tensão Constante) e Modo CC (Corrente Constante)

A fonte opera automaticamente em dois regimes distintos, com transição contínua e sem interrupção entre eles:

| Modo | Condição de ativação | O que é regulado | O que varia |
|---|---|---|---|
| **CV** — Tensão Constante | $I_{carga} < I_{limite}$ ajustado em R2 | Tensão de saída (pelo R3) | Corrente (conforme a resistência da carga) |
| **CC** — Corrente Constante | $I_{carga} \geq I_{limite}$ ajustado em R2 | Corrente de saída (pelo R2) | Tensão (cai livremente com a carga) |

A transição não envolve chaves, relés ou interrupções — é uma consequência suave do comportamento dos transistores no laço de controle.

---

### 2. Queda de Tensão ao Reduzir o Limite de Corrente

Ao diminuir o limite de corrente (R2) com uma carga conectada, a tensão de saída **cai automaticamente**. Trata-se de uma relação física inescapável decorrente da Lei de Ohm:

$$V_{saida} = I_{limitada} \times R_{carga}$$

Com corrente limitada e resistência de carga fixa, a tensão **necessariamente** cai. Na prática, ao configurar o limite de corrente para valores muito baixos, a tensão pode cair a valores próximos de **~5V**, independentemente do ajuste em R3 — porque o circuito de limitação de corrente sobrepõe o controle de tensão quando em modo CC.

O piso de ~5V reflete o ponto de saturação do par Q1/Q2 — abaixo desta tensão, o laço perde a capacidade de regular com precisão.

> Este comportamento não é um defeito: é o funcionamento correto e esperado de qualquer fonte com regulação CC real. Fontes profissionais de bancada (Korad, Rigol, OWON) operam pelo mesmo princípio.

---

### 3. Comportamento com Cargas de Corrente Controlada (ex.: Smartphones)

Ao conectar um smartphone ou qualquer dispositivo com controlador de carga interno (PMIC — *Power Management IC*):

1. **O dispositivo gerencia sua própria corrente de consumo.** Seu circuito interno negocia e limita o consumo conforme o protocolo de carga, o estado da bateria e a temperatura — ele não simplesmente absorve toda a corrente disponível.
2. **Se o limite da fonte (R2) estiver acima do consumo do dispositivo** → a fonte opera em modo **CV**. O PMIC do aparelho determina a corrente real. O potenciômetro R2 não interfere nesta condição.
3. **A fonte não injeta corrente na carga** — ela apenas estabelece um **teto máximo**. A corrente real é sempre a que a carga decide consumir, dentro deste teto.
4. **Se o limite da fonte (R2) estiver abaixo do que o dispositivo tenta consumir** → a fonte opera em modo **CC**, a tensão de saída cai e o carregamento fica lento ou é interrompido.

---

### 4. Dissipação de Calor e Eficiência do Regulador Linear

Por ser um regulador **linear** (não chaveado), toda a diferença de potencial entre entrada (~24V) e saída é convertida em calor em Q2. A eficiência varia diretamente com a tensão de saída configurada:

$$\eta = \frac{P_{saida}}{P_{entrada}} = \frac{V_{saida}}{V_{CC}} = \frac{V_{saida}}{24{,}05}$$

| $V_{saida}$ | Eficiência |
|---|---|
| 15 V | ≈ 62% |
| 12 V | ≈ 50% |
| 5 V | ≈ 21% |
| 0 V | 0% |

> Reguladores chaveados (*switching*) atingem 85–95% de eficiência, às custas de maior complexidade, ruído de alta frequência e custo. Reguladores lineares oferecem saída extremamente limpa (baixíssimo ruído), resposta rápida a transitórios e circuito simples — propriedades ideais para bancadas eletrônicas de precisão.

---

### 5. Ripple Residual e Qualidade da Regulação

O ripple do barramento CC é amplamente suprimido pelo laço de regulação ativo — Q1 e Q2 respondem às variações de tensão na entrada e as compensam continuamente. O ripple residual na **saída regulada** é muito inferior ao ripple bruto do retificador.

| Parâmetro | Característica |
|---|---|
| Frequência do ripple | 120 Hz (rede 60Hz, retificação onda completa) |
| Regulação de linha | Boa — variações de tensão da rede são compensadas pelo laço |
| Regulação de carga | Boa — tensão estável com variações de corrente dentro do limite |
| Ripple residual na saída | Muito baixo — a regulação ativa rejeita ativamente as variações de entrada |

---

### 6. Tensão de Dropout e Teto Máximo de Saída

O par Q1+Q2 necessita de uma diferença mínima entre tensão de entrada e saída para operar na região ativa — a chamada **tensão de dropout**. Para este circuito:

$$V_{dropout} \approx 2\text{ a }3\text{ V}$$

Como a tensão bruta no barramento é ~24V e a referência Zener é 15V, a saída máxima estável coincide exatamente com a referência de D5: **15V**. Acima desse valor, a referência Zener não sustenta o laço de controle e a regulação se torna instável.

---

### 7. Comportamento em Circuito Aberto (Sem Carga)

Sem carga conectada à saída, **R5 (1kΩ)** garante 15mA de corrente mínima, mantendo os transistores no ponto de operação correto e impedindo que a tensão de saída flutue ou suba além do ajustado. Sem R5, o laço de realimentação sem corrente suficiente perde o referencial de controle — a tensão de saída poderia subir incontrolavelmente.

---

## 📋 Componentes e Funções

| Componente | Modelo / Valor | Função |
|---|---|---|
| Transformador U1 | 18V CA / ≥ 1,5A | Reduz 220V CA para 18V CA; isolamento galvânico |
| Ponte retificadora | 4 × 1N4007 (1A / 1000V) | Converte CA em CC pulsante de onda completa |
| Capacitor C1 | 220 µF / 35V (eletrolítico) | Filtro principal de ripple de 120 Hz |
| Capacitor C2 | 100 nF (cerâmico) | Bypass de ruídos de alta frequência |
| Diodo Zener D5 | 15V / 1W | Referência de tensão; define teto absoluto da saída |
| Resistor R1 | 2,2 kΩ / ¼W | Polariza D5; limita corrente no Zener |
| Transistor Q1 | BD137 (NPN — 1,5A / 60V) | Driver do regulador; amplifica sinal de controle |
| Transistor Q2 | TIP42C (PNP — 6A / 100V) | Pass transistor; elemento série do regulador |
| Potenciômetro R3 | 10 kΩ linear | Controle contínuo da tensão de saída (0–15V) |
| Potenciômetro R2 | 5 kΩ linear | Limitação ajustável de corrente (0–1,5A) |
| Resistor R4 | 470 Ω / ¼W | Bias do driver Q1; estabiliza ponto de operação |
| Resistor R5 | 1 kΩ / 3W | Bleeder: carga mínima, estabilidade e descarga de C1 |
| Diodo D6 | IN5408 (3A / 1000V) | Proteção de Q2 contra picos de back-EMF indutivo |
| LED + R6 | LED 5mm + 1 kΩ | Indicador visual de energização da fonte |
| Dissipador térmico | $R_{\theta sa} \leq 1\text{ °C/W}$ | Resfriamento do TIP42C (Q2) |
| Conector J1 | Borne / Banana jack | Saída da tensão regulada |

---

## 🔄 Funcionamento Resumido

1. A tensão da rede (**220V CA / 60Hz**) é reduzida para **18V CA** pelo transformador U1, com isolamento galvânico completo.
2. A **ponte retificadora** (4 × 1N4007) converte os 18V CA em tensão CC pulsante (~24V de pico), aproveitando ambos os semiciclos da onda.
3. **C1 (220µF)** suaviza o ripple de 120Hz; **C2 (100nF)** elimina ruídos de alta frequência.
4. O **Zener D5** (polarizado por R1) estabelece a referência estável de **15V**, que define o teto absoluto da tensão de saída.
5. O **potenciômetro R3** ajusta a fração de tensão realimentada para Q1, definindo o ponto de equilíbrio do regulador e a **tensão de saída desejada (0 a 15V)**.
6. **Q1 (BD137)** amplifica o sinal de controle; **Q2 (TIP42C)** conduz proporcionalmente, regulando a tensão de saída em malha fechada de realimentação negativa.
7. O **potenciômetro R2** define o limite de corrente. Ao atingi-lo, a fonte transita automaticamente de modo **CV → CC**, mantendo a corrente constante e deixando a tensão cair livremente.
8. **R5 (1kΩ / 3W)** garante corrente mínima permanente para estabilidade do laço de controle e descarga segura de C1 ao desligar a fonte.
9. **D6 (IN5408)** protege Q2 contra picos de back-EMF gerados por cargas indutivas desconectadas abruptamente.
10. O **LED + R6** sinalizam que a fonte está energizada, independentemente da tensão de saída configurada.

---

## 💰 Lista de Materiais e Custos

| Código | Componente | Descrição | Qtd. | Valor Unit. (R$) | Total (R$) |
|---|---|---|---|---|---|
| — | Transformador | 18V CA / 1,5A | 1 | R$ — | R$ — |
| — | Diodo 1N4007 | 1A / 1000V | 4 | R$ — | R$ — |
| — | Capacitor eletrolítico | 220 µF / 35V | 1 | R$ — | R$ — |
| — | Capacitor cerâmico | 100 nF | 1 | R$ — | R$ — |
| — | Diodo Zener | 15V / 1W | 1 | R$ — | R$ — |
| — | Diodo IN5408 | 3A / 1000V | 1 | R$ — | R$ — |
| — | Transistor BD137 | NPN / TO-126 | 1 | R$ — | R$ — |
| — | Transistor TIP42C | PNP / TO-220 | 1 | R$ — | R$ — |
| — | Resistor 2,2kΩ | ¼W / 5% | 1 | R$ — | R$ — |
| — | Resistor 470Ω | ¼W / 5% | 1 | R$ — | R$ — |
| — | Resistor 1kΩ (R6) | ¼W / 5% | 1 | R$ — | R$ — |
| — | Resistor 1kΩ (R5) | **3W** | 1 | R$ — | R$ — |
| — | Potenciômetro 10kΩ | Linear | 1 | R$ — | R$ — |
| — | Potenciômetro 5kΩ | Linear | 1 | R$ — | R$ — |
| — | LED 5mm | Qualquer cor | 1 | R$ — | R$ — |
| — | Dissipador térmico | $R_{\theta sa} \leq 1\text{ °C/W}$ + pasta térmica | 1 | R$ — | R$ — |
| — | Conector de saída | Borne / Banana Jack | 1 | R$ — | R$ — |
| — | PCB / Protoboard | — | 1 | R$ — | R$ — |
| | | | | **TOTAL** | **R$ —** |

> 🛒 Loja de referência: 
