#include <AccelStepper.h>
#include <MultiStepper.h>

/*Motor que controla o eixo X*/
#define PINO_MOTOR_EIXOX_STEP 6
#define PINO_MOTOR_EIXOX_DIR 5
#define MOTOR_EIXO_X_VELOCIDADE 100 /*passos por segundo*/
#define MOTOR_EIXO_X_ACELERACAO 100 /*passos por segundo por segundo*/

#define PASSOS_PARA_O_PROXIMO_PONTO 10
/*Fim do motor eixo X*/

/*Motor que controla a punção*/
#define PINO_MOTOR_JOAO_STEP 4
#define PINO_MOTOR_JOAO_DIR 3
#define MOTOR_JOAO_VELOCIDADE 30 /*passos por segundo*/
#define MOTOR_JOAO_ACELERACAO 30 /*passos por segundo por segundo*/
/*Fim do motor joão*/

/*Motor que puxa o papel 1*/
#define PINO_MOTOR_PAPEL1_STEP 10
#define PINO_MOTOR_PAPEL1_DIR 12 /*não há*/
#define MOTOR_PAPEL1_VELOCIDADE 50 /*passos por segundo*/
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
  
  while (digitalRead(PINO_SENSOR_DE_FIM_DE_CURSO) == HIGH){
    if (!motor_eixo_x.isRunning()){
      motor_eixo_x.move(-20000);
    }
    motor_eixo_x.run();
  }
  motor_eixo_x.stop();
  while (motor_eixo_x.isRunning())
    motor_eixo_x.run();
  motor_eixo_x.setCurrentPosition(0);
  
  motor_eixo_x.move(20);
  motor_eixo_x.runToPosition();
  motor_eixo_x.setCurrentPosition(0);
  
  
}

void loop() {
  /*delay(1000);
  motor_joao.move(1200);
  motor_joao.runToPosition();*/
  /*motor_joao.setMaxSpeed(MOTOR_JOAO_VELOCIDADE);
  motor_joao.move(275);
  motor_joao.runToPosition();
  motor_joao.setMaxSpeed(MOTOR_JOAO_VELOCIDADE / 5);
  motor_joao.move(75);
  motor_joao.runToPosition();
  motor_joao.setMaxSpeed(MOTOR_JOAO_VELOCIDADE);
  motor_joao.move(50);
  motor_joao.runToPosition();*/
  motor_papel1.move(1500);
  motor_papel2.move(4500);
  while (motor_papel1.isRunning() || motor_papel2.isRunning()){
    motor_papel1.run();
    motor_papel2.run();
  }
}
