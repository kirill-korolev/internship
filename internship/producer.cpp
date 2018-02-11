//
//  producer.cpp
//  internship
//
//  Created by Kirill Korolev on 10/02/2018.
//  Copyright © 2018 Kirill Korolev. All rights reserved.
//


#include <immintrin.h>
#include <smmintrin.h>
#include <emmintrin.h>
#include "producer.hpp"

void matMultVec(double *result, double *mat, unsigned rows, unsigned cols, double *vec)
{
    __m128d m;
    __m128d v;
    __m128d res;
    
    double* matPtr;
    double* vecPtr;
    
    for (unsigned i = 0; i < rows; ++i)
    {
        res = _mm_setzero_pd();
        matPtr = mat + i * cols;
        vecPtr = vec;
        
        for (unsigned j = 0; j < cols; j += 2)
        {
            m = _mm_load_pd(matPtr + j);
            v = _mm_load_pd(vecPtr + j);
            res = _mm_add_pd(res, _mm_mul_pd(m, v));
        }
        
        res = _mm_add_pd(res, res);
        _mm_store_sd(result + i, res);
    }
}

//void matMultVec(double *result, double *mat, unsigned rows, unsigned cols, double *vec)
//{
//    for (unsigned i = 0; i < rows; ++i )
//    {
//        for (unsigned j = 0; j < cols; ++j )
//        {
//            result[i] += mat[i*cols + j] * vec[j];
//        }
//    }
//}


Producer::Producer(): totalJobs(100), jobsIssued(0), jobsReady(0), jobWidth(1000), jobHeight(2000) {}

unsigned Producer::getTotalJobs() { return totalJobs; }

std::function<double(void)> Producer::getJob()
{
    assert(jobsIssued < totalJobs);
    if (!jobsReady) {
        std::chrono::milliseconds generationTime(200);
        std::this_thread::sleep_for(generationTime);
        unsigned newJobs = 2;
        if (newJobs + jobsIssued > totalJobs)
            newJobs = totalJobs - jobsIssued;
        jobsReady += newJobs;
        //std::cout << "Added " << newJobs << " new jobs." << std::endl;
    }
    jobsReady--;
    jobsIssued++;
    unsigned jobInit = jobsIssued;
    return [jobInit, this] () {
        double sum = 0;
        for (unsigned iter = 0; iter != 10; iter++)
        {
            std::vector<double> matrix(jobWidth*jobHeight);
            for (unsigned i = 0; i != jobWidth*jobHeight; i++)
            {
                matrix[i] = (0.00001*((jobInit + 10)*(jobInit - iter)) + i*0.02 + 0.1*(jobInit%4))/100000;
            }
            std::vector<double> vec(jobWidth);
            for (unsigned i = 0; i != jobWidth; i++)
            {
                vec[i] = -(0.0001*((jobInit + 10) - iter) - i*0.01)/1000;
            }
            std::vector<double> result(jobHeight, 0);
            matMultVec(&result[0], &matrix[0], jobHeight, jobWidth, &vec[0]);
            sum += std::accumulate(result.begin(), result.end(), 0)/1000.0;
        }
        //std::cout << "Task " << jobInit << " produced " << sum << std::endl;
        return sum;
    };
}
