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


struct 
{
    const uint8_t nRedLightPin = 2;
    const uint8_t nYellowLightPin = 3;
    const uint8_t nGreenLightPin = 4;
    
    const uint8_t nWalkerWaitPin = 5;
    const uint8_t nWalkerGoPin = 6;
    
    bool boolRedLight = false;
    bool boolYellowLight = false;
    bool boolGreenLight = true;
    bool boolWalkerWait = true;

    uint16_t nRedTime = 60;
    uint16_t nYellowTime = 25;
    uint16_t nGreenTime = 70;

    uint16_t nNotifyAtTime=10;
} TraficLightData;


void setLocation (uint16_t nY, uint16_t nX)
{
    uint8_t szTemp [10];
    uint8_t nLen = snprintf ((char*) szTemp, sizeof(szTemp), "\033[%u;%uH", nY, nX);

    Serial.write (szTemp, nLen);
    Serial.flush();
}


//workis with 256 colors
void setColor (const uint8_t nFgColor, const uint8_t nBgColor)
{
    byte szTemp [10];
    uint8_t nLen = snprintf ((char*) szTemp, sizeof(szTemp), "\033[%u;%um", nFgColor + 30, nBgColor + 40);

    Serial.write (szTemp, nLen);
    Serial.flush();
}


void resetColor ()
{
    Serial.print (F("\033[0m"));
}


void hideCursor ()
{
    Serial.print (F("\033[?25l"));
}


void showCursor ()
{
    Serial.print (F("\033[?25h"));
}


void clearConsole ()
{
    Serial.print (F("\033[2J")); 
}


void reverseColor ()
{
    Serial.print (F("\033[7m"));   
}


void Delay (uint32_t nSleep)
{
    uint32_t nMomentum =  millis();

    //delay (nSleep); return;
    
    do {
        CorePartition_Yield();
    } while ((millis() - nMomentum ) <  nSleep);    
}



void TraficLight ()
{
    
    pinMode (TraficLightData.nRedLightPin, OUTPUT);
    pinMode (TraficLightData.nYellowLightPin, OUTPUT);
    pinMode (TraficLightData.nGreenLightPin, OUTPUT);
    
    while (CorePartition_Yield ())
    {
        digitalWrite (TraficLightData.nRedLightPin, TraficLightData.boolRedLight);
        digitalWrite (TraficLightData.nYellowLightPin, TraficLightData.boolYellowLight);
        digitalWrite (TraficLightData.nGreenLightPin, TraficLightData.boolGreenLight);
    }
}


void WalkerSign ()
{
    pinMode (TraficLightData.nWalkerWaitPin, OUTPUT);
    pinMode (TraficLightData.nWalkerGoPin, OUTPUT);
    
    while (CorePartition_Yield ())
    {
        if (TraficLightData.boolGreenLight == true) 
          digitalWrite (TraficLightData.nWalkerWaitPin, TraficLightData.boolWalkerWait ? HIGH : LOW);
        if (TraficLightData.boolGreenLight == false) 
          digitalWrite (TraficLightData.nWalkerGoPin, TraficLightData.boolWalkerWait ? LOW : HIGH);
    }

}


void TerminalHandler ()
{
    uint8_t nLineBuffer [81];
    bool    boolPrintPrompt = true;
    
    setLocation(1,1);
    clearConsole ();
    
    Serial.println (F("Trafic Light Manager v1.0"));
    Serial.println (F("By Gustavo Campos"));
    Serial.println ();
    
    Serial.flush ();

    while (CorePartition_Yield() || Serial)
    {   
        if (Serial.available () > 0)
        {


            while (Serial.available () > 0)
            {
                Serial.print ("Read: ");
                Serial.println (Serial.read (), HEX); 
            }
            
            Serial.println ("----------");
            Serial.flush ();

            boolPrintPrompt = true;
        }

        if (boolPrintPrompt == true )
        {
            Serial.print (Serial.available ());
            Serial.print (": \033[92mTraficLigth \033[94m>\033[0m ");
            Serial.flush ();
            boolPrintPrompt = false;
        }
    }
}

void Terminal ()
{
    bool boolActive = false;

    
    while (CorePartition_Yield() || true)
    {
        if (Serial)
        {
           TerminalHandler ();
        }
        else
        {
            Serial.println ("Turining Serial off...");
        }
    }

    
}



void TraficLightKernel ()
{
    uint8_t nPin = CorePartition_GetID() + 1;
    
    pinMode (nPin, OUTPUT);
    
    while (CorePartition_Yield ())
    {
        digitalWrite (nPin, HIGH);
        
        CorePartition_Yield ();

        digitalWrite (nPin, LOW);
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

    //Terminal ();
    //exit(0);
    
    CorePartition_Start (4);


    CorePartition_SetCurrentTimeInterface(getTimeTick);
    CorePartition_SetSleepTimeInterface(sleepTick);


    CorePartition_CreateThread (Terminal, 100, 200);

    CorePartition_CreateThread (TraficLight, 20, 50);
    
    CorePartition_CreateThread (WalkerSign, 20, 400);

    CorePartition_CreateThread (TraficLightKernel, 100, 823);
}



void loop()
{
    CorePartition_Join();
}
