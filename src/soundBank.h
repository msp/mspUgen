//
//  soundBank.h
//  mspUgen
//
//  Created by msp on 30/05/2013.
//
//

#ifndef mspUgen_soundBank_h
#define mspUgen_soundBank_h

#include "ofMain.h"
#include "avUgen.h"

namespace msp {
    class soundBank{

        bool loadFromXML = true;
        bool saveToXML = true;
        
        int currentPreset = 0;
        std::vector<string> avUgenNames;

        void loadPreset(ofxXmlSettings settings, int i, string name, int slot);

    public:
        bool audioReady = true;
        ~soundBank();

        // Constants
        static const int TOTAL_PRESETS = 5 - 1;
        static const int TOTAL_SLOTS = 4 - 1;

        // Constructors
        soundBank();
        soundBank(std::vector<string> _avUgenNames);

        // Collections
        std::vector<std::vector <msp::avUgen*> > presetSlots;
        std::vector<msp::avUgen*> activeSlots;
        void cyclePreset();

        // Serialisation
        void loadPresetsOrInitialise();
        void savePresetsToXML();
    };
}

#endif
