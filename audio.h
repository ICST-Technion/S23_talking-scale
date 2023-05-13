#ifndef AUDIO_H
#define AUDIO_H
#include "scale.h"
#include "helper.h"
#include <SoftwareSerial.h>

class Audio{
    public:
        int8_t play_indx_song[6];

        Audio();
        void sayArabic(int num);
        void sayHebrew(int num);
        void sayRussian(int num);
        void sayWord(int x, String language);
        void sayIntro(String language);
        void send_command_to_MP3_player(int8_t command[], int len);
        void sayNum(int num, String language, bool justNum);
        void PlayEmailAndPasswordAreNotAuthenticated();
        void PlayCantCollectDataFromDB(bool& first_time_played);
        void PlayShouldStartNewTask(float curr_val, bool& isNextJobReady, Worker& worker);
        void PlayGoodBye(Worker& worker);
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
void Audio::sayArabic(int num)
{
    bool first = true;
    if (num / 100 > 0)
    {
        play_indx_song[4] = 100 + num / 100;
        send_command_to_MP3_player(play_indx_song, 6);
        first = false;
        delay(250);
    }
    num = num % 100;
    if (num / 10 == 1) 
    {
        if (first) 
        {
            play_indx_song[4] = num;
            send_command_to_MP3_player(play_indx_song, 6);
            delay(500);
            return;
        } 
        else
        {
            play_indx_song[4] = num + 200;
            send_command_to_MP3_player(play_indx_song, 6);
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
            send_command_to_MP3_player(play_indx_song, 6);
            delay(250);
        } 
        else
        {
            play_indx_song[4] = (num % 10 + 200);
            send_command_to_MP3_player(play_indx_song, 6);
            delay(250);
        }
    }

    if (num / 10 > 0) 
    {
        if (first) 
        {
            play_indx_song[4] = (num / 10) * 10;
            send_command_to_MP3_player(play_indx_song, 6);
            delay(250);
        } 
        else 
        {
            play_indx_song[4] = (num / 10) + 220;
            send_command_to_MP3_player(play_indx_song, 6);
            delay(300);
        }
    }
}

/*****************************************************************/
void Audio::sayHebrew(int num)
{
    if (num < 10 && num > 0) {
      play_indx_song[4] = num + 20;
      send_command_to_MP3_player(play_indx_song, 6);
      delay(250);
      return;
    }
    if (num / 100 > 0) {
      play_indx_song[4] = 100 + num / 100;
      send_command_to_MP3_player(play_indx_song, 6);
      delay(250);
    }
    num = num % 100;
    if (num / 10 > 1) {
      play_indx_song[4] = 10 * (num / 10);
      send_command_to_MP3_player(play_indx_song, 6);
      delay(250);
    }
    if (num / 10 == 1) {
      play_indx_song[4] = num;
      send_command_to_MP3_player(play_indx_song, 6);
      delay(500);
      return;
    }
    num = num % 10;
    if (num > 0) {
      play_indx_song[4] = num;
      send_command_to_MP3_player(play_indx_song, 6);
      delay(250);
    }
}

/*****************************************************************/
void Audio::sayRussian(int num)
{
    if (num / 100 > 0)
    {
        play_indx_song[4] = 100 + num / 100;
        send_command_to_MP3_player(play_indx_song, 6);
        delay(250);
    }
    num = num % 100;
    if (num / 10 > 1)
    {
        play_indx_song[4] = 10 * (num / 10);
        send_command_to_MP3_player(play_indx_song, 6);
        delay(250);
    }
    if (num / 10 == 1)
    {
        play_indx_song[4] = num;
        send_command_to_MP3_player(play_indx_song, 6);
        delay(500);
        return;
    }

    num = num % 10;
    if (num > 0)
    {
        play_indx_song[4] = num;
        send_command_to_MP3_player(play_indx_song, 6);
        delay(250);
    }
}

/******************************************************************/
void Audio::sayWord(int x, String language)
{
    if (language == "Arabic") x += 10;
    if (language == "Hebrew") x += 20;
    if (language == "Russian") x += 30;
    int temp3 = play_indx_song[3];
    int temp4 = play_indx_song[4];
    play_indx_song[3] = 4;
    play_indx_song[4] = x;
    send_command_to_MP3_player(play_indx_song, 6);
    play_indx_song[3] = temp3;
    play_indx_song[4] = temp4;
    delay(200);
}

/******************************************************************/
void Audio::sayIntro(String language)
{
    int sound = 1;
    if (language == "Hebrew") sound = 11;
    if (language == "Russian") sound = 21;
    play_indx_song[3] = 10;
    play_indx_song[4] = sound++;
    send_command_to_MP3_player(play_indx_song, 6);
    delay(2200);
    play_indx_song[4] = sound++;
    send_command_to_MP3_player(play_indx_song, 6);
    delay(300);
    play_indx_song[4] = sound++;
    send_command_to_MP3_player(play_indx_song, 6);
    delay(2700);

    sayNum(goal, language, true);
    delay(200);
    play_indx_song[3] = 10;
    play_indx_song[4] = sound++;
    send_command_to_MP3_player(play_indx_song, 6);
    delay(200);
    sayNum(goal / unit, language, true);
    delay(200);
    play_indx_song[3] = 10;
    play_indx_song[4] = sound++;
    send_command_to_MP3_player(play_indx_song, 6);
    delay(200);
}

/******************************************************************/
void Audio::send_command_to_MP3_player(int8_t command[], int len)
{
    Serial.print("\nMP3 Command => ");
    for (int i = 0; i < len; i++)
    {
        Serial1.write(command[i]);
        Serial.print(command[i], HEX);
    }
    delay(1000);
}

/******************************************************************/
void Audio::sayNum(int num, String language, bool justNum) {
  if (language == "Arabic")
  {
      play_indx_song[3] = 1;
      sayArabic(num);
  }
  if (language == "Hebrew")
  {
      play_indx_song[3] = 2;
      sayHebrew(num);
  }
  if (language == "Russian")
  {
      play_indx_song[3] = 3;
      sayRussian(num);
  }
}

/******************************************************************/
void Audio::PlayEmailAndPasswordAreNotAuthenticated(){
    play_indx_song[3] = 10;
    play_indx_song[4] = 0;
    this->send_command_to_MP3_player(play_indx_song, 6);
    delay(5000);
}

/******************************************************************/
void Audio::PlayCantCollectDataFromDB(bool& first_time_played){
    play_indx_song[3] = 10;
    play_indx_song[4] = 10;
    this->send_command_to_MP3_player(play_indx_song, 6);
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
    this->send_command_to_MP3_player(play_indx_song, 6);
    getTime(start_time);
    delay(1500);
}

/******************************************************************/
void Audio::PlayGoodBye(Worker& worker){
    play_indx_song[3] = 10;
    play_indx_song[4] = 8;
    if (worker.language == "Hebrew") play_indx_song[4] = 18;
    if (worker.language == "Russian") play_indx_song[4] = 28;
    this->send_command_to_MP3_player(play_indx_song, 6);
    delay(3000);
}

#endif //AUDIO_H