#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <webots/led.h>
#include <stdio.h>


#define TIME_STEP 6


int main(int argc, char **argv)
{

  wb_robot_init();
  
    bool desviar_obstaculo = 0;
  
    //Definir os motores
    WbDeviceTag roda_direita, roda_esquerda;
    roda_direita = wb_robot_get_device("roda_direita");
    roda_esquerda = wb_robot_get_device("roda_esquerda");
    
    wb_motor_set_position(roda_direita, INFINITY);
    wb_motor_set_position(roda_esquerda, INFINITY);
  
    //Definir os ultrassons
    WbDeviceTag ultrassom1, ultrassom2, ultrassom3;
    ultrassom1 = wb_robot_get_device("ds_ultrassom1");
    ultrassom2 = wb_robot_get_device("ds_ultrassom2");
    ultrassom3 = wb_robot_get_device("ds_ultrassom3");

    //Ativar os ultrassons
    wb_distance_sensor_enable(ultrassom1, TIME_STEP);
    wb_distance_sensor_enable(ultrassom2, TIME_STEP);
    wb_distance_sensor_enable(ultrassom3, TIME_STEP);
    
  while (wb_robot_step(TIME_STEP) != -1)
  {
    //Definir os valores do ultrassom
    double distancia_ultrassom1, distancia_ultrassom2, distancia_ultrassom3;
    distancia_ultrassom1 = wb_distance_sensor_get_value(ultrassom1);
    distancia_ultrassom2 = wb_distance_sensor_get_value(ultrassom2);
    distancia_ultrassom3 = wb_distance_sensor_get_value(ultrassom3);
    
  
    if(desviar_obstaculo == 0)
    {
      if(distancia_ultrassom1 < 700.0 || distancia_ultrassom2 < 700.0)
      {
        desviar_obstaculo = 1;
      }
      else if(distancia_ultrassom3 < 700.0)
      {
        desviar_obstaculo = 2;
      }
    }
    else if (distancia_ultrassom1 > 900.0 && distancia_ultrassom2 > 900.0 && distancia_ultrassom3 > 900.0)
    {
      desviar_obstaculo = 0;
    }
    
    if (desviar_obstaculo == 1)
    {
      //Virar Direita
      wb_motor_set_velocity(roda_direita, -1.5);
      wb_motor_set_velocity(roda_esquerda, 3.0);
    
    }
    else if (desviar_obstaculo == 2)
    {
      //Virar Esquerda
      wb_motor_set_velocity(roda_direita, 3.0);
      wb_motor_set_velocity(roda_esquerda, -1.5);
    }
    else
    {
      //Ir para frente
      wb_motor_set_velocity(roda_direita, 4.0);
      wb_motor_set_velocity(roda_esquerda, 4.0);
      printf("1E: %0.1f | 2C: %0.1f | 3D: %0.1f\n", distancia_ultrassom1, distancia_ultrassom2, distancia_ultrassom3);
    }

  };

  
  wb_robot_cleanup();

  return 0;
}
