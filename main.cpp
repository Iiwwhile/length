#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <utility>
using namespace std;

//Structure of storage length
struct inputData{
	string measure;
	double value;
};

//According to one input length (id) mapping Lookup transformation rules (linearMeasureMap) to calculate and return the results to m unit for the unit, it returns a 0 not found
double toMetre(inputData &id, map<string, double> linearMeasureMap);
//Decomposition of the string "src", for analysis and expression
void split(const string &src, const string &separator, vector<string> &dest);
//Output results to a file
void outputResult(vector<double> dataResult);

int main (){
	vector<double> dataResult;
	ifstream is("input.txt",ios_base::in);
	if(is){
		string line;

		//Understand the different units and unit conversion meters between rules.
		map<string, double> linearMeasureMap;//Store map of transformation rules
		string measureName;
		double measureLength;
		while(!is.eof()){
			getline(is, line);
			if(line.find("=") != line.npos){
				measureName = line.substr(line.find_first_of(' ')+1, line.find_first_of('=')-line.find_first_of(' ')-2);
				measureLength = atof(line.substr(line.find_first_of('=')+1,11).c_str());
				linearMeasureMap.insert (make_pair(measureName, measureLength));
			}
			else{
				break;
			}
		}

		while(!is.eof()){
			getline(is, line);
			if(line.find("=")==line.npos && line.length()!=0){
				inputData readin;
				//The expressed in different units of length are converted to the standard unit meters.
				if(line.find("+")==line.npos && line.find("-")==line.npos){
					readin.measure = line.substr(line.find_first_of(' ')+1, line.length()-line.find_first_of(' ')+1);
					readin.value = atof(line.c_str());
					dataResult.push_back(toMetre(readin, linearMeasureMap));
				}
				else{//Calculation of arithmetic expressions of different units of length, the results obtained in meters.
					double r=0.0;
					int symbol=1;
					vector<string> lineVec;
					split(line, " ", lineVec);
					for(vector<string>::iterator itr=lineVec.begin(); itr!=lineVec.end(); ){
						readin.value = atof((*itr).c_str());
						readin.measure = *(++itr);
						r += symbol * toMetre(readin, linearMeasureMap);
						if(++itr != lineVec.end()){
							if(*itr == "+" || *itr == "-"){
								if(*itr == "+")symbol=1;
								if(*itr == "-")symbol=-1;
								++itr;
							}
						}
					}
					dataResult.push_back(r);
				}
			}
		}
		//output result to a file
		outputResult(dataResult);
	}else{
		cerr<<"can't open file.\n";
	}
	is.close();
	return 0;
}

double toMetre(inputData &id, map<string, double> linearMeasureMap){
	//singular
	if     (id.measure == "miles")   {id.measure = "mile";}
	else if(id.measure == "yards")   {id.measure = "yard";}
	else if(id.measure == "inches")  {id.measure = "inch";}
	else if(id.measure == "feet")    {id.measure = "foot";}
	else if(id.measure == "faths")   {id.measure = "fath";}
	else if(id.measure == "furlongs"){id.measure = "furlong";}

	map<string, double>::iterator iter = linearMeasureMap.find(id.measure);
	if(iter == linearMeasureMap.end()){
		cout << id.measure << "is not available" << endl;
		return 0;
	}
	else{
		return id.value * (iter->second);
	}
}


void split(const string &src, const string &separator, vector<string> &dest)
{
	string str = src;
	string substring;
	string::size_type start = 0, index;

	do
	{
		index = str.find_first_of(separator,start);
		if (index != string::npos)
		{
			substring = str.substr(start,index-start);
			dest.push_back(substring);
			start = str.find_first_not_of(separator,index);
			if (start == string::npos) return;
		}
	}while(index != string::npos);

	//the last token
	substring = str.substr(start);
	dest.push_back(substring);
}

void outputResult(vector<double> dataResult){
	ofstream fileOut("output.txt");
	fileOut << "1013465716@qq.com" << endl;
	fileOut << endl;
	for(vector<double>::iterator it=dataResult.begin(); it!=dataResult.end(); it++ ){
		fileOut << setiosflags(ios_base::fixed) << setprecision(2) << (*it) << " m" << endl;
	}
}