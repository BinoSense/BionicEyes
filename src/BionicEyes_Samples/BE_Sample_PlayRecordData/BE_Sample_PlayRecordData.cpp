//
// Created by ok_linux on 2022/2/18.
//
//
// Created by ok_linux on 2020/10/29.
//

#include "evo_be_Common.h"
#include <opencv2/core.hpp>
#include "evo_be_Common_Internal.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <dirent.h>
#include <sys/stat.h>

using namespace evo_be;

int main()
{
    string folderpath;
    int rate;
    cout << ">>> BionicEyes fake-server will start, please input some necessary parameters!"<<endl;
    cout << ">>>>>> Please input local data folder path (default is: ../bin/io_path/fastSave/. Press 'd' or 'D' choose default path.) :" << endl;
    //cout << ">>>>>> If using default folder path, press ENTER. Or input your folder path: "<<endl;
    cin >> folderpath;

    while (opendir(folderpath.c_str()) == NULL) {
        if(folderpath=="d" || folderpath=="D")
        {
            folderpath = "/usr/Evo_BionicEyes/io_path/fastSave/";
            break;
        }
        cout << ">>>>>> This folder doesn't exist! Reinput local data folder path (default is: /usr/Evo_BionicEyes/io_path/fastSave/. Press ENTER choose default path.) :" << endl;
        cin >> folderpath;
    }
    //folderpath = "../bin/io_path/fastSave_T/";
//    cout << ">>>>>> Then input the play rate(fps, Must be interger) you want \n(\nInput 0 means using real play rate\nInput -1 means one by one transmission mode\n)" <<endl;
//
//    cin >> rate;
    CBE_DataServer* dataServer = dataServer->create(folderpath+"", rate);

    cout<<dataServer->getDataListNum()<<endl;

    for(int i = 0; i< dataServer->getDataListNum();i++)
    {
        BE_GeneralData dataT;
        dataServer->getData_IncludeImage_listId(i, dataT);
        cv::imshow("img", dataT.image[2]);
        cv::waitKey(100);
    }
    return 0;
}