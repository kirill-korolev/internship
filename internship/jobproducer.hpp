//
//  jobproducer.hpp
//  internship
//
//  Created by Kirill Korolev on 10/02/2018.
//  Copyright © 2018 Kirill Korolev. All rights reserved.
//

#ifndef jobproducer_hpp
#define jobproducer_hpp

#include <queue>
#include <thread>
#include "producer.hpp"


class JobProducer {
private:
    Producer producer;
public:
    using Job = decltype(producer.getJob());
    void generateAll();
    void executeAll();
    double getResult() const;
private:
    double result = 0;
    std::queue<Job> jobs;
    std::mutex mutex;
    std::condition_variable cond;
};

#endif /* jobproducer_hpp */
