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
  BlobfinderProxy     bfp(&robot,0);
  playerc_blobfinder_blob_t blob;

    // read from the proxies
    robot.Read();

    // print out stuff for fun
    cout << bfp << endl;

    robot.Read();
    cout << bfp.GetCount() << " blobs found" << endl;

    for (int i=0;i<bfp.GetCount();i++) {
        blob = bfp.GetBlob(i);
        //blob = bfp[0];
        cout << "BLOB "<<i<<" -----------" <<endl;
        cout << "color = " << hex << blob.color << dec << endl;
        cout << "area = " << (int)blob.area << endl;
        cout << "(x,y) = (" << blob.x << ", " << blob.y << ")"<< endl;
        cout << "box =     " << blob.top << endl;
        cout << "       " << blob.left << "       "<<blob.right<<endl;
        cout << "          " << blob.bottom << endl;
    }

    return 0;
}

