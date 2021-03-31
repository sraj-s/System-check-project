// Project By: Sambeg Raj Subedi 
// Date: 03/31/2021

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <bits/stdc++.h>
//#include </usr/bin/clang++>

// Scoped Based timer class
// means timer starts at constrcution of the class and ends timer is calculated at destruction of class
class Timer
{
public:
    Timer(const char* name) 
        : m_Name(name)
    {
        m_Start = std::chrono::high_resolution_clock::now(); 
    }

    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> duration = (end - m_Start); 
        float duration_in_ms = duration.count() * 1000.0f;
        std::cout << m_Name << ": " << duration_in_ms << " ms\n";
    }

private:
    std::chrono::high_resolution_clock::time_point m_Start; // Time when timer is called
    const char* m_Name;
};

class FindMostDivisors {
public:
    FindMostDivisors(int start, int stop, int thread_amt = 1)
        : m_Start(start), m_Stop(stop), m_ThreadAmt(thread_amt)
    {
        Timer timer("FindMostDivisors");

        m_Counter = m_Stop/m_ThreadAmt; // Calculates how much to split the load to different threads

        // Creates multiple threads and begins to time them
        for(int i = 0; i < m_ThreadAmt; i++) {
            std::string name = "Thread " + std::to_string(i);
            Timer timer(name.c_str());
            m_Threads.emplace_back(&FindMostDivisors::run, this, m_Counter*i, m_Counter*(i+1));
        }       

        for(std::thread& t : m_Threads) 
            t.join();

        std::cout << "The number with the most divisors is: " << getNum() << " has " << getMaxDiv() << " divisors!\n";
    }

    ~FindMostDivisors() {
        m_Threads.clear();
    }

    // Does the calculation on multiple threads
    void run(int start, int end) {
        int counter = 0;
        
        if(start == 0) start++;
        for(int i = start; i < end; i++) {
            counter = 0;
            
            for(int j = 1; j <= sqrt(i); j++) {
                if(i % j == 0) {
                    counter += 2;
                }
            }

            {
                // Critical Section
                std::lock_guard<std::mutex> l(m_Mutex);
                if(counter > m_MaxDiv) {
                    m_MaxDiv = counter;
                    m_Num = i;
                }
            } // gets unlocked at destruction of lock_guard
        }
    }


    inline int getNum() const { return m_Num; }
    inline int getMaxDiv() const { return m_MaxDiv; }

private:
    std::vector<std::thread> m_Threads;
    std::mutex m_Mutex;
    int m_Start, m_Stop; // Start number and stops
    int m_ThreadAmt;
    int m_Counter;      // Increments for threads
    int m_Num = -1000, m_MaxDiv = -1000; // Should be locked behind mutex
};

int main(int argc, char** argv) {

    FindMostDivisors a(1, 10000);
    FindMostDivisors b(1, 10000, 2);
    FindMostDivisors c(1, 10000, 4);

}