#ifndef WORKER_H
#define WORKER_H
#include <SoftwareSerial.h>

class Worker{

    public:
        Worker(String name,String id,String product,String language) : 
            name(name), id(id),product(product),language(language){}

    public:
        String name;
        String id;
        String product;
        String language;
};


#endif //WORKER_H