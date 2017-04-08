using namespace std;

// Ideal Literal Lines of Code per Line is 1.05; we make allowances
// for some times when it would be better but in general it is better
// to spread the code out for readability.

#include "Metric_LoC.h"

Metric_LoC::Metric_LoC() : Metric() {
    this->overallLines = 0;
    this->overallCount = 0;
    this->maxLines = 0;
}; 
Metric_LoC::Metric_LoC(string fn) : Metric(fn) {
    this->overallLines = 0;
    this->overallCount = 0;
    this->maxLines = 0;
};

void Metric_LoC::analyze(string fn) {
    ifstream infile;
    bool inComment, inBlockComment;
    char b, c;
    int curCount, totalCount, lines;

    this->fn = fn;

    infile.open(fn.c_str());
    
    inComment = false;
    inBlockComment = false;
    lines = 0;
    curCount = 0;
    totalCount = 0;
    do {
        c = infile.get();

        // Ignore comments
        if(b=='/'&&c=='/') { inComment=true; }
        if(b=='/'&&c=='*') { inBlockComment=true; }

        if(b=='*'&&c=='/') { inBlockComment=false; }

        if(c=='\n') { 
            if(!(inBlockComment||inComment) && curCount!= 0) {
                totalCount+=curCount;
                if(this->maxLines < curCount) { this->maxLines = curCount; }
                curCount = 0;
                lines++;
            }
            inComment=false;
        }

        if(!(inBlockComment||inComment) && (c=='}'||c==';')) {
            curCount++;
        }

        b = c;
    } while (!infile.eof());


    // Handle empty files
    if(lines==0) {
        this->score = -1;
        return;
    }
    this->avgLines = (float)totalCount / (float)lines;

    this->overallLines+=lines;
    this->overallCount+=totalCount;
}

int Metric_LoC::basicScore(ofstream& o) {
    if(this->overallLines==0) return -1;
    double avgLines = (float)this->overallCount / (float)this->overallLines;
    score = (int)(abs(avgLines - 1.05) * 500.0);

    o << " Literal Lines of Code Score: " << score << endl;
    return 0;
}

int Metric_LoC::detailedScore(ofstream& o) {
    if(this->overallLines==0) return -1;
    double avgLines = (float)this->overallCount / (float)this->overallLines;
    score = (int)(abs(avgLines - 1.05) * 500.0);

    o << " Literal Lines of Code Score: " << score << endl;
    o << "  Average Literal Lines of Code: " << avgLines << endl;
    o << "  Total Physical Lines of Code: " << this->overallLines << endl;
    o << "  Most Literal Lines in Physical Line of Code: " << this->maxLines << endl;
    return 0;
    }

int Metric_LoC::total(){
    return score;
}
