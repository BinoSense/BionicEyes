//
// Created by be on 19-7-19.
//

#ifndef BIONICEYES_BE_VOICE_H
#define BIONICEYES_BE_VOICE_H

#include <string>
#include <evo_be.h>

using namespace std;

namespace evo_be
{
    class BE_Voice
    {
    public:
    	// str_IP: Bionic Eye device ip address
        static BE_Voice* create(const char *str_IP, BE_RunSite_Type type = enumRemote);
        virtual ~BE_Voice();

        // get voice angle (0~359)
        virtual bool getVoiceAngle(int &voiceangle) = 0;

	    // get voice result(#LISTEN#)
        virtual bool getVoiceResult(string &recognizeresult) = 0;

	    // speak something(#SPEAK#)
	    // only support Chinese or English
	    // for example: "你好","hello"
        virtual void sendTTSMessage(string text) = 0;

        // on/off Intellectual dialogue function
        virtual void sendAIUIMessage(bool enable) = 0;

        // Volume Set, range 7-15
        virtual void sendAdjustVoiceSize(int vol_value) = 0;
    };
}



#endif //BIONICEYES_BE_VOICE_H
