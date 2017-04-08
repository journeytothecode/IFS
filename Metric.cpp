using namespace std;

#include "Metric.h"

Metric::Metric() {
        this->score = -1;
        return;
}

Metric::Metric(string fn) {
        this->score = -1;
        this->analyze(fn);
}

void Metric::analyze(string fn) {
    this->fn = fn;
}

int Metric::basicScore(ofstream& o) {
}

int Metric::detailedScore(ofstream& o) {
}

