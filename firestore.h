#ifndef FIRESTORE_H
#define FIRESTORE_H

#include "helper.h"
#include "worker.h"

#include <SoftwareSerial.h>
#include <Firebase_ESP_Client.h>

#define FIREBASE_PROJECT_ID "aid-scale"

int start_time[3] = { 0 };
int curr_time[3] = { 0 };
float unit = -1;
float goal = -1;

class FireStore{
    public:
        FirebaseData fbdo;
        FirebaseAuth auth;
        FirebaseConfig config;
        unsigned long dataMillis;
        int count;

    public:
        FireStore() : dataMillis(0),count(0) {}
        bool CheckAccessFireStore(String projectId,String documentPath);
        void UpdateWorkerInfo(Worker& worker);
        void UploadData(float& weight,Worker& worker);
};

bool FireStore::CheckAccessFireStore(String projectId,String documentPath) {
    return (Firebase.Firestore.getDocument(&fbdo, projectId, "", documentPath.c_str(), ""));
}

void FireStore::UpdateWorkerInfo(Worker& worker){
    FirebaseJson payload;
    payload.setJsonData(fbdo.payload().c_str());

    FirebaseJsonData jsonData;
    payload.get(jsonData, "fields/Name/stringValue", true);
    worker.name = jsonData.stringValue;
    payload.get(jsonData, "fields/Product/stringValue", true);
    worker.product = jsonData.stringValue;
    payload.get(jsonData, "fields/Language/stringValue", true);
    worker.language = jsonData.stringValue;

    String documentPath = StringSumHelper("Products/") + worker.product;
    bool can_access_fire_store = CheckAccessFireStore(FIREBASE_PROJECT_ID,documentPath);
    if (can_access_fire_store){
        FirebaseJson payload;
        payload.setJsonData(fbdo.payload().c_str());

        FirebaseJsonData jsonData;
        payload.get(jsonData, "fields/Unit/stringValue", true);
        unit = GetNum(jsonData.stringValue);
        payload.get(jsonData, "fields/Pack/stringValue", true);
        goal = GetNum(jsonData.stringValue) * unit;
    } 
    else{
        Serial.println(fbdo.errorReason());
    }
}

void FireStore::UploadData(float& weight,Worker& worker) {
    //PlayUpdatingData(5)
  if (Firebase.ready() && (millis() - this->dataMillis > 5000 || this->dataMillis == 0)) {
    this->dataMillis = millis();
    this->count++;

    // The dyamic array of write object fb_esp_firestore_document_write_t.
    std::vector<struct fb_esp_firestore_document_write_t> writes;

    // A write object that will be written to the document.
    struct fb_esp_firestore_document_write_t transform_write;

    // Set the write object write operation type.
    // fb_esp_firestore_document_write_type_update,
    // fb_esp_firestore_document_write_type_delete,
    // fb_esp_firestore_document_write_type_transform
    transform_write.type = fb_esp_firestore_document_write_type_transform;

    // Set the document path of document to write (transform)
    transform_write.document_transform.transform_document_path = StringSumHelper("Jobs/") + GetDate();

    // Set a transformation of a field of the document.
    struct fb_esp_firestore_document_write_field_transforms_t field_transforms;

    // Set field path to write.
    field_transforms.fieldPath = "jobs";

    // Set the transformation type.
    // fb_esp_firestore_transform_type_set_to_server_value,
    // fb_esp_firestore_transform_type_increment,
    // fb_esp_firestore_transform_type_maaximum,
    // fb_esp_firestore_transform_type_minimum,
    // fb_esp_firestore_transform_type_append_missing_elements,
    // fb_esp_firestore_transform_type_remove_all_from_array
    field_transforms.transform_type = fb_esp_firestore_transform_type_append_missing_elements;

    // For the usage of FirebaseJson, see examples/FirebaseJson/BasicUsage/Create.ino
    FirebaseJson content;

    //String txt = "Hello World! " + String(count);
    content.set("values/[0]/mapValue/fields/Name/stringValue", worker.name);
    content.set("values/[0]/mapValue/fields/ID/stringValue", worker.id);
    content.set("values/[0]/mapValue/fields/SubmitTime/stringValue", GetTime(curr_time));
    content.set("values/[0]/mapValue/fields/Tolerance/doubleValue", (weight - goal) / goal);
    content.set("values/[0]/mapValue/fields/Weight/doubleValue", weight);
    content.set("values/[0]/mapValue/fields/WorkTime/doubleValue", GetTimer(start_time, curr_time));
    content.set("values/[0]/mapValue/fields/ProductName/stringValue", worker.product);

    // Set the transformation content.
    field_transforms.transform_content = content.raw();

    // Add a field transformation object to a write object.
    transform_write.document_transform.field_transforms.push_back(field_transforms);

    // Add a write object to a write array.
    writes.push_back(transform_write);
    if (Firebase.Firestore.commitDocument(&(this->fbdo), FIREBASE_PROJECT_ID, "" /* databaseId can be (default) or empty */, writes /* dynamic array of fb_esp_firestore_document_write_t */, "" /* transaction */))
      Serial.printf("ok\n%s\n\n", this->fbdo.payload().c_str());
    else
      Serial.println(this->fbdo.errorReason());
  }
}
#endif //FIRESTORE_H