#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#endif

// add or remove necessary headers as you please
#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleNurseHelper(const AvailabilityMatrix& avail, DailySchedule& sched, 
const size_t& dailyNeed, const size_t& maxShifts, size_t numShift,
const size_t& numNurses, const size_t& numDays, size_t currentDay,
std::map<Worker_T, size_t> currentAssigned);
size_t daysWorked(size_t currentDay, size_t currentNurse, DailySchedule& sched);


// Add your implementation of schedule() and other helper functions here
//std::vector<std::vector<Worker_T> > DailySchedule;
//std::vector<std::vector<bool>> AvailabilityMatrix;
bool schedule(const AvailabilityMatrix& avail, const size_t dailyNeed, const size_t maxShifts, DailySchedule& sched)
{
  //----do not touch---//
  if(avail.size() == 0U)
  {
    return false;
  }
  sched.clear();

  //------------------//
  // Add your code below
  
  const size_t numNurses = avail.at(0).size();
  const size_t numDays = avail.size();
  size_t numShift = 0;
  std::map<Worker_T, size_t> currentAssigned; //will be used to manage how many days assigned
  
  sched.assign(numDays, std::vector<Worker_T>(dailyNeed, 65535));
  
  //logic for intializing 2d vector
  for(size_t i = 0; i < numDays; i++)
  {
    currentAssigned.insert({i, 0});
  }

  bool determine = scheduleNurseHelper(avail, sched, dailyNeed, maxShifts, numShift, numNurses, numDays, 0, currentAssigned);
  return determine;
}

bool scheduleNurseHelper(const AvailabilityMatrix& avail, DailySchedule& sched, 
const size_t& dailyNeed, const size_t& maxShifts, size_t numShift,
const size_t& numNurses, const size_t& numDays, size_t currentDay,
std::map<Worker_T, size_t> currentAssigned)
{
  //Base case: if all days are scheduled, return true
  if(currentDay == numDays)
  {
    return true;
  }

  //logic to recurse to next day
  else if(sched.at(currentDay).at(sched.at(0).size() - 1) != 65535)
  {
    numShift = 0;
    currentDay++;
    
    if(scheduleNurseHelper(avail, sched, dailyNeed, maxShifts, numShift, numNurses, numDays, currentDay, currentAssigned))
    {
      return true;
    }
    numShift = maxShifts - 1;
    currentDay--;
  }

  else
  {
    //check if each worker is available on this day
    for(size_t i = 0; i < numNurses; i++) 
    {
      //logic to ensure nurse is available and hasnt been scheduled for that same shift
      if((avail.at(currentDay).at(i) == 1) && (std::find(sched.at(currentDay).begin(), sched.at(currentDay).end(), i) == sched.at(currentDay).end()))
      {
        size_t assigned = currentAssigned.find(i)->second;
        //check if the worker has worked too many maxShifts
        if(assigned < maxShifts)
        {
          //logic to determine we need to fill a shift
          if((sched.at(currentDay).at(numShift) == 65535))
          {
            //update
            sched.at(currentDay).at(numShift) = i;
            numShift++;
            currentAssigned.find(i)->second++;
            //recurse call
            if(scheduleNurseHelper(avail, sched, dailyNeed, maxShifts, numShift, numNurses, numDays, currentDay, currentAssigned))
            {
              return true;
            }
            //reset
            numShift--;
            sched.at(currentDay).at(numShift) = 65535;
            currentAssigned.find(i)->second--;
          }
        }
      }
    }
  }
 
  //if no worker could be schedule on this day, return false
  return false; 
}
