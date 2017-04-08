#ifndef METRIC_LOC_H
#define METRIC_LOC_H

#include "GoodEnough.h"
#include "Metric.h"

class Metric_LoC : public Metric {
    public:
        Metric_LoC();
        Metric_LoC(string fn);

        void analyze(string fn);

        int basicScore(ofstream &o);

        int detailedScore(ofstream &o);
int total();
    private:
        int maxLines;
        double avgLines;
        int overallLines;
        int overallCount;
};

#endif
