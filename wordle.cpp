#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
std::set<std::string> findAll(std::string in, std::string floating, size_t length, size_t nWords,  size_t nFloating, const std::set<std::string>& dict);
size_t getFixed(const std::string& in);

// Definition of primary wordle function
std::set<std::string> wordle(const std::string& in, const std::string& floating, const std::set<std::string>& dict)
{
  //all possible english words found with floating letters
  std::string floatCopy = floating;
  std::string inCopy = in;
  size_t inLength = in.length();
  size_t lengthCopy = getFixed(in);
  std::set<std::string> foundWords = findAll(inCopy, floatCopy, inLength, lengthCopy, 0, dict); 
  return foundWords;
}

std::set<std::string> findAll(std::string inCopy, std::string floatCopy, size_t length, size_t nLetters,  size_t nFloating, const std::set<std::string>& dict)
{
  std::set<std::string> valid;

  //logic for determining when to insert word into valid set
  if((nLetters == length) && (dict.find(inCopy) != dict.end()))
  {
    valid.insert(inCopy);
  }

  else
  {
    //logic for determining that there are still letters to be checked
    for(size_t i = 0; i < length; i++)
    {
      if(inCopy[i] == '-')
      {
        //logic to recursively check letters a through z, when no more floating letters
        if(nFloating == floatCopy.size())
	      {
          for(size_t j = 0; j <= 25; j++)
	        {
            //update values
            inCopy.at(i) = 'a' + j;
            nLetters++;
            //recursive call and add to valid set
            std::set<std::string> subVal = findAll(inCopy, floatCopy, length, nLetters, nFloating, dict);
            valid.insert(subVal.begin(), subVal.end());
            //reset values to original state
            nLetters--;
            inCopy.at(i) = '-';
          }
        }
        //logic to fill current word with floating letters
        else
	      {
          //update values
          inCopy.at(i) = floatCopy.at(nFloating);
          nLetters++;
          nFloating++;
          //recursive call and add to valid set
          std::set<std::string> subVal = findAll(inCopy, floatCopy, length, nLetters, nFloating, dict);
          valid.insert(subVal.begin(), subVal.end());
          //reset values to original state
          inCopy.at(i) = '-'; 
          nFloating--;
          nLetters--;
        }
      }
    }
  }
  return valid;
}

size_t getFixed(const std::string& in)
{
  size_t count = 0;
  for(size_t i = 0; i < in.length(); i++)
  {
    if(in.at(i) != '-')
    {
      count++;
    }
  }
  return count;
}
