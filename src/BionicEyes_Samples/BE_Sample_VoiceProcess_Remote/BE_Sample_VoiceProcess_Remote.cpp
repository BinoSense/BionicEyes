//
// Created by be_linux on 2020/6/16.
//

#include <evo_be_Common.h>
#include <evo_be_Voice.h>
#include <string.h>

using namespace evo_be;

//int voice_init(const char *device);
//bool getVoiceAngle(int &voiceangle);
//bool getVoiceResult(string &recognizeresult);
//void sendTTSMessage(string text);
//void sendAIUIMessage(bool enable);
//void sendAdjustVoiceSize(int vol_value);//voice range 7-15

int main()
{
	CBionicEyes *device = device->create(enumdisConnect);
	string deviceIP = device->getBeDevice_Ip_str();

	BE_Voice *voice = voice->create(deviceIP.c_str(), enumRemote);      // Ip address of Bionic eye III

	int angle = 0;
	string res;
	int count = 0;

	while(1)
	{

		if(voice->getVoiceResult(res))
		{
			printf("%s\n", res.c_str());
		}

		if(voice->getVoiceAngle(angle))
		{
			printf("%d\n", angle);
		}
		msleep(10);
		if((count++)%500  == 0)
		{
			voice->sendTTSMessage("我是小白");
		}
	}

	return 0;
}