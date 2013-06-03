//
//  soundBank.cpp
//  mspUgen
//
//  Created by msp on 30/05/2013.
//
//

#include "soundBank.h"

namespace msp {
    soundBank::soundBank(){

    }
    soundBank::soundBank(std::vector<string> _avUgenNames){
        avUgenNames = _avUgenNames;

        loadPresetsOrInitialise();

        for (int i = 0; i <= TOTAL_SLOTS; i++){
            for(int j = 0; j <= TOTAL_PRESETS; j++){
                ofLogVerbose() << presetSlots[i][j]-> getName();
            }
        }

        cyclePreset();
    }

    soundBank::~soundBank(){
    }

    void soundBank::cyclePreset(){

        ofLogVerbose() << "currentPresent: " << currentPreset << endl;

        audioReady = false;
        activeSlots.clear();
        
        for (int i = 0; i <= TOTAL_SLOTS; i++){
            activeSlots.push_back(presetSlots[i][currentPreset]);
            ofLogVerbose() << "active at slot [" << i << "] : " << activeSlots.at(i) -> getName();
        }
        audioReady = true;

        if (currentPreset == TOTAL_PRESETS){
            currentPreset = 0;
        } else {
            currentPreset++;
        }

        ofLogVerbose() << "cyclePreset: activeSlots: " << activeSlots.size() << endl;
    };

    
    void soundBank::loadPresetsOrInitialise(){

        string filename = "";
        ofxXmlSettings settings;

        for(int i = 0; i <= TOTAL_SLOTS; i++) {

            // initialise the 2d vector
            vector<msp::avUgen*> row;
            presetSlots.push_back(row);

            filename = "avUgen/" + avUgenNames.at(i) + ".xml";

            if(settings.loadFile(filename)){
                ofLogNotice() << filename + " loaded! +++++++++++++++++";

                settings.pushTag("avUgens", 0);

                ofLogVerbose() << "Found tags: " << settings.getNumTags("avUgen") ;
                ofLogVerbose() << "Found presets in file: " << settings.getNumTags("avUgen") << endl;

                for (int j = 0; j <= TOTAL_PRESETS; j++){

                    char buffer[100];
                    std::ostringstream buf;                    
                    buf << avUgenNames.at(i) << "-" << j;

                    loadPreset(settings, j, buf.str(), i);
                }

                settings.popTag(); //avUgens

            } else {
                ofLogWarning() << "unable to load "+ filename +" check data/ folder.";
            }
            
        }

    }


    void soundBank::loadPreset(ofxXmlSettings settings, int i, string name, int slot) {
        avUgen *preset = new msp::avUgen(settings.getValue("avUgen:name", name, i));

        if (loadFromXML){
            preset->setX(settings.getValue("avUgen:x", preset->getX(), i));
            preset->setY(settings.getValue("avUgen:y", preset->getY(), i));
            preset->setAudioEngine(settings.getValue("avUgen:audioEngine", preset->getAudioEngine(), i));
            preset->setRadius(settings.getValue("avUgen:radius", preset->getRadius(), i));
            //TODO raw set volume without sqrt
            preset->setVolume(settings.getValue("avUgen:volume", preset->getVolume(), i));
            //TODO raw set pan without /100
            preset->setPan(settings.getValue("avUgen:pan", preset->getPan(), i));
            preset->setFrequency(settings.getValue("avUgen:frequency", preset->getFrequency(), i));
            preset->setVisualOutputSwitch(settings.getValue("avUgen:visualOutputSwitch", preset->getVisualOutputSwitch(), i));
            preset->setAudioOutputSwitch(settings.getValue("avUgen:audioOutputSwitch", preset->getAudioOutputSwitch(), i));
            preset->setAudioEngine(settings.getValue("avUgen::audioEngine", msp::avUgen::MONO, i));
            preset->setRandomResolutionSwitch(settings.getValue("avUgen:randomResolutionSwitch", preset->getRandomResolutionSwitch(), i));
            preset->setAnimateRadiusSwitch(settings.getValue("avUgen:animateRadiusSwitch", preset->getAnimateRadiusSwitch(), i));
            preset->setThrottle(settings.getValue("avUgen:throttle", preset->getThrottle(), i));

            ofColor _color = *new ofColor();
            _color.setHue(settings.getValue("avUgen:hue", ofRandom(255), i));
            _color.setSaturation(settings.getValue("avUgen:saturation", ofRandom(255), i));
            _color.setBrightness(settings.getValue("avUgen:brightness", ofRandom(255), i));
            preset->setColor(_color);
        }

        preset->inspect();
        presetSlots[slot].push_back(preset);
    }

    void soundBank::saveActivePresetToXML(){
        if (saveToXML) {

            ofLogVerbose() << "saveActivePresetToXML" << endl;

            string filename = "";
            int tagId = 0;

            // file loop
            for (int i = 0; i <= TOTAL_SLOTS; i++){

                ofxXmlSettings settings;

                tagId = settings.addTag("avUgens");
                settings.pushTag("avUgens", tagId);
                avUgen *preset;

                for (int j = 0; j <= TOTAL_PRESETS; j++){

                    preset = presetSlots[i][j];
                    preset->inspect();

                    tagId = settings.addTag("avUgen");
                    settings.pushTag("avUgen",tagId);

                    settings.setValue("name", preset->getName(), i);
                    settings.setValue("x", preset->getX(), i);
                    settings.setValue("y", preset->getY(), i);
                    settings.setValue("audioEngine", preset->getAudioEngine(), i);
                    settings.setValue("radius", preset->getRadius(), i);
                    //TODO raw set volume without sqrt
                    settings.getValue("volume", preset->getVolume(), i);
                    //TODO raw set pan without /100
                    settings.setValue("pan", preset->getPan(), i);
                    settings.setValue("frequency", preset->getFrequency(), i);
                    settings.setValue("visualOutputSwitch", preset->getVisualOutputSwitch(), i);
                    settings.setValue("audioOutputSwitch", preset->getAudioOutputSwitch(), i);
                    settings.setValue(":audioEngine", msp::avUgen::MONO, i);
                    settings.setValue("randomResolutionSwitch", preset->getRandomResolutionSwitch(), i);
                    settings.setValue("animateRadiusSwitch", preset->getAnimateRadiusSwitch(), i);
                    settings.setValue("throttle", preset->getThrottle(), i);

                    settings.setValue("hue", preset->getColor().getHue());
                    settings.setValue("saturation", preset->getColor().getSaturation());
                    settings.setValue("brightness", preset->getColor().getBrightness());

                    filename = "avUgen/" + avUgenNames.at(i) + ".xml";

                    settings.popTag(); //avUgen
                }

                settings.popTag(); //avUgens

                if (settings.saveFile(filename)) {
                    ofLogNotice() << "written presets +++++++++++++++++: " << " to file: " << filename << endl;
                } else {
                    ofLogError() << "unable to save "+ filename;
                }


            }
        }
    }
}