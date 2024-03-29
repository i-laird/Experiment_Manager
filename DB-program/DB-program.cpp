/*
 * file: DB-program.cpp
 * author: Ian Laird
 * course: CSI 3335
 * due date: November 5, 2018
 *
 * This file contains the driver for the program.
 */
#include <iostream>
#include <mysqlx/xdevapi.h>
#include "DB.h"
#include <iomanip>

using std::cout;
using std::endl;
using std::cin;
using namespace mysqlx;
const char *url = "mysqlx://root:password@127.0.0.1",
		*prompt = "+-----------------------------------+\n"
	              "|      Please enter a selection:    |\n"
	              "+-----------------------------------+\n"
	              "| 0 |  End program                  |\n"
	              "| 1 |  Experiment Entry             |\n"
	              "| 2 |  Run Entry                    |\n"
	              "| 3 |  Experiment Info              |\n"
		          "| 4 |  Run information              |\n"
	              "| 5 |  Experiment Report            |\n"
	              "| 6 |  Aggregate Report             |\n"
	              "|   |                               |\n"
	              "+-----------------------------------+\n";
int main(int argc, const char* argv[]) {
	try {
		Session session(url);
		int selection;
		std::string experimentNum;
		cout << std::left;
		/*
		//This is tester code DEBUG DO NOT USE
		RowResult res = session.sql("SELECT * FROM University.student;").execute();
		Row temp;
		while (temp = res.fetchOne()) {
			for (int i = 0; i < temp.colCount(); i++)
				cout << temp[i] << ' ';
			cout << endl;
		}
		*/
		do {
			cout << prompt << endl;
			cin >> selection;
			if (selection <= 6 && selection >= 2) {
				experimentNum = DB::getExperimentNumberCheckExist(session, std::cin, std::cout, EXISTS);
			}
			//empty string means that the user went back without entering a valid string
			if (selection == 0 || selection == 1 || !experimentNum.empty()) {
				switch (selection) {
				case 1:
					//this case prompts user to enter experiment info and paramaters and results (i.e. METADATA)
					DB::enterExperiment(session, std::cin, std::cout);
					break;
				case 2:
					//this case prompts the user to enter in run data for an experiment
					DB::enterRun(session, experimentNum, std::cin, std::cout);
					break;
				case 3:
					//this case prints out the METADATA for an experiment
					//no user input is required
					DB::printExperimentMetaData(session, experimentNum, std::cout);
					break;
				case 4:
					//this case  lists the runs of an experiment and then the user can 
					//choose to list more information about a specific run
					DB::printRunData(session, experimentNum, std::cin, std::cout);
					break;
				case 5:
					//this case creats an HTML document that has tables of the runs of an experiment
					//opens a file and makes a bunch of html tables
					DB::experimentReport(session, experimentNum);
					break;
				case 6:
					//this case aggregates values in the experiment that are chosen by the user
					DB::displayAggregate(session, experimentNum, std::cin, std::cout);
					break;
				case 0:
					session.close();
					return 0;
				default:
					std::cerr << "Unknown selection" << endl;
				}
				cout << endl;
				experimentNum.clear();
			}
		} while (true);
	}
	catch (const mysqlx::Error &err)
	{
		std::cerr << err << endl;
		std::cerr << "closing the program" << std::endl;
		return 1;
	}
}
