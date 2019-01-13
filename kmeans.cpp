//	Daniel Chang
//	CSCI 4350
//	12/4/2018
//	kmeans.cpp
//	This program takes 3 command line argument, 	integer: seed for random()
//													integer: value of k
//													integer: the number of real-valued features in the data set, 
//													string: input training data filename, 
//													string: input testing data filename 
//
//	Program then read in training data to build a decision tree then use the decision tree to test the tree using test data.

#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <math.h>
#include <cstdlib>
#include <cmath>
using namespace std;

struct kpoints
{
	int classification;
	vector<double> coord;
	vector<double> sum;
	vector<int> sumindex;
	vector<int> classCnt;
	vector<int> data;
};

int main(int argc, char* argv[]) 
{
	int seed = atoi(argv[1]);
	int k = atoi(argv[2]);
	int feat = atoi(argv[3]);
	//Loading Data
	vector< vector <double> > data;
	vector<int> classification;
	fstream fs(argv[4]);
	double value;
	string line;
	
	getline(fs,line);
	stringstream ss(line);
	
	while (!ss.eof()) 
	{
		ss >> value;
		data.push_back(vector<double>());
	}
  
	while (!fs.eof()) 
	{
		stringstream ss(line);
		for (int i = 0; i < data.size(); i++) 
		{
			ss >> value;
			data[i].push_back(value);
			if(i == data.size()-1)
			{
				classification.push_back(value);
			}
		}
		getline(fs,line);
	}
	fs.close();
	//End of data loading
	
	//Retrieving max classification size
	int classSize = 0;
	for(int i = 0; i < classification.size(); i ++)
	{
		if (classSize < classification[i])
		{
			classSize = classification[i];
		}
	}
	//End retrieving max classification size
	
	//Initialize kmeans
	srand(seed);
	vector<kpoints> kmeans;
	vector<int> initialpoints;
	for(int j = 0; j < data[0].size(); j++)
	{
		initialpoints.push_back(j);
	}
	for(int i = 0; i < k; i++)
	{
		//Make sure unique initial coordinates
		random_shuffle(initialpoints.begin(),initialpoints.end());
		int entry = initialpoints[initialpoints.size()-1];
		initialpoints.pop_back();
		// Debug code for initial random K coord
		/*cout << "K = " << entry << endl;
		for(int j = 0; j < feat; j++)
		{
			cout << data[j][entry] << " ";
		}
		cout << classification[entry] << endl;
		*/
		vector<int>::iterator maxiter;
		maxiter = max_element(classification.begin(),classification.end());
		int numofClass = classification[distance(classification.begin(),maxiter)];
		kpoints temp;
		temp.classification = classification[entry];
		temp.coord.resize(feat);
		temp.sum.resize(feat);
		temp.sumindex.resize(feat);
		temp.classCnt.resize(numofClass+1);
		for(int j = 0; j < feat; j++)
		{
			temp.coord[j] = data[j][entry];
			temp.sum[j] = 0;
			temp.sumindex[j] = 0;
		}
		kmeans.push_back(temp);
	}
	/* Debug code for printing out K coords
	for(int i = 0; i < k; i++)
	{
		cout << "K" << i+1 << ":" << endl;
		for(int j = 0; j < feat; j++)
		{
			cout << kmeans[i].coord[j] << " ";
		}
		cout << endl << endl;
	}*/
	//End of kmeans initializing 
	
	//Assigning Cluster
	bool complete = false;
	vector<vector<double>> cluster;
	while(!complete)
	{
		for(int i = 0; i < kmeans.size(); i++)
		{
			kmeans[i].data.clear();
		}
		cluster.clear();
		//Cycling through all data
		vector<vector<double>> tempData = data;
		while(!tempData[0].empty())
		{
			int i = tempData[0].size()-1;
			//Determining closest k for data
			int index = -1; 
			int kindex = -1;
			double minDist = 99999;
			for(int j = 0; j < k; j++)
			{
				double sum = 0;
				for(int l = 0; l < feat; l++)
				{
					sum+= pow(data[l][i] - kmeans[j].coord[l],2);
				}
				double dist = pow(sum,0.5);
				//cout << "Distance to K" << j << ": " << dist << endl;
				if(dist < minDist)
				{
					minDist = dist;
					index = i;
					kindex = j;
				}
			}
			//cout << i << " -> " << kindex << " dist = " << minDist << endl;
			//Assigning points under closes kmeans and removing point from data
			vector<double> temp;
			temp.push_back(index);
			temp.push_back(kindex);
			cluster.push_back(temp);
			kmeans[kindex].data.push_back(index);
			for(int j = 0; j < feat; j++)
			{
				tempData[j].erase(tempData[j].begin()+index);
			}
		}
		//end of cluster assigning
		
		//Moving K point coordinates
		for(int i = 0; i < cluster.size(); i++)
		{
			for(int j = 0; j < feat; j++)
			{
				kmeans[cluster[i][1]].sum[j] += data[j][cluster[i][0]];
				kmeans[cluster[i][1]].sumindex[j]++;
			}
		}
		bool checkComplete = true;
		for(int i = 0; i < kmeans.size(); i++)
		{
			for(int j = 0; j < feat; j++)
			{
				double average = kmeans[i].sum[j]/kmeans[i].sumindex[j];
				if(fabs(kmeans[i].coord[j] - average) > 0)
				{
					checkComplete = false;
				}
				kmeans[i].coord[j] = average;
				kmeans[i].sumindex[j] = 0;
				kmeans[i].sum[j] = 0;
			}
		}
		//End of moving K point coordinates
		if(checkComplete)
		{
			complete = true;
		}
	}
	/* Debug code for printing out K coords
	for(int i = 0; i < k; i++)
	{
		cout << "K" << i+1 << ":" << endl;
		for(int j = 0; j < feat; j++)
		{
			cout << kmeans[i].coord[j] << " ";
		}
		cout << endl << endl;
	}*/
	/*Debug code for printing out cluster
	for(int i = 0; i < cluster.size(); i++)
	{
		for(int j = 0; j < cluster[i].size(); j++)
		{
			cout << cluster[i][j] << " ";
		}
		cout << classification[i];
		cout << endl;
	}*/
	
	//Assigning classification to K points
	//Summing up the amount of classification under each K point
	for(int i = 0; i < k; i++)
	{
		vector<int> temp (classSize+1,0);
		for(int j = 0; j < kmeans[i].data.size(); j++)
		{
			temp[classification[kmeans[i].data[j]]]++;
		}
		kmeans[i].classCnt = temp;
	}
	
	/*for(int i = 0; i < k; i++)
	{
		for(int j = 0; j < classSize; j++)
		{
			cout << j << ": " << kmeans[i].classCnt[j] << endl;
		}
	}*/
	
	for(int i = 0; i < k; i++)
	{
		int max = 0;
		int maxClass = 0;
		for(int j = 0; j < classSize+1; j++)
		{
			if(kmeans[i].classCnt[j] > max)
			{
				//cout << max << " -> ";
				max = kmeans[i].classCnt[j];
				//cout << max;
				maxClass = j;
				//cout << " class: " << maxClass << endl;
			}
		}
		kmeans[i].classification = maxClass;
	}
	//End of Assigning classification to K points
	
	// Debug code for printing out K info
	/*for(int i = 0; i < k; i++)
	{
		cout << "K" << i << " class: " << kmeans[i].classification << endl;
		cout << "Coord: ";
		for(int j = 0; j < feat; j++)
		{
			cout << kmeans[i].coord[j] << " ";
		}
		cout << endl << "class count: " << endl;
		for(int j = 0; j < kmeans[i].classCnt.size(); j++)
		{
			cout << "\t" << j << ": " << kmeans[i].classCnt[j] << endl;
		}
		cout << "entries under this K: " << endl;
		for(int j = 0; j < kmeans[i].data.size(); j++)
		{
			cout << kmeans[i].data[j] << " class: " << classification[kmeans[i].data[j]] << endl;
		}
		cout << endl << endl;
	}*/
	
	//Loading test data 
	vector< vector <double> > test;
	vector<int> testclass;
	fstream fs1(argv[5]);
	
	getline(fs1,line);
	stringstream ss1(line);
	
	while (!ss1.eof()) 
	{
		ss1 >> value;
		test.push_back(vector<double>());
	}
  
	while (!fs1.eof()) 
	{
		stringstream ss2(line);
		for (int i = 0; i < test.size(); i++) 
		{
			ss2 >> value;
			test[i].push_back(value);
			if(i == test.size()-1)
			{
				testclass.push_back(value);
			}
		}
		getline(fs1,line);
	}
	fs1.close();
	//End of loading test data 
	
	//Testing
	int cnt = 0;
	for(int i = 0; i < test[0].size(); i++)
	{
		double minDist = 99999;
		int tempclass;
		int temp;
		for(int j = 0; j < k; j++)
		{
			double sum = 0;
			for(int l = 0; l < feat; l++)
			{
				sum += pow(test[l][i] - kmeans[j].coord[l],2);
			}
			//cout << "Dist to K" << j << ": " << pow(sum,0.5) << endl;
			if(pow(sum,0.5) < minDist)
			{
				minDist = pow(sum,0.5);
				tempclass = kmeans[j].classification;
				temp = j;
			}
		}
		//cout << "MinDist = " << minDist << endl;
		//cout << "Data#" << i << " Closest K: K" << temp << " Class: " << tempclass << " Correct class: " <<testclass[i] << endl << endl;
		if(tempclass == testclass[i])
		{
			cnt++;
		}
	}
	//Result
	cout << cnt << endl;
	return 0;
}
