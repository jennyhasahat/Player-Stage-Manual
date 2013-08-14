/* Simple C++ client example.
 * Shows gripper, and how it works.
 *
 * To run, compile this program.
 * execute:
 * > player bigbob4.cfg (in one terminal window)
 * > ./ex_gripper (in another terminal window)
 *
 * K. Nickels 7/24/13
 */

#include <iostream>
#include <libplayerc++/playerc++.h>

const char *gripper_state(int s) {
    switch(s) {
        case 1: return "open"; break;
        case 2: return "closed"; break;
        case 3: return "moving"; break;
        case 4: return "error"; break;
        default: return "unknown"; break;
    }
}

int main(int argc, char *argv[]) {
  using namespace PlayerCc;

  PlayerClient    robot("localhost");
  GripperProxy    gp(&robot,0);
  Position2dProxy pp(&robot,0);

    // read from the proxies
    robot.Read();

    // print out stuff for fun
    std::cout << gp << std::endl;

    // go forward
    pp.SetSpeed(0.1, 0.0);

    // keep going till you see something
    while (gp.GetBeams()==0) { 
         robot.Read(); 
         // 1 is open, 2 is closed, 3 is moving
         std::cout << "Gripper is "<< gripper_state(gp.GetState()) << std::endl;
    }
    // stop and close gripper
    pp.SetSpeed(0.0, 0.0);
    gp.Close();
    while (gp.GetState()!=2) {
        robot.Read();
        std::cout << "Gripper is "<< gripper_state(gp.GetState()) << std::endl;
    }

    // back off, and give some time to move
    pp.SetSpeed(-0.1, 0.0);
    for (int i=1;i<20;i++) {
        robot.Read();
         std::cout << "Gripper is "<< gripper_state(gp.GetState()) << std::endl;
    }

    return 0;
}

