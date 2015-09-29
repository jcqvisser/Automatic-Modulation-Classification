#ifndef REALSTREAMFUNCTION
#define REALSTREAMFUNCTION

class RealStreamFunction
{
public:
    virtual double calc (const double & inpt )
    {
        return inpt;
    }

    virtual ~RealStreamFunction() {}
};


#endif // REALSTREAMFUNCTION

