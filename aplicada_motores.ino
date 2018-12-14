#include <AccelStepper.h>

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
#define PINO_MOTOR_PAPEL1_DIR 12 /*não há (pino desconectado)*/
#define MOTOR_PAPEL1_VELOCIDADE 100 /*passos por segundo*/
#define MOTOR_PAPEL1_ACELERACAO 50 /*passos por segundo por segundo*/
/*Fim do motor papel 1*/

/*Motor que puxa o papel 2*/
#define PINO_MOTOR_PAPEL2_STEP 2
#define PINO_MOTOR_PAPEL2_DIR 12 /*não há (pino desconectado)*/
#define MOTOR_PAPEL2_VELOCIDADE 150 /*passos por segundo*/
#define MOTOR_PAPEL2_ACELERACAO 50 /*passos por segundo por segundo*/
/*Fim do motor papel 2*/

#define PINO_SENSOR_DE_FIM_DE_CURSO 7 /*para descobrir a localização do zero*/

AccelStepper motor_eixo_x(AccelStepper::DRIVER, PINO_MOTOR_EIXOX_STEP, PINO_MOTOR_EIXOX_DIR);

AccelStepper motor_joao(AccelStepper::DRIVER, PINO_MOTOR_JOAO_STEP, PINO_MOTOR_JOAO_DIR);

AccelStepper motor_papel1(AccelStepper::DRIVER, PINO_MOTOR_PAPEL1_STEP, PINO_MOTOR_PAPEL1_DIR);

AccelStepper motor_papel2(AccelStepper::DRIVER, PINO_MOTOR_PAPEL2_STEP, PINO_MOTOR_PAPEL2_DIR);

short posicoes[54];

byte mini_linha[3][54] = {0};

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

  posicoes[0] = 0;
  for (int i = 1; i < 54; i++) {
    if ((i % 2) == 1) {
      posicoes[i] = posicoes[i - 1] + PASSOS_PARA_O_PROXIMO_PONTO;
    } else {
      posicoes[i] = posicoes[i - 1] + PASSOS_PARA_A_PROXIMA_LETRA;
    }
  }
  /*a posição de cada ponto já é computada no início do programa*/

  mini_linha[0][2] = 1;
  mini_linha[0][4] = 1;
  mini_linha[0][5] = 1;
  mini_linha[0][6] = 1;
  mini_linha[0][8] = 1;
  mini_linha[0][9] = 1;
  mini_linha[0][10] = 1;
  mini_linha[0][13] = 1;

  mini_linha[1][3] = 1;
  mini_linha[1][4] = 1;
  mini_linha[1][10] = 1;
  mini_linha[1][11] = 1;
  mini_linha[1][12] = 1;

  mini_linha[2][2] = 1;
  mini_linha[2][6] = 1;
  mini_linha[2][7] = 1;
  /*a impressora escreve " ofuchi" em braille*/
  /*                      ^                  */
  /*      com o espaço ---/                  */

  motor_papel1.move(1200);
  motor_papel2.move(1800);
  while (motor_papel1.isRunning() || motor_papel2.isRunning()) {
    motor_papel1.run();
    motor_papel2.run();
  }
  /*isso puxa a folha até o ponto necessário*/

  while (digitalRead(PINO_SENSOR_DE_FIM_DE_CURSO) == HIGH) {
    if (!motor_eixo_x.isRunning()) {
      motor_eixo_x.move(-20000);
    }
    motor_eixo_x.run();
  }
  motor_eixo_x.stop();
  while (motor_eixo_x.isRunning())
    motor_eixo_x.run();
  /*aqui encontramos o sensor de fim de curso*/

  motor_eixo_x.move(30);
  motor_eixo_x.runToPosition();
  motor_eixo_x.setCurrentPosition(0);
  /*início do eixo X real (após ajuste em relação ao fim de curso)*/
  
  delay(4000);
}

int cela_atual = 0;
int mini_linha_atual = 0;
int acabou = 0;

void loop() {
  if (acabou) {
    ;
    /*ao fim dessa impressão, a impressora nada faz; em versões futuras será aguardada a próxima impressão*/
  } else {
    if (mini_linha[mini_linha_atual][cela_atual] == 1) {
      /*se é necessário apertar o ponto atual, a impressora vai de posição atual para a posição do ponto e aperta*/
      motor_eixo_x.moveTo(posicoes[cela_atual]);
      motor_eixo_x.runToPosition();
      motor_joao.move(400);
      motor_joao.runToPosition();
    }
    cela_atual++;

    if (cela_atual == NUMERO_DE_CELAS_POR_LINHA) {
      cela_atual = 0;
      mini_linha_atual++;
      motor_eixo_x.moveTo(0);
      motor_papel1.move(18);
      motor_papel2.move(18);
      while (motor_papel1.isRunning() || motor_papel2.isRunning()) {
        motor_papel1.run();
        motor_papel2.run();
        motor_eixo_x.run();
      }
      /*desce a folha até a próxima minilinha enquanto o eixo X volta para o ponto 0*/
      if (mini_linha_atual == 3) {
        acabou = 1;
      }
      /*para essa impressão, só foi necessária uma linha*/
      /*para uma impressão com a extensão do computador, isso será mudado*/
      mini_linha_atual %= 3;
      if (mini_linha_atual == 0) {
        motor_papel1.move(30);
        motor_papel2.move(30);
        while (motor_papel1.isRunning() || motor_papel2.isRunning()) {
          motor_papel1.run();
          motor_papel2.run();
          motor_eixo_x.run();
        }
      }
      motor_eixo_x.runToPosition();
      /*eixo X anda o que falta para o ponto 0*/
      delay(1000);
    }
  }
}
