#ifndef METRIC_COMPLEX_H
#define METRIC_COMPLEX_H

#include "GoodEnough.h"
#include "Metric.h"

class Metric_Complexity : public Metric {
    public:
        Metric_Complexity();
        Metric_Complexity(string fn);

        void analyze(string fn);

        int basicScore(ofstream &o);

        int detailedScore(ofstream &o);
int total();
    private:
        int overallDepth;
        int overallCount;
        int mostDepth;
};

#endif
