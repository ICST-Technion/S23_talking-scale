#ifndef HELPER_H
#define HELPER_H

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#endif

#include <SoftwareSerial.h>
#include "time.h"
#include "worker.h"

#define greenPin 12
#define redPin 32

bool start;
char c;
float unit = -1;
float goal = -1;

int start_time[3] = { 0 };
int curr_time[3] = { 0 };

//FUNCTIONS//
int getTimer(int* start_time, int* end_time);
int stringToNum(String str);
String getDate();
String getTime(int* time_arr);
float getNum(String string);
void ConnectToWifi();

/* 1. Define the WiFi credentials */

//#define WIFI_SSID "Migdalor_WIFI"
//#define WIFI_PASSWORD "1122889900"

//#define WIFI_SSID "Karam"
//#define WIFI_PASSWORD "karam1234"

//#define WIFI_SSID "AndroidAP"
//#define WIFI_PASSWORD "sami0303"

#define WIFI_SSID "Kamil"
#define WIFI_PASSWORD "kamil196"

/******************************************************************/
float getNum(String string) {
  int i = 0;
  float sum = 0;
  char first = string[0];
  while (string[i] > '9' || string[i] < '0') {
    i++;
  }
  while (string[i] <= '9' && string[i] >= '0') {
    sum = 10 * sum + string[i] - '0';
    i++;
  }
  if (string[i] == '.') {
    sum = sum + (string[i + 1] - '0') / 10.0;
  }
  if (first == '-') sum *= -1;
  return sum;
}

/******************************************************************/
String getDate() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "Failed to obtain time";
  }
  //  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  uint8_t day = timeinfo.tm_mday;
  String dayStr = day < 10 ? StringSumHelper("0") + String(day) : String(day);

  uint8_t month = timeinfo.tm_mon + 1;
  String monthStr = month < 10 ? StringSumHelper("0") + String(month) : String(month);

  uint16_t year = timeinfo.tm_year + 1900;
  String yearStr = String(year);

  return StringSumHelper(dayStr) + StringSumHelper(".") + monthStr + StringSumHelper(".") + yearStr;
}

/******************************************************************/
String getTime(int* time_arr) {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "Failed to obtain time";
  }
  String hour_str = String(timeinfo.tm_hour);
  time_arr[0] = stringToNum(hour_str);
  String minute_str = String(timeinfo.tm_min);
  time_arr[1] = stringToNum(minute_str);
  String sec_str = String(timeinfo.tm_sec);
  time_arr[2] = stringToNum(sec_str);

  return StringSumHelper(hour_str) + StringSumHelper(":") + minute_str + StringSumHelper(":") + sec_str;
}

/******************************************************************/
int stringToNum(String str) {
    int num = 0, len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    if (len == 2)
    {
        num += (str[0] - '0') * 10;
        num += (str[1] - '0');
    } 
    else 
    {
        num += (str[0] - '0');
    }
    return num;
}

/******************************************************************/
int getTimer(int* start_time, int* end_time) {
  int timer_in_seconds = 0;
  timer_in_seconds += 3600 * (end_time[0] - start_time[0]);  //hours
  timer_in_seconds += 60 * (end_time[1] - start_time[1]);    //minutes
  timer_in_seconds += (end_time[2] - start_time[2]);         //seconds

  return timer_in_seconds;
}

/******************************************************************/

void ConnectToWifi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Attempting to connect to WEP network, SSID: ");
    Serial.println(WIFI_SSID);
    while (WiFi.status() != WL_CONNECTED)
    {
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, HIGH);
        delay(300);
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, LOW);
        delay(300);
    }
    Serial.print("You're connected to the network");
}

#endif //HELPER_H