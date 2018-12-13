#include <AccelStepper.h>
#include <MultiStepper.h>

/*Motor que controla o eixo X*/
#define PINO_MOTOR_EIXOX_STEP 6
#define PINO_MOTOR_EIXOX_DIR 5
#define MOTOR_EIXO_X_VELOCIDADE 100 /*passos por segundo*/
#define MOTOR_EIXO_X_ACELERACAO 400 /*passos por segundo por segundo*/

#define PASSOS_PARA_O_PROXIMO_PONTO 20
#define PASSOS_PARA_A_PROXIMA_LETRA 40

#define NUMERO_DE_CELAS_POR_LINHA 27
/*Fim do motor eixo X*/

/*Motor que controla a punção*/
#define PINO_MOTOR_JOAO_STEP 4
#define PINO_MOTOR_JOAO_DIR 3
#define MOTOR_JOAO_VELOCIDADE 400 /*passos por segundo*/
#define MOTOR_JOAO_ACELERACAO 3200 /*passos por segundo por segundo*/
/*Fim do motor joão*/

/*Motor que puxa o papel 1*/
#define PINO_MOTOR_PAPEL1_STEP 10
#define PINO_MOTOR_PAPEL1_DIR 12 /*não há*/
#define MOTOR_PAPEL1_VELOCIDADE 75 /*passos por segundo*/
#define MOTOR_PAPEL1_ACELERACAO 50 /*passos por segundo por segundo*/
/*Fim do motor papel 1*/

/*Motor que puxa o papel 2*/
#define PINO_MOTOR_PAPEL2_STEP 2
#define PINO_MOTOR_PAPEL2_DIR 12 /*não há*/
#define MOTOR_PAPEL2_VELOCIDADE 150 /*passos por segundo*/
#define MOTOR_PAPEL2_ACELERACAO 50 /*passos por segundo por segundo*/
/*Fim do motor papel 2*/

#define PINO_SENSOR_DE_FIM_DE_CURSO 7 /*para descobrir a localização do zero*/

AccelStepper motor_eixo_x(AccelStepper::DRIVER, PINO_MOTOR_EIXOX_STEP, PINO_MOTOR_EIXOX_DIR);

AccelStepper motor_joao(AccelStepper::DRIVER, PINO_MOTOR_JOAO_STEP, PINO_MOTOR_JOAO_DIR);

AccelStepper motor_papel1(AccelStepper::DRIVER, PINO_MOTOR_PAPEL1_STEP, PINO_MOTOR_PAPEL1_DIR);

AccelStepper motor_papel2(AccelStepper::DRIVER, PINO_MOTOR_PAPEL2_STEP, PINO_MOTOR_PAPEL2_DIR);

void setup() {
  pinMode(PINO_SENSOR_DE_FIM_DE_CURSO, INPUT_PULLUP);
  motor_eixo_x.setMaxSpeed(MOTOR_EIXO_X_VELOCIDADE);
  motor_eixo_x.setSpeed(MOTOR_EIXO_X_VELOCIDADE);
  motor_eixo_x.setAcceleration(MOTOR_EIXO_X_ACELERACAO);

  motor_joao.setMaxSpeed(MOTOR_JOAO_VELOCIDADE);
  motor_joao.setSpeed(MOTOR_JOAO_VELOCIDADE);
  motor_joao.setAcceleration(MOTOR_JOAO_ACELERACAO);

  motor_papel1.setMaxSpeed(MOTOR_PAPEL1_VELOCIDADE);
  motor_papel1.setSpeed(MOTOR_PAPEL1_VELOCIDADE);
  motor_papel1.setAcceleration(MOTOR_PAPEL1_ACELERACAO);

  motor_papel2.setMaxSpeed(MOTOR_PAPEL2_VELOCIDADE);
  motor_papel2.setSpeed(MOTOR_PAPEL2_VELOCIDADE);
  motor_papel2.setAcceleration(MOTOR_PAPEL2_ACELERACAO);
  
  /*motor_papel1.move(2250);
    motor_papel2.move(4500);
    while (motor_papel1.isRunning() || motor_papel2.isRunning()){
    motor_papel1.run();
    motor_papel2.run();
    }*/

  while (digitalRead(PINO_SENSOR_DE_FIM_DE_CURSO) == HIGH) {
    if (!motor_eixo_x.isRunning()) {
      motor_eixo_x.move(-20000);
    }
    motor_eixo_x.run();
  }
  motor_eixo_x.stop();
  while (motor_eixo_x.isRunning())
    motor_eixo_x.run();

  motor_eixo_x.move(10);
  motor_eixo_x.runToPosition();
  motor_eixo_x.setCurrentPosition(0);
  delay(1000);
  /*considerando que o joão começa na horizontal*/
  /*motor_joao.move(100);
  motor_joao.runToPosition();
  motor_joao.move(550);
  motor_joao.setMaxSpeed(MOTOR_JOAO_VELOCIDADE / 4);
  while (motor_joao.distanceToGo() > 400){
    motor_joao.run();
  }*/
}

int cela_atual = 0;
int mini_linha_atual = 0;

void loop() {
  if (Serial.available() > 0) {
    if (Serial.peek() == 0x5A) {
      Serial.write(0xA5);
    }
  }
}

void faz_o_urro() {
  cela_atual++;
  
  motor_joao.move(400);
  motor_joao.runToPosition();
  
  motor_eixo_x.move(PASSOS_PARA_O_PROXIMO_PONTO);
  motor_eixo_x.runToPosition();

  motor_joao.move(400);
  motor_joao.runToPosition();
  
  motor_eixo_x.move(PASSOS_PARA_A_PROXIMA_LETRA);
  motor_eixo_x.runToPosition();
  
  if (cela_atual == NUMERO_DE_CELAS_POR_LINHA) {
    cela_atual = 0;
    mini_linha_atual++;
    motor_eixo_x.moveTo(0);
    motor_papel1.move(10);
    motor_papel2.move(20);
    while (motor_papel1.isRunning() || motor_papel2.isRunning()){
      motor_papel1.run();
      motor_papel2.run();
      motor_eixo_x.run();
    }
    mini_linha_atual %= 3;
    if (mini_linha_atual == 0) {
      motor_papel1.move(15);
      motor_papel2.move(30);
      while (motor_papel1.isRunning() || motor_papel2.isRunning()){
        motor_papel1.run();
        motor_papel2.run();
        motor_eixo_x.run();
      }
    }
    motor_eixo_x.runToPosition();
    delay(1000);
  }
}
