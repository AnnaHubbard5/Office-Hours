/*READ ME
  Questions take between 1 and 10 mins each, so I use (rand() % 10) + 1 when a question is asked to determine its length.
  Max # of students = 100 because there are 100 students in the class.
  Priority of queue ranges from 1 to 3.
  Tracks topic each student asked about in each office hour, gives final sorted report at end.
  Option to search for student.
*/

#include <iostream>
#include <cassert>
#include <queue>
#include <map>
#include <iterator>
#include <fstream>
#include <string>
#include "prof.h"

using namespace std;

averager avg_wait;
averager avg_time_spent;
averager avg_extra_time;
multimap<string, string> report;

class Student{
  public:
    Student(){
      name = "Student" + to_string((rand() % 100) + 1); //random student name created for 1 of 100 students
      urgency = (rand() % 3) + 1; //generates random urgency from 1 to 3 and pushes value into priority queue
      question = topics[rand() % 5]; //generates random question from list of topics
    }
    string name;
    int arrival_time;
    int urgency;
    string question;
    string topics[5] = {"Stacks", "Queues", "Binary Trees", "B-Trees", "Heaps"};

};

void officeHours(double arrival_prob, unsigned int total_time){
  priority_queue <Student> arrivals; //queue of waiting students
  unsigned int next; //value taken from queue
  bool_source arrival(arrival_prob);
  prof professor;
  averager wait_times;
  averager time_spent;
  unsigned int extra_time = 0;
  int num_students = 0; //number of students that went to office hours, max is 100

  unsigned int current_min;
  for (current_min = 1; (current_min <= total_time) && (num_students <= 100); ++current_min){   // Simulate the passage of one minute of time
   // Check whether a new student has arrived
   if (arrival.query( )){
     Student s;
     report.insert(pair<string, string>(s.name, s.question));
     s.arrival_time = current_min;

     arrivals.push(s);
     num_students++;
   }
   //Check whether a student can start asking a question
   if ((!professor.is_busy( ))  &&  (!arrivals.empty( ))){
     next = arrivals.top().arrival_time;
     arrivals.pop();
     wait_times.next_number(current_min - next);
     time_spent.next_number(professor.ask_question());
   }
    //Tell the prof to simulate the passage of one min
    professor.one_min( );
  }

  while(!arrivals.empty( ) && (num_students <= 100)){
    extra_time++;
    if ((!professor.is_busy( ))  &&  (!arrivals.empty( ))){
      next = arrivals.top().arrival_time;
      arrivals.pop();
      wait_times.next_number(current_min - next);
      professor.ask_question();
    }
     //Tell the prof to simulate the passage of one min
     professor.one_min( );
  }

  //Find averages of simulation
    avg_wait.next_number(wait_times.average( ));
    avg_time_spent.next_number(time_spent.average());
    avg_extra_time.next_number(extra_time);
}

bool operator <(const Student& s1, const Student& s2){
  return (s1.urgency < s2.urgency);
}

void createFile(){
  ofstream out("report.txt"); //creates file for all num_students
  assert(out.is_open());
  string name = report.begin()->first;
  for (multimap<string,string>::iterator it = report.begin(); it != report.end(); ++it){
    string next_name = it->first; //name of next student in map
    if(next_name != name){
      out << name << ": " << endl; //outputs students name
      int n = 0; //number of times student visited office hours
      for (auto i = report.lower_bound(name); i != report.upper_bound(name); i++) {
        out << i -> second << endl;
        n++;
      }
    out << "Times visited: " << n << endl; //outputs topics that student asked about
    }
    name = next_name;
  }
  out.close();

    cout << "Final sorted report file has been created." << endl << endl;
}

void searchStudent(){
  int n = 0; //number of times visited office hours
  cout << "What number student do you want to search for? (enter number 1-100) ";
  string num;
  cin >> num;
  string name = "Student" + num;

  cout << "Here is the report for " << num << ": " << endl;
  for (auto i = report.lower_bound(name); i != report.upper_bound(name); i++) {
    cout << i -> second << ", ";
    n++;
  }
  cout << "Times visited: " << n << endl << endl;

}

void searchSubject(){
  cout << "What # chapter do you want to search for? (Acceptable Answers: 1(Stacks), 2(Queues), 3(Binary Trees), 4(B-Trees), or 5(Heaps) ";
  int topic;
  cin >> topic;
  string topics[5] = {"Stacks", "Queues", "Binary Trees", "B-Trees", "Heaps"};
  string subject = topics[topic-1];

  cout << "Here is the report for " << subject << ": " << endl;

  string name = report.begin()->first;
  for (multimap<string,string>::iterator it = report.begin(); it != report.end(); ++it){
    string next_name = it->first; //name of next student in map
    if(it->second == subject && next_name != name){
      cout << it->first << ", "; //outputs students name
    }
    name = next_name;
  }
  cout << endl;
}


int main(){

  srand(time(0));

  for(int i = 0; i < 100; i++){
    //studetns arrive on average once every 5 mins
    //simulation time is 60 mins
    officeHours(1.0/5, 60);
  }

  cout << "The office hour simulation has been run 100 times. Here's the summary:" << endl;
  cout << "Average time a student spends waiting during an office hour visit = " << avg_wait.average() << " mins" << endl;
  cout << "Average time a student spends with the professor during an office hour visit = " << avg_time_spent.average() << " mins" << endl;
  cout << "Average time a Professor spends at an office hour beyond the 1 hour he intended to = ";
  cout << avg_extra_time.average() << " mins" << endl;

  createFile();
  searchStudent();
  searchSubject();



  return 0;
}
