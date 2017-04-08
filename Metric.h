#ifndef METRIC_H
#define METRIC_H

#include "GoodEnough.h"

class Metric {
    public:
        Metric();
        Metric(string s);
        // Analyze file
        virtual void analyze(string fn);
        // Output the basic score for this metric to ostream
        // Returns 0 if successful, -1 if no metric has been determined 
        virtual int basicScore(ofstream &o);
        // Output the detailed score for this metric to ostream
        // Returns 0 if successful, -1 if no metric has been determined 
        virtual int detailedScore(ofstream &o);    
    protected:
        // All metrics score between 0 and 100 'goodness'
        // Score initalizes to -1 before analysis is done
        int score;
        string fn;
};

#endif
