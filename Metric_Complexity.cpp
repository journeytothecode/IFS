using namespace std;

// Determine the complexity of the code by seeing how deep we go past the first {} in each function.  If functions are too
// deep then the code may be too complicated.
// Score is determined by 100 - (abs(avgDepthOfFunction - 1.5) * 20) and cannot be below 0.

#include "Metric_Complexity.h"

Metric_Complexity::Metric_Complexity() : Metric() {
    this->overallDepth = 0;
    this->overallCount = 0;
    this->mostDepth = 0;
}; 
Metric_Complexity::Metric_Complexity(string fn) : Metric(fn) {
    this->overallDepth = 0;
    this->overallCount = 0;
    this->mostDepth = 0;
};

void Metric_Complexity::analyze(string fn) {
    ifstream infile;
    char b, c;
    int cDepth=0;
    int cMax=0;

    int depthTotals=0;
    int depthNum=0;

    this->fn = fn;

    infile.open(fn.c_str());
    
    do {
        c = infile.get();

        if ( c == '{' ) cDepth++;

        if ( c == '}' ) 
        {
            if (cDepth > cMax) cMax = cDepth;

            cDepth--;

            if(cDepth == 0) {
                depthTotals += cMax;
                depthNum++;
                if(this->mostDepth < cMax) this->mostDepth = cMax;
                cMax = 0;
            }
        }

    } while (!infile.eof());


    this->overallDepth+=depthTotals;
    this->overallCount+=depthNum;
}

int Metric_Complexity::basicScore(ofstream& o) {
    if(this->overallDepth==0) return -1;
    float avgDepth = (float)this->overallDepth / (float)this->overallCount;
    score = (fabsf(avgDepth - 1.5) * 20);
    if(score > 100) this->score=100;
    o << " Overall Complexity Score: " << score << endl;
    return 0;
}

int Metric_Complexity::detailedScore(ofstream& o) {
    if(this->overallDepth==0) return -1;
    float avgDepth = (float)this->overallDepth / (float)this->overallCount;
    score = (fabsf(avgDepth - 1.5) * 20);
    if(score > 100) this->score=100;
    o << " Overall Complexity Score: " << score << endl;
    o << "  Average Complexity: " << (float)this->overallDepth / (float)this->overallCount << endl;
    o << "  Deepest Function Depth: " << this->mostDepth << endl;
    return 0;
}
int Metric_Complexity::total(){

    return score;
}
