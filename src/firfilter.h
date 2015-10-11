#ifndef FIRFILTER_H
#define FIRFILTER_H

#include <liquid/liquid.h>
#include <cstdlib>
#include <complex>
#include <iostream>

class FirFilter
{
public:
    FirFilter();
    FirFilter(float relLowF, float relHighF, unsigned int filterLength = 128);
    ~FirFilter();


    std::complex<double> filter(std::complex<double> input);
    void redesign(float relLowF, float relHighF, unsigned int filterLength = 128);
    void reset();

private:
    void destroyAll();
    void filterDesign(float relLowF, float relHighF);
    void designLowPass(float relHighF);
    void designHighPass(float relLowF);
    void designBandPass(float relLowF, float relHighF);
    void createFilter();

    unsigned int _hLength;
    float * _hCoeffs;
    unsigned int _numBands;
    float * _bands;
    float * _des;
    float * _weights;
    liquid_firdespm_btype _bType;
    liquid_firdespm_wtype * _wType;

    firfilt_crcf _filter;
};

#endif // FIRFILTER_H
