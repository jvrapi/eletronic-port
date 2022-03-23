/* ******************** Controle de acesso por senha Manual do Mundo ********************
   Guia de conexão:
    
   Arduino:
   LINHA 1 TECLADO: Pino Digital 11
   LINHA 2 TECLADO: Pino Digital 10
   LINHA 3 TECLADO: Pino Digital 9
   LINHA 4 TECLADO: Pino Digital 8

   COLUNA 1 TECLADO: Pino Digital 7
   COLUNA 2 TECLADO: Pino Digital 6
   COLUNA 3 TECLADO: Pino Digital 5
   COLUNA 4 TECLADO: Pino Digital 4
   
   LED VERMELHO: Pino Digital 12
   LED VERDE: Pino Digital 13
   BUZZER: Pino Digital 2
   MÓDULO RELÊ: Pino Digital 3 (na simulação do Tinkercad temos um led azul no lugar do módulo relê, quando ligamos o módulo não precisamos usar o resistor)

   
  
   Este código utiliza a biblioteca Keypad
      
 ***************************************************************************** */

// Inclusão da biblioteca do módulo teclado
#include <Keypad.h>

/************************ DEFINIÇÃO DE SENHA ******************************* */

char* senha = "ABC123";  // defina a senha aqui, ela deve conter seis dígitos

/************************ CONFIGURAÇÕES TECLADO ******************************* */

const byte LINHAS = 4; // quantidade de linhas do teclado
const byte COLUNAS = 4; // quantidade de colunas do teclado

char digitos[LINHAS][COLUNAS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}}; // todos os caracteres do teclado

byte pinosLinha[LINHAS] = { 11, 10, 9, 8 }; // pinos de conexão das linhas no arduino 
byte pinosColuna[COLUNAS] = { 7, 6, 5, 4 }; // pinos de conexão das colunas no arduino 

Keypad keypad = Keypad( makeKeymap(digitos), pinosLinha, pinosColuna, LINHAS, COLUNAS ); // objeto de leitura das teclas com os parâmetros lidos

/************************ VARIÁVEIS AUXILIARES ******************************* */

int position = 0;
int botaoAbertura = 1;
int ledVermelho = 12;
int ledVerde = 13;
int releFechadura = 3; //led azul substituir por módulo relé
int buzzer = 2;
int tempoBuzz = 50;
int tempoAberto = 2000;

/***************************************************************************** */

void setup()
{ 
  
estadoPorta(true); // diz se a porta está ou não trancada

pinMode(ledVermelho,OUTPUT); 
pinMode(ledVerde,OUTPUT);
pinMode(releFechadura,OUTPUT);
pinMode(buzzer,OUTPUT); // define os pinos de saída do arduino
pinMode(botaoAbertura, INPUT);

}

void loop()
{
  
  if(digitalRead(botaoAbertura) == 1){
  estadoPorta(false);
    delay(100);
  }else{
  
char digito = keypad.getKey(); // faz a leitura das teclas

if (digito !=0)
{
  digitalWrite(buzzer, HIGH);
  delay(tempoBuzz);
  digitalWrite(buzzer, LOW); // acionamento do buzzer ao apertar qualquer tecla

if (digito == senha[position]) // verifica se o dígito apertado corresponde ao equivalente da senha
{
position ++;
}
else { // retorna a leitura para a primeira posição de leitura da senha quando o dígito apertado não corresponde ao correto 
position = 0;
}
if (position == 6) // altera estado da porta  
{
estadoPorta(false);
}
delay(100);
}
  }
}

void estadoPorta(int trancado)
{
  
if (trancado) // trancamento da porta
{
digitalWrite(ledVermelho, HIGH);
digitalWrite(ledVerde, LOW);
digitalWrite(releFechadura, LOW); // importante: para uso com rele de acionamento da fechadura LOW deve ser subsstituído por HIGH
}

else // abertura da porta
{
digitalWrite(ledVermelho, LOW);
digitalWrite(ledVerde, HIGH);
digitalWrite(releFechadura, HIGH); // importante: para uso com rele de acionamento da fechadura HIGH deve ser subsstituído por LOW
digitalWrite(buzzer, HIGH);
delay(tempoBuzz*10);
digitalWrite(buzzer, LOW); // buzzer indica o acerto da senha 
delay(tempoAberto);
position = 0;
digitalWrite(releFechadura, HIGH);
estadoPorta(true); // tranca a porta novamente 
}
} 