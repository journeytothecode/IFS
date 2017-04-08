#ifndef METRIC_FSIZE_H
#define METRIC_FSIZE_H

#include "GoodEnough.h"
#include "Metric.h"

class Metric_FunctionSize : public Metric {
    public:
        Metric_FunctionSize();
        Metric_FunctionSize(string fn);

        void analyze(string fn);

        int basicScore(ofstream &o);

        int detailedScore(ofstream &o);
int total();
    private:
        int totalLength;
        int maxLength;
        int numFunc;
};

#endif
