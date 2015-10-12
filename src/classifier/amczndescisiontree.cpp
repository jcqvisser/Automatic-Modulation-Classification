#include "amczndescisiontree.h"

AmcZnDecisionTree::AmcZnDecisionTree(
        boost::shared_ptr<ZnNode> startNode):
    _startNode(startNode)
{}

AmcZnDecisionTree::AmcZnDecisionTree()
{

	boost::shared_ptr<ZnNode> amDsbFcUnaryLeafPtr(
            new ZnUnaryLeafNode(
				AMC::ModType::AM_DSB_FC,
				AMC::Feature::MU_42_A ));
    boost::shared_ptr<ZnNode> fmUnaryLeafPtr(
            new ZnUnaryLeafNode(
				AMC::ModType::FM,
				AMC::Feature::GAMMA_MAX));

    boost::shared_ptr<ZnNode> usbLsbLeafPtr(
                new ZnLeafNode(
                    AMC::ModType::AM_USB_SC,
                    AMC::ModType::AM_LSB_SC,
                    AMC::Feature::P));
    boost::shared_ptr<ZnNode> dsb2PskLeafPtr(
                new ZnLeafNode(
                    AMC::ModType::AM_DSB_SC,
                    AMC::ModType::PSK_2,
                    AMC::Feature::SIGMA_A));
    boost::shared_ptr<ZnNode> mqamMpskLeafPtr(
                new ZnLeafNode(
                    AMC::ModType::MQAM,
                    AMC::ModType::MPSK,
                    AMC::Feature::SIGMA_A));
    boost::shared_ptr<ZnNode> mAsk2AskLeafPtr(
                new ZnLeafNode(
                    AMC::ModType::AM_DSB_FC,
                    AMC::ModType::MASK,
                    AMC::Feature::MU_42_A));


    boost::shared_ptr<ZnNode> gammaMaxBranchPtr(
                new ZnBranchNode(
                    mqamMpskLeafPtr,
                    fmUnaryLeafPtr,
                    AMC::Feature::GAMMA_MAX));

    boost::shared_ptr<ZnNode> sigmaApBranchPtr(
                new ZnBranchNode(
                    dsb2PskLeafPtr,
                    gammaMaxBranchPtr,
                    AMC::Feature::SIGMA_AP));

    boost::shared_ptr<ZnNode> pBranchPtr(
                new ZnBranchNode(
                    usbLsbLeafPtr,
                    sigmaApBranchPtr,
                    AMC::Feature::P));

	boost::shared_ptr<ZnNode> mu42ABranchPtr(
			    new ZnBranchNode(
					amDsbFcUnaryLeafPtr,
					mAsk2AskLeafPtr,
                    AMC::Feature::MU_42_A));

    boost::shared_ptr<ZnNode> sigmaDpBranchPtr(
                new ZnBranchNode(
                    pBranchPtr,
                    mu42ABranchPtr,
                    AMC::Feature::SIGMA_DP));

    _startNode = sigmaDpBranchPtr;
}


AMC::ModType AmcZnDecisionTree::classify(const std::vector<double> &predictData)
{
    return _startNode->classify(predictData);
}

bool AmcZnDecisionTree::train(const std::vector<std::vector<double> > &trainData, const std::vector<AMC::ModType> &responses)
{
    _startNode->train(trainData, responses);
    return true;
}

void AmcZnDecisionTree::load(const std::string &fileName)
{
    std::ifstream f(fileName);
    std::vector<std::string> lA;
    std::string line;

    while (std::getline(f,line))
        lA.push_back(line);

    _startNode->load(lA);
    f.close();
}

void AmcZnDecisionTree::save(const std::string &fileName)
{
    std::ofstream f(fileName);
    std::vector<std::string> l = _startNode->save();

    for (auto line: l)
        f << line << std::endl;

    f.close();
}
