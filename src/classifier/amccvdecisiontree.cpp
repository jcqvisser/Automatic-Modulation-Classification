#include "amccvdecisiontree.h"

AmcCvDecisionTree::AmcCvDecisionTree(int max_categories,
                                     int max_depth,
                                     int min_sample_count,
                                     int cv_folds,
                                     bool use_surrogates,
                                     bool use_1se_rule,
                                     bool truncate_pruned_tree,
                                     float regression_accuracy,
                                     const float *priors) :
    _treeParams(new CvDTreeParams( max_categories,
                                   max_depth,
                                   min_sample_count,
                                   cv_folds,
                                   use_surrogates,
                                   use_1se_rule,
                                   truncate_pruned_tree,
                                   regression_accuracy,
                                   priors) ),
    _tree(new CvDTree())
{

}

AMC::ModType AmcCvDecisionTree::classify(const std::vector<double> &predictData)
{
    cv::Mat predictMat(predictData.size(), 1, CV_32FC1);
    for(unsigned int rowN = 0; rowN < predictData.size(); ++rowN)
    {
        predictMat.at<float>(rowN, 0) = (float)predictData.at(rowN);
    }
    return (AMC::ModType) _tree->predict(predictMat)->value;
}

bool AmcCvDecisionTree::train(const std::vector<std::vector<double> > &trainData, const std::vector<AMC::ModType> &responses)
{
    cv::Mat dataMat(trainData.size(), trainData[0].size(), CV_32FC1);
    cv::Mat responseMat(responses.size(), 1, CV_32FC1);

    for(unsigned int rowN = 0; rowN < trainData.size(); ++rowN)
    {
        for(unsigned int colN = 0; colN < trainData[0].size(); ++colN)
        {
            dataMat.at<float>(rowN, colN) = (float)trainData.at(rowN).at(colN);
        }
        responseMat.at<float>(rowN, 0) = (float)responses.at(rowN);
    }

    cv::Mat varType(dataMat.cols + 1, 1, CV_8U);
    varType.setTo(CV_VAR_CATEGORICAL);
    cv::Mat missingMask(dataMat.size(), CV_8U, 0);
    cv::Mat varIdx;
    cv::Mat sampleIdx;

   return _tree->train(dataMat,
                 CV_ROW_SAMPLE,
                 responseMat);
}

void AmcCvDecisionTree::save(const std::string & fileName)
{
    std::string newFileName = fileName + ".xml";
    _tree->save(newFileName.c_str());
}

void AmcCvDecisionTree::load(const std::string & fileName)
{
    std::string newFileName = fileName + ".xml";
    _tree->load(newFileName.c_str());
}
