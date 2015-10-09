#include "firfilter.h"

FirFilter::FirFilter()
{
    _hLength = 128;
    filterDesign(0.0f, 0.5f);
}

FirFilter::FirFilter(float relLowF, float relHighF, unsigned int filterLength)
{
    _hLength = filterLength;

    filterDesign(relLowF, relHighF);
}

void FirFilter::filterDesign(float relLowF, float relHighF)
{
    _hCoeffs = (float*)malloc(sizeof(float) * _hLength);

    if(relLowF <= 0.0f && relHighF >= 0.5f)
        _numBands = 1;
    else if(relLowF <= 0.0f || relHighF >= 0.5f)
        _numBands = 2;
    else
        _numBands = 3;

    if(relHighF - relLowF < 0.005)
        _numBands = 1;

    _bands = (float*)malloc(sizeof(float) * _numBands);
    _des = (float*)malloc(sizeof(float) * _numBands);
    _weights = (float*)malloc(sizeof(float) * _numBands);
    _bType = LIQUID_FIRDESPM_BANDPASS;
    _wType = (liquid_firdespm_wtype*)malloc(sizeof(liquid_firdespm_wtype) * _numBands);

    if(_numBands == 2 && relLowF <= 0.0f)
        designLowPass(relHighF);
    else if(_numBands == 2 && relHighF >= 0.5f)
        designHighPass(relLowF);
    else if(_numBands == 3)
        designBandPass(relLowF, relHighF);
}


void FirFilter::designLowPass(float relHighF)
{
    _bands[0] = 0.0f;
    _bands[3] = 0.5f;
    if(relHighF + 0.01f < 0.5)
    {
        _bands[1] = relHighF;
        _bands[2] = relHighF + 0.01f;
    }
    else
    {
        _bands[1] = 0.49f;
        _bands[2] = 0.5f;
    }
    _des[0] = 1.0f;
    _des[1] = 0.0f;

    createFilter();
}

void FirFilter::designHighPass(float relLowF)
{
    _bands[0] = 0.0f;
    _bands[3] = 0.5f;
    if(relLowF - 0.01f > 0.0f)
    {
        _bands[1] = relLowF - 0.01f;
        _bands[2] = relLowF;
    }
    else
    {
        _bands[1] = 0.0f;
        _bands[2] = 0.1f;
    }
    _des[0] = 0.0f;
    _des[1] = 1.0f;

    createFilter();
}

void FirFilter::designBandPass(float relLowF, float relHighF)
{
    _des[1] = 1.0f;
    _bands[0] = 0.0f;
    if(relLowF - 0.01f <= 0)
    {
        _bands[1] = 0.0f;
        _des[0] = 1.0f;
    }
    else
    {
        _bands[1] = relLowF - 0.01f;
        _des[0] = 0.0f;
    }
    _bands[2] = relLowF;
    _bands[3] = relHighF;
    if(relHighF + 0.01f >= 0.5f)
    {
        _bands[4] = 0.5f;
        _des[2] = 1.0f;
    }
    else
    {
        _bands[4] = relHighF + 0.01f;
        _des[2] = 0.0f;
    }
    _bands[5] = 0.5f;

    createFilter();
}

void FirFilter::createFilter()
{
    for(unsigned int i = 0; i < _numBands; ++i)
    {
        _weights[i] = 1.0f;
        _wType[i] = LIQUID_FIRDESPM_EXPWEIGHT;
    }
    firdespm_run(_hLength,_numBands,_bands,_des,_weights,_wType,_bType, _hCoeffs);
    _filter = firfilt_crcf_create(_hCoeffs,_hLength);
}

void FirFilter::redesign(float relLowF, float relHighF, unsigned int filterLength)
{
    destroyAll();
    _hLength = filterLength;
    filterDesign(relLowF, relHighF);
}

FirFilter::~FirFilter()
{
    destroyAll();
}

void FirFilter::destroyAll()
{
    free(_hCoeffs);
    free(_bands);
    free(_des);
    free(_weights);
    free(_wType);
    if(_numBands > 1)
        firfilt_crcf_destroy(_filter);
}

std::complex<double> FirFilter::filter(std::complex<double> input)
{
    if(_numBands > 1)
    {
        liquid_float_complex res;

        firfilt_crcf_push(_filter, {(float)input.real(), (float)input.imag()});
        firfilt_crcf_execute(_filter, &res);

        return std::complex<double>(res.real, res.imag);
    }
    else
    {
        return input;
    }
}

void FirFilter::reset()
{
    firfilt_crcf_reset(_filter);
}
