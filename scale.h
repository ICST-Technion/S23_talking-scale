#ifndef SCALE_H
#define SCALE_H

#include "helper.h"
#include "audio.h"
#include "firestore.h"
#include <SoftwareSerial.h>

char char_to_read;

class Scale{
    public:
        String currWeightString;
        String oldWeightString;
        float oldVal;
        bool isNextJobReady;

    public:
        Scale() : currWeightString(""),oldWeightString(""), oldVal(0), isNextJobReady(true){}
        void ReadScaleWeight();
        void Run(Audio& audio, Worker& worker, FireStore& firestore);
        bool CheckIfTaskEnded(float& curr_val);
        bool CheckingIfShouldStartNewTask(float& curr_val);
        bool CheckingStableWeight(float& curr_val);
        void PlayTaskEnded(Audio& audio, FireStore& firestore, Worker& worker,float& curr_val);
};

void Scale::ReadScaleWeight()
{
    Serial.print("reading the scale...");
    Serial.println();

    this->oldWeightString = currWeightString;
    this->currWeightString = "";
    delay(200);
    while (Serial2.available())
    {
        char_to_read = Serial2.read();
        this->currWeightString += char_to_read;
    }
    Serial.print("reading the scale ended");
    Serial.println();

}

void Scale::Run(Audio& audio, Worker& worker, FireStore& firestore)
{
    if (this->currWeightString == ""){
        return; //current val is 0
    }

    float curr_val = GetNum(this->currWeightString);
    this->oldVal = this->oldVal * 0.50 + curr_val * 0.50;

    bool should_start_new_task = CheckingIfShouldStartNewTask(curr_val);
    if (should_start_new_task){
        audio.PlayShouldStartNewTask(curr_val,isNextJobReady,worker);
    }
    
    Serial.println("\ncurrent weight: ");
    Serial.println(curr_val);

    if(CheckingStableWeight(curr_val)){
        if (CheckIfTaskEnded(curr_val)){
            this->PlayTaskEnded(audio,firestore,worker,curr_val);
        }
        else{
            audio.SayNum(int((curr_val + unit / 2) / unit), worker.language, false);
            this->oldVal = -10;
            delay(1000);
        }
    }
    this->currWeightString = "";
}

bool Scale::CheckIfTaskEnded(float& curr_val)
{
    return (curr_val > goal - unit / 2 && curr_val < goal + unit / 2);
}

bool Scale::CheckingIfShouldStartNewTask(float& curr_val){
    return (this->oldVal < curr_val + 0.01 && this->oldVal > curr_val - 0.01 && this->oldVal < goal * 0.7 && !(this->isNextJobReady));
}

bool Scale::CheckingStableWeight(float& curr_val){
    return ((this->oldVal < curr_val + 0.01 ) && (this->isNextJobReady));
}

void Scale::PlayTaskEnded(Audio& audio,FireStore& firestore, Worker& worker,float& curr_val){
    //task ended audio
    audio.play_indx_song[3] = 10;
    audio.play_indx_song[4] = 6;
    if (worker.language == "Hebrew") audio.play_indx_song[4] = 16;
    if (worker.language == "Russian") audio.play_indx_song[4] = 26;
    audio.SendCommandToMP3Player(audio.play_indx_song, 6);
    firestore.UploadData(curr_val,worker);
    this->oldVal = -10;
    this->isNextJobReady = false;
}
#endif //SCALE_H