#ifndef PROF_H
#define PROF_H
#include <cassert>
#include <cstdlib>

using namespace std;


  class bool_source{
    public:
    //constructor
    bool_source(double p = 0.5){
      assert(p >= 0);
      assert(p <= 1);
      prob = p;
    }

    bool query( ) const{ return (rand( ) < prob * RAND_MAX);}
    private:
      double prob; //Prob of query( ) returning true
  };
  class averager{
    public:
      //constructor
      averager( ){
        count = 0;
        sum = 0;
      }
      void next_number(double value){
        ++count;
        sum += value;
      }
      size_t how_many_numbers( ) const { return count; }
      double average( ) const{
        assert(how_many_numbers( ) > 0);
        return sum/count;
      }
    private:
      std::size_t count; //How many numbers have been given to the averager
      double sum;   //Sum of all the numbers given to the averager
  };
  class prof{
    public:
      //constructor
      prof(){
        q_time_left = 0;
      }
      void one_min( ){
        if (is_busy( ))
        --q_time_left;
      }
      int ask_question( ){
         assert(!is_busy( ));
         mins_for_q = (rand() % 10) + 1;
         q_time_left = mins_for_q;
         return mins_for_q;
      }
      //CONSTANT function
      bool is_busy( ) const { return (q_time_left > 0); }
    private:
      unsigned int mins_for_q; //mins for a single question
       int q_time_left;   //mins until prof no longer busy
  };

#endif
