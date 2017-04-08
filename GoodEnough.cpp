using namespace std;

#include "GoodEnough.h"
#include "Metric_LoC.h"
#include "Metric_Variables.h"
#include "Metric_CodeReuse.h"
#include "Metric_Complexity.h"
#include "Metric_FunctionSize.h"

void usage(char* pgname) {
    cout << "Usage: " << pgname << " [-b|-v] INPUTFILE OUTPUTFILE\n"
       << " -b: Brief output\n -v: Verbose output\n"
       << " INPUTFILE: file containing path to code to analyze\n"
       << " OUTPUTFILE: file to output analysis to\n";
}

void scan(string input_path, ofstream& ostream, char output_type){
    // Get list of files and folders in directory
    // If file, open and pass thru each of the metrics
    // If folder, recurse back to scan()

    DIR *dp;
    struct dirent *entry;
    struct stat s;
    string filenm; 
    queue<string> dirstoscan;

    Metric_LoC m_LoC;
    Metric_CodeReuse m_cr;
    Metric_Complexity m_complex;
    Metric_FunctionSize m_fs;
    Metric_Variables m_var;
    int total =0;

nextDir:
    //cout << " Scanning " << input_path << endl;
    dp = opendir(input_path.c_str());

    if(dp==NULL) return;

    while(entry = readdir(dp)) {
        filenm = input_path + "/" + entry->d_name;

        if( stat(filenm.c_str(), &s) == 0 ) {
            switch(s.st_mode & S_IFMT) {
                case S_IFDIR:
                    if ( strncmp(entry->d_name, ".", 1) != 0 && strncmp(entry->d_name, "..", 2) != 0 ) {
                        dirstoscan.push(filenm);
                        //scan(filenm, ostream, output_type);
                    }
                    break;
                case S_IFREG:
                    // Verify that it is a valid filetype
                    if ( filenm.substr(filenm.size() - 4, 4) == ".cpp" ||
                         filenm.substr(filenm.size() - 4, 4) == ".hpp" ||
                         filenm.substr(filenm.size() - 2, 2) == ".c" ||
                         filenm.substr(filenm.size() - 2, 2) == ".h" ) {

                        // Analyze

                        m_LoC.analyze(filenm);
                        m_cr.analyze(filenm);
                        m_complex.analyze(filenm);
                        m_fs.analyze(filenm);
                        m_var.analyze(filenm);
                        //total += m_LoC.total() + m_cr.total() + m_complex.total() +  m_fs.total() + m_var.total();
                    }

                    break;
            }
        }
        else
        {
            cerr << "Failed to stat file " << filenm;
        }
    }

    closedir(dp);
    
    if(dirstoscan.size()>0) {
        input_path = dirstoscan.front();
        dirstoscan.pop();
        goto nextDir;
    }

    //int total = m_LoC.total() + m_cr.total() + m_complex.total() +  m_fs.total() + m_var.total();

    if(output_type=='b') {
        cout << "Outputting Brief Score!" << endl;
        ostream << endl;
        m_LoC.basicScore(ostream);
        ostream << endl;
        m_cr.basicScore(ostream);
        ostream << endl;
        m_complex.basicScore(ostream);
        ostream << endl;
        m_fs.basicScore(ostream);
        ostream << endl;
        m_var.basicScore(ostream);
        ostream << endl;
        total = m_LoC.total() + m_cr.total() + m_complex.total() +  m_fs.total() + m_var.total();
        ostream << "================================================";
        ostream << "\n  Total score: " << total << " points"<< endl;
    } else
    {
        cout << "Outputting Verbose Score!" << endl;
        ostream << endl;
        m_LoC.detailedScore(ostream);
        ostream << endl;
        m_cr.detailedScore(ostream);
        ostream << endl;
        m_complex.detailedScore(ostream);
        ostream << endl;
        m_fs.detailedScore(ostream);
        ostream << endl;
        m_var.detailedScore(ostream);
        ostream << endl;
        total = m_LoC.total() + m_cr.total() + m_complex.total() +  m_fs.total() + m_var.total();
        ostream << "================================================";
        ostream << "\n  Total score: " << total << " points"<< endl;
    }

}

int main(int argc, char* argv[]) {
    string input_path;
    char output_type;
    ifstream istream;
    ofstream ostream;

    // Exit if invalid number of parameters
	if (argc != 4) { 
        cerr << "Invalid number of arguments.\n\n";
        usage(argv[0]); 
        return -1; 
    }

    // Confirm output_type and exit if invalid
    output_type = 'x';
    if( strncmp(argv[1], "-b", 2)==0 || strncmp(argv[1], "-v", 2)==0 ) { 
         output_type = argv[1][1];
    } 
    
	if (output_type == 'x') {
        cerr << "Invalid output type.\n\n";
        usage(argv[0]); return -1; 
    }

    // Open our files
    istream.open(argv[2]);
    if(!istream) {
        cerr << argv[2] << " file does not exist!" << endl;
        return -1;
    }
    istream >> input_path;
    istream.close();

    ostream.open(argv[3]);

    scan(input_path, ostream, output_type);

    ostream.close();

    return 0;
}
