#include <Keypad.h>



/************************ DEFINIÇÃO DE SENHA ******************************* */

char* senha = "ABC123"; // instancia a senha

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

int botaoAbertura = 3;
int ledVermelho = 12;
int ledVerde = 13;
int releFechadura = 1; //led azul substituir por módulo relé
int buzzer = 2;
int tempoAberto = 2000;
int errado = 0; // Conta quantas vezes tentou um acesso errado
int registeringPassword = 0;

String passwordRegistered = "";


/***************************************************************************** */

void setup(){ 
  
  lockPort(); // diz se a porta está ou não trancada
  pinMode(ledVermelho,OUTPUT); 
  pinMode(ledVerde,OUTPUT);
  pinMode(releFechadura,OUTPUT);
  pinMode(buzzer,OUTPUT); // define os pinos de saída do arduino
  pinMode(botaoAbertura, INPUT);
  Serial.begin(9600);

}

void loop() {
  
  int buttonState = digitalRead(botaoAbertura);
  
  if(buttonState == HIGH){
    handlePortState();
    delay(100);
  }else{
    
  	checkPassword();
  }
  
}


void checkPassword(){
  char digit = keypad.getKey();

  
  if(digit != 0 && errado < 3) {
    keyboardSound();
    
    if(digit == '*' && registeringPassword == 0){
      registeringPassword = 1;
    }else if(digit == '#' && registeringPassword == 1){
     registeringPassword = 0;
     verifyPasswordIsCorrect();
    }else{
    	passwordRegistered += digit;
    }
    
    
  }
}

void verifyPasswordIsCorrect(){
  if(passwordRegistered == senha){
    handlePortState();
  }else{
    accessDenied();
  }
  passwordRegistered = "";
}



void keyboardSound(){
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
}


void activateAlarm(){
  digitalWrite(buzzer, HIGH);
  delay (5000);
  digitalWrite(buzzer, LOW);
  errado = 0;

}

void accessDenied(){
  errado++;
  if(errado < 3){
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(buzzer, LOW);
  } else{
    activateAlarm();
  }

}


void handlePortState(){
  accessGranted();
  delay (1000);
  lockPort();
} 


void accessGranted(){
  acessGrantedSound();
  digitalWrite(ledVermelho, LOW);
  digitalWrite(ledVerde, HIGH);
  digitalWrite(releFechadura, HIGH); // importante: para uso com rele de acionamento da fechadura HIGH deve ser subsstituído por LOW
  delay(tempoAberto);
  digitalWrite(releFechadura, HIGH);
  
}

void acessGrantedSound(){
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  
}

void lockPort(){
  digitalWrite(ledVermelho, HIGH);
  digitalWrite(ledVerde, LOW);
  digitalWrite(releFechadura, LOW);// importante: para uso com rele de acionamento da fechadura LOW deve ser subsstituído por HIGH
}

  