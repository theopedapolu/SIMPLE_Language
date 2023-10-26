#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <array>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <algorithm>


using namespace std; // use the standard naming library 

int symIndex = 0; //Index for accessing variables in the VarTable
int counter = 0; //Index that shows how many lines are in the SIMPLE Code
string LineTable[100][4]; //Table with Line Number - Keyword - Place in SIMPLE - Place in SML
string VarTable[100][2]; //Table with Variable Name and SML Memory Index
int valueIndex = 99; //SML Memory Index
int varIndex = 0; //Index for passing all variables through memory first
ofstream SMLFile ("SMLFile.txt", ios::out); //File to print out SML Code

array<string, 100> keywords = {"rem", "input", "print", "end", "goto","if","let"}; //List of possible keywords
array<array<string, 100>, 1000> code = {}; //Written SIMPLE Code

  array<string, 100> split(string temp) { //Split a string by "_" characters.
    array<string, 100> tempArray = {}; //Most of this code was taken from StackOverflow
    char temp2[temp.size() + 1];
    strcpy(temp2,temp.c_str());
    int i = 0;
    char* token = strtok(temp2, "_");
    while (token != NULL) {
      tempArray[i] = token;
      token = strtok(NULL, "_");
      i++;
  } 
  return tempArray;
}

int lookForIndex (string var) { //Look for a variable's index in memory
  int value = -1; //Default return value
  int i = 0;
  
  while (i < 100) { //Look through every element of the Var Table
     if (VarTable[i][0] == var) { //If the element in the table matches the argument
       stringstream s(VarTable[i][1]); 
       s >> value; //Return its SML Index
       break; //Stop Looking
     }
     i++;
  }
  return value;
}

int lookForIndex2 (string var) { //Look for keyword's number for the switch-case
  int value;
  for (int i = 0; i < keywords.size(); i++) { //Look through all of the keywords
    if (keywords[i] == var) { //If the element in the table matches the argument
      value = i; //Return the string's index of that table
      break;
    }
    else
    {
      value = -1; //Default to -1 if nothing found (Will Crash)
    }
  }
  return value;
}

int lookForLineIndex (string LineNum) { //Return SML Index
  int value;
  for (int i = 0; i < counter; i++) { //Look through the Line Numbers f0r its SML Index
    if (LineTable[i][0] == LineNum) { //If the Line Number matches the argument
        stringstream s(LineTable[i][3]); //Returns its SML Index
        s >> value;
    }
  }
  return value;
}

bool is_number(const string& s) //Checks if the string has only numbers
{
    string::const_iterator it = s.begin(); //Taken from StackOverflow, but basically uses the isdigit function on every character
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool is_alphanum(const string& s) //Checks if the string has only alphanumeric characters
{
    string::const_iterator it = s.begin(); //Also taken from StackOverflow, but basically uses isalnum function of every character
    while (it != s.end() && isalnum(*it)) ++it;
    return !s.empty() && it == s.end();
}

void compare (vector<string> firstExp, string compare, vector<string> secondExp, string gotoNum) { //Compare two expressions from postfix form
	

	int tempIndex = valueIndex; //Temporary value index prevents overwriting
  int value;
	for (int i = 0; i < firstExp.size(); i++) { //For every character in the first expression
		if (is_number(firstExp[i])) { //If its a number
			  stringstream s(firstExp[i]);
        s >> value;
        SMLFile << to_string(2200 + (value % 100)) << endl; // Store the Tens and Ones digits to accumulator
        SMLFile << to_string(2300 + (value / 100)) << endl; // Add Thousands and Hundreds digit to accumulator
        SMLFile << to_string(2100 + tempIndex) << endl; // Add to SML Memory
        tempIndex--;
    }
    else if (is_alphanum(firstExp[i])) { //Else if its a variable
        varIndex = lookForIndex(firstExp[i]);
        SMLFile << to_string(2000 + varIndex) << endl; //Store the variable's value into the accumulator
        SMLFile << to_string(2100 + tempIndex) << endl; //Add to SML Memory
        tempIndex--;
    }

    //OPERATIONS (Repeated Idea for All of Them)
    else if (firstExp[i] == "+") {
      SMLFile << to_string(2000 + tempIndex + 2) << endl; //Store the first value into accumulator
      SMLFile << to_string(3000 + tempIndex + 1) << endl; //Perform binary operation with second value
      SMLFile << to_string(2100 + tempIndex + 2) << endl; //Override first value with the new value
      tempIndex++; //Overwrite second value on next iteration
    }
    else if (firstExp[i] == "-") {
      SMLFile << to_string(2000 + tempIndex + 2) << endl;
      SMLFile << to_string(3100 + tempIndex + 1) << endl;
      SMLFile << to_string(2100 + tempIndex + 2) << endl;
      tempIndex++;
    }
    else if (firstExp[i] == "*") {
      SMLFile << to_string(2000 + tempIndex + 2) << endl;
      SMLFile << to_string(3300 + tempIndex + 1) << endl;
      SMLFile << to_string(2100 + tempIndex + 2) << endl;
      tempIndex++;
    }
    else if (firstExp[i] == "/") {
      SMLFile << to_string(2000 + tempIndex + 2) << endl;
      SMLFile << to_string(3200 + tempIndex + 1) << endl;
      SMLFile << to_string(2100 + tempIndex + 2) << endl;
      tempIndex++;
    }
    else {
      cerr << "Error. Unidentified operator in expression" << endl; //If illegal expression found
      exit (EXIT_FAILURE); //explode
    }
  }

//Repeat Everything from the First Expression, except do it with the second expression
  for (int i = 0; i < secondExp.size(); i++) {
		if (is_number(secondExp[i])) {
			  stringstream s(secondExp[i]);
        s >> value;
        SMLFile << to_string(2200 + (value % 100)) << endl;
        SMLFile << to_string(2300 + (value / 100)) << endl;
        SMLFile << to_string(2100 + tempIndex) << endl;
        tempIndex--;
    }
    else if (is_alphanum(secondExp[i])) {
        varIndex = lookForIndex(secondExp[i]);
          if (varIndex == -1)
          {
            cout << "The variable " << secondExp[i] << " was not found" << endl;
            exit (EXIT_FAILURE);
          }
        SMLFile << to_string(2000 + varIndex) << endl;
        SMLFile << to_string(2100 + tempIndex) << endl;
        tempIndex--;
    }
    else if (secondExp[i] == "+") {
      SMLFile << to_string(2000 + tempIndex + 2) << endl;
      SMLFile << to_string(3000 + tempIndex + 1) << endl;
      SMLFile << to_string(2100 + tempIndex + 2) << endl;
      tempIndex++;
    }
    else if (secondExp[i] == "-") {
      SMLFile << to_string(2000 + tempIndex + 2) << endl;
      SMLFile << to_string(3100 + tempIndex + 1) << endl;
      SMLFile << to_string(2100 + tempIndex + 2) << endl;
      tempIndex++;
    }
    else if (secondExp[i] == "*") {
      SMLFile << to_string(2000 + tempIndex + 2) << endl;
      SMLFile << to_string(3300 + tempIndex + 1) << endl;
      SMLFile << to_string(2100 + tempIndex + 2) << endl;
      tempIndex++;
    }
    else if (secondExp[i] == "/") {
      SMLFile << to_string(2000 + tempIndex + 2) << endl;
      SMLFile << to_string(3200 + tempIndex + 1) << endl;
      SMLFile << to_string(2100 + tempIndex + 2) << endl;
      tempIndex++;
    }
    else {
      cerr << "Error. Unidentified operator in expression" << endl; //E X P L O D E
      exit (EXIT_FAILURE);
    }
  }

  int firstExpNum = tempIndex + 2; //Result of First Expression was stored two memory spaces earlier
  int secondExpNum = tempIndex + 1; //Result of Second Expression was stored one memory space earlier
  int lineNum = lookForLineIndex(gotoNum); //Find the line number to branch to
  SMLFile << to_string(2000 + firstExpNum) << endl; //Store First Expression into accumulator
  SMLFile << to_string(3100 + secondExpNum) << endl; //Subtract Second Expression
  //Depending on what the expression is looking for
  if (compare == "==") { 
    SMLFile << to_string(4200 + lineNum) << endl; //Branch Zero
  }
  else if (compare == ">") {
    SMLFile << to_string(4300 + lineNum) << endl; //Branch Positive
  }
  else if (compare == "<") {
    SMLFile << to_string(4100 + lineNum) << endl; //Branch Negative
  }
  else if (compare == ">=") {
    SMLFile << to_string(4300 + lineNum) << endl; //Branch Positive AND
    SMLFile << to_string(4200 + lineNum) << endl; //Branch Zero
  }
  else if (compare == "<=") {
    SMLFile << to_string(4100 + lineNum) << endl; //Branch Negative AND
    SMLFile << to_string(4200 + lineNum) << endl; //Branch Zero
  }
  else if (compare == "!=") {
    SMLFile << to_string(4300 + lineNum) << endl; //Branch Positive
    SMLFile << to_string(4100 + lineNum) << endl; //Branch Negative
  }
  else {
    cerr << "Error. Unidentified operator in expression" << endl;
    exit (EXIT_FAILURE); //If illegal comparison operator found, explode
  }
  
}

void calculate (string var, vector<string> express) { //Calculation for One Expression
    int tempIndex = valueIndex;
    int value;
	  for (int i = 0; i < express.size(); i++) {

      //Same expression function as the Compare Functions
		  if (is_number(express[i])) {
			    stringstream s(express[i]);
          s >> value;
          SMLFile << to_string(2200 + (value % 100)) << endl;
          SMLFile << to_string(2300 + (value / 100)) << endl;
          SMLFile << to_string(2100 + tempIndex) << endl;
          tempIndex--;
      }
      else if (is_alphanum(express[i])) {
          varIndex = lookForIndex(express[i]);
          if (varIndex == -1)
          {
            cout << "The variable " << express[i] << " was not found" << endl;
            exit (EXIT_FAILURE);
          }
          SMLFile << to_string(2000 + varIndex) << endl;
          SMLFile << to_string(2100 + tempIndex) << endl;
          tempIndex--;
      }
      else if (express[i] == "+") {
        SMLFile << to_string(2000 + tempIndex + 2) << endl;
        SMLFile << to_string(3000 + tempIndex + 1) << endl;
        SMLFile << to_string(2100 + tempIndex + 2) << endl;
        tempIndex++;
      }
      else if (express[i] == "-") {
        SMLFile << to_string(2000 + tempIndex + 2) << endl;
        SMLFile << to_string(3100 + tempIndex + 1) << endl;
        SMLFile << to_string(2100 + tempIndex + 2) << endl;
        tempIndex++;
      }
      else if (express[i] == "*") {
        SMLFile << to_string(2000 + tempIndex + 2) << endl;
        SMLFile << to_string(3300 + tempIndex + 1) << endl;
        SMLFile << to_string(2100 + tempIndex + 2) << endl;
        tempIndex++;
      }
      else if (express[i] == "/") {
        SMLFile << to_string(2000 + tempIndex + 2) << endl;
        SMLFile << to_string(3200 + tempIndex + 1) << endl;
        SMLFile << to_string(2100 + tempIndex + 2) << endl;
        tempIndex++;
      }
      else {
        cerr << "Error. Unidentified operator in expression" << endl;
        exit (EXIT_FAILURE);
      }
    }
    int trueIndex = lookForIndex(var); //Look for the variables position in memory
    SMLFile << to_string(2000 + valueIndex) << endl; //Input the evaluated expression into accumulator
    SMLFile << to_string(2100 + trueIndex) << endl; //Override the variables current value with this expression
}

// takes in a string and converts it into a vector using | as a delimiter
vector<string> convertToVec(string s)
{
  // initialize varaibles
  vector<string> tokens;  
  stringstream conArr(s);
  string intermediate;
  // loop as long as there is a | and add the separated strings to the tokens vector
  while(getline(conArr,intermediate,'|'))
  {
    tokens.push_back(intermediate);
  }
  return tokens;  // return the vector of string 
}

// function that finds what index of a vector a given key lies in
// returns -1 if the key is not in the vector
int findMarker(vector<string> v, string key)
{
  // initialize variables.
  int j = -1;
  int i = 0;
  bool found = false;
  // loop through the vector as long as the key is not found
  while (i < v.size() && !found)
  {
    if (v[i] == key) // if the key is found
    {
      j = i;  // store the index
      found = true; // end the loop
    }
    i++;
  }
  return j; // return j. If the key is not found, the initialized -1 value of j will be returned
}

// function that returns the minimum positive value of two integers
int minPos(int a, int b)
{
  int m = min(a,b); // get the minimum using the built-in min function
  if (m < 0) // if the minimum is negative
  {
    m = max(a,b);  // the minimum positive value must be the other number
  }
  return m; // return the minimum positive number
}

// Raw postfix function. Converts vector string in infix and returns a vector string in postfix
// Handles everything except parentheses 
vector<string> postfix(vector<string> tokens)
{
  if (tokens.size() == 1)  // if it is a single string
  {
    return tokens;  // return the original vector because it is already in postfix
  }
  // initialize variables
   int index = 0;
   string n;
   vector<string> newS;

   // handle the multiplication and division first because they have precedence
   // loop while there is a * or / sign in the vector
   while ((count(tokens.begin(),tokens.end(),"*") != 0) || (count(tokens.begin(),tokens.end(),"/") != 0))   {
      index = minPos(findMarker(tokens,"*"),findMarker(tokens,"/")); // find the first occurrence of a * or / sign

      // cases to deal with where the $ place holder occurs in the vector and if it does not occur 
      // creates a new string vector newS that contains the converted postfix expression
      if (tokens[index+1] == "$" && tokens[index-1] == "$")
      {
        newS.push_back(tokens[index]);
      }
      else if (tokens[index-1] == "$")
      {
        newS.push_back(tokens[index+1]);
        newS.push_back(tokens[index]);
      }
      else if (tokens[index+1] == "$") {
        n = tokens[index - 1];
        newS.push_back(tokens[index]);
        newS.insert(newS.begin(),n);
      }
      else {
        newS.push_back(tokens[index-1]);
        newS.push_back(tokens[index + 1]);
        newS.push_back(tokens[index]);
      }
      // replace the operands and operator with a $ as a placeholder. Ex. 3*4 is replaced with $.
    tokens.erase(tokens.begin() + index - 1,tokens.begin() + index+1); 
    tokens[index-1] = "$";
   }
  
  // handles the cases with addition and subtraction afterwards since these have lower precedence
  // loop as long as there is a + or - sign in the vector
  while ((count(tokens.begin(),tokens.end(),"+") != 0) || (count(tokens.begin(),tokens.end(),"-") != 0))
  {
    index = minPos(findMarker(tokens,"+"),findMarker(tokens,"-")); // find the first occurrence of a + or - sign
      
      // cases to deal with where the $ placeholder occurs in the expression and where to add the operators
      // and operands to the newS postifix vector
      if (tokens[index+1] == "$" && tokens[index-1] == "$")
      {
        newS.push_back(tokens[index]);
      }
      else if (tokens[index-1] == "$") 
      {
        newS.push_back(tokens[index+1]);
        newS.push_back(tokens[index]);
      }
      else if (tokens[index+1] == "$") {
        n = tokens[index - 1];
        newS.push_back(tokens[index]);
        newS.insert(newS.begin(),n);
      }
      else {
        newS.push_back(tokens[index-1]);
        newS.push_back(tokens[index + 1]);
        newS.push_back(tokens[index]);
      }
    // replace the operands and operator with a $ as a placeholder. Ex. 3 + 4 is replaced with $
    tokens.erase(tokens.begin() + index - 1,tokens.begin() + index+1);
    tokens[index-1] = "$";
  }
 return newS; // return the converted postifix vector
}

// Mature postfix function that uses recursion and the raw postfix function to handle parentheses as well
vector<string> postfix2(vector<string> tokens)
{
  if ((count(tokens.begin(),tokens.end(),"(") == 0)) // base case, if there are no parentheses
  {
    return postfix(tokens); // the postfix expression can just be done using the raw function
  }
  else  // if there are parentheses
  {
    int Fparen = findMarker(tokens,"(");  // find index of first open parenthesis
    reverse(tokens.begin(),tokens.end()); // reverse the vector of strings
    // find the index of the corresponding close parenthesis
    int Lparen = findMarker(tokens,")"); 
    Lparen = tokens.size() - Lparen - 1;
    reverse(tokens.begin(),tokens.end()); // reverse the vector back to its original state
    vector<string> subVec; // initialize a subvector
    copy(tokens.begin() + Fparen + 1,tokens.begin()+Lparen,back_inserter(subVec)); // store the subvector contained by the parentheses into subVec
    tokens.erase(tokens.begin() + Fparen + 1, tokens.begin()+Lparen+1); // erase the expression inside the parentheses
    tokens[findMarker(tokens,"(")] = "#"; // replace the removed expression, including parentheses with # as a place holder
    vector<string> newTok = postfix(tokens); // perform the raw postfix on the expression with placeholder #
    int hash = findMarker(newTok,"#"); // find the index of the # mark
    vector<string> firstPart; 
    copy(newTok.begin(),newTok.begin() + hash,back_inserter(firstPart)); // get the part of the vector before the # mark and store it in firstPart
    vector<string> lastPart;  
    copy(newTok.begin()+hash+1,newTok.end(),back_inserter(lastPart)); // get the part of the vector after # and store it in lastPart
    vector<string> sub = postfix2(subVec); // recursion. Perform the mature postfix on the subvector we got before inside the parentheses. Store the converted subvector into the sub vector
    firstPart.insert(firstPart.end(),sub.begin(),sub.end()); // add the converted subvector to the end of firstPart
    firstPart.insert(firstPart.end(),lastPart.begin(),lastPart.end()); // add lastPart to the end of the previous vector
    return firstPart; // return the fully converted postifx expression as a string vector
  }
}

void compile() { //Takes the SML Code from the program and compile it
  array<int, 100> compile = {};
  
//Give Error if file does not exist

  ifstream inFile2 ("SMLFile.txt", ios::in);
  if(!inFile2) {
     cerr<< "File not able to be opened" <<endl;
     exit(EXIT_FAILURE);
  }

  int command = 0;
  int accumulator;
  int indexC = 0; 
  bool run = true;

   inFile2 >> command;

//Put all of the numbers from the file into the array

  while(!inFile2.eof()) 
  {
    compile[indexC] = command;
    indexC++;
    inFile2 >> command;
  }

  indexC = 0;
  cout << endl;

//The Program

  while (run && indexC < 100) {
    //cout << code[index] / 100 << endl;
     
    switch(compile[indexC] / 100) { //Check the first two digits of the number
        case 10: { //READ
          cout << endl;
          int i;
          cout << "Enter a value: ";
          cin >> i;
          compile[compile[indexC] % 100] = i;
          //cout << "I just read " << i << " into index " << (code[index] % 100) << endl;
          indexC++;
        } break;

        case 11: { //WRITE
          cout << endl;
          cout << "The value is: " << compile[compile[indexC] % 100] << endl;
          indexC++;
        } break;

        case 20: { //LOAD
          accumulator = compile[compile[indexC] % 100];
          //cout << "I just loaded " << code[code[index] % 100] << " into the accumulator" << endl;
          indexC++;
        } break;

        case 21: { //STORE
          compile[compile[indexC] % 100] = accumulator; 
          //cout << "I just stored " << accumulator << " into index " << (code[index] % 100) << endl;
          indexC++;
        } break;

        case 22: { //Input Tens and Ones
          accumulator = compile[indexC] % 100;
          indexC++;
        } break;

        case 23: { //Add Hundreds and Thousands
          accumulator += ((compile[indexC] % 100) * 100);
          indexC++;
        } break;

        case 30: { //ADD
          accumulator += compile[compile[indexC] % 100];
          //cout << "I just added " << code[code[index] % 100] << " into the accumulator " << endl;
          //cout << "The accumulator now has value " << accumulator << endl;
          indexC++;
        } break;

        case 31: { //SUBTRACT
          accumulator = accumulator - compile[compile[indexC] % 100];
          //cout << "I just subtracted " << code[code[index] % 100] << " from the accumulator " << endl;
          //cout << "The accumulator now has value " << accumulator << endl;
          indexC++;
        } break;

        case 32: { //DIVIDE
          accumulator = accumulator / compile[compile[indexC] % 100];
          //cout << "I just divided " << code[code[index] % 100] << " from the accumulator " << endl;
          //cout << "The accumulator now has value " << accumulator << endl;
          indexC++;
        } break;

        case 33: { //MULTIPLY
          accumulator = accumulator * compile[compile[indexC] % 100];
          //cout << "I just multiplied " << code[code[index] % 100] << " into the accumulator " << endl;
          //cout << "The accumulator now has value " << accumulator << endl;
          indexC++;
        } break;

        case 40: { //BRANCH
          //cout << "The accumulator has the value " << accumulator << endl;

            indexC = (compile[indexC] % 100);
        
          //cout << "I just branched to index " << index << endl;
        } break;

        case 41: { //BRANCHNEGATIVE
          //cout << "The accumulator has the value " << accumulator << endl;          
          if (accumulator < 0) {
            indexC = (compile[indexC] % 100);
          }
          else {
            indexC++;
          }
          //cout << "I just branched to index " << index << endl;
        } break;
        
        case 42: { //BRANCHZERO
          //cout << "The accumulator has the value " << accumulator << endl;
          if (accumulator == 0) {
            indexC = (compile[indexC] % 100);
          }
          else {
            indexC++;
          }
          //cout << "I just branched to index " << index << endl;
        } break;

        case 43: { //BRANCHPOS
          if (accumulator > 0) {
            indexC = (compile[indexC] % 100);
          }
          else {
            indexC++;
          }
        } break;

        default: { //HALT (Caused by error or end of file)
          run = false;
          indexC = 101;
          cout << "HALT! (Caused by error or end of file)" << endl;
        } break;
    }
  } 
}

// Main function that is executed
int main() {

  
  bool run = true; 
  for (int i = 0; i < 100; i++) {
    VarTable[i][0] = to_string(-1); //Fill the Variable Table with -1 (Prevents Crashing)
    VarTable[i][1] = to_string(-1);
  }
 
//Enter File
  string filename;
  cout << "Enter the name of the file to be processed: "; //Input SIMPLE Code
  cin >> filename;

//Give Error if file does not exist
  ifstream inFile (filename, ios::in);
  if(!inFile) {
     cerr<< "File not able to be opened. File not found." <<endl; //If File Name doesn't exist
     exit(EXIT_FAILURE); //explode
  }

  string command;
  int index = 0; 
  

//Put all of the numbers from the file into the array
  while(!inFile.eof()) 
  {
    inFile >> command;
    code[index] = split(command);
    index++;
    counter++;
  }
   
  index = 0;
  int inputIndex = 0;
  int SMLIndex = 0;
  valueIndex = 99;
  symIndex = 0;

  while (index < counter) {  //First Pass (Look for Variables)
    if (code[index][1] == "input" || code[index][1] == "let") { //Only two places where a variable can be stored
      if (lookForIndex(code[index][2]) == -1) { //If the variable has not been initialized already
        VarTable[symIndex][0] = code[index][2]; //Add variable name to VarTable
        VarTable[symIndex][1] = to_string(valueIndex); //Add its respective index in memory
        symIndex++;
        valueIndex--;
      }
    }
    index++;
  }

  index = 0;
  symIndex = 0;

  while (index < counter) { //Second Pass to find SML Lines (Used for Goto)
      LineTable[inputIndex][0] = code[index][0]; //Line Number
      LineTable[inputIndex][1] = code[index][1]; //Command or Keyword
      LineTable[inputIndex][2] = to_string(index); //Location of line in Text File
      LineTable[inputIndex][3] = to_string(SMLIndex);
      if (code[index][1] == "rem") {
        //Rem statements don't change the SML Index
      }
      else if (code[index][1] == "if") { //If the line has an if statement
        vector<string> firstExp = postfix2(convertToVec(code[index][2])); //Find the postfix forms of both expressions
        vector<string> secondExp = postfix2(convertToVec(code[index][4]));
        for (int i = 0; i < firstExp.size(); i++) {
          if (is_number(firstExp[i])) {
            SMLIndex += 3; //Numbers require 3 spaces in SML
          }
          else if (is_alphanum(firstExp[i])) {
            SMLIndex += 2; //Variables require 2 spaces in SML
          }
          else {
            SMLIndex += 3; //Operations require 3 spaces in SML
          }
        }

        //Same situation for the second expression
        for (int i = 0; i < secondExp.size(); i++) {
          if (is_number(secondExp[i])) {
            SMLIndex += 3; 
          }
          else if (is_alphanum(secondExp[i])) {
            SMLIndex += 2;
          }
          else {
            SMLIndex += 3;
          }
        }
        //If the comparison only has one branch statement, it takes up 3 SML Lines
        if (code[index][3] == ">" || code[index][3] == "<" || code[index][3] == "==") {
          SMLIndex += 3;
        }
        //All other comparison statements take up 4 SML Lines
        else {
          SMLIndex += 4;
        }
        
      }

      //Rinse and repeat expression SML Line algorithm
      else if (code[index][1] == "let") {
        vector<string> express = postfix2(convertToVec(code[index][4]));
        for (int i = 0; i < express.size(); i++) {
          if (is_number(express[i])) {
            SMLIndex += 3;
          }
          else if (is_alphanum(express[i])) {
            SMLIndex += 2;
          }
          else {
            SMLIndex += 3;
          }
        }
        SMLIndex += 2;
      }
      
      //All other keywords only require one SML Line
      else {
         SMLIndex++;
      }
      inputIndex++;
      index++;
  }

  index = 0;
  int g = 0;
  
  // Third Pass. Convert the SIMPLE Language commands to SML code and write it to a file
  // Loop through the code array which contains the SIMPLE Language code line by line
  while (index < counter) {
    int k = lookForIndex2(code[index][1]); // get the command 
    if (k == -1)  // if the command is not recognized 
    {
      cerr << "The command " << (code[index][1]) << " is not recognized" << endl; // throw an error
      exit(EXIT_FAILURE); // terminate the program
    }
    switch(k) {
      case 0: {//rem
          index++;
      }
      break;
      case 1: {//input
        SMLFile << to_string(1000 + lookForIndex(code[index][2])) << endl;
        index++;
      }
      break;
      case 2: {//print
          int n;
          SMLFile << to_string(1100 + lookForIndex(code[index][2])) << endl;
          index++;
      }
      break;
      case 3: {//end
          SMLFile << "4400" << endl;
          index++;
      }
      break;
      case 4: { //goto
           SMLFile << to_string(4000 + lookForLineIndex(code[index][2])) << endl;
           index++;
      }
      break;
      case 5: { //if
          vector<string> firstExp = postfix2(convertToVec(code[index][2]));
          vector<string> secondExp = postfix2(convertToVec(code[index][4]));
          compare(firstExp, code[index][3], secondExp, code[index][6]);
          index++;
      } break;
      case 6: {// let 
          string variable = code[index][2];
          string exp = code[index][4];
          vector<string> expPostfix = postfix2(convertToVec(exp));
          calculate(variable,expPostfix);
          index++;
      } 
      break;
    }

  }
    
    inFile.close();
    SMLFile.close();

    // Determine if the SML file contains 100 or more lines. Throw an error if it does
    ifstream inFileEnd("SMLFile.txt",ios::in); // Take in the output file

    // calculate the number of lines
    int lines = 0;
    string s;
    while(!inFileEnd.eof())
    {
      inFileEnd >> s;
      lines++;
    }

    // Show an error message if the SML memory overflows (there are more than 100 lines of SML code in the output file)
    if (lines > 100)
    {
      cout << "Memory Overflow. There is not enough space." << endl;
    }

    inFileEnd.close();  // close the output file

    compile();
}