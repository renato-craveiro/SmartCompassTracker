#include <Wire.h>
#include "MPUManager.h"
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "utils.h"
#include <Arduino.h>
#include <math.h>
#include "StepsManager.h"
#include "EEPROMManager.h"



namespace MPUManager {
    Adafruit_MPU6050 mpu;
    unsigned long lastStepTime = 0;
    //int stepCount = 0;
    float stepLength = 0.7; // meters per step (adjust for user)

    #define BUFFER_SIZE 20
    float accelBuffer[BUFFER_SIZE];
    int bufferIndex = 0;

    void init() {
        mpu.begin();

        mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
        mpu.setGyroRange(MPU6050_RANGE_500_DEG);
        mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

        Serial.println("MPU6050 ready!");
    }

    void update(){
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        // Magnitude da aceleração
        float mag = sqrt(a.acceleration.x * a.acceleration.x +
                        a.acceleration.y * a.acceleration.y +
                        a.acceleration.z * a.acceleration.z);

        // Guarda no buffer
        accelBuffer[bufferIndex] = mag;
        bufferIndex = (bufferIndex + 1) % BUFFER_SIZE;

        // Calcula média e desvio padrão do buffer
        float mean = 0;
        for (int i = 0; i < BUFFER_SIZE; i++) mean += accelBuffer[i];
        mean /= BUFFER_SIZE;

        float variance = 0;
        for (int i = 0; i < BUFFER_SIZE; i++) variance += (accelBuffer[i] - mean) * (accelBuffer[i] - mean);
        variance /= BUFFER_SIZE;
        float stddev = sqrt(variance);

        // Threshold dinâmico
        float threshold = mean + stddev * 1.0;

        // Deteção de passo (pico acima do threshold)
        unsigned long now = millis();
        if (mag > threshold && (now - lastStepTime) > 350) { // mínimo 350ms
            StepsManager::passosAtuais++;
            StepsManager::passosGlobais++;
            EEPROMManager::passosGlobais = StepsManager::passosGlobais; // atual
            //EEPROMManager::passosAtuais = StepsManager::passosAtuais;
            EEPROMManager::save();
            lastStepTime = now;
        }

        // Calcula distância
        float distance = (StepsManager::passosAtuais * stepLength) / 1000.0; // km
        float totalDistance = (StepsManager::passosGlobais * stepLength) / 1000.0; // km
        StepsManager::distanciaAtual = distance;
        StepsManager::distanciaGlobal = totalDistance;
        EEPROMManager::distanciaGlobal = StepsManager::distanciaGlobal;
        EEPROMManager::save();
    }

}