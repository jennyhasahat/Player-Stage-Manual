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

    // get and re-set the color
    float green[]= {0.67, 0.88, 0.43, 1};
    float puckcolor[4];
    robot.Read();
    sp.GetProperty((char *)"puck1",(char*)"color",puckcolor,4*sizeof(float));
    cout << "Puck1 is color = ("<<puckcolor[0]<<","<<puckcolor[1]<<","
         << puckcolor[2]<<","<<puckcolor[3]<<")"<<endl;
    sp.GetProperty((char *)"puck3",(char*)"color",puckcolor,4*sizeof(float));
    cout << "Puck3 is color = ("<<puckcolor[0]<<","<<puckcolor[1]<<","
         << puckcolor[2]<<","<<puckcolor[3]<<")"<<endl;

    cout << "setting puck1 to green"<<endl;
    sp.SetProperty((char *)"puck1",(char*)"color",green,4*sizeof(float));
    sp.GetProperty((char *)"puck1",(char*)"color",puckcolor,4*sizeof(float));
    cout << "Puck1 is color = ("<<puckcolor[0]<<","<<puckcolor[1]<<","
         << puckcolor[2]<<","<<puckcolor[3]<<")"<<endl;

    return 0;
}

