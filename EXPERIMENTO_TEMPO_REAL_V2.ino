// Programa : EXPERIMENTO TEMPO REAL 
// Autor : WALNER DE OLIVEIRA

#include "Thread.h"
#include "ThreadController.h"
#include "Ultrasonic.h"

//Processos para controle de Ambinte de berço
ThreadController cpu;
Thread leituraSensor1;
Thread leituraSensor2;

//Portas para os sensores
int pinoledAzul = 5; //Pino ligado ao led Azul
int pinoledverm = 2; //Pino ligado ao led vermelho
int pinopir = 3;  //Pino ligado ao sensor presença  PIR
int acionamento;  //Variavel para guardar valor do sensor

//=== INICIALIZANDO  == SENSOR ULTRASONICO PARA MEDIR DISTÂNCIA
//Define os pinos do Arduino ligados ao Trigger e Echo
#define PINO_TRG  7
#define PINO_ECHO 6

//Inicializa o sensor ultrasonico nos pinos especificados
Ultrasonic ultrasonic(PINO_TRG, PINO_ECHO);
void setup()
{
//Inicializa a serial
  Serial.begin(9600);
}



//===INICIALIZANDO == SENSOR PRESENÇA PIR
void setup(){
  pinMode(pinopir, INPUT);   //Define pino sensor presença pir como entrada
 // Serial.begin(9600);
}

//Funcoes

void lerDistancia()
{
    //Variaveis para guardar os valores em 
    //cm (cmSec) 
    float cmMsec;
  
    //Le os valores do sensor ultrasonico
    long microsec = ultrasonic.timing();
    //Atribui os valores em cm  as variaveis
    cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  
    //Mostra os valores na serial
    Serial.print("Centimetros: ");
    Serial.print(cmMsec);

    if (cmMsec>50.00)  
        {
        digitalWrite(pinoledAzul, LOW);
        }
    else  //Caso seja detectado posição de não dormindo, aciona o led azul
        {
        digitalWrite(pinoledAzul, HIGH);
        }
    }

  
   //Aguarda 1 segundo e reinicia o processo
   delay(1000);
}

void lerPresenca()
{
    acionamento = digitalRead(pinopir); //Le o valor do sensor PIR
 
    // Mostra o valor lido do PIR no monitor serial  
    Serial.print(" = SENSOR : "); 
    Serial.println(acionamento);  

    if (acionamento == LOW)  //Sem movimento, mantem led vermelho apagado
        {
        digitalWrite(pinoledverm, LOW);
        }
    else  //Caso seja detectado um movimento, aciona o led vermelho ligado
        {
        digitalWrite(pinoledverm, HIGH);
        }
    }


//Funcoes principais
void setup(){
 Serial.begin(9600);


 leituraSensor1.setInterval(300);
 leituraSensor1.onRun(lerPresenca);

 leituraSensor1.setInterval(200);
 leituraSensor2.onRun(lerDistancia); 
 
 
 cpu.add(&leituraSensor1);
 cpu.add(&leituraSensor2);
}

void loop(){
 cpu.run();
}
