#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

#include <Arduino.h>

template <int order>
class LowPass
{
  private:
    float a[order];
    float b[order+1];
    float omega0;
    float dt;
    bool adapt;
    float tn1;
    float x[order+1]; // Raw values
    float y[order+1]; // Filtered values

  public:
    // Constructor
    LowPass(float f0, float fs, bool adaptive);
    
    // Set filter coefficients
    void setCoef();
    
    // Apply the filter to the input signal
    float filt(float xn);
};

#include "LowPassFilter.tpp"  // Include the implementation file for templates

#endif // LOWPASSFILTER_H