// Desc: Bigbob code for controlling a junk finding robot.
// Author:  Jennifer Owen
// Date: 16/04/2010
// Updates (Stage4) Kevin Nickels 7 Aug 2013

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <libplayerc++/playerc++.h>
//#include <values.h>

struct Item
{
      char name[16];
      double x;
      double y;
}typedef item_t;


using namespace PlayerCc;

/**
Randomly assigns new speeds into the given addresses. 
This function will always write to the given addresses.
@param *forwardSpeed the address of the forward speed 
variable you want this function to change.
@param *turnSpeed the address of the turn speed variable 
you want this function to change.
*/
void Wander(double *forwardSpeed, double *turnSpeed)
{
      int maxSpeed = 1;
      int maxTurn = 90;
      double fspeed, tspeed;
	
      //fspeed is between 0 and 10
      fspeed = rand()%11;
      //(fspeed/10) is between 0 and 1
      fspeed = (fspeed/10)*maxSpeed;
	
      tspeed = rand()%(2*maxTurn);
      tspeed = tspeed-maxTurn;
	
      *forwardSpeed = fspeed;
      *turnSpeed = tspeed;
} 


/**
Checks sonars for obstacles and updates the given addresses 
with wheel speeds. This function will write to the addresses 
only if there is an obstacle present. Very basic obstacle avoidance only.
@param *forwardSpeed the address of the forward speed variable 
you want this function to change.
@param *turnSpeed the address of the turn speed variable you 
want this function to change.
@param &sp The sonar proxy that you want this function to monitor.
*/
void AvoidObstacles(double *forwardSpeed, double *turnSpeed, \
      RangerProxy &sp)
{
      //will avoid obstacles closer than 40cm
      double avoidDistance = 0.4;
      //will turn away at 60 degrees/sec
      int avoidTurnSpeed = 60;
      
      //left corner is sonar no. 2
      //right corner is sonar no. 3
      if(sp[2] < avoidDistance)
      {
            *forwardSpeed = 0;
            //turn right
            *turnSpeed = (-1)*avoidTurnSpeed;
            printf("avoiding obstacle\n");
            return;
      }
      else if(sp[3] < avoidDistance)
      {
            *forwardSpeed = 0;
            //turn left
            *turnSpeed = avoidTurnSpeed;
            printf("avoiding obstacle\n");
            return;
      }
      else if( (sp[0] < avoidDistance) && \
               (sp[1] < avoidDistance))
      {
            //back off a little bit
            *forwardSpeed = -0.2;
            *turnSpeed = avoidTurnSpeed;  
            printf("avoiding obstacle\n");
            return;
      }
      
      return; //do nothing
}


/**
If blobs have been detected this function will turn the robot 
towards the largest blob. This will be the closest blob (hopefully!). 
If called this function will always overwrite information in the 
given addresses.
@param *forwardSpeed the address of the forward speed variable 
you want this function to change.
@param *turnSpeed the address of the turn speed variable you 
want this function to change.
@param &bfp The blobfinder proxy that you want this function 
to monitor.
*/
void MoveToItem(double *forwardSpeed, double *turnSpeed, \
      BlobfinderProxy &bfp)
{
      int i, centre;
      int noBlobs = bfp.GetCount();
      playerc_blobfinder_blob_t blob;
      int turningSpeed = 5; // in deg/s

      /*number of pixels away from the image centre a blob
      can be to be in front of the robot*/
      int margin = 10;
      
      int biggestBlobArea = 0;
      int biggestBlob = 0;
      
      //find the largest blob
      for(i=0; i<noBlobs; i++)
      {
            //get blob from proxy
            playerc_blobfinder_blob_t currBlob = bfp[i];
            
            // (.area is a negative cast into an unsigned int! oops.)
            if( abs((int)currBlob.area) > biggestBlobArea)
            {
                  biggestBlob = i;
                  biggestBlobArea = abs((int)currBlob.area);
            }
      }
      blob = bfp[biggestBlob];
      //printf("biggest blob is %i with area %d\n",biggestBlob,biggestBlobArea);
            
      // find centre of image
      centre = bfp.GetWidth()/2;
      
      //adjust turn to centre the blob in image
      /*if the blob's centre is within some margin of the image 
      centre then move forwards, otherwise turn so that it is 
      centred. */
      //printf("blob.x=%d, c=%d\n",blob.x,centre);

      //blob to the left of centre
      if(blob.x < centre-margin)
      {
            *forwardSpeed = 0;
            //turn left
            *turnSpeed = turningSpeed;
            //printf("turning left\n");
      }
      //blob to the right of centre
      else if(blob.x > centre+margin)
      {
            *forwardSpeed = 0;
            //turn right
            *turnSpeed = -turningSpeed;
            //printf("turning right\n");
      }
      //otherwise go straight ahead
      else
      {
            *forwardSpeed = 0.1;
            *turnSpeed = 0;      
            //printf("straight on\n");
      }
      
      return;
}



/**
Fills the item list array with the names and positions of items 
in the simulation
@param itemList this is the item list which contains the names 
and positions of all the items in the simulation.
@param simProxy the simulation proxy for the Player/Stage simulation.
*/
void RefreshItemList(item_t *itemList, SimulationProxy &simProxy)
{
      int i;
	
	//get the poses of the oranges
	for(i=0;i<4;i++)
	{
	      char orangeStr[] = "orange%d";
	      sprintf(itemList[i].name, orangeStr, i+1);
	      double dummy;  //dummy variable, don't need yaws.
	      simProxy.GetPose2d(itemList[i].name, \
	            itemList[i].x, itemList[i].y, dummy);
	}
	
	//get the poses of the cartons
	for(i=4;i<8;i++)
	{
	      char cartonStr[] = "carton%d";
	      sprintf(itemList[i].name, cartonStr, i-3);
	      double dummy;  //dummy variable, don't need yaws.
	      simProxy.GetPose2d(itemList[i].name, \
	            itemList[i].x, itemList[i].y, dummy);
	}
	
	return;
}






/**
Finds an item in the simulation which is near the robot's teeth. 
@param itemList this is the item list which contains the names and 
positions of all the items in the simulation.
@param listLength The number of items in the simulation
@param sim the simulation proxy for the Player/Stage simulation.
@return returns the index of the item in the array which is within 
the robot's teeth. If no item is found then this will return -1.
*/
int FindItem(item_t *itemList, int listLength, SimulationProxy &sim)
{
	/*
		This function works by creating a search area just
		in front of the robot's teeth. The search circle is a
		fixed distance in front of the robot, and has a 
		fixed radius.
		This function finds objects within this search circle
		and then deletes the closest one.
	*/
	
	//radius of the search circle
      double radius = 0.375;
      
      //The distance from the centre of the robot to 
      //the centre of the search circle
      double distBotToCircle = 0.625;
      double robotX, robotY, robotYaw;
      double circleX, circleY;
      
      //find the robot...
      sim.GetPose2d((char*)"bob1", robotX, robotY, robotYaw);
      
      /*now we find the centre of the search circle. 
      this is distBotToCircle metres from the robot's origin 
      along its yaw*/
           
      /*horizontal offset from robot origin*/
      circleX = distBotToCircle*cos(robotYaw);
           
      /*vertical offset from robot origin*/
      circleY = distBotToCircle*sin(robotYaw);
           
      //find actual centre relative to simulation.
      circleX = robotX + circleX;
      circleY = robotY + circleY;
           
      /* to find which items are within this circle we
      find their Euclidian distance to the circle centre.
      Find the closest one and if it's distance is smaller than
      the circle radius then return its index */
      
      double smallestDist = 1000000;
      int closestItem = 0; 
      int i;
      
      for(i=0; i<listLength; i++)
      {
            double x, y, dist; 
            
            // get manhattan distance from circle centre to item
            x = circleX - itemList[i].x;
            y = circleY - itemList[i].y;
            
            //find euclidian distance from circle centre to item
            dist = (x*x) + (y*y);
            dist = sqrt(dist);
                        
            if(dist < smallestDist)
            {
                  smallestDist = dist;
                  closestItem = i;
            }
      }
 
      if(smallestDist > (radius + distBotToCircle))
      {
      	printf("no objects were close enough, false alarm!\n");
      	return -1;
      }   
        
      return closestItem;
}



int main(int argc, char *argv[])
{	
      /*need to do this line in c++ only*/
      using namespace PlayerCc;
	
      PlayerClient    robot("localhost", 6665);

      Position2dProxy p2dProxy(&robot,0);
      RangerProxy      sonarProxy(&robot,0);
      BlobfinderProxy blobProxy(&robot,0);
      RangerProxy      laserProxy(&robot,1);
      SimulationProxy simProxy(&robot,0);
      
      double forwardSpeed, turnSpeed;
      item_t itemList[8];
	
      RefreshItemList(itemList, simProxy);
	
      srand(time(NULL));
	
      //enable motors
      p2dProxy.SetMotorEnable(1);

      //request geometries
      p2dProxy.RequestGeom();
      sonarProxy.RequestGeom();
      laserProxy.RequestGeom();
      laserProxy.RequestConfigure();
      //blobfinder doesn't have geometry
	
      while(true)
      {		
            // read from the proxies
            robot.Read();
		
            if(blobProxy.GetCount() == 0)
            {
                  //wander
                  printf("wandering\n");
                  Wander(&forwardSpeed, &turnSpeed);
            }
            else
            {
                  //move towards the item
                  printf("moving to item\n");
                  MoveToItem(&forwardSpeed, &turnSpeed, blobProxy);
            }
      	
      	
            if(laserProxy.GetRangeCount() >= 90 && laserProxy[90] < 0.25)
            {
                  int destroyThis;

                  destroyThis = FindItem(itemList, 8, simProxy);
                  
                  if(destroyThis != -1)
                  {
		            //move it out of the simulation
		            printf("collecting item\n");
		            simProxy.SetPose2d(itemList[destroyThis].name, -10, -10, 0);
		            RefreshItemList(itemList, simProxy);
		      }
            }
            //avoid obstacles
            AvoidObstacles(&forwardSpeed, &turnSpeed, sonarProxy);
		
            //set motors
            p2dProxy.SetSpeed(forwardSpeed, dtor(turnSpeed));
            sleep(1);
      }
}

