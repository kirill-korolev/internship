#include <chrono>
#include <thread>
#include <functional>
#include <iostream>

#include "jobpool.hpp"
#include "producer.hpp"
#include "jobproducer.hpp"

void m1(){
    Producer jobSource;
    unsigned jobsLeft = jobSource.getTotalJobs();
    double result = 0;
    
    while(jobsLeft){
        auto job = jobSource.getJob();
        result += job();
        jobsLeft--;
    }
    
    std::cout << "Done. Result is " << result << "." << std::endl;
}

void m2()
{
    Producer jobSource;
    using Job = std::function<double()>;

    JobPool<Job> pool(jobSource.getTotalJobs(), [](double acc, double rhs){
        return acc + rhs;
    });
    
    //Generation thread
    std::thread genThread(&JobPool<Job>::generate, &pool, [&jobSource]{
        return jobSource.getJob();
    });
    
    //Execution thread
    std::thread excThread(&JobPool<Job>::execute, &pool);
    
    genThread.join();
    
    //Additional thread which is responsible for computations after the generation is done
    std::thread subThread(&JobPool<Job>::execute, &pool);
    
    excThread.join();
    subThread.join();
    
    std::cout << "Done. Result is " << pool.getResult() << "." << std::endl;
}

// Comparison with loose coupling approach

void m3(){
    JobProducer producer;
    std::thread genThread(&JobProducer::generateAll, &producer);
    std::thread excThread(&JobProducer::executeAll, &producer);
    genThread.join();
    std::thread subThread(&JobProducer::executeAll, &producer);
    excThread.join();
    subThread.join();
    std::cout << "Done. Result is " << producer.getResult() << "." << std::endl;
}

template <typename Function>
long long elapse(Function f) {
    auto start = std::chrono::steady_clock::now();
    f();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int main(){
    auto functions = {m1, m2};
    
    for(auto &f: functions){
        std::cout << elapse(f) << std::endl;
    }
    
    return 0;
}
