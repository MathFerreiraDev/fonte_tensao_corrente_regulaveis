#include <Wire.h>
#include <Adafruit_INA219.h>
#include <LiquidCrystal_I2C.h>

const uint8_t INA219_I2C_ADDR = 0x40;

Adafruit_INA219 ina219(INA219_I2C_ADDR);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const uint8_t MEU_INA219_REG_CONFIG = 0x00;

void configurarOversamplingHardwareINA219() {
  uint16_t config = 0;
  config |= (1UL << 13);
  config |= (0b11 << 11);
  config |= (0b1110 << 7);
  config |= (0b1110 << 3);
  config |= 0b111;

  Wire.beginTransmission(INA219_I2C_ADDR);
  Wire.write(MEU_INA219_REG_CONFIG);
  Wire.write((uint8_t)(config >> 8));
  Wire.write((uint8_t)(config & 0xFF));
  Wire.endTransmission();
}

const int NUM_AMOSTRAS         = 5;
const int DESCARTE_PONTAS      = 1;
const int INTERVALO_AMOSTRA_MS = 8;

const float ALFA_EMA_LENTO  = 0.12;
const float ALFA_EMA_RAPIDO = 0.60;

const float LIMIAR_MUDANCA_TENSAO_V    = 0.15;
const float LIMIAR_MUDANCA_CORRENTE_MA = 30.0;
const float LIMIAR_MUDANCA_POTENCIA_MW = 150.0;

const float LIMIAR_ABRE_CA  = 5.0;
const float LIMIAR_FECHA_CA = 10.0;
const int   CONFIRMACOES_NECESSARIAS = 3;

float correnteFiltrada_EMA = 0;
float tensaoFiltrada_EMA   = 0;
float potenciaFiltrada_EMA = 0;
bool  primeiraLeitura = true;

bool circuitoAberto = true;
int  contadorConfirmacao = 0;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  Wire.begin();

  if (!ina219.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("Erro: sensor");
    lcd.setCursor(0, 1);
    lcd.print("nao encontrado");
    while (1) { delay(10); }
  }

  configurarOversamplingHardwareINA219();

  lcd.setCursor(0, 0);
  lcd.print("Iniciando...");
  delay(1000);
  lcd.clear();
}

void escreverLinha(int linha, String texto) {
  while (texto.length() < 16) texto += " ";
  lcd.setCursor(0, linha);
  lcd.print(texto);
}

float lerCorrenteMA() { return ina219.getCurrent_mA(); }
float lerTensaoCarga() {
  return ina219.getBusVoltage_V() + (ina219.getShuntVoltage_mV() / 1000.0);
}
float lerPotenciaMW() { return ina219.getPower_mW(); }

float amostraFiltrada(float (*leitura)()) {
  float amostras[NUM_AMOSTRAS];

  for (int i = 0; i < NUM_AMOSTRAS; i++) {
    amostras[i] = leitura();
    delay(INTERVALO_AMOSTRA_MS);
  }

  for (int i = 1; i < NUM_AMOSTRAS; i++) {
    float chave = amostras[i];
    int j = i - 1;
    while (j >= 0 && amostras[j] > chave) {
      amostras[j + 1] = amostras[j];
      j--;
    }
    amostras[j + 1] = chave;
  }

  float soma = 0;
  int usados = 0;
  for (int i = DESCARTE_PONTAS; i < NUM_AMOSTRAS - DESCARTE_PONTAS; i++) {
    soma += amostras[i];
    usados++;
  }

  return soma / usados;
}

float suavizarAdaptativo(float valorNovo, float valorAnteriorEMA, float limiarMudanca) {
  float diferenca = fabs(valorNovo - valorAnteriorEMA);
  float alfa = (diferenca > limiarMudanca) ? ALFA_EMA_RAPIDO : ALFA_EMA_LENTO;
  return (alfa * valorNovo) + ((1.0 - alfa) * valorAnteriorEMA);
}

void loop() {
  float correnteAmostrada = amostraFiltrada(lerCorrenteMA);
  float tensaoAmostrada   = amostraFiltrada(lerTensaoCarga);
  float potenciaAmostrada = amostraFiltrada(lerPotenciaMW);

  if (primeiraLeitura) {
    correnteFiltrada_EMA = correnteAmostrada;
    tensaoFiltrada_EMA   = tensaoAmostrada;
    potenciaFiltrada_EMA = potenciaAmostrada;
    primeiraLeitura = false;
  } else {
    correnteFiltrada_EMA = suavizarAdaptativo(correnteAmostrada, correnteFiltrada_EMA, LIMIAR_MUDANCA_CORRENTE_MA);
    tensaoFiltrada_EMA   = suavizarAdaptativo(tensaoAmostrada,   tensaoFiltrada_EMA,   LIMIAR_MUDANCA_TENSAO_V);
    potenciaFiltrada_EMA = suavizarAdaptativo(potenciaAmostrada, potenciaFiltrada_EMA, LIMIAR_MUDANCA_POTENCIA_MW);
  }

  bool sugestaoMudarEstado;
  if (circuitoAberto) {
    sugestaoMudarEstado = fabs(correnteFiltrada_EMA) > LIMIAR_FECHA_CA;
  } else {
    sugestaoMudarEstado = fabs(correnteFiltrada_EMA) < LIMIAR_ABRE_CA;
  }

  if (sugestaoMudarEstado) {
    contadorConfirmacao++;
    if (contadorConfirmacao >= CONFIRMACOES_NECESSARIAS) {
      circuitoAberto = !circuitoAberto;
      contadorConfirmacao = 0;
    }
  } else {
    contadorConfirmacao = 0;
  }

  escreverLinha(0, "Tensao:" + String(tensaoFiltrada_EMA, 2) + "V");

  if (circuitoAberto) {
    escreverLinha(1, "I:C.A  P:C.A");
  } else {
    float corrente_A = correnteFiltrada_EMA / 1000.0;
    float potencia_W = potenciaFiltrada_EMA / 1000.0;
    escreverLinha(1, "I:" + String(corrente_A, 2) + "A P:" + String(potencia_W, 1) + "W");
  }

  Serial.print("Tensao: "); Serial.print(tensaoFiltrada_EMA); Serial.print(" V | ");
  Serial.print("Corrente: "); Serial.print(correnteFiltrada_EMA); Serial.print(" mA | ");
  Serial.print("Potencia: "); Serial.print(potenciaFiltrada_EMA); Serial.println(" mW");

  delay(300);
}
