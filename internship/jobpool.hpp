//
//  jobpool.hpp
//  internship
//
//  Created by Kirill Korolev on 09/02/2018.
//  Copyright © 2018 Kirill Korolev. All rights reserved.
//

#ifndef jobpool_hpp
#define jobpool_hpp

#include <functional>
#include <mutex>
#include <queue>

template <typename Job>
class JobPool {
public:
    using T = typename Job::result_type;
    using Accumulator = T (*)(T, T);
    using Generator = std::function<Job()>;
    
    JobPool(unsigned n, Accumulator acc);
    void generate(const Generator& generator);
    void execute();
    T getResult() const;
private:
    std::mutex mutex;
    std::condition_variable condition;
    std::queue<Job> jobs;
    unsigned jobsLeft;
    Accumulator accumulator;
    T result;
};

template <typename Job>
JobPool<Job>::JobPool(unsigned n, Accumulator acc): jobsLeft(n), accumulator(acc), result(0){}


template <typename Job>
void JobPool<Job>::generate(const Generator& generator){
    
    if(!generator) return;
    
    while(jobsLeft){
        auto result = generator();
        std::unique_lock<std::mutex> lock(mutex);
        jobs.push(result);
        jobsLeft--;
        condition.notify_one();
    }
}

template <typename Job>
void JobPool<Job>::execute(){
    
    do{
        std::unique_lock<std::mutex> lock(mutex);
        
        if(!jobs.size()){
            condition.wait(lock);
        }
        
        auto job = jobs.front();
        jobs.pop();
        
        lock.unlock();
        
        result = accumulator(result, job());
    } while(jobs.size());
}

template <typename Job>
typename JobPool<Job>::T JobPool<Job>::getResult() const{
    return result;
}

#endif /* jobpool_hpp */
