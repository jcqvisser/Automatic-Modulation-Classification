#include <QCoreApplication>
#include "amc.h"
#include "classifier/amcclassifier.h"
#include "classifier/amczndescisiontree.h"
#include "classifier/znbranchnode.h"
#include "classifier/znleafnode.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    boost::shared_ptr<ZnLeafNode> lnl(
                new ZnLeafNode(AMC::ModType::AM_DSB_FC,
                               AMC::ModType::AM_USB_FC,
                               AMC::Feature::MU_42_F));
    boost::shared_ptr<ZnLeafNode> lnr(
                new ZnLeafNode(AMC::ModType::AM_DSB_SC,
                               AMC::ModType::AM_USB_SC,
                               AMC::Feature::SIGMA_AF));
    boost::shared_ptr<ZnBranchNode> bn(
                new ZnBranchNode(
                    lnl,
                    lnr,
                    AMC::Feature::SIGMA_DP));
    AmcZnDecisionTree dt(bn);

    std::vector<std::vector<double> > f;
    std::vector<AMC::ModType> m;
    for (size_t n = 0; n < 20; ++n)
    {
        std::vector<double> f1(9);
        for (size_t n1 = 0; n1 < 9; ++n1)
        {
            f1[AMC::SIGMA_DP] = n;
            f1[AMC::SIGMA_AF] = n;
            f1[AMC::MU_42_F] = n;
        }
        f.push_back(f1);

        if (n < 5)
            m.push_back(AMC::AM_DSB_FC);
        else if ((n >= 5) && (n < 10))
            m.push_back(AMC::AM_USB_FC);
        else if ((n >= 10) && (n < 15))
            m.push_back(AMC::AM_DSB_SC);
        else
            m.push_back(AMC::AM_USB_SC);
    }

    dt.train(f,m);
    dt.unload("test0");

    boost::shared_ptr<ZnLeafNode> lnl1(
                new ZnLeafNode(AMC::ModType::AM_DSB_FC,
                               AMC::ModType::AM_USB_FC,
                               AMC::Feature::MU_42_F));
    boost::shared_ptr<ZnLeafNode> lnr1(
                new ZnLeafNode(AMC::ModType::AM_DSB_SC,
                               AMC::ModType::AM_USB_SC,
                               AMC::Feature::SIGMA_AF));
    boost::shared_ptr<ZnBranchNode> bn1(
                new ZnBranchNode(
                    lnl1,
                    lnr1,
                    AMC::Feature::SIGMA_DP));
    AmcZnDecisionTree dt1(bn1);

    dt1.load("test0");
    dt1.unload("test1");



    return a.exec();
}
