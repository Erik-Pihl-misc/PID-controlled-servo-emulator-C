/********************************************************************************
* input.h: Contains miscellaneous input functions to read data such as strings,
*          integers and floating point numbers from the terminal.
********************************************************************************/
#ifndef INPUT_H_
#define INPUT_H_

/* Include directives: */
#include <stdio.h>
#include <stdlib.h>

/********************************************************************************
* input_readline: Reads a line from the terminal and stores in referenced string.
*                 A new line is printed to generate space between the entered
*                 line and next input/output.
*
*                 - s   : Reference to string for storage of entered content.
*                 - size: The string capacity.
********************************************************************************/
static void readline(char* s, 
                     const int size)
{
   fgets(s, size, stdin);
   printf("\n");

   for (char* i = s; *i; ++i)
   {
      if (*i == '\n')
      {
         *i = '\0';
      }
   }
   return;
}

/********************************************************************************
* input_get_integer: Returns an integer read from the terminal. A new line is 
*                    printed to generate space between the entered line and 
*                    next input/output.
********************************************************************************/
static int input_get_integer(void)
{
   char s[20] = { '\0' };
   readline(s, sizeof(s));
   return atoi(s);
}

/********************************************************************************
* input_get_unsigned: Returns an unsigned integer read from the terminal. 
*                     A new line is printed to generate space between the 
*                     entered line and next input/output.
********************************************************************************/
static inline size_t input_get_unsigned(void)
{
   return (size_t)(input_get_integer());
}

/********************************************************************************
* input_get_double: Returns a floating point number read from the terminal. 
*                   Supported decimal points are comma (',') and dot ('.').
*                   As default, a new line is printed to generate space between 
*                   the entered line and next input/output.
********************************************************************************/
static double input_get_double(void)
{
   char s[40] = { '\0' };
   readline(s, sizeof(s));
   
   for (char* i = s; *i; ++i)
   {
      if (*i == ',')
      {
         *i = '.';
      }
   }

   return atof(s);
}

#endif /* INPUT_H_ */