// Hemisphere Applet Boilerplate. Follow these steps to add a Hemisphere app:
//
// (1) Save this file as HEM_GateCounter.ino
// (2) Find and replace "GateCounter" with the name of your Applet class
// (3) Implement all of the public methods below
// (4) Add text to the help section below in SetHelp()
// (5) Add a declare line in hemisphere_config.h, which looks like this:
//     DECLARE_APPLET(id, categories, GateCounter), 
// (6) Increment HEMISPHERE_AVAILABLE_APPLETS in hemisphere_config.h
// (7) Add your name and any additional copyright info to the block below

// Copyright (c) 2018, __________
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "SegmentDisplay.h"

class GateCounter : public HemisphereApplet {
public:

   const char* applet_name() { // Maximum 10 characters
       return "GateCounter";
   }

	/* Run when the Applet is selected */
   
   void Start() {
      segment.Init(SegmentSize::BIG_SEGMENTS); 
      clock_div = 1; 
      clock_pulses = 0; 
   }
   

	/* Run during the interrupt service routine, 16667 times per second */
   void Controller() {
       if (Clock(0)) { 

           if ((clock_pulses % clock_div) == 0 || clock_pulses == 0) { 
                gate_count = gate_count == 4096 ? 0 : gate_count + 1;
           }

           clock_pulses = clock_pulses == 255 ? 0 : clock_pulses + 1; 
       }
    
       if (Clock(1)) {
            gate_count = 0;
            clock_pulses = 0; 
       }
  }
       
   

	/* Draw the screen */
   void View() {
       gfxHeader(applet_name());
       gfxSkyline();
       segment.PrintWhole(1, 15, gate_count); 
       gfxPrint(1, 40, "/");
       gfxPrint(clock_div); 
   }

	/* Called when the encoder button for this hemisphere is pressed */
   void OnButtonPress() {
        gate_count = 0;
        clock_pulses = 0; 
   }

	/* Called when the encoder for this hemisphere is rotated
	 * direction 1 is clockwise
	 * direction -1 is counterclockwise
	 */
   void OnEncoderMove(int direction) {
        if (clock_div + direction >= 1 && clock_div + direction <= 255) { 
            clock_div += direction; 
        }
   }
       
   /* Each applet may save up to 32 bits of data. When data is requested from
    * the manager, OnDataRequest() packs it up (see HemisphereApplet::Pack()) and
    * returns it.
    */
   uint32_t OnDataRequest() {
       uint32_t data = 0;
       // example: pack property_name at bit 0, with size of 8 bits
       // Pack(data, PackLocation {0,8}, property_name); 
       return data;
   }

   /* When the applet is restored (from power-down state, etc.), the manager may
    * send data to the applet via OnDataReceive(). The applet should take the data
    * and unpack it (see HemisphereApplet::Unpack()) into zero or more of the applet's
    * properties.
    */
   void OnDataReceive(uint32_t data) {
       // example: unpack value at bit 0 with size of 8 bits to property_name
       // property_name = Unpack(data, PackLocation {0,8}); 
   }

protected:
   /* Set help text. Each help section can have up to 18 characters. Be concise! */
   void SetHelp() {
       //                               "------------------" <-- Size Guide
       help[HEMISPHERE_HELP_DIGITALS] = "Digital in help";
       help[HEMISPHERE_HELP_CVS]      = "CV in help";
       help[HEMISPHERE_HELP_OUTS]     = "Out help";
       help[HEMISPHERE_HELP_ENCODER]  = "123456789012345678";
       //                               "------------------" <-- Size Guide
   }
   
private:
   uint16_t gate_count;
   uint8_t clock_pulses; 
   uint8_t clock_div; 
   SegmentDisplay segment; 
};


////////////////////////////////////////////////////////////////////////////////
//// Hemisphere Applet Functions
///
///  Once you run the find-and-replace to make these refer to GateCounter,
///  it's usually not necessary to do anything with these functions. You
///  should prefer to handle things in the HemisphereApplet child class
///  above.
////////////////////////////////////////////////////////////////////////////////
GateCounter GateCounter_instance[2];

void GateCounter_Start(bool hemisphere) {GateCounter_instance[hemisphere].BaseStart(hemisphere);}
void GateCounter_Controller(bool hemisphere, bool forwarding) {GateCounter_instance[hemisphere].BaseController(forwarding);}
void GateCounter_View(bool hemisphere) {GateCounter_instance[hemisphere].BaseView();}
void GateCounter_OnButtonPress(bool hemisphere) {GateCounter_instance[hemisphere].OnButtonPress();}
void GateCounter_OnEncoderMove(bool hemisphere, int direction) {GateCounter_instance[hemisphere].OnEncoderMove(direction);}
void GateCounter_ToggleHelpScreen(bool hemisphere) {GateCounter_instance[hemisphere].HelpScreen();}
uint32_t GateCounter_OnDataRequest(bool hemisphere) {return GateCounter_instance[hemisphere].OnDataRequest();}
void GateCounter_OnDataReceive(bool hemisphere, uint32_t data) {GateCounter_instance[hemisphere].OnDataReceive(data);}
