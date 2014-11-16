#include <iostream>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include <cmath>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <utility>
using namespace std;


string PublisherIRI(string the_string, string con){
	ifstream agents;
	stringstream ss;
	int found,id;
	string name,iri;
	ss<<"id/"<<con<<"_"<<"publishers.txt";
	agents.open(ss.str().c_str());
	ss.str("");
	id=1;
	if (agents.is_open()) {
		while(!agents.eof()){
			agents>>id>>name>>iri;
			if (name.compare(the_string)==0){
				agents.close();
				return iri;
				}
			id++;
			}
		}
	ss<<con<<"_publisher"<<id;
	iri=ss.str();
	agents.close();
	ofstream agents2;
	ss.str("");
	ss<<"id/"<<con<<"_"<<"publishers.txt";
	agents2.open(ss.str().c_str(),ios::app);
	agents2<<id<<'\t'<<the_string<<'\t'<<iri<<endl;
	agents2.close();
	return iri;
	}

string PlaceOfPublicationIRI(string the_string, string con){
	ifstream agents;
	stringstream ss;
	int found,id;
	string name,iri;
	ss<<"id/"<<con<<"_"<<"places.txt";
	agents.open(ss.str().c_str());
	ss.str("");
	id=1;
	if (agents.is_open()) {
		while(!agents.eof()){
			agents>>id>>name>>iri;
			if (name.compare(the_string)==0){
				agents.close();
				return iri;
				}
			id++;
			}
		}
	ss<<con<<"_place"<<id;
	iri=ss.str();
	agents.close();
	ofstream agents2;
	ss.str("");
	ss<<"id/"<<con<<"_"<<"places.txt";
	agents2.open(ss.str().c_str(),ios::app);
	agents2<<id<<'\t'<<the_string<<'\t'<<iri<<endl;
	agents2.close();
	return iri;
	}

string AgentIRI(string the_string, string con){
	ifstream agents;
	stringstream ss;
	int found,id;
	string name,iri;
	ss<<"id/"<<con<<"_"<<"agent.txt";
	agents.open(ss.str().c_str());
	ss.str("");
	id=1;
	if (agents.is_open()) {
		while(!agents.eof()){
			agents>>id>>name>>iri;
			if (name.compare(the_string)==0){
				agents.close();
				return iri;
				}
			id++;
			}
		}
	ss<<con<<"_agent"<<id;
	iri=ss.str();
	agents.close();
	ofstream agents2;
	ss.str("");
	ss<<"id/"<<con<<"_"<<"agent.txt";
	agents2.open(ss.str().c_str(),ios::app);
	agents2<<id<<'\t'<<the_string<<'\t'<<iri<<endl;
	agents2.close();
	return iri;
	}

string StringClean(string the_string){
	char bad[] = {'.', ',', ' '};
	string::size_type n = 0;

	while ((n=the_string.find(bad[0], n))!=the_string.npos){
		the_string.erase(n,1);
	}

	n = 0;
	while ((n=the_string.find(bad[1], n))!=the_string.npos){
		the_string.erase(n,1);
	}

	n = 0;
	while ((n=the_string.find(bad[2], n))!=the_string.npos){
		the_string.replace(n,1,"_");
	}
	return the_string;
}

string StringClean2(string the_string, int a){
	char bad[] = {'{', '}'};
	string::size_type n = 0;
	if (a==1){
	while ((n=the_string.find(bad[0], n))!=the_string.npos){
		the_string.erase(n,1);
	}
	}
	
	n = 0;
	if (a==2){
	while ((n=the_string.find(bad[1], n))!=the_string.npos){
		the_string.erase(n,1);
	}
	}

		return the_string;
}


string ClassFinder(string value, string clas[][2],int max_clas){
//this function finds the Class of the object that the record describes,the velue is the string of the field 200 $b,clas is the array with the classes 
//and max_clas is the number of the classes
	int i;
	string found="default";
	
	for (i=0;i<max_clas;i++){
		if (value.compare(clas[i][0])==0){
			found=clas[i][1];
		}
	}
	if(found.compare("default")==0){
		found=clas[0][1];
	}
		return found;
}

int FieldFind(string fields[][2],string tag, int max_records){
//this function returns the position of a specific tag with a specific subfield
	int i;
	int found=0;
	for (i=0;i<max_records;i++){
		if(fields[i][0]==tag){
			found=1;
			break;
		}
	}
	if(found==1){
		return i;
	}
	else{
		return -1;
	}
}



int main(){
/*dhlwsh metavlitwn :o pinakas data exei ws eisodo olo to record
 ta i kai j einai metrites pou xrisimopoiountai sta for
to record_limit einai to mege8os ths ka8e eggrafhs pou to pernoume apo tis theseis apo 0-4
multi einai mia metavliti pou xrhsimopoihte gia ton ypologismo tou oriou*/
	char data[99999];
    string field,check2,check3,fields[5000][2],*author,key,clas[100][2];
	string map[5000][2],prefix[50],subject;
	stringstream ss;
	int i,j,z,m,p,x;
	int record_limit,directory_length,no_records,no_lines,max_fields,max_prefix,max_clas;
	int multi,orio,check,found,class_check;
	string record,iri;
	mkdir("rdf",0777);
	mkdir("id",0777);
	string pre="authref:";


//reading of classes
	ifstream classes;
	classes.open("initialization/classes.txt");
	z=0;
	cout<< "Loading classes..."<<endl;
	if (classes.is_open()) {
		while(!classes.eof()){
			getline(classes,record,'\r');
			int temp=record.find('=');
			cout<<temp;
			clas[z][0]=record.substr(0,temp);
			clas[z][1]=record.substr(temp+1);
			
			cout<<clas[z][0]<<'\t'<<'\t'<<clas[z][1]<<endl;
			z++;
		}
		max_clas=z;
		cout << max_clas<<endl;
	}
	else {
		cout<< "The file classes.txt does not exist. Make sure there is a proper syntaxed file named classes.txt in the folder";
		return 0;
	}
	classes.close();
	//reading of the mappings
	ifstream mappings;
	
	mappings.open("initialization/mappings.txt");
	z=0;
	int invalid_mappings=0;
	cout << "Loading mappings..."<<endl;
	if (mappings.is_open()) {
		while(!mappings.eof()){
			mappings >> map[z][0]>>map[z][1];
			if(map[z][1]!=""){
			cout << map[z][0]<<"----"<<map[z][1]<<endl;
			z++;
			}
			else {
				invalid_mappings++;
				cout << "WARNING!!!!!!Mapping with field tag "<<map[z][0]<<" is empty!!!Please check the file mappings.txt"<<endl;
				}
		}
		max_fields=z;
		mappings.close();
		cout<< "Valid mappings #: "<<max_fields<<"                               "<<"Invalid mappings #: "<< invalid_mappings<<endl<<endl<<endl;
	}
	else {
		cout<< "The file mappings.txt does not exist. Make sure there is a proper syntaxed file named mappings.txt in the folder";
		return 0;
	}
	
	//reading of prefixes
	ifstream prefixes;
	prefixes.open("initialization/prefixes.txt");
	z=0;
	cout<< "Loading prefixes..."<<endl;
	if (prefixes.is_open()) {
		while(!prefixes.eof()){
			getline (prefixes,prefix[z]);
			cout << prefix[z]<<endl;
			z++;
		}
		max_prefix=z;
		cout << max_prefix<<endl;
	}
	else {
		cout<< "The file prefixes.txt does not exist. Make sure there is a proper syntaxed file named prefixes.txt in the folder";
		return 0;
	}
	
	//anoigma arxeiwn
	cout<<"Please provide the school ref, like math or auth11 :";
	string con;
	cin>>con;
	ifstream bib;
	ss.str("");
	//ofstream line;
	ss<<"data/"<<con<<".txt";
	bib.open(ss.str().c_str());
	cout<<"Loading records, please standby..."<<endl;
	ss.str("");
	if (bib.is_open()==0){
		cout << "The file does not exist. Make sure there is a proper syntaxed file named "<<con<<".txt  in folder data";
		return 0;
	}
	found=0;
	//find the begin of the records passing information lines
	while (found==0) {
		getline (bib,record,'\r');
		if(record.compare("BEGIN EXPORTED REFERENCES")==0){
			for (i=0;i<4;i++){
				getline(bib,record,'\r');
			}
		break;
		}
	}
	
	
	//open the stream
	ofstream out;
	ss<<"rdf/"<<con<<".n3";
	out.open(ss.str().c_str());
	ss.str("");
	
	//write the prefixes
	for (z=0;z<max_prefix;z++){
				out<<prefix[z]<<endl;
				}
	//make the data		
	while(!bib.eof()){
		getline(bib,record,'\r');
		cout<<"----------------------------------------------"<<endl;
		//make the table for each record
		i=0;
		while((record.compare("")!=0) && (!bib.eof())){
		fields[i][0]=record.substr(0,2);
		fields[i][1]=record.substr(3,-1);
		getline(bib,record,'\r');
		i++;
		}
		int id = FieldFind(fields,"ID",i);
		stringstream ss;
		ss<<pre<<con<<"_pubid"<<fields[id][1];
		subject=ss.str();
		cout<<subject<<fields[0][1]<<endl;
		//cleaning strings
		int clean=0;
		for(j=0;j<i;j++){
            if(fields[j][1].compare("")!=0){//prosperash kenwn pediwn
				fields[clean][0]=fields[j][0];
				int temp=fields[j][1].find('"');
				while (temp!=-1){
					fields[j][1].replace(temp,1,"\'");
					temp=fields[j][1].find('"',temp+3);
				}
				fields[clean][1]=fields[j][1];
				fields[clean][1]=StringClean2(fields[clean][1],1);
				fields[clean][1]=StringClean2(fields[clean][1],2);
				cout<<fields[clean][0]<<"-----"<<fields[clean][1]<<endl;
				clean++;
			}
		}//clearing invalid characters
		
		for (j=0;j<clean;j++){
			int check2= fields[j][1][0];
			//passing fault causing characters
			if (check2==46){
				continue;}
			
			//class check
			if (fields[j][0].compare("RT")==0){
				class_check=FieldFind(fields,"RT",i);
				if(x==-1) {
					cout<<"There was some missing mapping on tag "<<fields[j][0]<<".Please check the file mappings.txt and fill it.";
					return 0;
					}
				out<<subject<<'\t'<<"rdf:type"<<'\t'<<ClassFinder(fields[class_check][2],clas,max_clas)<<" ."<<endl;
				
			}
			
			
			//agent iri creation
			if((fields[j][0].compare("A1")==0)||(fields[j][0].compare("A2")==0)||(fields[j][0].compare("A3")==0)||(fields[j][0].compare("A4")==0)||(fields[j][0].compare("A5")==0)||(fields[j][0].compare("A6")==0)){
				iri=StringClean(fields[j][1]);
				iri=AgentIRI(iri,con);
				int prop;
				prop=FieldFind(map,fields[j][0],max_fields);
				out<<subject<<'\t'<<map[prop][1]<<'\t'<<pre<<iri<<" ."<<endl;
				out<<pre<<iri<<'\t'<<"rdf:type"<<'\t'<<"foaf:Person ."<<endl;
				out<<pre<<iri<<'\t'<<"foaf:name"<<'\t'<<'\"'<<fields[j][1]<<'\"'<<" ."<<endl;
				
			}
			//publisher iri creation
			else if(fields[j][0].compare("PB")==0){
				iri=StringClean(fields[j][1]);
				iri=PublisherIRI(iri,con);
				out<<subject<<'\t'<<"bibtex:hasPublisher"<<'\t'<<pre<<iri<<" ."<<endl;
				out<<pre<<iri<<'\t'<<"rdf:type"<<'\t'<<"foaf:Organization ."<<endl;
				out<<pre<<iri<<'\t'<<"foaf:name"<<'\t'<<'\"'<<fields[j][1]<<'\"'<<" ."<<endl;
			}
			//place published iri creation
			else if(fields[j][0].compare("PP")==0){
				iri=StringClean(fields[j][1]);
				iri=PlaceOfPublicationIRI(iri,con);
				out<<subject<<'\t'<<"bibtex:hasLocation"<<'\t'<<pre<<iri<<" ."<<endl;
				out<<pre<<iri<<'\t'<<"rdf:type"<<'\t'<<"schema:Place ."<<endl;
				out<<pre<<iri<<'\t'<<"schema:name"<<'\t'<<'\"'<<fields[j][1]<<'\"'<<" ."<<endl;
				out<<pre<<iri<<'\t'<<"rdfs:label"<<'\t'<<'\"'<<fields[j][1]<<'\"'<<" ."<<endl;
			}
			
			//publisher iri creation
			else if(fields[j][0].compare("T1")==0){
				out<<subject<<'\t'<<"bibtex:hasTitle"<<'\t'<<'\"'<<fields[j][1]<<'\"'<<" ."<<endl;
				out<<subject<<'\t'<<"rdfs:label"<<'\t'<<'\"'<<fields[j][1]<<'\"'<<" ."<<endl;
			}
			else if(fields[j][0].compare("SP")==0){
				if(fields[j+1][0].compare("OP")==0){
					out<<subject<<'\t'<<"bibtex:hasPages"<<'\t'<<'\"'<<fields[j][1]<<"-"<<fields[j+1][1]<<'\"'<<" ."<<endl;
					j++;
				}
				else{
					out<<subject<<'\t'<<"bibtex:hasPages"<<'\t'<<'\"'<<fields[j][1]<<'\"'<<" ."<<endl;
					
					}
			}
			
			//all other cases
			
			else {
				x=FieldFind(map,fields[j][0],max_fields);
				if(fields[j][1].compare("skip")==0){
					continue;}
				
				if(x==-1) {
					cout<<"There was some missing mapping on tag "<<fields[j][0]<<".Please check the file mappings.txt and fill it. If you think that the field does not contain any useful data provide the value skip.";
					return 0;
					}
				out<<subject<<'\t'<<map[x][1]<<'\t'<<'\"'<<fields[j][1]<<'\"'<<" ."<<endl;
			}
			
		}	
	}
return 0;	
}			