#include <QCoreApplication>
#include "amc.h"
#include "classifier/amcclassifier.h"
#include "classifier/amczndescisiontree.h"
#include "classifier/znbranchnode.h"
#include "classifier/znleafnode.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    boost::shared_ptr<ZnLeafNode> ln0(
                new ZnLeafNode(AMC::ModType::AM_DSB_FC,
                               AMC::ModType::FM,
                               AMC::Feature::GAMMA_MAX));
    boost::shared_ptr<ZnLeafNode> ln1(
                new ZnLeafNode(AMC::ModType::AM_DSB_FC,
                   AMC::ModType::FM,
                   AMC::Feature::SIGMA_A));

    boost::shared_ptr<ZnBranchNode> bn0(
                new ZnBranchNode(
                    boost::shared_ptr<ZnLeafNode>(ln0),
                    boost::shared_ptr<ZnLeafNode>(ln1),
                    AMC::Feature::SIGMA_AA));

    AmcZnDecisionTree dt(bn0);

    return a.exec();
}
