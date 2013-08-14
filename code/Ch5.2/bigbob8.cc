/* Simple C++ client example.
 * Based on example0.cc from player distribution
 * K. Nickels 7/24/13
 */

#include <iostream>
#include <libplayerc++/playerc++.h>

int main(int argc, char *argv[]) {
  using namespace PlayerCc;

  PlayerClient    robot("localhost");
  Position2dProxy p2dProxy(&robot,0);

  // read from the proxies
  robot.Read();

  // for carlike robots
  //p2dProxy.SetSpeed(0.1,0.1); // XSpeed (m/s), YawSpeed (rad/s)
  p2dProxy.SetCarlike(0.2,dtor(3)); // XSpeed, Yaw (rad)
  // for holonomic robots
  //p2dProxy.SetSpeed(0.1,0.1,0.1); // XSpeed (m/s), YawSpeed (rad/s)

  for(int i=0;i<10;i++) {
        robot.Read();
        std::cout << "Iter "<<i<<std::endl;
        std::cout << "XSpeed = " << p2dProxy.GetXSpeed() << " m/s, " <<
                "YSpeed = " << p2dProxy.GetYSpeed() << " m/s, " <<
                "YawSpeed = " << p2dProxy.GetYawSpeed() << " rad/s"<< std::endl;
        std::cout << "XPos = " << p2dProxy.GetXPos() << " m, " <<
                "YPos = " << p2dProxy.GetYPos() << " m, " <<
                "Yaw = " << p2dProxy.GetYaw() << " rad"<< std::endl;
    }

    return 0;
}

