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
  RangerProxy     sonarProxy(&robot,0);
  RangerProxy     toothProxy(&robot,1);
  RangerProxy     laserProxy(&robot,2);

    // read from the proxies
    robot.Read();

    sonarProxy.RequestGeom();
    sonarProxy.RequestConfigure();

    toothProxy.RequestGeom();
    toothProxy.RequestConfigure();

    laserProxy.RequestGeom();
    laserProxy.RequestConfigure();

    cout << sonarProxy << endl;
    cout << toothProxy << endl;
    cout << laserProxy << endl;

    robot.Read();

    // Method 1
    cout << sonarProxy.GetRangeCount() << " sonar ranges: ";
    for (int i=0;i<sonarProxy.GetRangeCount()-1;i++) 
        cout<< sonarProxy[i] << ", ";
    cout << sonarProxy[sonarProxy.GetRangeCount()-1] << endl;

    // Method 2, same
    cout << sonarProxy.GetRangeCount() << " sonar ranges: ";
    for (int i=0;i<sonarProxy.GetRangeCount()-1;i++) 
        cout<< sonarProxy.GetRange(i) << ", ";
    cout << sonarProxy.GetRange(sonarProxy.GetRangeCount()-1) << endl;

    cout << laserProxy.GetRangeCount() << " laser ranges"<<endl;
    for (int i=0;i<laserProxy.GetRangeCount()-1;i++) 
        cout<< laserProxy[i] << ", ";
    cout << laserProxy[laserProxy.GetRangeCount()-1] << endl;


    return 0;
}

