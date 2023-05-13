#ifndef WORKER_H
#define WORKER_H

#include <SoftwareSerial.h>

class Worker{
    public:
        String name;
        String id;
        String product;
        String language;

    public:
        Worker(String name,String id,String product,String language) : 
            name(name), id(id),product(product),language(language){}
};


#endif //WORKER_H