// Programa : EXPERIMENTO TEMPO REAL 
// Autor : WALNER DE OLIVEIRA
// 24/10/2017

#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>

// Programa : EXPERIMENTO TEMPO REAL 
// Autor : WALNER DE OLIVEIRA
// 24/10/2017

#include "Thread.h"
#include "ThreadController.h"
#include "Ultrasonic.h"
#include <Ultrasonic.h>


//Processos para controle de Ambinte de berço
ThreadController cpu;
Thread leituraSensor1;
Thread leituraSensor2;

//Portas para os sensores
//int pinoledAzul = 12; //Pino ligado ao led Azul
int pinoledverm = 2; //Pino ligado ao led vermelho
int pinSensorPIR = 9; //Pino ligado ao sensor presença  PIR
int valorSensorPIR = 0;
int acionamento = 0;  //Variavel para guardar valor do sensor , inicia com 0.

//=== INICIALIZANDO  == SENSOR ULTRASONICO PARA MEDIR DISTÂNCIA
//Define os pinos do Arduino ligados ao Trigger e Echo
#define PINO_TRG  7
#define PINO_ECHO 6

//Inicializa o sensor ultrasonico nos pinos especificados
Ultrasonic ultrasonic(PINO_TRG, PINO_ECHO);

//Funcoes principais
void setup()
//void inicializar()
{
    Serial.begin(9600);
    pinMode(pinSensorPIR,INPUT);//Define pino sensor presença pir como entrada
    pinMode(12, OUTPUT); // //Declara que o pino 5 do arduino é de Saída. Vai mandar dados, energia...


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
    Serial.print(cmMsec); //previsão da criança deitada ou o berço vazio a medida será entre 90cm e 70 cm

    if (cmMsec>15.00)  
        {
        Serial.println("  ");
        digitalWrite(12, LOW); 
        //digitalWrite(pinoledAzul, LOW);
        }
    else  //Caso seja detectado posição de não dormindo, aciona o led azul
        {
        Serial.println(" -------------------------LIGAR LED AZUL ------------- ");
        digitalWrite(12, HIGH); 
        //digitalWrite(pinoledAzul, HIGH);
        }
  
   //Aguarda 0,5 segundo e reinicia o processo
   delay(500);
}

void lerPresenca()
{
  //Lendo o valor do sensor PIR. Este sensor pode assumir 2 valores
  //1 quando detecta algum movimento e 0 quando não detecta.
  valorSensorPIR = digitalRead(pinSensorPIR);
   
  Serial.print("Valor do Sensor PIR: ");  
  Serial.println(valorSensorPIR);
   
  //Verificando se ocorreu detecção de movimentos
  if (valorSensorPIR == 1) {//Sem movimento, mantem led vermelho apagado
   // ligarAlarme();
  Serial.println(" +++++++++++++++++++++++++++LIGAR LED VERMELHO ++++++++++++ ");  
       //   digitalWrite(pinoledverm, LOW);


  } else { //Caso seja detectado um movimento, aciona o led vermelho ligado
   // desligarAlarme();
     Serial.println("              desligar led vermelho              ");  
     //  digitalWrite(pinoledverm, HIGH);
  }   

} 


