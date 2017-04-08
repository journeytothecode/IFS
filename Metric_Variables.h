#ifndef METRIC_VAR_H
#define METRIC_VAR_H

#include "GoodEnough.h"
#include "Metric.h"
#include "AVLTree.h"

class Metric_Variables : public Metric {
    public:
        Metric_Variables();
        Metric_Variables(string fn);

        void analyze(string fn);

        int basicScore(ofstream &o);

        int detailedScore(ofstream &o);
int total();
    private:
        void initVariables();
        void judgeWord(string w);
        int getScore();

        AVLSearchTree<string> variables;

        int count_camelcase;
        int count_delimiter;
        int count_nonsense;
        int count_varlength;
        int count_numofvars;
};

#endif
