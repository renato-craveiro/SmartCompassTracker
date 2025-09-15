#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

Adafruit_MPU6050 mpu;

unsigned long lastStepTime = 0;
int stepCount = 0;
float stepLength = 0.7; // metros por passo (ajustar para pessoa)

#define BUFFER_SIZE 20
float accelBuffer[BUFFER_SIZE];
int bufferIndex = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(4, 5); // SDA=GPIO4, SCL=GPIO5

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Step Counter Init");
  display.display();
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(1000);
}

void loop() {
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
    stepCount++;
    lastStepTime = now;
  }

  // Calcula distância
  float distance = (stepCount * stepLength) / 1000.0; // km

  // Mostra no OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Steps:");
  display.println(stepCount);
  display.print("Dist: ");
  display.print(distance, 2);
  display.println(" km");
  display.display();

  delay(100);
}
