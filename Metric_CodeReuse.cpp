using namespace std;

#include "Metric_CodeReuse.h"

Metric_CodeReuse::Metric_CodeReuse() : Metric() {}; 
Metric_CodeReuse::Metric_CodeReuse(string fn) : Metric(fn) {};

void Metric_CodeReuse::analyze(string fn) {
    ifstream infile;
    string line;

    this->fn = fn;

    infile.open(fn.c_str());
    
    do {
        getline(infile, line);

        // Trim whitespace
        //
        line.erase( remove_if(line.begin(), line.end(), ::isspace ), line.end());

        //cout << endl << "L: " << line << "|" << line.length() << endl;

        // Ignore lines shorter than 7 characters.
        if(line.length() > 7) {
            // Check if this is in the hashtable
            if(h.get(line) == -1) {
                // New Entry
                h.put(line, 1);
            } else {
                // Increment
                h.put(line, h.get(line) + 1);
            }
        }

    } while (!infile.eof());

    score = (h.getValsOverTwo() * 2);
    if(score >100) score = 100;
}

int Metric_CodeReuse::basicScore(ofstream& o) {
    if(this->score==-1) return -1;
    o << " Code Re-Use Score: " << score << endl;
    return 0;
}




int Metric_CodeReuse::detailedScore(ofstream& o) {
    if(this->score==-1) return -1;
    o << " Code Re-Use Score: " << score << endl;
    o << "  Number of reused lines: " << h.getValsOverTwo() << endl;
    o << "  Average # of Lines of Reuse: " << h.getAvgValue() << endl;
    o << "  Most reused line # of times: " << h.getHighestValue() << endl;
    o << "  Most reused line: " << h.getLargestValKey() << endl;

    return 0;
}
int Metric_CodeReuse::total(){
    return score;
}
