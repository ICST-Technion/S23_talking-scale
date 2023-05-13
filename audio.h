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
        void SayWord(int x, String language);
        void SayIntro(String language);
        void SendCommandToMP3Player(int8_t command[], int len);
        void SayNum(int num, String language, bool justNum);
        void PlayEmailAndPasswordAreNotAuthenticated();
        void PlayCantCollectDataFromDB(bool& first_time_played, bool& start);
        void PlayShouldStartNewTask(float curr_val, bool& isNextJobReady, Worker& worker);
        void PlayGoodBye(Worker& worker);
        int ReadResponseFromMP3Player(int8_t response[], int max_len);
        void WaitUntilAudioEnded();

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
void Audio::SayWord(int x, String language)
{
    if (language == "Arabic") x += 10;
    if (language == "Hebrew") x += 20;
    if (language == "Russian") x += 30;
    int temp3 = play_indx_song[3];
    int temp4 = play_indx_song[4];
    play_indx_song[3] = 4;
    play_indx_song[4] = x;
    SendCommandToMP3Player(play_indx_song, 6);
    play_indx_song[3] = temp3;
    play_indx_song[4] = temp4;
    delay(200);
}

/******************************************************************/
void Audio::SayIntro(String language)
{
    int sound = 1;
    if (language == "Hebrew") sound = 11;
    if (language == "Russian") sound = 21;
    play_indx_song[3] = 10;
    play_indx_song[4] = sound++;
    SendCommandToMP3Player(play_indx_song, 6);
    delay(2200);
    play_indx_song[4] = sound++;
    SendCommandToMP3Player(play_indx_song, 6);
    delay(300);
    play_indx_song[4] = sound++;
    SendCommandToMP3Player(play_indx_song, 6);
    delay(2700);

    SayNum(goal, language, true);
    delay(200);
    play_indx_song[3] = 10;
    play_indx_song[4] = sound++;
    SendCommandToMP3Player(play_indx_song, 6);
    delay(200);
    SayNum(goal / unit, language, true);
    delay(200);
    play_indx_song[3] = 10;
    play_indx_song[4] = sound++;
    SendCommandToMP3Player(play_indx_song, 6);
    delay(200);
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
    this->WaitUntilAudioEnded();
    delay(1000);
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
      play_indx_song[3] = 2;
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
void Audio::WaitUntilAudioEnded()
{
    while (true) {
        // Send command to query MP3 player status
        int8_t query_status[] = { 0x7e, 0x02, 0x42, 0x00, (int8_t)0xef };
        this->SendCommandToMP3Player(query_status, 5);

        // Read response from MP3 player
        int8_t response[10];
        int response_len = this->ReadResponseFromMP3Player(response, 10);

        // Check if playback has finished
        if (response_len >= 10 && response[3] == 0x08 && response[6] == 0x00) {
            // Playback has finished
            break;
        }

        // Wait for a short time before checking again
        delay(100);
    }
}

#endif //AUDIO_H