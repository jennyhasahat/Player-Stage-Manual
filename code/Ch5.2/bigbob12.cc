/* Simple C++ client example.
 * Based on example0.cc from player distribution
 * K. Nickels 7/24/13
 */

#include <iostream>
#include <libplayerc++/playerc++.h>

int main(int argc, char *argv[]) {
    using namespace PlayerCc;
    using namespace std;

    PlayerClient    robot("localhost");
    Position2dProxy p2dProxy(&robot);
    SimulationProxy sp(&robot);

    // read from the proxies
    robot.Read();

    // print out stuff for fun
    cout << sp << endl;
    cout << p2dProxy << endl;

    // command the motors
    p2dProxy.SetSpeed(0.1,0.1); // XSpeed (m/s), YawSpeed (rad/s)
    robot.Read();
    double x,y,a;

    robot.Read();
    sp.GetPose2d((char *)"puck1",x,y,a);
    cout << "Puck1 is at Pose = "<<x<<", "<<y<<", "<<a<<endl;
    sp.GetPose2d((char *)"puck3",x,y,a);
    cout << "Puck3 is at Pose = "<<x<<", "<<y<<", "<<a<<endl;

    sp.SetPose2d((char *)"puck1",4,5,0);
    robot.Read();
    sp.GetPose2d((char *)"puck1",x,y,a);
    cout << "Puck1 is at Pose = "<<x<<", "<<y<<", "<<a<<endl;

    for(int i=0;i<5;i++) {
        robot.Read();
        sp.GetPose2d((char *)"bob1",x,y,a);
        cout << "bob1 Pose = "<<x<<", "<<y<<", "<<a<<endl;
    }

    return 0;
}

