#include <Adafruit_LiquidCrystal.h>
#define btn1 2
#define btn2 3
#define led1 4
#define led2 5
#define buzz1 9
#define buzz2 10

Adafruit_LiquidCrystal lcd_1(0);

unsigned long int inicial[2] = {0, 0}; // guarda os valores do millis para comparação para ambas portas
float atual[2] = {0, 0}; // guarda o tempo atual para cada porta
int pessoas[2] = {0, 0}; // contadores de pessoas para cada porta
int botao[2] = {0, 0}; // botões para cada porta
float media[2] = {0, 0}; // variáveis para receber a média entre as 10 pessoas que passaram em cada porta
unsigned long int i[2] = {0, 0}; // utilizadas para separar cada grupo de 10 pessoas para cada porta

void conta_pessoas(int porta);
void verifica_premio(int porta);
void atualizar_lcd();

void setup()
{
  // Setando as portas
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buzz1, OUTPUT);
  pinMode(buzz2, OUTPUT);

  // Começando a tela
  lcd_1.begin(16, 2);
  lcd_1.setBacklight(1);
  
  // Printando pela primeira vez a interface
  atualizar_lcd();
}

void loop()
{
  // Leitura dos botões
  botao[0] = digitalRead(btn1);
  botao[1] = digitalRead(btn2);
  
  if (botao[0] == 0) {
    conta_pessoas(0); // Conta pessoas na porta 1
    verifica_premio(0); // Verifica prêmio na porta 1
  }
  
  if (botao[1] == 0) {
    conta_pessoas(1); // Conta pessoas na porta 2
    verifica_premio(1); // Verifica prêmio na porta 2
  }

  for (int j = 0; j < 2; j++) {
    if (pessoas[j] == 10) { // Verifica se 10 pessoas entraram
      media[j] = (atual[j] - i[j]) / 10; // Calcula a média
      i[j] = millis(); // Atualiza o tempo da última média
      lcd_1.setCursor(j == 0 ? 2 : 11, 1);
      lcd_1.print("E:    "); // Limpa o último número
      lcd_1.setCursor(j == 0 ? 2 : 11, 1);
      lcd_1.print(media[j] / 1000); // Mostra a média
      pessoas[j] = 0; // Reseta o contador
    }
  }
}

void conta_pessoas(int porta)
{
  pessoas[porta] += 1; // Adiciona 1 no contador de pessoas
  atual[porta] = millis() - inicial[porta]; // Calcula o tempo entre a passagem das pessoas
  inicial[porta] = millis(); // Guarda o tempo para utilização na próxima vez
  lcd_1.setCursor(porta == 0 ? 0 : 9, 0);
  lcd_1.print("E:    "); // Limpa o último número
  lcd_1.setCursor(porta == 0 ? 2 : 11, 0);
  lcd_1.print(atual[porta] / 1000); // Mostra o tempo atual

  while (botao[porta] != 1) { // Não deixa o código ser rodado várias vezes
    botao[porta] = digitalRead(porta == 0 ? btn1 : btn2);
  }
}

void verifica_premio(int porta)
{
  int ganhou = random(1, 11); // Realiza o sorteio do prêmio
  int ganhou_cinema = random(1, 31); // Realiza o sorteio do cinema
  
  if (ganhou == 1) {
    digitalWrite(porta == 0 ? led1 : led2, HIGH); // Indica que a pessoa ganhou o prêmio
    delay(500);
    digitalWrite(porta == 0 ? led1 : led2, LOW);
  }
  
  if (ganhou_cinema == 24) {
    tone(porta == 0 ? buzz1 : buzz2, 300, 500); // Indica que a pessoa ganhou o ingresso para o cinema
    delay(500);
    noTone(porta == 0 ? buzz1 : buzz2); // Desliga o buzzer
  }
}

void atualizar_lcd()
{
  lcd_1.setCursor(0, 0);
  lcd_1.print("E:0");
  lcd_1.setCursor(0, 1);
  lcd_1.print("M:0");
  lcd_1.setCursor(9, 0);
  lcd_1.print("E:0");
  lcd_1.setCursor(9, 1);
  lcd_1.print("M:0");
}
