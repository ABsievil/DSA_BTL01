#include "main.h"
#include "Restaurant.cpp"
#include <fstream>
#include <string>
int MAXSIZE;

void simulate(string filename, imp_res* r)
{
	ifstream ss(filename);
	string str, maxsize, name, energy, num;
	while(ss >> str)
	{ 
		if(str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize); 
    	}
        else if(str == "RED") // RED <NAME> <ENERGY>
        {
			cout<<"RED\n";
            ss >> name;
            ss >> energy;
            r->RED(name, stoi(energy));
    	}
    	else if(str == "BLUE") // BLUE <NUM>
    	{
			cout<<"BLUE\n";
                ss >> num;
    			r->BLUE(stoi(num));
		}
    	else if(str == "PURPLE") // PURPLE
    	{
			cout<<"PURPLE\n";
    			r->PURPLE();
		}
		else if(str == "REVERSAL") // REVERSAL
		{
			cout<<"REVERSAL\n";
    			r->REVERSAL();	
		}
    	else if(str == "UNLIMITED_VOID") // UNLIMITED_VOID
     	{   	
			cout<<"UNLIMITED_VOID\n";
    			r->UNLIMITED_VOID();
    	}
    	else if(str == "DOMAIN_EXPANSION") // DOMAIN_EXPANSION
    	{
			cout<<"DOMAIN_EXPANSION\n";
    			r->DOMAIN_EXPANSION();
    	}
    	else // LIGHT <NUM>
    	{
			cout<<"LIGHT\n";
                ss >> num;
    			r->LIGHT(stoi(num));
    	}
    }
}

int main(int argc, char* argv[]) {
	if(argc<2){
		string fileName = "test.txt";
		imp_res* r = new imp_res();
		simulate(fileName, r);
		system("pause");
		delete r;
	}
	else if(argc ==2){
		 string fileName = "input\\input";
		 fileName+= argv[1];
		 fileName+=".txt";
		imp_res* r = new imp_res();
		//string fileName = "test.txt";
		simulate(fileName, r);
		//system("pause");
		delete r;
	}
	else if(argc ==3){
		int argv1 = stoi(argv[1]);
		int argv2 = stoi(argv[2]);
		cout<<"LOADING TO :";
		for(int i=argv1; i<=argv2;i++){
			cout<<i<<" ";
			string fileName = "input\\input";
			fileName+= i;
			fileName+=".txt";
			imp_res* r = new imp_res();
			simulate(fileName, r);
			delete r;
		}
		cout<<"\nFINISHED\n";
		//system("pause");
	}
	
	
	return 0;
}


