using namespace std;

// Determine the average size of functions; functions that are very long are 
// not efficient
// Score determined by score = 100 - (avgLength - 30 * 1.2) for functions with length > 30.
// For length <= 30 score is 100.  It cannot be below 0

#include "Metric_FunctionSize.h"

Metric_FunctionSize::Metric_FunctionSize() : Metric() {
    this->maxLength = 0;
    this->totalLength = 0;
    this->numFunc = 0;
};
Metric_FunctionSize::Metric_FunctionSize(string fn) : Metric(fn) {
    this->totalLength = 0;
    this->numFunc = 0;
    this->maxLength = 0;
};

void Metric_FunctionSize::analyze(string fn) {
    ifstream infile;
    int flen;
    int fdepth;
    bool inFunc;
    char c;

    this->fn = fn;

    infile.open(fn.c_str());
    
    inFunc = false;
    flen = 0;
    fdepth = 0;
    do {
        c = infile.get();

        if ( c == '{' )  {
            if(!inFunc) fdepth = 0;
            inFunc = true;
            fdepth++;
        }

        if ( c == '\n' ) {
            flen++;
        }

        if ( c == '}' ) 
        {
            if(--fdepth==0) {
                this->totalLength += flen;
                this->numFunc++;

                if(flen>this->maxLength) {
                    this->maxLength = flen;
                }

                flen = 0;
                inFunc = false;
            }
        }

    } while (!infile.eof());
}

int Metric_FunctionSize::basicScore(ofstream& o) {
    if(this->maxLength==0) return -1;
    float avgLength = this->totalLength / (float)this->numFunc;
    this->score = (int)(abs(avgLength - 30) * 1.2);
    if(avgLength<=30) this->score=0;
    if(this->score > 100) this->score=100;
    o << " Function Size Score: " << this->score << endl;
    return 0;
}

int Metric_FunctionSize::detailedScore(ofstream& o) {
    if(this->maxLength==0) return -1;
    float avgLength = this->totalLength / (float)this->numFunc;
    this->score = (int)(abs(avgLength - 30) * 1.2);
    if(avgLength<=30) this->score=0;
    if(this->score > 100) this->score=100;

    o << " Function Size Score: " << this->score << endl;
    o << "  Average Length of Function: " << avgLength << endl;
    o << "  Total Function Length: " << this->totalLength << endl;
    o << "  Number of Functions: " << this->numFunc << endl;

    return 0;
}

int Metric_FunctionSize::total(){

    return score;
}
