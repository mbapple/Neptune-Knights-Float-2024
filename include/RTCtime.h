#include <Arduino.h>

void setupRTCTime();
void resetTime();
String getRTCTime();
int timeInSeconds();
String convertSecondsToTime(int seconds);