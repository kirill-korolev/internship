//
//  producer.hpp
//  internship
//
//  Created by Kirill Korolev on 09/02/2018.
//  Copyright © 2018 Kirill Korolev. All rights reserved.
//

#ifndef producer_hpp
#define producer_hpp

#include <chrono>
#include <thread>
#include <functional>
#include <random>
#include <cassert>
#include <iostream>
#include <numeric>


void matMultVec(double *result, double *mat, unsigned rows, unsigned cols, double *vec);
void matMultVec2(double *result, double *mat, unsigned rows, unsigned cols, double *vec);

class Producer {
    const unsigned totalJobs;
    unsigned jobsIssued;
    unsigned jobsReady;
    
    const unsigned jobWidth;
    const unsigned jobHeight;
public:
    Producer();
    unsigned getTotalJobs();
    std::function<double(void)> getJob();
};

#endif /* producer_hpp */
