/***************************************************

Class for Timer Function
	Created a namespace ProgTimer for calling
	timer function.

Return Runtime in seconds: 
Elapsed time = present time – start time; 
***************************************************/

#include <chrono>

namespace ProgTimer {

class Timer
{
private:
   
   typedef std::chrono::high_resolution_clock Clock_T;
   
public:
   
   Timer() : start_( Clock_T::now() ) {}
    

//Reset the clock_ to present time   

   void reset() { start_ = Clock_T::now(); }
    
//Return elapsed time as (present time - start time)
   double elapsed() const
   {
      using namespace std::chrono;
      return duration_cast< duration< double, seconds::period > >( Clock_T::now() - start_ ).count();
   }

private:

   Clock_T::time_point start_;
};

}
