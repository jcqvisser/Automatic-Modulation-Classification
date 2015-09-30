#include <QString>
#include <QtTest>
#include <amc.h>
#include <math.h>
#include <limits>

class AMCUnitTest : public QObject
{
    Q_OBJECT

public:
    AMCUnitTest();

private:
    double accuracyMultiplier;
    int accuracyRound(double x);
    std::complex<int> accuracyRound(std::complex<double> x);

private Q_SLOTS:
    void initTestCase();
    void phase();
    void unwrapPhase();
    void removeLinearPhase();
    void removeNegFreq();
    void meanReal();
    void meanComplex();
    void stdDevReal();
    void stdDevComplex();
    void stdDevKurtosisReal();
    void stdDevKurtosisComplex();
    void differentiate();
    void differentiateInf();
    void differentiateZero();
    void symmetryDSB();
    void symmetryUSB();
    void symmetryLSB();
    void maxPowerReal();
    void maxPowerComplex();
    void centerReal();
    void centerComplex();
    void normalizeReal();
    void normalizeComplex();
    void absReal();
    void absComplex();
    void absMaxReal();
    void absMaxComplex();
};

AMCUnitTest::AMCUnitTest()
{
}

int AMCUnitTest::accuracyRound(double x)
{
   return (int) round(x*accuracyMultiplier);
}

std::complex<int> AMCUnitTest::accuracyRound(std::complex<double> x)
{
    return std::complex<int>(round(std::real(x)*accuracyMultiplier), round(std::imag(x)*accuracyMultiplier));
}

void AMCUnitTest::initTestCase()
{
    accuracyMultiplier = 10e5;
}

void AMCUnitTest::phase()
{
    size_t N = 20;
    std::vector<std::complex<double> > x(N);
    std::vector<double> xPhase(N);
    std::vector<double> xPhaseRecovered(N);
    double fs = 20e3;
    double fc = 0.2e3;
    double fnc = fc/fs;

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = std::complex<double>(cos(2*PI*fnc*n), sin(2*PI*fnc*n));
        xPhase[n] = std::fmod((2*PI*fnc*n),(PI));
    }

    xPhaseRecovered = AMC::phase(x);

    for (size_t n = 0; n < N; ++n)
    {
        auto phaseActual = accuracyRound(xPhaseRecovered[n]);
        auto phaseExpected = accuracyRound(xPhase[n]);
        QCOMPARE(phaseActual, phaseExpected);
    }
}

void AMCUnitTest::unwrapPhase()
{
    size_t N = 20;
    std::vector<std::complex<double> > x(N);
    std::vector<double> xPhase(N);
    std::vector<double> xPhaseRecovered(N);
    double fs = 20e3;
    double fc = 5e3;
    double fnc = fc/fs;


    for (size_t n = 0; n < N; ++n)
    {
        x[n] = std::complex<double>(cos(2*PI*fnc*n), sin(2*PI*fnc*n));
        xPhase[n] = (2*PI*fnc*n);
    }

    xPhaseRecovered = AMC::unwrapPhase(AMC::phase(x));

    for (size_t n = 0; n < N; ++n)
    {
        auto phaseActual = accuracyRound(xPhaseRecovered[n]);
        auto phaseExpected = accuracyRound(xPhase[n]);
        QCOMPARE(phaseActual, phaseExpected);
    }
}

void AMCUnitTest::removeLinearPhase()
{
    size_t N = 20;
    std::vector<std::complex<double> > x(N);
    std::vector<double> xNLPhaseRecovered(N);
    double fs = 20e3;
    double fc = 5e3;
    double fnc = fc/fs;


    for (size_t n = 0; n < N; ++n)
    {
        x[n] = std::complex<double>(cos(2*PI*fnc*n), sin(2*PI*fnc*n));
    }

    xNLPhaseRecovered = AMC::removeLinearPhase(AMC::unwrapPhase(AMC::phase(x)), fnc);

    for (size_t n = 0; n < N; ++n)
    {
        auto xNonLinearPhaseActual = accuracyRound(xNLPhaseRecovered[n]);
        auto xNonLinearPhaseExpected = accuracyRound(0);
        QCOMPARE(xNonLinearPhaseActual, xNonLinearPhaseExpected);
    }
}

void AMCUnitTest::removeNegFreq()
{
    size_t N = 20;
    std::vector<std::complex<double> > x0(N);
    std::vector<std::complex<double> > x1(N);

    for(size_t n = 0; n < N/2; ++n)
    {
        x0[n] = std::complex<double>(3.0, 4.0);
        x0[(size_t) n+N/2] = std::complex<double>(3.0, 4.0);

        x1[n] = std::complex<double>(3.0, 4.0);
        x1[(size_t) n+N/2] = std::complex<double>(0.0, 0.0);
    }

    x0 = AMC::removeNegFreq(x0);

    QCOMPARE(x0, x1);
}

void AMCUnitTest::meanReal()
{
    size_t N = 20;
    std::vector<double> x(N);
    for (size_t n = 0; n < N; ++n)
    {
        x[n] = (double) n;
    }
    double mu = AMC::mean(x);

    auto muActual = accuracyRound(mu);
    auto muExpected = accuracyRound(9.5);
    QCOMPARE(muActual, muExpected);
}

void AMCUnitTest::meanComplex()
{
    size_t N = 20;
    std::vector<std::complex<double> > x(N);
    for (size_t n = 0; n < N; ++n)
    {
        x[n] = std::complex<double>((double) n, (double) n);
    }

    auto muActual = accuracyRound(AMC::mean(x));
    auto muExpected = accuracyRound(std::complex<double>(9.5, 9.5));

    QCOMPARE(muActual, muExpected);
}

void AMCUnitTest::stdDevReal()
{
    size_t N = 10;
    std::vector<double> a(N);
    for (size_t n = 0; n<N; ++n)
    {
        a[n] = (double) n;
    }
    auto b = AMC::stdDev(a);
    int stdDevActual = accuracyRound(b);
    int stdDevExpected = accuracyRound(2.872281323269);

    QCOMPARE(stdDevActual, stdDevExpected);
}

void AMCUnitTest::stdDevComplex()
{
    size_t N = 10;
    std::vector<std::complex<double> > a(N);
    for (size_t n = 0; n<N; ++n)
    {
        a[n] = std::complex<double>((double) n,0);
    }
    auto b = AMC::stdDev(a);
    int stdDevActual = accuracyRound(b);
    int stdDevExpected = accuracyRound(2.872281323269);

    QCOMPARE(stdDevActual, stdDevExpected);
}

void AMCUnitTest::stdDevKurtosisReal()
{
    size_t N = 10;
    std::vector<std::complex<double> > a(N);
    for (size_t n = 0; n<N; ++n)
    {
        a[n] = std::complex<double>((double) n, 0);
    }

    double stdDev = 0;
    double kurt = 0;

    AMC::stdDevKurtosis(a,stdDev,kurt);

    auto stdDevActual = accuracyRound(stdDev);
    auto stdDevExpected = accuracyRound(2.872281323269);

    auto kurtosisActual = accuracyRound(kurt);
    auto kurtosisExpected = accuracyRound(1.775757575757);

    QCOMPARE(stdDevActual, stdDevExpected);
    QCOMPARE(kurtosisActual, kurtosisExpected);
}

void AMCUnitTest::stdDevKurtosisComplex()
{
    size_t N = 10;
    std::vector<double> a(N);
    for (size_t n = 0; n<N; ++n)
    {
        a[n] = (double) n;
    }

    double stdDev = 0;
    double kurt = 0;

    AMC::stdDevKurtosis(a,stdDev,kurt);
    auto stdDevActual = accuracyRound(stdDev);
    auto stdDevExpected = accuracyRound(2.872281323269);

    auto kurtosisActual = accuracyRound(kurt);
    auto kurtosisExpected = accuracyRound(1.775757575757);

    QCOMPARE(stdDevActual, stdDevExpected);
    QCOMPARE(kurtosisActual, kurtosisExpected);
}

void AMCUnitTest::differentiate()
{
    size_t N = 20;
    std::vector<double> x(N);


    for (size_t n = 0; n < N; ++n)
    {
        x[n] = (double) n/2;
    }

    auto dxds = AMC::differentiate(x);

    for (size_t n = 0; n < dxds.size(); ++n)
    {
        auto dxdsActual = accuracyRound(dxds[n]);
        auto dxdsExpected = accuracyRound(0.5);
        QCOMPARE(dxdsActual, dxdsExpected);
    }
}

void AMCUnitTest::differentiateInf()
{
    size_t N = 2;
    std::vector<double> x(N);

    x[0] = std::numeric_limits<double>::min();
    x[1] = std::numeric_limits<double>::max();

    auto dxds = AMC::differentiate(x);

    for (size_t n = 0; n < dxds.size(); ++n)
    {
        auto dxdsActual = accuracyRound(dxds[n]);
        auto dxdsExpected = accuracyRound(INFINITY);
        QCOMPARE(dxdsActual, dxdsExpected);
    }
}

void AMCUnitTest::differentiateZero()
{
    size_t N = 20;
    std::vector<double> x(N);

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = (double) 2;
    }

    auto dxds = AMC::differentiate(x);
    for (size_t n = 0; n < dxds.size(); ++n)
    {
        auto dxdsActual = accuracyRound(dxds[n]);
        auto dxdsExpected = accuracyRound(0);
        QCOMPARE(dxdsActual, dxdsExpected);
    }
}

void AMCUnitTest::symmetryDSB()
{
    size_t N = 20;
    std::vector<std::complex<double> > x(N);
    double fcn = 0.4;

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = std::complex<double>(5.0, 5.0);
    }

    auto sActual = accuracyRound(AMC::symmetry(x, fcn));
    auto sExpected = accuracyRound(0);

    QCOMPARE(sActual, sExpected);
}

void AMCUnitTest::symmetryUSB()
{
    size_t N = 20;
    std::vector<std::complex<double> > x(N);
    double fcn = 0.45;

    for (size_t n = 0; n < N/2; ++n)
    {
        x[n] = std::complex<double>(0, 0);
        x[n+N/2] = std::complex<double>(5, 5);
    }

    auto sActual = accuracyRound(AMC::symmetry(x, fcn));
    auto sExpected = accuracyRound(-1);

    QCOMPARE(sActual, sExpected);
}

void AMCUnitTest::symmetryLSB()
{
    size_t N = 20;
    std::vector<std::complex<double> > x(N);
    double fcn = 0.45;

    for (size_t n = 0; n < N/2; ++n)
    {
        x[n] = std::complex<double>(5, 5);
        x[n+N/2] = std::complex<double>(0, 0);
    }

    auto sActual = accuracyRound(AMC::symmetry(x, fcn));
    auto sExpected = accuracyRound(1);

    QCOMPARE(sActual, sExpected);
}

void AMCUnitTest::maxPowerReal()
{
    std::vector<double> x = {1,2,3,4,5,-6,5,4,3,2,1};
    size_t k = 0;
    auto maxPowActual = accuracyRound(AMC::maxPower(x, k));
    auto maxPowExpected = accuracyRound(36);

    QCOMPARE(maxPowActual, maxPowExpected);
    QCOMPARE(k, (size_t) 5);
}

void AMCUnitTest::maxPowerComplex()
{
    size_t N = 10;
    std::vector<std::complex<double> > x(N);

    size_t k = 0;
    for (size_t n = 0; n < N; ++n)
    {
        x[n] = std::complex<double>((double) n, (double) N-n-1);
    }
    auto maxPowActual = accuracyRound(AMC::maxPower(x,k));
    auto maxPowExpected = accuracyRound(9*9);

    QCOMPARE(maxPowActual, maxPowExpected);
    QCOMPARE(k, (size_t) 0);
}

void AMCUnitTest::centerReal()
{
    size_t N = 10;
    std::vector<double> x(N);

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = 5.123123123123123;
    }

    auto xc = AMC::center(x);

    for (size_t n = 0; n < N; ++n)
    {
        auto cActual = accuracyRound(xc[n]);
        auto cExpected = accuracyRound(0);

        QCOMPARE(cActual, cExpected);
    }
}

void AMCUnitTest::centerComplex()
{
    size_t N = 10;
    std::vector<std::complex<double> > x(N);

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = std::complex<double>(PI,PI);
    }

    auto xc = AMC::center(x);

    for (size_t n = 0; n < N; ++n)
    {
        auto cActual = accuracyRound(xc[n]);
        auto cExpected = accuracyRound(std::complex<double>(0,0));

        QCOMPARE(cActual.imag(), cExpected.imag());
        QCOMPARE(cActual.real(), cExpected.real());
    }
}

void AMCUnitTest::normalizeReal()
{
    size_t N = 10;
    std::vector<double> x(N);
    std::vector<double> xn(N);

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = (double) n;
        xn[n] = (double) n/9;
    }

    x = AMC::normalize(x);

    for (size_t n = 0; n < N; ++n)
    {
        auto xnActual = accuracyRound(x[n]);
        auto xnExpected = accuracyRound(xn[n]);

        QCOMPARE(xnActual, xnExpected);
    }
}

void AMCUnitTest::normalizeComplex()
{
    size_t N = 10;
    std::vector<std::complex<double> > x(N);
    std::vector<std::complex<double> > xn(N);

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = std::complex<double>((double) n, (double) n);
        xn[n] = std::complex<double>((double) n, (double) n) / std::abs(std::complex<double>(9,9));
    }

    x = AMC::normalize(x);

    for (size_t n = 0; n < N; ++n)
    {
        auto xnActual = accuracyRound(x[n]);
        auto xnExpected = accuracyRound(xn[n]);

        QCOMPARE(xnActual.imag(), xnExpected.imag());
        QCOMPARE(xnActual.real(), xnExpected.real());
    }
}

void AMCUnitTest::absReal()
{
    size_t N = 10;
    std::vector<double> x(N);
    std::vector<double> xn(N);

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = double(n) * std::pow(-1, n);
        xn[n] = (double) n;
    }

    x = AMC::abs(x);

    for (size_t n = 0; n < N; ++n)
    {
        auto xnActual = accuracyRound(x[n]);
        auto xnExpected = accuracyRound(xn[n]);

        QCOMPARE(xnActual, xnExpected);
    }
}

void AMCUnitTest::absComplex()
{
    size_t N = 10;
    std::vector<std::complex<double> > x(N);
    std::vector<double> xn(N);

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = std::complex<double>((double) n * std::pow(-1,n), (double) n * std::pow(-1,n+1));
        xn[n] = std::sqrt(2*n*n);
    }

    auto x1 = AMC::abs(x);

    for (size_t n = 0; n < N; ++n)
    {
        auto xActual = accuracyRound(x1[n]);
        auto xExpected = accuracyRound(xn[n]);

        QCOMPARE(xActual, xExpected);
    }
}


void AMCUnitTest::absMaxReal()
{
    size_t N = 10;
    std::vector<double> x(N);

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = double(n) * std::pow(-1, n);
    }

    auto am = AMC::absMax(x);

    auto xnActual = accuracyRound(am);
    auto xnExpected = accuracyRound(9);

    QCOMPARE(xnActual, xnExpected);
}

void AMCUnitTest::absMaxComplex()
{
    size_t N = 10;
    std::vector<std::complex<double> > x(N);

    for (size_t n = 0; n < N; ++n)
    {
        x[n] = std::complex<double>((double) n * std::pow(-1,n), (double) n * std::pow(-1,n+1));
    }

    auto am = AMC::absMax(x);

    auto xActual = accuracyRound(am);
    auto xExpected = accuracyRound(std::sqrt(2*9*9));

    QCOMPARE(xActual, xExpected);
}

QTEST_APPLESS_MAIN(AMCUnitTest)

#include "tst_amcunittest.moc"
