#include<iostream>
#include<cstring>
#include<conio.h>
#include<cmath>
#include<iomanip>
#include<fstream>
#include<Windows.h>
#include <limits>

using namespace std;

struct prisonerInfo
{
	char name[40];
	int age;
	char block[5];
	int cell;
	int person;
	char crime[5];
	int monthIn, yearIn; //Date of entry: Month | Year
	int yearPunished ; //Years of punishment
	int monthOut, yearOut; //Date of release: Month | Year
	char status; // F-freed, P-parole, L-locked up *if freed his record will be cleared
	char ID[10]; // Block Room Person Crime           		e.g.  :  MS 8 1 TR 
	//													Index[7]  :	 01 2 3 45 
};

struct Name
{
	char name[40];
	int age;
	char crime[5];
	int yearPunished;
};

struct cellInfo
{
	int status;
};

const int BlockType = 3;
const int CellNum = 8;
const int maxNumPrisoner = 40;


void welcome();
void password();
int setMenu();
int countPrisoner(ifstream&, prisonerInfo[], int);
void list(ofstream&, ofstream&, ofstream&, prisonerInfo[], int, Name[]); //list of all prisoners(sort name), list of prisoners by specific crime, list of prisoners by spesific block
void data(prisonerInfo[],int); /*minimum = lowest Month and Year for release date
maximum = lowest month and year for entry date (the most senior will enter on 01/2001 which is when the prison started to operate)
average years punished for each crime
count total for each block, for each crime, all block*/
void search(prisonerInfo[], int); //search 1 name or ID then his full record will be shown
void update(prisonerInfo[], int, cellInfo[][CellNum], ofstream&); //View cell availablity and can update data to output file
void tab(int);
void endl(int);

int main()
{
	
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 1200, 650, TRUE); //width and height of console
	//SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE),CONSOLE_FULLSCREEN_MODE, 0);
	
	int menu, realPrisoner;
	prisonerInfo prisoner[maxNumPrisoner];
	Name copyName[maxNumPrisoner];
	cellInfo room[BlockType][CellNum];
	
	ifstream input1;
	ofstream output1, output2, output3, output4;
	input1.open("ListPrisoner.txt");
	output1.open("Sort By Crime.txt");
	output2.open("Sort By Block.txt");
	output3.open("Sort By Name.txt");
	output4.open("New Updated Data.txt",ofstream::app);
	
	if(!input1)
	{
		cout<<"File not exist !";
		
		_getch();
		return false;
	}
	
	welcome();
	password();
	realPrisoner = countPrisoner(input1, prisoner, maxNumPrisoner) ;
	
	do{
		
		menu = setMenu();
		
		if(menu == 1)
		{
			list(output1, output2, output3, prisoner, realPrisoner, copyName) ;
		
		}
		else if(menu == 2)
		{
			data(prisoner,realPrisoner) ;
			
		}
		else if(menu==3)
		{
			search(prisoner, realPrisoner) ;
			
		}
		else if(menu==4)
		{
			update(prisoner, realPrisoner, room, output4) ;
			
		}
		
	}while(menu!=5);
	
	cout<< endl << endl ;
	cout<<"                                  E N D  O F  T H E  D A Y                             ";
	cout<< endl ;
	cout<<"                    THANK YOU FOR USING PRISON ALCATRAZ MANAGEMENT SYSTEM !            " <<  endl << endl ;
	cout<<"---------------------------------------- C S C 1 3 8 ----------------------------------" << endl ;
	cout<<"                   ALL RIGHTS RESERVED TM (R) & COPYRIGHT (C) 2018 BY UITM             " << endl ;

	input1.close();
	output1.close();
	output2.close();
	output3.close();
	output4.close();
	
	_getch();
	return 0;
}

void welcome()
{
	cout<<"\t\t   =================================================================" << endl ;
	cout<<"\t\t   ||         ||    ||          ||        ||            ||        ||" << endl ;
	cout<<"\t\t   ||         ||    ||          ||        ||            ||        ||" << endl ;
	cout<<"\t\t   || WELCOME || TO || ALCATRAZ || PRISON || MANAGEMENT || SYSTEM ||" << endl;
	cout<<"\t\t   ||         ||    ||          ||        ||            ||        ||" << endl ;
	cout<<"\t\t   ||         ||    ||          ||        ||            ||        ||" << endl ;
	cout<<"\t\t   =================================================================" ;
	endl(2);
	
	cout<<"\t\t\t\t         !! WARDEN'S GUIDELINE !!" << endl ;
	cout<<"\t\t\t\t===========================================" << endl << endl;
	cout<<"\t\t\t\t         MAXIMUM SECURITY BLOCK [MS]              " << endl  ;
	cout<<"\t\t\t\t===========================================" << endl ;
	cout<<"\t\t\t\t| Terrorism [TR]          = 35 years max  |" << endl ;
	cout<<"\t\t\t\t| Homicide [HM]           = 20 years max  |"<<endl;
	cout<<"\t\t\t\t===========================================" << endl ;
	cout<<"\t\t\t\t         GENERAL POPULATON BLOCK [GP]             " << endl ;
	cout<<"\t\t\t\t===========================================" << endl ;
	cout<<"\t\t\t\t| Fraud [FD]              = 2 years max   |" << endl ;
	cout<<"\t\t\t\t| Theft [TF]              = 3 years max   |"<<endl;
	cout<<"\t\t\t\t===========================================" << endl ;
	cout<<"\t\t\t\t           SPECIAL UNIT BLOCK [SU]                  " << endl ;
	cout<<"\t\t\t\t===========================================" << endl ;
	cout<<"\t\t\t\t| Rape [RP]               = 10 years max  |" << endl ;
	cout<<"\t\t\t\t| Child Sexual Abuse [CX] = 15 years max  |" << endl ;
	cout<<"\t\t\t\t===========================================" ;
	endl(2);
	cout<<"\t\t   There are 8 cells in each block" << endl ;
	cout<<"\t\t   Each cell = 2 persons EXCEPT Maximum Security cell = 1 person";
	endl(1);
	cout<<"\t\t   " ;
	system("pause");
	system("cls");
}

void password()
{	
	char password[20], truePassword[20] = {"warden"};
	bool pass = true;
	while (pass)
	{
		cout << "---------------------------------- ALCATRAZ PRISON TM --------------------------------------- " << endl << endl << endl ;
	
		cout << "\t\t\t\t =====================" << endl ;
	    cout << "\t\t\t\t ||   PRISON STAFF  ||" << endl ;
	    cout << "\t\t\t\t ||                 ||" << endl ;
		cout << "\t\t\t\t || P.A.S.S.W.O.R.D ||" << endl ;
		cout << "\t\t\t\t =====================" << endl ;
		cout << "\t\t\t\t [=] " ;
		cin >> ws;
		cin.getline(password, 20) ;
		
		if (strcmp(password,truePassword) == 0)
		{
			pass = false;
		}
		else
		{
			Beep(1000,500) ;
			cout << "\n Incorrect staff password !" << endl << endl ;
			system("pause");
			system("cls");
		}
     }
     
     system("cls");
}

int setMenu()
{
	int menu;
	
	do{
		cout << "\t----------------------- ALCATRAZ PRISON TM ---------------------- " << endl << endl ;
		cout << "\t=================================================================" << endl ;
		cout << "\t||  ___    ___  ||  ______  ||  ___      ___  ||  ___     ___  ||" << endl ;
		cout << "\t|| |   |  |   | || |  ____| || |   |__  |   | || |   |   |   | ||" << endl ;
		cout << "\t|| |   |  |   | || | |____  || |    _ |_|   | || |   |   |   | ||" << endl ;
		cout << "\t|| |   |__|   | || |  ____| || |   | |_     | || |   |   |   | ||" << endl ;
		cout << "\t|| | |______| | || | |____  || |   |   |_   | || |   |___|   | ||" << endl ;
		cout << "\t|| |  |    |  | || |      | || |   |     |  | || |           | ||" << endl ;
		cout << "\t|| |__|    |__| || |______| || |___|     |__| || |___________| ||" << endl ;
		cout << "\t=================================================================" << endl ;
		
		
		cout << "\t\t\t   _______________________" << endl ;
		cout << "\t\t\t  |                       |" << endl ;
		cout << "\t\t\t  |       [1] LIST        |" << endl ;
		cout << "\t\t\t  |                       |" << endl ;
		cout << "\t\t\t  |       [2] DATA        |" << endl ;
		cout << "\t\t\t  |                       |" << endl ;
		cout << "\t\t\t  |      [3] SEARCH       |" << endl ;
		cout << "\t\t\t  |                       |" << endl ;
		cout << "\t\t\t  |      [4] UPDATE       |" << endl ;
		cout << "\t\t\t  |                       |" << endl ;	
		cout << "\t\t\t  |       [5] EXIT        |" << endl ;
		cout << "\t\t\t  |_______________________|" << endl ;
		
		
		cin.clear();
		while ((cout << "\t\t CHOICE ==> ") && (!(cin >> menu) || menu < 1 || menu > 5))
		{
		    cout << "That's not a number between 1 and 5;";
		    cin.clear();
		    cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		system("cls");
		
		if(menu<1 || menu>5)
		Beep(1000,500);
	
	}while( menu<1 || menu>5 );

	return menu;
}

	
int countPrisoner(ifstream& input1, prisonerInfo prisoner[], int maxNumPrisoner)
{
	int i = 0, count = 0;
	
	while( (!input1.eof()) && (i<maxNumPrisoner) )
	{
		input1 >> ws;
		input1.getline(prisoner[i].name, 40,'*');
		input1 >> ws;
		input1 >> prisoner[i].age ;
		input1 >> ws;
		input1.getline(prisoner[i].block,3,'*');
		input1 >> ws;
		input1 >> prisoner[i].cell >> prisoner[i].person ;
		input1 >> ws;
		input1.getline(prisoner[i].crime,3,'*');
		input1 >> ws;
		input1 >> prisoner[i].monthIn >> prisoner[i].yearIn >> prisoner[i].yearPunished >> prisoner[i].monthOut >> prisoner[i].yearOut ;
		input1 >> ws;
		input1 >> prisoner[i].status ;
		input1 >> ws;
		input1.getline(prisoner[i].ID,7,'*');
		input1 >> ws;
		
		if(input1)
		{
			count++;
		}
				
		i++;
	}
	
	return count;
}

void list(ofstream& output1, ofstream& output2, ofstream& output3, prisonerInfo prisoner[], int realPrisoner, Name copyName[])
{
	int listChoice ;
	
	cout << endl << endl << endl << endl ;
	cout << "\t\t\t\t\t\t      SORT BY " << endl ;
	cout << "\t\t\t ============== \t ================ \t ================" << endl ;
	cout << "\t\t\t |    CRIME   | \t |     BLOCK    | \t |     NAME     |" << endl ;
	cout << "\t\t\t ============== \t ================ \t ================" << endl ;
	cout << "\t\t\t       [1]      \t        [2]       \t        [3]      " << endl ;
	cout << "\t\t\t                       = " ;
	cin >> listChoice ;
	
	system("cls");
	
	if(listChoice==1)
	{
	int num = 0;
	output1 << "\nTerorrism [TR]" << endl ;
	output1 << "_______________________________" << endl;
	cout << "\nTerorrism [TR]" << endl ;
	cout << "_______________________________" << endl;
	for(int i=0; i<realPrisoner; i++)
	{
		if(strcmp(prisoner[i].crime, "TR") ==0 )
		{
			output1 << num+1 << ". " << prisoner[i].name << endl;
			cout << num+1 << ". " << prisoner[i].name << endl;
			num++;
		}
	}
	output1 << "\nHomicide [HM]" << endl ;
	output1 << "_______________________________" << endl;
	cout << "\nHomicide [HM]" << endl ;
	cout << "_______________________________" << endl;
	for(int i=0; i<realPrisoner; i++)
	{
		if(strcmp(prisoner[i].crime, "HM") ==0 )
		{
			output1 << num+1 << ". " << prisoner[i].name << endl;
			cout << num+1 << ". " << prisoner[i].name << endl;
			num++;
		}
	}
	output1 << "\nFraud [FD]" << endl ;
	output1 << "_______________________________" << endl;
	cout << "\nFraud [FD]" << endl ;
	cout << "_______________________________" << endl;
	for(int i=0; i<realPrisoner; i++)
	{
		if(strcmp(prisoner[i].crime, "FD") ==0 )
		{
			output1 << num+1 << ". " << prisoner[i].name << endl;
			cout << num+1 << ". " << prisoner[i].name << endl;
			num++;
		}
	}
	output1 << "\nTheft [TF]" << endl ;
	output1 << "_______________________________" << endl;
	cout << "\nTheft [TF]" << endl ;
	cout << "_______________________________" << endl;
	for(int i=0; i<realPrisoner; i++)
	{
		if(strcmp(prisoner[i].crime, "TF") ==0 )
		{
			output1 << num+1 << ". " << prisoner[i].name << endl;
			cout << num+1 << ". " << prisoner[i].name << endl;
			num++;
		}
	}
	output1 << "\nRape [RP]" << endl ;
	output1 << "_______________________________" << endl;
	cout << "\nRape [RP]" << endl ;
	cout << "_______________________________" << endl;
	for(int i=0; i<realPrisoner; i++)
	{
		if(strcmp(prisoner[i].crime, "RP") ==0 )
		{
			output1 << num+1 << ". " << prisoner[i].name << endl;
			cout << num+1 << ". " << prisoner[i].name << endl;
			num++;
		}
	}
	output1 << "\nChild Sexual Abuse [CX]" << endl ;
	output1 << "_______________________________" << endl;
	cout << "\nChild Sexual Abuse [CX]" << endl ;
	cout << "_______________________________" << endl;
	for(int i=0; i<realPrisoner; i++)
	{
		if(strcmp(prisoner[i].crime, "CX") ==0 )
		{
			output1 << num+1 << ". " << prisoner[i].name << endl;
			cout << num+1 << ". " << prisoner[i].name << endl;
			num++;
		}
	}
		
	}
	
	else if(listChoice==2)
	{
		
	int num = 0;
	output2 << "\nMaximum Security Block [MS]" << endl ;
	output2 << "_______________________________" << endl;
	cout << "\nMaximum Security Block [MS]" << endl ;
	cout << "_______________________________" << endl;
	for(int i=0; i<realPrisoner; i++)
	{
		if(strcmp(prisoner[i].block, "MS") ==0 )
		{
			output2 << num+1 << ". " << prisoner[i].name << endl;
			cout << num+1 << ". " << prisoner[i].name << endl;
			num++;
		}
	}
	output2 << "\nGeneral Population Block [GP]" << endl ;
	output2 << "_______________________________" << endl;
	cout << "\nGeneral Population Block [GP]" << endl ;
	cout << "_______________________________" << endl;
	for(int i=0; i<realPrisoner; i++)
	{
		if(strcmp(prisoner[i].block, "GP") ==0 )
		{
			output2 << num+1 << ". " << prisoner[i].name << endl;
			cout << num+1 << ". " << prisoner[i].name << endl;
			num++;
		}
	}
	output2 << "\nSpecial Unit Block [SU]" << endl ;
	output2 << "_______________________________" << endl;
	cout << "\nSpecial Unit Block [SU]" << endl ;
	cout << "_______________________________" << endl;
	for(int i=0; i<realPrisoner; i++)
	{
		if(strcmp(prisoner[i].block, "SU") ==0 )
		{
			output2 << num+1 << ". " << prisoner[i].name << endl;
			cout << num+1 << ". " << prisoner[i].name << endl;
			num++;
		}
	}
		
	}
	else if(listChoice==3)
	{
	int num = 0, temAge, temyearPunished;
	char temName[40], temCrime[5], crimeName[20];
	
	for(int i = 0; i < realPrisoner; i++) // copy to another struct
	{
		strcpy(copyName[i].name, prisoner[i].name);
		copyName[i].age = prisoner[i].age;
		strcpy(copyName[i].crime, prisoner[i].crime);
		copyName[i].yearPunished = prisoner[i].yearPunished;
	}
	
	for(int i = 0; i < realPrisoner-1; i++) //Sort List of Prisoners In Alphabetical Order
	{
		for(int j = i; j < realPrisoner; j++)
		{
			if (strcmp(copyName[i].name, copyName[j].name)>0)
			{
		        strcpy(temName, copyName[i].name);
		        strcpy(copyName[i].name, copyName[j].name);
		        strcpy(copyName[j].name, temName);
		        
		        temAge = copyName[i].age;
		        copyName[i].age = copyName[j].age;
		        copyName[j].age = temAge;
		        
		        strcpy(temCrime, copyName[i].crime);
		        strcpy(copyName[i].crime, copyName[j].crime);
		        strcpy(copyName[j].crime, temCrime);
		        
		        temyearPunished = copyName[i].yearPunished;
		        copyName[i].yearPunished = copyName[j].yearPunished;
		        copyName[j].yearPunished = temyearPunished;
			}
		}
	}
	
	cout<<"\n\nSort List of Prisoners In Alphabetical Order\n\n";
	cout<<"===================================================================="<<endl;
	cout << "No.\tName\t\tAge\tCrime\t\t\tPunishment" << endl;
	cout<<"===================================================================="<<endl;
	
	output3<<"Sort List of Prisoners In Alphabetical Order\n\n";
	output3<<"===================================================================="<<endl;
	output3<< "No.\tName\t\tAge\tCrime\t\t\tPunishment" << endl;
	output3<<"===================================================================="<<endl;
	
	for(int i = 0; i < realPrisoner; i++)
	{
		if(strcmp(copyName[i].crime, "TR")==0)
			strcpy(crimeName, "Terrorism         ");
		else if(strcmp(copyName[i].crime, "HM")==0)
			strcpy(crimeName, "Homicide          ");
		else if(strcmp(copyName[i].crime, "TF")==0)
			strcpy(crimeName, "Theft             ");
		else if(strcmp(copyName[i].crime, "FD")==0)
			strcpy(crimeName, "Fraud             ");
		else if(strcmp(copyName[i].crime, "RP")==0)
			strcpy(crimeName, "Rape              ");
		else if(strcmp(copyName[i].crime, "CX")==0)
			strcpy(crimeName, "Child Sexual Abuse");
		
		cout << num+1 << ". \t" << copyName[i].name << "\t" << copyName[i].age << "\t";
		cout << crimeName << "\t" << copyName[i].yearPunished << " years";
		endl(2);
		
		output3 << num+1 << ". \t" << copyName[i].name << "\t" << copyName[i].age << "\t";
		output3 << crimeName << "\t" << copyName[i].yearPunished << " years" <<endl<<endl;
		
		num++;
	}
		
	}
	else if(listChoice!=1||listChoice!=2||listChoice!=3)
	{
		Beep(1000,500) ;
	}
	
	endl(3);
	
	system("pause");
	system("cls");
}

void data(prisonerInfo prisoner[],int realPrisoner)
{
	cout << " ----------------------  ALCATRAZ PRISON TM ---------------------- " << endl ;
	cout << " Alcatraz Prison | Jalan Pudu King 3/2A | Klang | Selangor | 47610 " << endl ;
	cout << "                             Since 2001                            " << endl ;
	cout << endl << endl ;
	
	int minMonth = prisoner[0].monthOut ;
	int minYear  = prisoner[0].yearOut ;
	int indexMin = 0 ;

	for (int x = 0 ; x<realPrisoner ; x++)
	{
		if((prisoner[x].monthOut<minMonth)&&(prisoner[x].yearOut<minYear))
		{
			minMonth = prisoner[x].monthOut ;
			minYear = prisoner[x].yearOut ;
		
			indexMin = x ;
		}
	}
	
	cout << " EARLIEST PRISONER THAT WILL BE RELEASE : " <<  endl ;
	cout << " --------------------------------------   " << endl ;
	cout << " ID              : " << prisoner[indexMin].ID << endl ;
	cout << " Name            : " << prisoner[indexMin].name << endl ;
	cout << " Age             : " << prisoner[indexMin].age << endl ;
	cout << " Crime           : " << prisoner[indexMin].crime << endl ;
	cout << " Block           : " << prisoner[indexMin].block << endl ;
	cout << " Cell Number     : " << prisoner[indexMin].cell << endl ;
	cout << " Date of Entry   : " << prisoner[indexMin].monthIn << "/" << prisoner[indexMin].yearIn << endl ;
	cout << " Date of Release : " << prisoner[indexMin].monthOut << "/" << prisoner[indexMin].yearOut << endl << endl ;
	
	int maxMonth = prisoner[0].monthOut ;
	int maxYear  = prisoner[0].yearOut ;
	int indexMax = 0 ;

	for (int x = 0 ; x<realPrisoner ; x++)
	{
		if((prisoner[x].monthOut>maxMonth)&&(prisoner[x].yearOut>maxYear))
		{
			maxMonth = prisoner[x].monthOut ;
			maxYear = prisoner[x].yearOut ;
		
			indexMax = x ;
		}
	}
	
	
	cout << " THE LAST PRISONER TO BE RELEASE  : " << endl ;
	cout << " --------------------------------------   " << endl ;
	cout << " ID              : " << prisoner[indexMax].ID << endl ;
	cout << " Name            : " << prisoner[indexMax].name << endl ;
	cout << " Age             : " << prisoner[indexMax].age << endl ;
	cout << " Crime           : " << prisoner[indexMax].crime << endl ;
	cout << " Block           : " << prisoner[indexMax].block << endl ;
	cout << " Cell Number     : " << prisoner[indexMax].cell << endl ;
	cout << " Date of Entry   : " << prisoner[indexMax].monthIn << "/" << prisoner[indexMax].yearIn << endl ;
	cout << " Date of Release : " << prisoner[indexMax].monthOut << "/" << prisoner[indexMax].yearOut << endl << endl ;
	cout << endl << endl ;
	
	int countTR = 0, countHM = 0, countTF = 0 , countFD = 0 , countRP = 0 , countCX = 0 ;
	int totalTR = 0 , totalHM = 0 , totalTF = 0 , totalFD = 0 , totalRP = 0 , totalCX = 0 ;
	double avgTR = 0 , avgHM = 0 , avgTF = 0 , avgFD = 0 , avgRP = 0 , avgCX = 0 ; 
	
	for(int x = 0 ; x<maxNumPrisoner ; x++)
	{
		if(strcmp(prisoner[x].crime,"TR")==0)
		{
			totalTR = totalTR + prisoner[x].yearPunished ; 
			countTR++ ;
		}
		else if(strcmp(prisoner[x].crime,"HM")==0)
		{
			totalHM = totalHM + prisoner[x].yearPunished ; 
			countHM++ ;
		}
		else if(strcmp(prisoner[x].crime,"TF")==0)
		{
			totalTF = totalTF + prisoner[x].yearPunished ; 
			countTF++ ;
		}
		else if(strcmp(prisoner[x].crime,"FD")==0)
		{
			totalFD = totalFD + prisoner[x].yearPunished ; 
			countFD++ ;
		}
		else if(strcmp(prisoner[x].crime,"RP")==0)
		{
			totalRP = totalRP + prisoner[x].yearPunished ; 
			countRP++ ;
		}
		else if(strcmp(prisoner[x].crime,"CX")==0)
		{
			totalCX = totalCX + prisoner[x].yearPunished ; 
			countCX++ ;
		}
	}
	
	avgTR = totalTR / countTR ;
	avgHM = totalHM / countHM ;
	avgTF = totalTF / countTF ;
	avgFD = totalFD / countFD ;
	avgRP = totalRP / countRP ;
	avgCX = totalCX / countCX ;
	
	
	cout << " AVERAGE YEAR PUNISHED PER CRIME : " << endl ;
	cout << " ================================== " << endl ;
	cout << " Terrorism          [TR] = " << avgTR << " years " << endl ;
	cout << " Homocide           [HM] = " << avgHM << " years " << endl ;
	cout << " Theft              [TF] = " << avgTF << "  years " << endl ;
	cout << " Fraud              [FD] = " << avgFD << "  years " << endl ;
	cout << " Rape               [RP] = " << avgRP << " years " << endl ;
	cout << " Child Sexual Abuse [CX] = " << avgCX << " years " << endl ;	
	cout << " ===================================" <<  endl << endl ;
	
	
	
	
	
	
	cout << " PRISONERS COUNT " << endl ;
	cout << " ==================================" << endl ;
	cout << " BY CRIME : " << endl ;
	cout << " Terrorism          [TR] = " << countTR << " prisoner(s) "<< endl ;
	cout << " Homocide           [HM] = " << countHM << " prisoner(s) "<< endl ;
	cout << " Theft              [TF] = " << countTF << " prisoner(s) "<< endl ;
	cout << " Fraud              [FD] = " << countFD << " prisoner(s) "<< endl ;
	cout << " Rape               [RP] = " << countRP << " prisoner(s) "<< endl ;
	cout << " Child Sexual Abuse [CX] = " << countCX << " prisoner(s) "<< endl ;
	cout << " ==================================" << endl ;



	int countMS = 0 , countSU = 0 , countGP = 0  ;
	
	for(int x = 0 ; x<maxNumPrisoner ; x++)
	{
		if(strcmp(prisoner[x].block,"MS")==0)
		{
			countMS++ ;
			
		}
		else if(strcmp(prisoner[x].block,"GP")==0)
		{
			countGP++ ;
			
		}
		else if(strcmp(prisoner[x].block,"SU")==0)
		{
			countSU++ ;
			
		}
		
	}
	
	
	
	cout << " BY BLOCK : " << endl ;
	cout << " Maximum Security   [MS] = " << countMS << " prisoner(s) "<< endl ;
	cout << " General Population [GP] = " << countGP << " prisoner(s) "<< endl ;
	cout << " Special Unit       [SU] = " << countSU << " prisoner(s) "<< endl ;
	cout << " TOTAL                   = " << countMS + countGP + countSU << " prisoner(s) "<< endl ;
	cout << " ==================================" << endl ;

	

	
	
	system("pause");
	system("cls");
}

void search(prisonerInfo prisoner[], int realPrisoner)
{
	char searchPrisoner[30], searchID[10], answer;
	bool found = false;
	int ind, search;
	
//	do{
		cout << " ------------------ ALCATRAZ PRISON TM------------------- " << endl ;
		cout << "                                                          " << endl ;
		cout << "                S E A R C H -- M E N U                    " << endl ;
		cout << "                                                          " << endl ;
		cout << " ======================          ======================== " << endl ;
		cout << "|                      |        |                        |" << endl ;
		cout << "|   SEARCH BY NAME     |   <->  |      SEARCH BY ID      |" << endl ;
		cout << "|                      |        |                        |" << endl ;
		cout << "|      press [1]       |        |        press [2]       |" << endl ;
		cout << "|                      |        |                        |" << endl ;
		cout << " ======================          ======================== " << endl ;
		cout << "                    CHOICE = " ;
		cin>>search;
		system("cls");
		
//	}while(search == 1 && search == 2);

	if(search == 1)
	{
		do{
			system("cls");
			
			cout <<"------------------------------------------------------------------" << endl ;
			cout <<"         - T I P S   F O R  S E A R C H   B Y   N A M E -         " << endl << endl ;
			cout <<"                     [1] Write Full Name                          " << endl ;
			cout <<"             [2] First Alphabet Must Be Uppercase                 " << endl ;
			cout <<"     ";
			endl(2);
			cout<<"Enter Prisoner's Name : ";
			cin>>ws;
			cin.getline(searchPrisoner, 30);
			
			
			
			
			
			
			for(int x=0; x<realPrisoner; x++)
			{
				if(strcmp(searchPrisoner, prisoner[x].name) == 0)
				{
					found = true;
					cout<<"\n\nNAME DETECTED !";
					endl(3);
					cout<<"Name        : "<<prisoner[x].name<<endl;
					cout<<"Age         : "<<prisoner[x].age<<endl;
					cout<<"Block       : "<<prisoner[x].block<<endl;
					cout<<"Cell        : "<<prisoner[x].cell<<endl;
					cout<<"Crime       : "<<prisoner[x].crime<<endl;
					cout<<"Punishment  : "<<prisoner[x].yearPunished<<" years"<<endl;
					cout<<"Status      : "<<prisoner[x].status<<endl;
					cout<<"ID          : "<<prisoner[x].ID<<endl;
					endl(3);
				}
			}

			if(found==false)
			{
				Beep(1000,500) ;
				cout<<"\n\n Name you entered do not exist\n";
				cout<<"\n Do you wish to try again? " << endl ; 
				cout<<" [Y] Y E S | [N] N O : ";
				cin>>answer;
				
				if(answer=='N' || answer=='n')
				{
					found = true ;
				}
			}			
		
		}while(!found);
		
	}
	else if( search == 2)
	{
		do{
			system("cls");
			cout<<"Example : MS11TR"<<endl;
			cout<<"Meaning : Block = Maximum Security/ Cell = 1/ Person = 1/ Crime = Terrorism";
			endl(3);
			cout<<"Enter Prisoner's ID : ";
			cin>>ws;
			cin.getline(searchID, 10);
			
			for(int x=0; x<realPrisoner; x++)
			{
				if(strcmp(searchID, prisoner[x].ID) == 0)
				{
					found = true;
					cout<<"\n\nID DETECTED !";
					endl(3);
					cout<<"Name        : "<<prisoner[x].name<<endl;
					cout<<"Age         : "<<prisoner[x].age<<endl;
					cout<<"Block       : "<<prisoner[x].block<<endl;
					cout<<"Cell        : "<<prisoner[x].cell<<endl;
					cout<<"Crime       : "<<prisoner[x].crime<<endl;
					cout<<"Punishment  : "<<prisoner[x].yearPunished<<" years"<<endl;
					cout<<"Status      : "<<prisoner[x].status<<endl;
					cout<<"ID          : "<<prisoner[x].ID<<endl;
					endl(3);
				}
			}

			if(found==false)
			{
				Beep(1000,500) ;
				cout<<"\n\n ID you entered do not exist\n";
				cout<<"\n Do you wish to try again?" ;
				cout<<"\n [Y] Y E S | [N] N O : " ;
				cin>>answer;
				if(answer=='n'||answer=='N')
				{
					found = true ;
				}
			}			
		
		}while(!found);
	}
	else if(search!=1 || search!=2)
	{
		Beep(1000,500) ;
	}

	endl(3);
	system("pause");
	system("cls");	
}

void update(prisonerInfo prisoner[], int realPrisoner, cellInfo room[][CellNum], ofstream& output4)
{
	//assign number of person inside every room = 0
	int MS1 = 0, MS2 = 0, MS3 = 0, MS4 = 0, MS5 = 0, MS6 = 0, MS7 = 0, MS8 = 0;
	int GP1 = 0, GP2 = 0, GP3 = 0, GP4 = 0, GP5 = 0, GP6 = 0, GP7 = 0, GP8 = 0;
	int SU1 = 0, SU2 = 0, SU3 = 0, SU4 = 0, SU5 = 0, SU6 = 0, SU7 = 0, SU8 = 0;
		
	//assign all room status = 0 which means there is vacancy
	for(int i=0; i<BlockType; i++)
	{
		for(int j=0; j<CellNum; j++)
		{
			room[i][j].status = 0;
		}
	}
	
	//when the number of person inside a cell has reached maximum, cell status = 1
	for(int x=0; x<maxNumPrisoner; x++)
	{
		
		if(strcmp(prisoner[x].block, "MS")==0)
		{
			if(prisoner[x].cell == 1)
			{
				MS1++;
				if(MS1==1)
					room[0][0].status=1;
			}
			else if(prisoner[x].cell == 2)
			{
				MS2++;
				if(MS2==1)
					room[0][1].status=1;
			}
			else if(prisoner[x].cell == 3)
			{
				MS3++;
				if(MS3==1)
					room[0][2].status=1;
			}
			else if(prisoner[x].cell == 4)
			{
				MS4++;
				if(MS4==1)
					room[0][3].status=1;
			}
			else if(prisoner[x].cell == 5)
			{
				MS5++;
				if(MS5==1)
					room[0][4].status=1;
			}
			else if(prisoner[x].cell == 6)
			{
				MS6++;
				if(MS6==1)
					room[0][5].status=1;
			}			
			else if(prisoner[x].cell == 7)
			{
				MS7++;
				if(MS7==1)
					room[0][6].status=1;
			}
			else if(prisoner[x].cell == 8)
			{
				MS8++;
				if(MS8==1)
					room[0][7].status=1;
			}
		}
		else if(strcmp(prisoner[x].block, "GP")==0)
		{
			if(prisoner[x].cell == 1)
			{
				GP1++;
				if(GP1==2)
					room[1][0].status=1;
			}
			else if(prisoner[x].cell == 2)
			{
				GP2++;
				if(GP2==2)
					room[1][1].status=1;
			}
			else if(prisoner[x].cell == 3)
			{
				GP3++;
				if(GP3==2)
					room[1][2].status=1;
			}
			else if(prisoner[x].cell == 4)
			{
				GP4++;
				if(GP4==2)
					room[1][3].status=1;
			}
			else if(prisoner[x].cell == 5)
			{
				GP5++;
				if(GP5==2)
					room[1][4].status=1;
			}
			else if(prisoner[x].cell == 6)
			{
				GP6++;
				if(GP6==2)
					room[1][5].status=1;
			}			
			else if(prisoner[x].cell == 7)
			{
				GP7++;
				if(GP7==2)
					room[1][6].status=1;
			}
			else if(prisoner[x].cell == 8)
			{
				GP8++;
				if(GP8==2)
					room[1][7].status=1;
			}
		}
		else if(strcmp(prisoner[x].block, "SU")==0)
		{
			if(prisoner[x].cell == 1)
			{
				SU1++;
				if(SU1==2)
					room[2][0].status=1;
			}
			else if(prisoner[x].cell == 2)
			{
				SU2++;
				if(SU2==2)
					room[2][1].status=1;
			}
			else if(prisoner[x].cell == 3)
			{
				SU3++;
				if(SU3==2)
					room[2][2].status=1;
			}
			else if(prisoner[x].cell == 4)
			{
				SU4++;
				if(SU4==2)
					room[2][3].status=1;
			}
			else if(prisoner[x].cell == 5)
			{
				SU5++;
				if(SU5==2)
					room[2][4].status=1;
			}
			else if(prisoner[x].cell == 6)
			{
				SU6++;
				if(SU6==2)
					room[2][5].status=1;
			}			
			else if(prisoner[x].cell == 7)
			{
				SU7++;
				if(SU7==2)
					room[2][6].status=1;
			}
			else if(prisoner[x].cell == 8)
			{
				SU8++;
				if(SU8==2)
					room[2][7].status=1;
			}
		}
	}
	
//Prison Floor Plan
    cout << "\t\t ________________________________________________________________________________________________    _________________ " << endl ;
	cout << "\t\t|                                                                                                |  |                 |" << endl ;
	cout << "\t\t|     G E N E R A L   P O P U L A T I O N                     S P E C I A L   U N I T            |  |   ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	cout << "[ ] VACANT";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "    |" << endl ;
	cout << "\t\t|    _______________________________________          _______________________________________    |  |                 |" << endl ;
	cout << "\t\t|   |         |         |         |         |        |         |         |         |         |   |  |  ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	cout << "[ ] OCCUPIED";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "   |" << endl ;
	cout << "\t\t|   |         |         |         |         |        |         |         |         |         |   |  |_________________|" << endl ;
	cout << "\t\t|   |  ";
	if(room[1][0].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "G P 1";
	}	
	else if(room[1][0].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"G P 1";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[1][1].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "G P 2";
	}
	else if(room[1][1].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"G P 2";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[1][2].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "G P 3";
	}	
	else if(room[1][2].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"G P 3";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[1][3].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "G P 4";
	}	
	else if(room[1][3].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"G P 4";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |        |  ";
	if(room[2][0].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "S U 1";
	}	
	else if(room[2][0].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"S U 1";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[2][1].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "S U 2";
	}	
	else if(room[2][1].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"S U 2";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[2][2].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "S U 3";
	}	
	else if(room[2][2].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"S U 3";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
		if(room[2][3].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "S U 4";
	}	
	else if(room[2][3].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"S U 4";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |   |   " << endl ;
	cout << "\t\t|   |         |         |         |         |        |         |         |         |         |   |   " << endl ;
	cout << "\t\t|   |_________|_________|_________|_________|        |_________|_________|_________|_________|   |   " << endl ;
	cout << "\t\t|                                                                                                |   " << endl ;
	cout << "\t\t|                                                                                                |   " << endl ;
	cout << "\t\t|    _______________________________________          _______________________________________    |   " << endl ;
	cout << "\t\t|   |         |         |         |         |        |         |         |         |         |   |   " << endl ;
	cout << "\t\t|   |         |         |         |         |        |         |         |         |         |   |   " << endl ;
	cout << "\t\t|   |  ";
	if(room[1][4].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "G P 5";
	}	
	else if(room[1][4].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"G P 5";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[1][5].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "G P 6";
	}	
	else if(room[1][5].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"G P 6";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[1][6].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "G P 7";
	}	
	else if(room[1][6].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"G P 7";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[1][7].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "G P 8";
	}	
	else if(room[1][7].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"G P 8";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |        |  ";
		if(room[2][4].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "S U 5";
	}	
	else if(room[2][4].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"S U 5";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[2][5].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "S U 6";
	}	
	else if(room[2][5].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"S U 6";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[2][6].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "S U 7";
	}	
	else if(room[2][6].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"S U 7";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[2][7].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "S U 8";
	}	
	else if(room[2][7].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"S U 8";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |   |   " << endl ;
	cout << "\t\t|   |         |         |         |         |        |         |         |         |         |   |   " << endl ;
	cout << "\t\t|   |_________|_________|_________|_________|        |_________|_________|_________|_________|   |   " << endl ;
	cout << "\t\t|   |         |         |         |         |        |         |  __________________________     |   " << endl ;
	cout << "\t\t|   |    S    |         |         |         |        |         | |                          |    |   " << endl ;
	cout << "\t\t|   |    H    |  ";
	if(room[0][0].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "M S 1";
	}	
	else if(room[0][0].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"M S 1";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[0][1].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "M S 2";
	}	
	else if(room[0][1].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"M S 2";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[0][2].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "M S 3";
	}	
	else if(room[0][2].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"M S 3";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |        |  ";
	if(room[0][3].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "M S 4";
	}	
	else if(room[0][3].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"M S 4";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  | |                          |    |   " << endl ;
	cout << "\t\t|   |    O    |         |         |         |        |         | |                          |    |   " << endl ;
	cout << "\t\t|   |    W    |_________|_________|_________|        |_________| |                          |    |   " << endl ;
	cout << "\t\t|   |    E                                                       |    B A S K E T B A L L   |    |   " << endl ;
	cout << "\t\t|   |    R                                                       |                          |    |   " << endl ;
	cout << "\t\t|   |         ______________________________          _________  |        C O U R T         |    |   " << endl ;
	cout << "\t\t|   |    R    |         |         |         |        |         | |                          |    |   " << endl ;
	cout << "\t\t|   |    O    |         |         |         |        |         | |                          |    |   " << endl ;
	cout << "\t\t|   |    O    |  ";
	if(room[0][4].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "M S 5";
	}	
	else if(room[0][4].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"M S 5";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[0][5].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "M S 6";
	}	
	else if(room[0][5].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"M S 6";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |  ";
	if(room[0][6].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "M S 7";
	}	
	else if(room[0][6].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"M S 7";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  |        |  ";
	if(room[0][7].status==0)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		cout << "M S 8";
	}	
	else if(room[0][7].status==1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		cout<<"M S 8";
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "  | |                          |    |   " << endl ;
	cout << "\t\t|   |    M    |         |         |         |        |         | |                          |    |   " << endl ;
	cout << "\t\t|   |_________|_________|_________|_________|        |_________| |__________________________|    |   " << endl ;
	cout << "\t\t|                                                                                                |   " << endl ;
	cout << "\t\t|        M A X I M U M   S E C U R I T Y                                                         |   " << endl ;
	cout << "\t\t|________________________________________________________________________________________________|   " << endl ;

//Update
	endl(3);
	int answer2;
	cout<<"By referring to the above Prison Floor Plan,"<<endl;
	cout<<"Do you want to update any prisoner records? (1. Yes | 2. No) : ";
	cin>>answer2;
	
	if(answer2 == 1)
	{
		char name2[40], block2[5], crime2[5], status2;
		int cell2, yearPunished2;
		
		cout<<"\n\nEnter Name       : ";
		cin>>ws;
		cin.getline(name2,(40));
	
		cout<<"Enter Punishment (years) : ";
		cin>>yearPunished2;
		
		cout<<"Enter Status     : ";
		cin>>status2;
			
		//output File New Data
		output4<<"Name       : "<<name2<<endl;
		output4<<"Punishment : "<<yearPunished2<<endl;
		output4<<"Status     : "<<status2<<endl;
		output4<<"_______________________________________________"<<endl<<endl;
		
		_getch();
	}
	
	system("pause");
	system("cls");
}

void tab(int a)
{
	for(int i=0; i<(a+1); i++)
	{
		cout<<"\t";	
	}
}

void endl(int b)
{
	for(int i=0; i<(b+1); i++)
	{
		cout<<endl;	
	}
}


