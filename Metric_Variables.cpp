using namespace std;

// A Metric judging variable and function names by their deviation from
// various standards, or if there is a consistant standard used

#include "Metric_Variables.h"

Metric_Variables::Metric_Variables() : Metric() {
    initVariables();
}; 
Metric_Variables::Metric_Variables(string fn) : Metric(fn) {
    initVariables();
};

void Metric_Variables::initVariables() {
    variables.add("bool");
    variables.add("char");
    variables.add("int");
    variables.add("float");
    variables.add("double");
    variables.add("void");
    variables.add("string");

    this->count_numofvars=0;
    this->count_nonsense=0;
    this->count_varlength=0;
    this->count_camelcase=0;
    this->count_delimiter=0;
}

int Metric_Variables::getScore() {
    // If the difference between camel is less than 25% of total
    // penalize 2 points for each percentage too close
    int retVal=100;
    
    float camelTotal = (float)this->count_camelcase / this->count_numofvars;
    float delTotal = (float)this->count_delimiter / this->count_numofvars;

    if(abs(delTotal-camelTotal) < 0.25) {
        retVal-= (int)(abs(delTotal-camelTotal) * 100 * 2);
    }

    // Lose 1 point for each nonsense variable
    retVal-= (int)((float)(this->count_nonsense)/this->count_numofvars * 100);

    // Lose 5 points for each number over average of 10 you are
    float s = (float)this->count_varlength / this->count_numofvars;

    if(s>10) {
        retVal-= ((s-10) * 5);
    }

    retVal = abs(retVal-100);

    if(retVal>100) retVal=100;

    return retVal;
}

void Metric_Variables::judgeWord(string word) {
    if (word.length()<2) return;

    this->count_numofvars++;

    int camelcount;
    int delcount;
    delcount = 0;
    camelcount = 0;
    // Look for CamelCase
    for (int x=0; x<word.length(); x++) {
        // One of the first two letters should be uppercase
        if(isupper(word.at(0)) || isupper(word.at(1))) {
            if(isupper(word.at(x))) camelcount++;
        }
    }
    if(camelcount/(float)word.length()<0.21 && camelcount/(float)word.length()>0.05) {
        this->count_camelcase++;
    } else {
        // Look for delimiter
        for (int x=1; x<word.length(); x++) {
            if(word.at(x)=='_') {
                this->count_delimiter++;
                break;
            }
        }
    }

    this->count_varlength+=word.length();

    // Nonsense names (too many of one letter in a row, odd casing
    int crazycount = 0;
    camelcount = 0;
    char a, b, c, d;
    for (int x=0; x<word.length(); x++) {
        a = word.at(x);
        if(isupper(word.at(x))) camelcount++;
        if(x>2) {
           if(a==b && b==c && c==d) 
               crazycount++;
        }
        
        b = a;
        if(x>0) c = b;
        if(x>1) d = c;
    }

    if(crazycount>1 || (camelcount/(float)word.length()>0.4 && camelcount/(float)word.length()<0.8)) {
        this->count_nonsense++;
    }
}

void Metric_Variables::analyze(string fn) {
    ifstream infile;
    string line;
    string word;
    bool nextVariableName;
    this->fn = fn;

    infile.open(fn.c_str());
    
    word = "";
    nextVariableName = false;
    do {
        getline(infile, line);

        for(int x=0; x<line.length(); x++) {
            // Space, end of word
            if(line.at(x) == ' ' || line.at(x) == '}' || line.at(x) == ')' || line.at(x) == '\n' || line.at(x)=='(') {
                // This is a variable name
                if(nextVariableName==true) {
                    judgeWord(word);
                }
                // Is this a variable type?
                if(variables.getEntry(word)!=NULL) {
                    // Our next word will be a variable name
                    nextVariableName = true;
                }
                x++;
                word = "";
            }

            // Ignore the indication that it is a pointer
            if(x<line.length() && line.at(x)!='*')
                word+=line.at(x);
        }

        nextVariableName = false;
        word = "";
    } while (!infile.eof());
}

int Metric_Variables::basicScore(ofstream& o) {
    if(this->count_numofvars==0) return -1;
    o << " Variable/Function Quality Score: " << this->getScore() << endl;
    score = this->getScore();
    return 0;
}

int Metric_Variables::detailedScore(ofstream& o) {
    //cout << "count_numofvars " << count_numofvars << endl;
    if(this->count_numofvars==0) return -1;
    o << " Variable/Function Quality Score: " << this->getScore() << endl;
    o << "  CamelCase names: " << this->count_camelcase << endl;
    o << "  Delimited names: " << this->count_delimiter << endl;
    o << "  Non-standard names: " << this->count_nonsense << endl;
    o << "  Number of names total: " << this->count_numofvars << endl;
    this->score = this->getScore();

    return 0;


}

int Metric_Variables::total(){
    return score;
}

