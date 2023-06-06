#ifndef AUDIO_H
#define AUDIO_H

#include "scale.h"
#include "helper.h"
#include "firestore.h"
#include <SoftwareSerial.h>

class Audio{
    public:
        int8_t play_indx_song[6];
    
    public:
        Audio();
        void SayArabic(int num);
        void SayHebrew(int num);
        void SayRussian(int num);
        //void SayWord(int x, String language);
        void SayIntro(String language);
        void SendCommandToMP3Player(int8_t command[], int len);
        void SayNum(int num, String language, bool justNum);
        void PlayEmailAndPasswordAreNotAuthenticated();
        void PlayCantCollectDataFromDB(bool& first_time_played, bool& start);
        void PlayShouldStartNewTask(float curr_val, bool& isNextJobReady, Worker& worker);
        void PlayGoodBye(Worker& worker);
        int ReadResponseFromMP3Player(int8_t response[], int max_len);
        void PlayConnectingToWiFi(String language);
        void PlayPutYourChip(String language);
        void PlayConnectingToWorkerData(String language);
        
        //void WaitUntilAudioEnded();
        void PlayCombine();

};

/******************************************************************/
Audio::Audio(){
    play_indx_song[0] = 0x7e;
    play_indx_song[1] = 0x04;
    play_indx_song[2] = 0x42;
    play_indx_song[3] = 0x02;
    play_indx_song[4] = 101;
    play_indx_song[5] = (int8_t)0xef;
}
/******************************************************************/
void Audio::SayArabic(int num)
{
    bool first = true;
    if (num / 100 > 0)
    {
        play_indx_song[4] = 100 + num / 100;
        SendCommandToMP3Player(play_indx_song, 6);
        first = false;
        delay(250);
    }
    num = num % 100;
    if (num / 10 == 1) 
    {
        if (first) 
        {
            play_indx_song[4] = num;
            SendCommandToMP3Player(play_indx_song, 6);
            delay(500);
            return;
        } 
        else
        {
            play_indx_song[4] = num + 200;
            SendCommandToMP3Player(play_indx_song, 6);
            delay(500);
            return;
        }
    }
    if (num % 10 > 0) 
    {
        if (first) 
        {
            first = false;
            play_indx_song[4] = (num % 10);
            SendCommandToMP3Player(play_indx_song, 6);
            delay(250);
        } 
        else
        {
            play_indx_song[4] = (num % 10 + 200);
            SendCommandToMP3Player(play_indx_song, 6);
            delay(250);
        }
    }

    if (num / 10 > 0) 
    {
        if (first) 
        {
            play_indx_song[4] = (num / 10) * 10;
            SendCommandToMP3Player(play_indx_song, 6);
            delay(250);
        } 
        else 
        {
            play_indx_song[4] = (num / 10) + 220;
            SendCommandToMP3Player(play_indx_song, 6);
            delay(300);
        }
    }
}

/*****************************************************************/
void Audio::SayHebrew(int num)
{
    if (num / 100 > 0){
      play_indx_song[4] = (num) - (num%100);
      SendCommandToMP3Player(play_indx_song, 6);
      if(play_indx_song[4] == 100 || play_indx_song[4] == 200){
        delay(700);
      }
      else{
        delay(1200);
      }
      num = num % 100;
    }
    play_indx_song[4] = num;
    SendCommandToMP3Player(play_indx_song, 6);
    delay(500); 
    return;

    if (num < 10 && num > 0) {
      play_indx_song[4] = num + 20;
      SendCommandToMP3Player(play_indx_song, 6);
      delay(250);
      return;
    }
    if (num / 100 > 0) {
      play_indx_song[4] = 100 + num / 100;
      SendCommandToMP3Player(play_indx_song, 6);
      delay(250);
    }
    num = num % 100;
    if (num / 10 > 1) {
      play_indx_song[4] = 10 * (num / 10);
      SendCommandToMP3Player(play_indx_song, 6);
      delay(250);
    }
    if (num / 10 == 1) {
      play_indx_song[4] = num;
      SendCommandToMP3Player(play_indx_song, 6);
      delay(500);
      return;
    }
    num = num % 10;
    if (num > 0) {
      play_indx_song[4] = num;
      SendCommandToMP3Player(play_indx_song, 6);
      delay(250);
    }
}

/*****************************************************************/
void Audio::SayRussian(int num)
{
    if (num / 100 > 0)
    {
        play_indx_song[4] = 100 + num / 100;
        SendCommandToMP3Player(play_indx_song, 6);
        delay(250);
    }
    num = num % 100;
    if (num / 10 > 1)
    {
        play_indx_song[4] = 10 * (num / 10);
        SendCommandToMP3Player(play_indx_song, 6);
        delay(250);
    }
    if (num / 10 == 1)
    {
        play_indx_song[4] = num;
        SendCommandToMP3Player(play_indx_song, 6);
        delay(500);
        return;
    }

    num = num % 10;
    if (num > 0)
    {
        play_indx_song[4] = num;
        SendCommandToMP3Player(play_indx_song, 6);
        delay(250);
    }
}

/******************************************************************/
void Audio::SayIntro(String language)
{
    play_indx_song[3] = 30;
    play_indx_song[4] = 19;
    SendCommandToMP3Player(play_indx_song, 6);
    delay(4000);

    SayNum(int(goal), language, true);
    delay(1000);
    
    play_indx_song[3] = 30;
    play_indx_song[4] = 14;
    SendCommandToMP3Player(play_indx_song, 6);
    delay(950);

    SayNum(int(goal / unit), language, true);
    delay(900);

    play_indx_song[3] = 30;
    play_indx_song[4] = 15;
    SendCommandToMP3Player(play_indx_song, 6);
    delay(950);
}

/******************************************************************/
void Audio::SendCommandToMP3Player(int8_t command[], int len)
{
    Serial.print("\nMP3 Command => ");
    for (int i = 0; i < len; i++)
    {
        Serial1.write(command[i]);
        Serial.print(command[i], HEX);
    }
    //delay(1000);
}

/******************************************************************/
void Audio::SayNum(int num, String language, bool justNum) {
  if (language == "Arabic")
  {
      play_indx_song[3] = 1;
      SayArabic(num);
  }
  if (language == "Hebrew")
  {
      play_indx_song[3] = 40;
      SayHebrew(num);
  }
  if (language == "Russian")
  {
      play_indx_song[3] = 3;
      SayRussian(num);
  }
}

/******************************************************************/
void Audio::PlayEmailAndPasswordAreNotAuthenticated(){
    play_indx_song[3] = 10;
    play_indx_song[4] = 0;
    this->SendCommandToMP3Player(play_indx_song, 6);
    delay(5000);
}

/******************************************************************/
void Audio::PlayCantCollectDataFromDB(bool& first_time_played, bool& start){
    play_indx_song[3] = 10;
    play_indx_song[4] = 10;
    this->SendCommandToMP3Player(play_indx_song, 6);
    start = false;
    first_time_played = true;
    digitalWrite(redPin, LOW);
    unit = -1;
    delay(5000);
}

/******************************************************************/
void Audio::PlayShouldStartNewTask(float curr_val, bool& isNextJobReady,Worker& worker)
{
    isNextJobReady = true;
    Serial.print("waiting -- ");
    Serial.println(curr_val);
    play_indx_song[3] = 10;
    play_indx_song[4] = 7;
    if (worker.language == "Hebrew") play_indx_song[4] = 17;
    if (worker.language == "Russian") play_indx_song[4] = 27;
    this->SendCommandToMP3Player(play_indx_song, 6);
    GetTime(start_time);
    delay(1500);
}

/******************************************************************/
void Audio::PlayGoodBye(Worker& worker){
    play_indx_song[3] = 10;
    play_indx_song[4] = 8;
    if (worker.language == "Hebrew") play_indx_song[4] = 18;
    if (worker.language == "Russian") play_indx_song[4] = 28;
    this->SendCommandToMP3Player(play_indx_song, 6);
    delay(3000);
}

/******************************************************************/
int Audio::ReadResponseFromMP3Player(int8_t response[], int max_len)
{
    int len = 0;
    while (Serial1.available() && len < max_len)
    {
        response[len++] = Serial1.read();
        delay(2); // Delay to prevent buffer overflow
    }
    return len;
}

/******************************************************************/
void Audio::PlayConnectingToWiFi(String language)
{
  if (language == "Arabic")
  {
    play_indx_song[3] = 20;
    play_indx_song[4] = 01;
    this->SendCommandToMP3Player(play_indx_song, 6);
    delay(1200);
  }
  if (language == "Hebrew")
  {
    play_indx_song[3] = 20;
    play_indx_song[4] = 11;
    this->SendCommandToMP3Player(play_indx_song, 6);
    delay(1200);
  }
  if (language == "Russian")
  {
    play_indx_song[3] = 20;
    play_indx_song[4] = 21;
    this->SendCommandToMP3Player(play_indx_song, 6);
    delay(2200);
  }
}

/******************************************************************/
void Audio::PlayPutYourChip(String language)
{
  if (language == "Arabic")
  {
    play_indx_song[3] = 20;
    play_indx_song[4] = 02;
    this->SendCommandToMP3Player(play_indx_song, 6);
    delay(2200);
  }
  if (language == "Hebrew")
  {
    play_indx_song[3] = 20;
    play_indx_song[4] = 12;
    this->SendCommandToMP3Player(play_indx_song, 6);
    delay(1200);
  }
  if (language == "Russian")
  {
    play_indx_song[3] = 20;
    play_indx_song[4] = 22;
    this->SendCommandToMP3Player(play_indx_song, 6);
    delay(2200);
  }
}

/******************************************************************/
void Audio::PlayConnectingToWorkerData(String language)
{
  if (language == "Arabic")
  {
    play_indx_song[3] = 20;
    play_indx_song[4] = 03;
    this->SendCommandToMP3Player(play_indx_song, 6);
    delay(2200);
  }
  if (language == "Hebrew")
  {
    play_indx_song[3] = 20;
    play_indx_song[4] = 13;
    this->SendCommandToMP3Player(play_indx_song, 6);
    delay(1200);
  }
  if (language == "Russian")
  {
    play_indx_song[3] = 20;
    play_indx_song[4] = 23;
    this->SendCommandToMP3Player(play_indx_song, 6);
    delay(3200);
  }
}

/******************************************************************/
// void Audio::WaitUntilAudioEnded()
// {
//     while (true) {
//         // Send command to query MP3 player status
//         int8_t query_status[] = { 0x7e, 0x02, 0x42, 0x00, (int8_t)0xef };
//         this->SendCommandToMP3Player(query_status, 5);

//         // Read response from MP3 player
//         int8_t response[10];
//         int response_len = this->ReadResponseFromMP3Player(response, 10);

//         // Check if playback has finished
//         if (response_len >= 10 && response[3] == 0x08 && response[6] == 0x00) {
//             // Playback has finished
//             break;
//         }

//         // Wait for a short time before checking again
//         delay(100);
//     }
// }
void Audio::PlayCombine(){
    
    int8_t play_combine[] = {0x7E, 16, 0x45, 30, 11, 30, 12, 30, 13, 02, 30, 30, 14, 02, 30, 30, 15, (int8_t)0xEF};
    //int8_t play_combine[] = {0x7E, 0x08, 0x45, 0x02, 10, 0x02, 11, 0x02, 12, (int8_t)0xEF};
    this->SendCommandToMP3Player(play_combine, 18);
}
#endif //AUDIO_H