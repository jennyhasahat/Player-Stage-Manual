# bigbob.inc
# model for the robot "Bigbob"
# Author: Jennifer Owen
# Date: 16/04/2010
# Updated for Stage4 KNickels 1 Aug 2013

define bigbobs_sonar sensor
(
    # define the size of each transducer [xsize ysize zsize] in meters
    size [0.01 0.05 0.01 ] 
    # define the range bounds [min max]
    range [0.3 2.0]
    # define the angular field of view (by default in degrees)
    fov 10
    # define the color that ranges are drawn in the gui
    color_rgba [ 0 1 0 0.5] 
    # how many rays in a beam
    samples 180
)

define bigbobs_sonars ranger
( 
  # one line for each sonar [xpos ypos zpos heading]
  bigbobs_sonar( pose [ 0.75 0.1875 0 0])
  bigbobs_sonar( pose [ 0.75 -0.1875 0 0])
  bigbobs_sonar( pose [ 0.25 0.5 0 30])
  bigbobs_sonar( pose [ 0.25 -0.5 0 -30])
)

define bigbobs_eyes blobfinder
(
	colors_count 2
	colors ["orange" "DarkBlue"]
	image [160 120] # resolution
	range 5.00 		 # meters
	fov 60 # fov (in unit_angle (degrees by def))
	)

define bigbobs_one_laser sensor
(
    # define the size of each transducer [xsize ysize zsize] in meters
    size [0.025 0.025 0.025]
    # define the range bounds [min max]
    range [0 0.25]
    # define the angular field of view (by default in degrees)
    fov 20
    # define the color that ranges are drawn in the gui
    color_rgba [ 1 0 0 0.5] 
    samples 180
)

define bigbobs_laser ranger
( 
  # one line for each laser [xpos ypos zpos heading]
  bigbobs_one_laser( pose [ 0.625 0.125 -0.975 270 ])
)

define bigbob position
(
		  # actual size
		  size [1.25 1 1]
		  # centre of rotation offset
		  origin [0.125 0 0 0]

		  # the shape of Bigbob
		  block
		  (
					 points 6
					 point[5] [0 0]
					 point[4] [0 1]
					 point[3] [0.75 1]
					 point[2] [1 0.75]
					 point[1] [1 0.25]
					 point[0] [0.75 0]
					 z [0 1]
		  )
		  # top tooth
		  block
		  (
					 points 4
					 point[3] [1 0.75]
					 point[2] [1.25 0.75]
					 point[1] [1.25 0.625]
					 point[0] [1 0.625]
					 z [0 0.5]
		  )
		  # bottom tooth
		  block
		  (
					 points 4
					 point[3] [1 0.375]
					 point[2] [1.25 0.375]
					 point[1] [1.25 0.25]
					 point[0] [1 0.25]
					 z [0 0.5]
		  )
		  #positional things
		  drive "diff"

		  #sensors attached to bigbob
		  bigbobs_sonars( ) 
          bigbobs_eyes( )
          bigbobs_laser( )
)
