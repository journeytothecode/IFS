#ifndef METRIC_CODER_H
#define METRIC_CODER_H

#include "GoodEnough.h"
#include "Metric.h"
#include "HashTable.h"

class Metric_CodeReuse : public Metric {
    public:
        Metric_CodeReuse();
        Metric_CodeReuse(string fn);

        void analyze(string fn);

        int basicScore(ofstream &o);

        int detailedScore(ofstream &o);

        int total();
    private:
        HashTable<string> h;

};

#endif
