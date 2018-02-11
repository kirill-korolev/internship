//
//  jobproducer.cpp
//  internship
//
//  Created by Kirill Korolev on 10/02/2018.
//  Copyright © 2018 Kirill Korolev. All rights reserved.
//

#include "jobproducer.hpp"

void JobProducer::generateAll(){
    
    unsigned n = producer.getTotalJobs();
    
    while(n){
        auto job = producer.getJob();
        std::unique_lock<std::mutex> lock(mutex);
        jobs.push(job);
        cond.notify_one();
        --n;
    }
}

void JobProducer::executeAll(){
    
    do{
        std::unique_lock<std::mutex> lock(mutex);
        if(!jobs.size()) cond.wait(lock);
        auto job = jobs.front();
        jobs.pop();
        lock.unlock();
        result += job();
    } while(jobs.size() != 0);
    
}

double JobProducer::getResult() const {
    return result;
}
