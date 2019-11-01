///
/// @author   GUSTAVO CAMPOS
/// @author   GUSTAVO CAMPOS
/// @date   28/05/2019 19:44
/// @version  <#version#>
///
/// @copyright  (c) GUSTAVO CAMPOS, 2019
/// @copyright  Licence
///
/// @see    ReadMe.txt for references
///
//               GNU GENERAL PUBLIC LICENSE
//                Version 3, 29 June 2007
//
//Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
//Everyone is permitted to copy and distribute verbatim copies
//of this license document, but changing it is not allowed.
//
//Preamble
//
//The GNU General Public License is a free, copyleft license for
//software and other kinds of works.
//
//The licenses for most software and other practical works are designed
//to take away your freedom to share and change the works.  By contrast,
//the GNU General Public License is intended to guarantee your freedom to
//share and change all versions of a program--to make sure it remains free
//software for all its users.  We, the Free Software Foundation, use the
//GNU General Public License for most of our software; it applies also to
//any other work released this way by its authors.  You can apply it to
//your programs, too.
//
// See LICENSE file for the complete information




#include "CorePartition.h"

#include "Arduino.h"


void setLocation (uint16_t nY, uint16_t nX)
{
    byte szTemp [10];
    uint8_t nLen = snprintf ((char*) szTemp, sizeof(szTemp), "\033[%u;%uf", nY, nX);

    Serial.write (szTemp, nLen);
}


//workis with 256 colors
void setColor (const uint8_t nFgColor, const uint8_t nBgColor)
{
    byte szTemp [10];
    uint8_t nLen = snprintf ((char*) szTemp, sizeof(szTemp), "\033[%u;%um", nFgColor + 30, nBgColor + 40);

    Serial.write (szTemp, nLen);
}


void resetColor ()
{
    Serial.print ("\033[0m");
}


void hideCursor ()
{
    Serial.print ("\033[?25l");
}


void showCursor ()
{
    Serial.print ("\033[?25h");
}


void clearConsole ()
{
    Serial.print ("\033[2J"); 
}


void reverseColor ()
{
    Serial.print ("\033[7m");   
}


void Delay (uint64_t nSleep)
{
    uint32_t nMomentum =  millis();

    //delay (nSleep); return;
    
    do {
        CorePartition_Yield();
    } while ((millis() - nMomentum ) <  nSleep);    
}


void Thread ()
{
    unsigned long start = millis();
    size_t nValue = 0;
    
    while (1)
    {
        Serial.print ("\e[");
        Serial.print ((CorePartition_GetID()*2) + 6);
        Serial.print (";10H\e[K>> Thread");
        Serial.print (CorePartition_GetID()+1);
        Serial.print (": ");
        Serial.print (nValue++);
        Serial.print (", Sleep Time: ");
        Serial.print (millis() - start);  //start = millis();
        Serial.print (", Nice: ");
        Serial.print (CorePartition_GetNice());
        Serial.print (", CTX: ");
        Serial.print (CorePartition_GetThreadStructSize ());
        Serial.print ("b, Stack: ");
        Serial.print (CorePartition_GetStackSize ());
        Serial.print ("/");
        Serial.print (CorePartition_GetMaxStackSize ());
        Serial.println ("\n");

        Serial.flush ();
          
        start = millis();
        CorePartition_Yield ();
    }
}



static uint64_t getTimeTick()
{
   return (uint64_t) millis();
}

static void sleepTick (uint64_t nSleepTime)
{
    delayMicroseconds  (nSleepTime * 1000);
}


void setup()
{
    bool status; 
    
    //Initialize serial and wait for port to open:
    Serial.begin(115200);

    resetColor ();
    clearConsole ();
    hideCursor ();
    setLocation (1,1);

    Serial.print ("CoreThread ");
    Serial.println (CorePartition_version);
    Serial.println ("");
    
    Serial.println ("Starting up Thread...."); Serial.flush();Serial.flush();

    
    CorePartition_Start (3);
    
    CorePartition_SetCurrentTimeInterface(getTimeTick);
    CorePartition_SetSleepTimeInterface(sleepTick);

    CorePartition_CreateThread (Thread, 70, 612);
    
    CorePartition_CreateThread (Thread, 70, 1000);

    CorePartition_CreateThread (Thread, 70, 100);

}



void loop()
{
    CorePartition_Join();
}
