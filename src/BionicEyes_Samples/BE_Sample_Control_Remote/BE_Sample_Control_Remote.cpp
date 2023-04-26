//
// Created by be_linux on 2020/11/02.
//

#include <evo_be_Common.h>
using namespace evo_be;

int main()
{
    CBionicEyes *device = device->create(enumConnect_Control); // if only control eye_movement, enumConnect_Control can be used.

    // vor and sv function can affect eye_movement control
    device->onoff_VOR(false);
    device->onoff_SV(false);

    //// wait to take effort
    msleep(1000);

    float up_bd[6], low_bd[6], init_pos[6];;

    //// get eye motor initial position
    device->getInitPosition(enumAllMotor, init_pos);

	for (int i = 0; i < 6; i++)
	{
		device->getUpDownLimit((MotorType)i, up_bd[i], low_bd[i]);
		msleep(40);
		printf("Motor %d(up, down)(%f, %f)\n", i, up_bd[i], low_bd[i]);
	}

	//// set all motor to move to initial position
	device->goInitPosition(enumAllMotor);
	//// same effect
	//device->setAbsolutePosition(enumAllMotor, init_pos);

    //// wait to take effort(Motor move need at least 200ms, and it depends on amplitude you set)
    msleep(1000);

	//// set specialized motors absolution position
	//// For example, we set four motors work. The subscript definition can be
	//// seen in @MotorType
    bool motorWorkFlag[6] = {1,1,0,1,0,1};
    float motor_value[6] = {
            init_pos[0] + 10,
            init_pos[1] - 6,
            init_pos[2] + 8,
            init_pos[3] + 15,
            init_pos[4] - 12,
            init_pos[5] + 20,
    };
    device->setAbsolutePosition(motorWorkFlag, motor_value);

    //// wait some time
    msleep(5000);

    device->goInitPosition(enumAllMotor);

    //// wait to take effort
    msleep(1000);

    //// set specialized motors relative position
    //// For example, we set four motors work. The subscript definition can be
    //// seen in @MotorType
    bool motorWorkFlag2[6] = {0,1,1,0,1,1};
    float motor_value2[6] = {-8, 9, 7, 7, -9, -10};
    device->setRelativePosition(motorWorkFlag2, motor_value2);

    //// wait some time
    msleep(5000);

    device->goInitPosition(enumAllMotor);

    //// wait to take effort
    msleep(1000);

    return 0;
}
