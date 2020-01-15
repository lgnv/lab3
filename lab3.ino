#include <Arduino.h>
#include <MD_TCS230.h>
#include <LedControl.h>

#define FirstRColumn 0
#define SecondRColumn 1

#define FirstGColumn 3
#define SecondGColumn 4

#define FirstBColumn 6
#define SecondBColumn 7

LedControl lc = LedControl(12, 10, 11, 1);
MD_TCS230 ColorSensor(4, 5, 2, 3);

void calibrate()
{
    sensorData whiteCalibration;
    whiteCalibration.value[TCS230_RGB_R] = 152580;
    whiteCalibration.value[TCS230_RGB_G] = 143030;
    whiteCalibration.value[TCS230_RGB_B] = 180470;
    sensorData blackCalibration;
    blackCalibration.value[TCS230_RGB_R] = 17370;
    blackCalibration.value[TCS230_RGB_G] = 15920;
    blackCalibration.value[TCS230_RGB_B] = 21170;

    ColorSensor.begin();
    ColorSensor.setDarkCal(&blackCalibration);
    ColorSensor.setWhiteCal(&whiteCalibration);
}

void setup()
{
    Serial.begin(115200);
    calibrate();
    lc.shutdown(0, false);
    lc.setIntensity(0, 16);
}

void loop() 
{
    colorData rgb;
    ColorSensor.read();
    while (!ColorSensor.available())
    ;
    ColorSensor.getRGB(&rgb);    
    lc.clearDisplay(0);
    fillLed(rgb.value[TCS230_RGB_R], rgb.value[TCS230_RGB_G], rgb.value[TCS230_RGB_B]);    
}

void fillLed(int r, int g, int b)
{
  fillColorComponent(FirstRColumn, SecondRColumn, r);
  fillColorComponent(FirstGColumn, SecondGColumn, g);
  fillColorComponent(FirstBColumn, SecondBColumn, b);
}

void fillColorComponent(int column1, int column2, int value){
    int countRow = value / 32;
    fillColumnOnLed(column1, 0, countRow);
    fillColumnOnLed(column2, 0, countRow);
}

void fillColumnOnLed(int column, int startNumber, int endNumber)
{
  for (int i = startNumber; i <= endNumber; i++)
  {
    lc.setLed(0, column, i, true);
  }
}
