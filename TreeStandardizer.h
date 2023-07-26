
#ifndef TREESTANDARDIZER_H_
#define TREESTANDARDIZER_H_

#include "lexer.h"
#include "treeNode.h"

using namespace std;

// This class is used to standardize the tree. It is used to convert the tree
// into a form that is easier to work with. It is used to convert the tree
class TreeStandardizer {
    // The top node of the tree
    void standardize(treeNode*);
    void standardizeLET(treeNode*);
    void standardizeWHERE(treeNode*);
    void standardizeWITHIN(treeNode*);
    void standardizeREC(treeNode*);
    void standardizeFCNFORM(treeNode*);
    void standardizeLAMBDA(treeNode*);
    void standardizeAND(treeNode*);
    void standardizeAT(treeNode*);

public:
    TreeStandardizer(treeNode* topNode);
    virtual ~TreeStandardizer();
};

#endif /* TREESTANDARDIZER_H_ */
