# Fonte de Bancada Ajustável 0–15V / 1,5A com Monitoramento Digital

> Fonte linear regulável, ajuste independente de tensão (0–15V) e corrente (0–1,5A), operação automática CV/CC, com monitoramento digital via INA219 + Arduino Nano + LCD 16x2.
>
> **Disciplina:** Eletrônica Para Computação — [SSC0180] &nbsp;|&nbsp; **Docente:** Eduardo do Valle Simões

---

## Objetivo

Projetar, simular, calcular e montar uma fonte linear regulável que converte 220V CA / 60Hz da rede em CC ajustável de 0–15V, corrente máxima 1,5A com limite ajustável, e exibir tensão/corrente de saída em tempo real via sistema de monitoramento digital independente. 

## Topologia do Circuito

1. **Transformador** — 220V CA → 18V CA, isolamento galvânico, externo à placa (conector JP1).
   
2. **Ponte retificadora** — módulo integrado `PONTE_RET`, equivalente a 4 diodos em ponte.
   
3. **Capacitor de 4700µF** (eletrolítico) — filtro principal de ripple.
   
4. **Capacitor de 100nF** (cerâmico) — bypass de alta frequência.
   
5. **Referência de tensão** — diodo Zener de 15V, polarizado por um resistor de 2kΩ.
    
6. **Transistor BD139** (NPN) — driver do regulador.
    
7. **Transistor TIP36C** (PNP) — transistor de passagem (elemento série).
    
8. **Potenciômetro de 10kΩ** (linear) — controle contínuo da tensão de saída (0–15V).
    
9. **Potenciômetro de 5kΩ** (linear) — limitação ajustável de corrente (0–1,5A).
    
10. **Resistor de 470Ω** — bias do transistor BD139, para estabilidade do ponto de operação.
    
11. **Resistor de 4,4kΩ** — bleeder (carga mínima permanente).
    
12. **Diodo 1N5408** (3A/1000V) — proteção contra picos de back-EMF.
    
13. **LED + resistor de 2,2kΩ** — indicador de que a fonte está energizada.
    
14. **Conversor buck LM2596** (módulo, +5V fixo) — derivado do barramento CC bruto, alimenta exclusivamente o sistema de monitoramento (e uma saída USB auxiliar).
    
15. **Sistema de monitoramento** — sensor INA219 (I2C) + Arduino Nano + display LCD 16x2.
    
16. **Conector de saída** — bornes `PIN_VAR+`/`PIN_VAR-` e/ou jack USB.


### Imagens do Circuito

**I) Simulação no Falstad:**
- 🔗 [Simulação interativa no Falstad](https://tinyurl.com/23xghob4)

<img width="922" height="358" alt="image" src="https://github.com/MathFerreiraDev/fonte_tensao_corrente_regulaveis/blob/main/assets/fonte.png?raw=true" />

---

**II) Circuito físico implementado:**

| 📸 Protótipo Montado |
| :---: |
| **Circuito na Protoboard** |
| <img src="https://github.com/MathFerreiraDev/fonte_tensao_corrente_regulaveis/blob/main/assets/ft1.jpg?raw=true" width="900" /> |
| <img src="https://github.com/MathFerreiraDev/fonte_tensao_corrente_regulaveis/blob/main/assets/ft2.jpg?raw=true" width="900" /> |
| **Vídeo Explicativo** |
| <a href="https://www.youtube.com/watch?v=gZYxSjBT_20"> <img width="1920" height="1080" alt="thumb" src="https://github.com/user-attachments/assets/b953d018-7a13-4838-95b2-d2f6e0dd6fc9" /> </a> |

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

## Cálculos

> Rede elétrica: 220V CA / 60Hz. 

### 1. Tensão de barramento CC (pós-retificação)

$$V_{RMS}=18\text{V}\text{ (secundário)}\quad\rightarrow\quad V_{pico}=V_{RMS}\times\sqrt2\approx25{,}46\text{V}$$

Após a queda de tensão na ponte retificadora, o barramento CC bruto resulta em:

$$V_{CC}\approx\boxed{24{,}3\text{V}}$$

— valor usado como referência em todos os cálculos seguintes. Na simulação, com os potenciômetros próximos do máximo, mediu-se **14,812V** de saída, validando o teto teórico de 15V do diodo Zener.

### 2. Ripple e dimensionamento do capacitor de filtro (4700µF)

Frequência do ripple (onda completa): $2f_{rede}=120\text{Hz}$.

$$\Delta V_{ripple}=\frac{I_{carga}}{2f_{rede}\cdot C}$$

Para ripple-alvo de 10% a plena carga (1,5A): $\Delta V_{alvo}=0{,}10\times24{,}3\approx2{,}43\text{V}$

$$C_{min}=\frac{I_{carga}}{2f\cdot\Delta V_{alvo}}=\frac{1{,}5}{2\times60\times2{,}43}\approx\boxed{5{,}14\text{mF}}$$

**O capacitor de 4700µF instalado fica bem próximo desse mínimo, garantindo boa filtragem mesmo em carga máxima.**

**Ripple real com o capacitor de 4700µF instalado:**

| $I_{carga}$ | $\Delta V_{ripple}$ | % de $V_{CC}$ |
|---|---|---|
| 100 mA | ≈0,18 V | ≈0,7% |
| 500 mA | ≈0,89 V | ≈3,7% |
| 1,0 A | ≈1,77 V | ≈7,3% |
| 1,5 A | ≈2,66 V | ≈11,0% |

**Headroom do regulador** ($V_{dropout}\approx3\text{V}$): o piso instantâneo do barramento precisa satisfazer

$$V_{CC,min}=V_{CC}-\Delta V_{ripple}\geq V_{saida}+V_{dropout}$$

Corrente máxima que o capacitor de 4700µF sustenta sem prejudicar a regulação:

$$I_{max}=\Delta V_{max}\times2f\times C,\qquad \Delta V_{max}=V_{CC}-V_{saida}-V_{dropout}$$

| $V_{saida}$ | $\Delta V_{max}$ disponível | $I_{max}$ (capacitor de 4700µF) |
|---|---|---|
| 15 V | 6,3 V | ≈3,55 A |
| 12 V | 9,3 V | ≈5,25 A |
| 5 V | 16,3 V | ≈9,19 A |
| 0 V | 21,3 V | ≈12,01 A |

Em toda a faixa, $I_{max}$ é muito superior aos 1,5A de projeto — regulação garantida em qualquer ponto de operação.


### 3. Dissipação de potência no transistor TIP36C

$$P_{TIP36C}=(V_{CC}-V_{saida})\times I_{carga}$$


| $V_{saida}$ | $I_{carga}$ | $P_{TIP36C}$ |
|---|---|---|
| **0 V** | **1,5 A** | **≈36,45 W** ← pior caso absoluto |
| 5 V | 1,5 A | ≈28,95 W |
| 7,5 V | 1,5 A | ≈25,20 W |
| 12 V | 1,5 A | ≈18,45 W |
| 15 V | 1,5 A | ≈13,95 W |

⚠️ O pior caso ocorre com **saída em 0V** (não na saída máxima), pois toda a tensão do barramento cai sobre o TIP36C. O TIP36C (25A/100V, $P_{tot}$=125W @ $T_{case}$=25°C) tem margem confortável.


### 4. Resistor bleeder (4,4kΩ) — verificação de carga mínima

$$I_{bleeder}=\frac{V_{saida,max}}{4400\Omega}=\frac{15\text{V}}{4400\Omega}\approx\boxed{3{,}4\text{mA}}$$

$$P_{bleeder,max}=\frac{V_{saida,max}^2}{4400\Omega}=\frac{15^2}{4400}\approx\boxed{51\text{mW}}$$

O resistor bleeder consome apenas ≈0,23% da corrente máxima de saída (1,5A); com potência dissipada tão baixa, um resistor comum de 1/4W atende com folga.

---

## Explicação Técnica dos Componentes

- **Transformador** (18V/≥1,5A): reduz 220V→18V CA, servindo como alimentação da fonte.
  
- **Ponte retificadora** reponsável por transformar a corrente alternada 18V CA em CC amplificada em =24.3V

- **LED + resistor de 2,2kΩ**: ligado na saída da ponte retificadora (antes do regulador); acende sempre que a fonte estiver energizada, mesmo com saída em 0V. O resistor limita a corrente do LED a um valor seguro.
  
- **Capacitor de 4700µF** (eletrolítico): reservatório de energia principal, suaviza o ripple de 120Hz; valor instalado próximo do mínimo calculado. 
  
- **Capacitor de 100nF** (cerâmico): filtra ruído de alta frequência em paralelo com o capacitor de 4700µF.
  
- **Diodo Zener de 15V + resistor de 2kΩ**: o Zener, em avalanche reversa, define o teto absoluto da saída (15V); o resistor de 2kΩ limita a corrente do Zener a ~4,65mA.
  
- **Par BD139 + TIP36C** — núcleo da regulação,  o TIP36C é o elemento série entre o barramento bruto (~24,3V) e a saída — quanto mais corrente de base o BD139 fornece, mais o TIP36C conduz e menor sua queda, elevando a saída; dissipa toda a diferença entrada-saída como calor (por isso o dissipador); rating de 25A/125W dá boa margem frente ao pior caso do projeto (~36,45W). O BD139 amplifica o sinal de erro do laço e aciona a base do TIP36C; NPN de 80V/1,5A, folgado frente aos ~24,3V do barramento. 
  
- **Potenciômetro de 10kΩ** (controle de tensão): divisor ajustável; muda a fração da saída realimentada à base do BD139, deslocando o ponto de equilíbrio e ajustando a saída de forma contínua entre 0–15V.
  
- **Potenciômetro de 5kΩ** (limite de corrente): quando a carga atinge a corrente ajustada nele, o circuito de limitação reduz progressivamente a condução do BD139 e do TIP36C, travando a corrente e deixando a tensão cair livremente.
  
- **Resistor de 470Ω**: fixa o BD139, mantendo-o na região ativa em toda a faixa de ajuste dos dois potenciômetros; sem ele, o BD139 poderia saturar, instabilizando a regulação.
  
- **Resistor de 4,4kΩ** (bleeder): mantém ~3,4mA permanentes mesmo sem carga, cumprindo duas funções: (1) estabilidade do laço em circuito aberto (sem ele, a saída pode flutuar ou subir sem controle) e (2) descarga segura do capacitor de 4700µF ao desligar.
  
- **Diodo 1N5408** (3A/1000V): protege o TIP36C contra picos.

---

## Sistema de Monitoramento (INA219 + Arduino Nano + LCD 16x2)

Domínio elétrico **independente** do laço de regulação principal:

- O **LM2596** deriva do barramento bruto (~24,3V, o mesmo nó que alimenta o TIP36C) um rail fixo de **+5V**, única alimentação de INA219, Arduino Nano e LCD 16x2; o mesmo rail alimenta também um conector USB auxiliar (`USB_OUT`), independente da saída ajustável. Por ser um domínio separado, o consumo dessa eletrônica não onera o resistor bleeder nem os dois potenciômetros, e instabilidades na malha 0–15V não afetam o display.
  
- **INA219**: sensor I2C de tensão/corrente (até 26V, ±3,2A via shunt); seus pinos VIN+/VIN- ficam em série com `PIN_VAR+`, medindo simultaneamente tensão e corrente reais da carga; comunica-se via I2C (SDA/SCL), alimentado pelo rail de 5V.
  
- **Arduino Nano** (ATmega328P): lê o INA219 via I2C, calcula potência e formata os valores, exibindo tensão/corrente em tempo real no **LCD 16x2** — dispensa multímetro externo; atualização contínua conforme carga ou ajuste dos potenciômetros.
  
---

## Modos de Operação e Comportamentos Importantes

**1. CV × CC** (transição automática, sem chaves/relés):
| Modo | Condição | Regulado | Varia |
|---|---|---|---|
| CV | $I_{carga}<I_{limite}$ (potenciômetro de 5kΩ) | Tensão (via potenciômetro de 10kΩ) | Corrente (conforme a carga) |
| CC | $I_{carga}\geq I_{limite}$ (potenciômetro de 5kΩ) | Corrente (via potenciômetro de 5kΩ) | Tensão (cai livremente) |

**2. Queda de tensão ao reduzir o limite de corrente:** $V_{saida}=I_{limitada}\times R_{carga}$ — com o limite reduzido no potenciômetro de 5kΩ e carga fixa, a tensão cai necessariamente; independentemente do ajuste do potenciômetro de tensão.

**3. Cargas com controlador de carga (ex.: smartphones):** o PMIC do dispositivo gerencia sua própria corrente. Se o limite ajustado no potenciômetro de 5kΩ for maior que o consumo desejado → fonte em CV, o potenciômetro de corrente não interfere (a fonte só define um teto, nunca empurra corrente). Se o limite for menor que o consumo desejado → fonte entra em CC, tensão cai, carregamento fica lento ou para.

**4. Dissipação/eficiência:** $\eta=V_{saida}/V_{CC}=V_{saida}/24{,}3$ (potência de entrada e saída têm a mesma corrente num regulador série).
| $V_{saida}$ | Eficiência |
|---|---|
| 15 V | ≈62% |
| 12 V | ≈49% |
| 5 V | ≈21% |
| 0 V | 0% |

**5. Ripple residual:** o laço ativo (BD139/TIP36C) rejeita variações de entrada continuamente.
| Parâmetro | Característica |
|---|---|
| Frequência do ripple | 120Hz |
| Regulação de linha | Boa |
| Regulação de carga | Boa |
| Ripple residual na saída | Muito baixo |

**6. Dropout e teto de saída:** o par BD139/TIP36C precisa de $V_{dropout}\approx2$–$3\text{V}$ entre entrada e saída para operar ativo. Com barramento ~24,3V e referência de 15V, a saída máxima estável é 15V; acima disso o Zener não sustenta mais a referência do laço.

**7. Sem carga (circuito aberto):** o resistor de 4,4kΩ garante ~3,4mA mínimos, mantendo os transistores no ponto de operação correto.
