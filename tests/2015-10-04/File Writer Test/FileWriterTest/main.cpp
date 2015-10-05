#include <QCoreApplication>
#include "amc.h"
#include "filewriter.h"
#include <iostream>
#include "sharedbuffer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << "this is a test of the FileWriter" << std::endl;

    FileWriter fw;
    fw.newFile(AMC::ModType::AM_DSB_FC);
    std::vector<double> b(1000);

    size_t N = 100000;

    for(size_t n = 0; n < N; ++n)
    {
        for (size_t n = 0; n < b.size(); ++n)
        {
            b[n] = n;
        }
        fw.writeToFile(b);
    }
    std::cout << "loop done" << std::endl;


    return a.exec();
}
