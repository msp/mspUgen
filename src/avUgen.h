//
//  avUgen.h
//  mspUgen
//
//  Created by msp on 06/02/2013.
//
//

#ifndef mspUgen_avUgen_h
#define mspUgen_avUgen_h

#include "ofMain.h"
#include "math.h"
#include "ofxMaxim.h"
#include "ofxMidi.h"

namespace msp {
    class avUgen : public ofxMidiListener{

        bool debug = false;

        // visual
        bool visualOutputSwitch, randomResolutionSwitch;
        int x, y, radius, speed, throttle;
        ofColor color;

        // audio
        bool audioOutputSwitch;
        int audioEngine;
        int frequency;
        double volume, lastMIDIVolume;
        int currentCount, lastCount;
        double VCO1out,VCO2out,LFO1out,LFO2out,VCFout,ADSRout, audio;
        double adsrEnv[8]={0.5,100,0.5,250,0.125,125,0,500};

        ofxMaxiOsc VCO1,VCO2,LFO1,LFO2, osc;
        ofxMaxiFilter VCF;
        ofxMaxiEnvelope ADSR;
        ofxMaxiOsc timer;

        ofstream logger;
    public:
        
        ~avUgen();
        
        // Constants
        enum { LIGHT_ALPHA = 220, HEAVY_ALPHA = 50 };
        enum { SINE = 999, MONO = 998 };

        static const int DEFAULT_RADIUS = 300;
        static const int DEFAULT_RADIUS_MULTPLIER = 10;
        static const int DEFAULT_VOLUME = 3;

        
        // Constructor
        avUgen();
        avUgen(int _x, int _y, int _radius, ofColor _color, int _speed);

        void initialize();
        
        // Methods
        void moveTo(int _xDestiny, int _yDestiny);
        void draw();
        void update();
        
        // Mutators
        void setX(int _x);
        void setY(int _y);
        void setRadius(int _radius);
        void setSpeed(int _speed);
        void setColor(ofColor _color);
        void setOscillator(ofxMaxiOsc _osc);
        void setFrequency(int _frequency);
        void setAudioEngine(int _engine);
        void setVolume(double _volume);
        void switchOffAudio();
        void switchOffVisual();
        void setRandomResolution();

        void setMIDIMapping(int _channel, int _control);
        
        // Accessors
        double getAudio();
        double getAudioOutput();
        double getVolume();
        int getRadius();
        bool isAudioOn();
        bool isVisualOn();
        bool isFireMIDI(ofxMidiMessage& msg);

        // ofxmidi
        void newMidiMessage(ofxMidiMessage& eventArgs);
        ofxMidiMessage midiMessage;
        std::vector<int> midiChannel;
        std::vector<int> midiControlNumber;
        std::vector<int> midiValue;

    };
}

#endif
