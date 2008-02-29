//Framework Headers
#include "FWCore/Utilities/interface/Exception.h"

//CMSSW Headers
#include "DataFormats/GeometrySurface/interface/Surface.h"
#include "DataFormats/GeometrySurface/interface/BoundCylinder.h"
#include "DataFormats/GeometrySurface/interface/SimpleCylinderBounds.h"
#include "DataFormats/GeometrySurface/interface/BoundDisk.h"
#include "DataFormats/GeometrySurface/interface/SimpleDiskBounds.h"

// Tracker/Tracking Headers
#include "RecoTracker/TkDetLayers/interface/GeometricSearchTracker.h"
#include "TrackingTools/PatternTools/interface/MediumProperties.h"
//#include "TrackingTools/DetLayers/interface/DetLayer.h"
#include "TrackingTools/DetLayers/interface/BarrelDetLayer.h"
#include "TrackingTools/DetLayers/interface/ForwardDetLayer.h"

//FAMOS Headers
#include "FastSimulation/TrackerSetup/interface/TrackerInteractionGeometry.h"

#include<iostream>

TrackerInteractionGeometry::TrackerInteractionGeometry(const GeometricSearchTracker* theGeomSearchTracker)
{
 
  // Check that the Reco Tracker Geometry has been loaded
  if ( !theGeomSearchTracker ) 
    throw cms::Exception("FastSimulation/TrackerInteractionGeometry") 
      << "The pointer to the GeometricSearchTracker was not set"; 

  // The vector of Barrel Tracker Layers 
  std::vector< BarrelDetLayer*> barrelLayers = 
    theGeomSearchTracker->barrelLayers();

  // The vector of Forward Tracker Layers (positive z)
  std::vector< ForwardDetLayer*>  posForwardLayers = 
    theGeomSearchTracker->posForwardLayers();

  // Local pointers
  BoundCylinder* theCylinder;
  BoundDisk* theDisk;

  // Fraction of radiation length : had oc values to account 
  // for detectors, cables, support, ...
  // Note : the second argument is not used in FAMOS
  // Note : the first argument is tuned to reproduce the CMSIM material
  //        in terms or radiation length.

  // The Beam pipe
  _theMPBeamPipe = new MediumProperties(0.0038,0.0001);  
  // The pixel barrel layers
  _theMPPixelBarrel = new MediumProperties(0.0222,0.0001);  
  // The pixel endcap disks
  _theMPPixelEndcap = new MediumProperties(0.044,0.0001);  
  // The various cables thicknesses for each layer / disks
  _theMPPixelOutside1 = new MediumProperties(0.100,0.0001);  
  _theMPPixelOutside2 = new MediumProperties(0.040,0.0001);  
  _theMPPixelOutside3 = new MediumProperties(0.030,0.0001);  
  _theMPPixelOutside4 = new MediumProperties(0.040,0.0001);  
  _theMPPixelOutside  = new MediumProperties(0.025,0.0001);  
  _theMPPixelOutside5 = new MediumProperties(0.023,0.0001);  
  _theMPPixelOutside6 = new MediumProperties(0.085,0.0001);  
  // The tracker inner barrel layers
  _theMPTIB1 = new MediumProperties(0.060,0.0001);  
  _theMPTIB2 = new MediumProperties(0.047,0.0001);  
  _theMPTIB3 = new MediumProperties(0.035,0.0001);  
  _theMPTIB4 = new MediumProperties(0.033,0.0001);  
  // The tracker outer barrel layers
  _theMPTOB1 = new MediumProperties(0.044,0.0001);  
  _theMPTOB2 = new MediumProperties(0.044,0.0001);  
  _theMPTOB3 = new MediumProperties(0.033,0.0001);  
  _theMPTOB4 = new MediumProperties(0.033,0.0001);  
  _theMPTOB5 = new MediumProperties(0.033,0.0001);  
  _theMPTOB6 = new MediumProperties(0.033,0.0001);  
  // The tracker inner disks
  _theMPInner = new MediumProperties(0.050,0.0001);  
  // The tracker endcap disks
  _theMPEndcap = new MediumProperties(0.041,0.0001);  
  // Various cable thicknesses 
  _theMPTOBBInside = new MediumProperties(0.017,0.0001);  
  _theMPTIBEOutside = new MediumProperties(0.040,0.0001);  
  _theMPTIDEOutside = new MediumProperties(0.070,0.0001);  
  _theMPTOBEOutside = new MediumProperties(0.090,0.0001);  
  _theMPBarrelOutside = new MediumProperties(0.100,0.0001);  
  _theMPEndcapOutside = new MediumProperties(0.260,0.0001);  
  _theMPEndcapOutside2 = new MediumProperties(0.080,0.0001);  

  // Create the nest of cylinders
  const Surface::PositionType thePosition(0.,0.,0.);
  const Surface::RotationType theRotation(1.,0.,0.,0.,1.,0.,0.,0.,1.);
  // Beam Pipe
  //  const SimpleCylinderBounds  PIPE( 0.997,   1.003,  -300., 300.);
  const SimpleCylinderBounds  PIPE( 2.997,   3.003,  -26.4, 26.4);

  // Take the active layer position from the Tracker Reco Geometry
  // Pixel barrel

  std::vector< BarrelDetLayer*>::const_iterator bl = barrelLayers.begin();

  double maxLength = (**bl).specificSurface().bounds().length()/2.;
  double maxRadius = (**bl).specificSurface().radius()+0.01;
  // First pixel barrel layer: r=4.41058, l=53.38
  const SimpleCylinderBounds  PIXB1( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);

  // "Cables" 
  const SimpleDiskBounds PIXBOut1(3.6,maxRadius+0.01,-0.5,0.5);
  const Surface::PositionType PPIXBOut1(0.0,0.0,maxLength);

  // Second pixel barrel layer: r=7.30732, l=53.38
  ++bl;
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2., maxLength+0.000 );
  maxRadius = (**bl).specificSurface().radius();
  const SimpleCylinderBounds  PIXB2( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);

  // "Cables"
  const SimpleDiskBounds PIXBOut2(6.1,maxRadius+0.005,-0.5,0.5);
  const Surface::PositionType PPIXBOut2(0.0,0.0,maxLength);

  // More cables
  ++bl;
  maxRadius = (**bl).specificSurface().radius();
  const SimpleDiskBounds PIXBOut3(8.5,maxRadius,-0.5,0.5);
  const Surface::PositionType PPIXBOut3(0.0,0.0,maxLength);

  // Third pixel barrel layer: r=10.1726, l=53.38
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2., maxLength+0.000 );
  const SimpleCylinderBounds  PIXB3( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);

  bl++;
  // Stack 1 pixel barrel layer
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2., maxLength+0.000 );
  maxRadius = (**bl).specificSurface().radius();
  const SimpleCylinderBounds  PIXBS1Down( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);
  maxRadius=maxRadius + 0.2;
  const SimpleCylinderBounds  PIXBS1Up( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);

  bl++;
  // Stack 2 pixel barrel layer
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2., maxLength+0.000 );
  maxRadius = (**bl).specificSurface().radius();
  const SimpleCylinderBounds  PIXBS2Down( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);
  maxRadius=maxRadius + 0.2;
  const SimpleCylinderBounds  PIXBS2Up( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);

  bl++;
  // Stack 3 pixel barrel layer
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2., maxLength+0.000 );
  maxRadius = (**bl).specificSurface().radius();
  const SimpleCylinderBounds  PIXBS3Down( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);
  maxRadius=maxRadius + 0.2;
  const SimpleCylinderBounds  PIXBS3Up( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);

  bl++;
  // Stack 4 pixel barrel layer
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2., maxLength+0.000 );
  maxRadius = (**bl).specificSurface().radius();
  const SimpleCylinderBounds  PIXBS4Down( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);
  maxRadius=maxRadius + 0.2;
  const SimpleCylinderBounds  PIXBS4Up( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);

  bl++;
  // Stack 5 pixel barrel layer
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2., maxLength+0.000 );
  maxRadius = (**bl).specificSurface().radius();
  const SimpleCylinderBounds  PIXBS5Down( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);
  maxRadius=maxRadius + 0.2;
  const SimpleCylinderBounds  PIXBS5Up( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);

  bl++;
  // Stack 6 pixel barrel layer
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2., maxLength+0.000 );
  maxRadius = (**bl).specificSurface().radius();
  const SimpleCylinderBounds  PIXBS6Down( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);
  maxRadius=maxRadius + 0.2;
  const SimpleCylinderBounds  PIXBS6Up( maxRadius-0.005, maxRadius+0.005, -maxLength, +maxLength);

/*
  // Pixel Barrel Outside walls and cables
  const SimpleDiskBounds PIXBOut4(11.9,15.5,-0.5,0.5);
  const Surface::PositionType PPIXBOut4(0.0,0.0,27.999);

  const SimpleDiskBounds PIXBOut(3.8,16.5,-0.5,0.5);
  const Surface::PositionType PPIXBOut(0.0,0.0,28.0);

  const SimpleCylinderBounds  PIXBOut5( 17.0, 17.2, -64.8, 64.8);

  const SimpleDiskBounds PIXBOut6(3.0,17.3,-0.5,0.5);
  const Surface::PositionType PPIXBOut6(0.0,0.0,64.9);
*/

  // Tracker Inner Barrel : thin detectors (300 microns)
/*
  // First TIB layer: r=25.6786, l=130.04
  ++bl;
  maxRadius = (**bl).specificSurface().radius();
  maxLength = (**bl).specificSurface().bounds().length()/2.+7.0;
  const SimpleCylinderBounds  TIB1( maxRadius-0.0150, maxRadius+0.0150, -maxLength, +maxLength);

  // Second TIB layer: r=34.0341, l=131.999
  ++bl;
  maxRadius = (**bl).specificSurface().radius();
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2.+7.0, maxLength+0.000 );
  const SimpleCylinderBounds  TIB2( maxRadius-0.0150, maxRadius+0.0150, -maxLength, +maxLength);
  // Third TIB layer: r=41.9599, l=131.628  !!!! Needs to be larger than TIB2
  ++bl;
  maxRadius = (**bl).specificSurface().radius();
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2.+7.0, maxLength+0.000 );
  const SimpleCylinderBounds  TIB3( maxRadius-0.0150, maxRadius+0.0150, -maxLength, +maxLength);
  // Fourth TIB layer: r=49.8924, l=132.78
  ++bl;
  maxRadius = (**bl).specificSurface().radius();
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2.+7.0, maxLength+0.000 );
  const SimpleCylinderBounds  TIB4( maxRadius-0.0150, maxRadius+0.0150, -maxLength, +maxLength);

  // Inner Barrel Cylinder & Ends : Cables and walls
  const SimpleDiskBounds TIBEOut(22.5,53.9,-0.05,0.05);
  const Surface::PositionType PTIBEOut(0.0,0.0,75.001);

  const SimpleDiskBounds TIBEOut2(35.5,53.901,-0.05,0.05);
  const Surface::PositionType PTIBEOut2(0.0,0.0,75.001);

  // Inner Tracker / Outer Barrel Wall
  const SimpleCylinderBounds  TOBCIn ( 54.0, 55.0,-108.2,108.2);

*/
/*
  // First TOB layer: r=60.7671, l=216.576
  ++bl;
  maxRadius = (**bl).specificSurface().radius();
  maxLength = (**bl).specificSurface().bounds().length()/2.+0.0;
  const SimpleCylinderBounds  TOB1( maxRadius-0.0150, maxRadius+0.0150, -maxLength, +maxLength);

  // Second TOB layer: r=69.3966, l=216.576
  ++bl;
  maxRadius = (**bl).specificSurface().radius();
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2.+0.0, maxLength+0.000 );
  const SimpleCylinderBounds  TOB2( maxRadius-0.0150, maxRadius+0.0150, -maxLength, +maxLength);
  // Third TOB layer: r=78.0686, l=216.576
  ++bl;
  maxRadius = (**bl).specificSurface().radius();
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2.+0.0, maxLength+0.000 );
  const SimpleCylinderBounds  TOB3( maxRadius-0.0150, maxRadius+0.0150, -maxLength, +maxLength);
  // Fourth TOB layer: r=86.8618, l=216.576
  ++bl;
  maxRadius = (**bl).specificSurface().radius();
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2.+0.0, maxLength+0.000 );
  const SimpleCylinderBounds  TOB4( maxRadius-0.0150, maxRadius+0.0150, -maxLength, +maxLength);
  // Fifth TOB layer: r=96.5557, l=216.576
  ++bl;
  maxRadius = (**bl).specificSurface().radius();
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2.+0.0, maxLength+0.000 );
  const SimpleCylinderBounds  TOB5( maxRadius-0.0150, maxRadius+0.0150, -maxLength, +maxLength);
  // Sixth TOB layer: r=108.05, l=216.576
  ++bl;
  maxRadius = (**bl).specificSurface().radius();
  maxLength = std::max( (**bl).specificSurface().bounds().length()/2.+0.0, maxLength+0.000 );
  const SimpleCylinderBounds  TOB6( maxRadius-0.0150, maxRadius+0.0150, -maxLength, +maxLength);

  const SimpleDiskBounds TOBEOut(55.0,109.5,-0.5,0.5);
  const Surface::PositionType PTOBEOut(0.0,0.0,110.0);
*/

  const Surface::RotationType theRotation2(1.,0.,0.,0.,1.,0.,0.,0.,1.);

  // Outside : Barrel
  const SimpleCylinderBounds  TBOut ( 119.0, 120.0,-299.9,299.9);

  // And now the disks...
  std::vector< ForwardDetLayer*>::const_iterator fl = posForwardLayers.begin();

  // Pixel disks 
  // First Pixel disk: Z pos 35.5 radii 5.42078, 16.0756
  double innerRadius = (**fl).specificSurface().innerRadius()-1.0;
  double outerRadius = (**fl).specificSurface().outerRadius()+2.0+0.5;
  const SimpleDiskBounds PIXD1(innerRadius, outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PPIXD1(0.0,0.0,(**fl).surface().position().z()); 
  // Second Pixel disk: Z pos 48.5 radii 5.42078, 16.0756
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-1.0;
  outerRadius = std::max( (**fl).specificSurface().outerRadius()+2.0, outerRadius+0.000 );
  const SimpleDiskBounds PIXD2(innerRadius, outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PPIXD2(0.0,0.0,(**fl).surface().position().z()); 
/*
  // Tracker Inner disks (add 3 cm for the outer radius to simulate cables, 
  // and remove 1cm to inner radius to allow for some extrapolation margin)
  // First TID : Z pos 78.445 radii 23.14, 50.4337
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-2.0;
  outerRadius = (**fl).specificSurface().outerRadius()+3.5;
  const SimpleDiskBounds TID1(innerRadius,outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PTID1(0.,0.,(**fl).surface().position().z()); 
  // Second TID : Z pos 90.445 radii 23.14, 50.4337
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-2.0;
  outerRadius = std::max( (**fl).specificSurface().outerRadius()+3.5, outerRadius+0.000);
  const SimpleDiskBounds TID2(innerRadius,outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PTID2(0.,0.,(**fl).surface().position().z()); 
  // Third TID : Z pos 105.445 radii 23.14, 50.4337
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-2.0;
  outerRadius = std::max( (**fl).specificSurface().outerRadius()+3.5, outerRadius+0.000);
  const SimpleDiskBounds TID3(innerRadius,outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PTID3(0.,0.,(**fl).surface().position().z()); 

  // TID Wall and cables
  const SimpleDiskBounds TIDEOut(32.0,outerRadius,-0.5,0.5);
  const Surface::PositionType PTIDEOut(0.0,0.0,108.0);

*/

  // Tracker Endcaps : Add 11 cm to outer radius to correct for a bug, remove
  // 5cm to the inner radius (TEC7,8,9) to correct for a simular bug, and
  // remove other 2cm to inner radius to allow for some extrapolation margin
  // First TEC: Z pos 131.892 radii 23.3749, 99.1967
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-1.5;
  outerRadius = (**fl).specificSurface().outerRadius()+2.0;
  const SimpleDiskBounds TEC1(innerRadius,outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PTEC1(0.,0,(**fl).surface().position().z()); 

  // Second TEC: Z pos 145.892 radii 23.3749, 99.1967
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-1.5;
  outerRadius = std::max( (**fl).specificSurface().outerRadius()+2.0, outerRadius+0.000 );
  const SimpleDiskBounds TEC2(innerRadius,outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PTEC2(0.,0.,(**fl).surface().position().z());
  // Third TEC: Z pos 159.892 radii 23.3749, 99.1967
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-1.5;
  outerRadius = std::max( (**fl).specificSurface().outerRadius()+2.0, outerRadius+0.000 );
  const SimpleDiskBounds TEC3(innerRadius,outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PTEC3(0.,0.,(**fl).surface().position().z());
  // Fourth TEC: Z pos 173.892 radii 32.1263, 99.1967
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-2.5;
  outerRadius = std::max( (**fl).specificSurface().outerRadius()+2.0, outerRadius+0.000 );
  const SimpleDiskBounds TEC4(innerRadius,outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PTEC4(0.,0.,(**fl).surface().position().z());
  // Fifth TEC: Z pos 187.892 radii 32.1263, 99.1967
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-2.5;
  outerRadius = std::max( (**fl).specificSurface().outerRadius()+2.0, outerRadius+0.000 );
  const SimpleDiskBounds TEC5(innerRadius,outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PTEC5(0.,0.,(**fl).surface().position().z());
  // Sixth TEC: Z pos 205.392 radii 32.1263, 99.1967
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-2.5;
  outerRadius = std::max( (**fl).specificSurface().outerRadius()+2.0, outerRadius+0.000 );
  const SimpleDiskBounds TEC6(innerRadius,outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PTEC6(0.,0.,(**fl).surface().position().z());
  // Seventh TEC: Z pos 224.121 radii 44.7432, 99.1967
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-9.5;
  outerRadius = std::max( (**fl).specificSurface().outerRadius()+2.0, outerRadius+0.000 );
  const SimpleDiskBounds TEC7(innerRadius,outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PTEC7(0.,0.,(**fl).surface().position().z());

  // Eighth TEC: Z pos 244.621 radii 44.7432, 99.1967
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-9.5;
  outerRadius = std::max( (**fl).specificSurface().outerRadius()+2.0, outerRadius+0.000 );
  const SimpleDiskBounds TEC8(innerRadius,outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PTEC8(0.,0.,(**fl).surface().position().z());
  // Nineth TEC: Z pos 266.121 radii 56.1781, 99.1967
  ++fl;
  innerRadius = (**fl).specificSurface().innerRadius()-20.5;
  outerRadius = std::max( (**fl).specificSurface().outerRadius()+2.0, outerRadius+0.000 );
  const SimpleDiskBounds TEC9(innerRadius,outerRadius,-0.0150,+0.0150);
  const Surface::PositionType PTEC9(0.,0.,(**fl).surface().position().z());

  // Outside : Endcap
  const SimpleDiskBounds TEOut(6.0,120.001,-0.5,0.5);
  const Surface::PositionType PTEOut(0.0,0.0,300.0);

  const SimpleDiskBounds TEOut2(70.0,120.001,-0.5,0.5);
  const Surface::PositionType PTEOut2(0.0,0.0,300.0);

  // The ordering of disks and cylinders is essential here
  // (from inside to outside)
  // Do not change it thoughtlessly.


  // Beam Pipe

  theCylinder = new BoundCylinder(thePosition,theRotation,PIPE);
  theCylinder->setMediumProperties(_theMPBeamPipe);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,100));

  // Pixels 

  theCylinder = new BoundCylinder(thePosition,theRotation,PIXB1);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,1,0.03));

  theDisk = new BoundDisk(PPIXBOut1,theRotation2,PIXBOut1);
  theDisk->setMediumProperties(_theMPPixelOutside1);
  _theCylinders.push_back(TrackerLayer(theDisk,true,101));

  theCylinder = new BoundCylinder(thePosition,theRotation,PIXB2);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,2,0.03));

  theDisk = new BoundDisk(PPIXBOut2,theRotation2,PIXBOut2);
  theDisk->setMediumProperties(_theMPPixelOutside2);
  _theCylinders.push_back(TrackerLayer(theDisk,true,102));

  theDisk = new BoundDisk(PPIXBOut3,theRotation2,PIXBOut3);
  theDisk->setMediumProperties(_theMPPixelOutside3);
  _theCylinders.push_back(TrackerLayer(theDisk,true,103));

  theCylinder = new BoundCylinder(thePosition,theRotation,PIXB3);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,3,0.03));

  theDisk = new BoundDisk(PPIXD1,theRotation2,PIXD1);
  theDisk->setMediumProperties(_theMPPixelEndcap);
  _theCylinders.push_back(TrackerLayer(theDisk,true,4,0.03));
  
  theDisk = new BoundDisk(PPIXD2,theRotation2,PIXD2);
  theDisk->setMediumProperties(_theMPPixelEndcap);
  _theCylinders.push_back(TrackerLayer(theDisk,true,5,0.03));

  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBS1Down);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,28,0.03));

  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBS1Up);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,29,0.03));

  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBS2Down);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,30,0.03));
  
  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBS2Up);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,31,0.03));

  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBS3Down);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,32,0.03));
  
  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBS3Up);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,33,0.03));

  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBS4Down);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,34,0.03));
  
  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBS4Up);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,35,0.03));
  
  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBS5Down);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,36,0.03));
  
  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBS5Up);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,37,0.03));

  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBS6Down);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,38,0.03));
  
  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBS6Up);
  theCylinder->setMediumProperties(_theMPPixelBarrel);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,39,0.03));

/*
  theDisk = new BoundDisk(PPIXBOut4,theRotation2,PIXBOut4);
  theDisk->setMediumProperties(_theMPPixelOutside4);
  _theCylinders.push_back(TrackerLayer(theDisk,true,104));

  theDisk = new BoundDisk(PPIXBOut,theRotation2,PIXBOut);
  theDisk->setMediumProperties(_theMPPixelOutside);
  _theCylinders.push_back(TrackerLayer(theDisk,true,105));

*/

/*
  theCylinder = new BoundCylinder(thePosition,theRotation,PIXBOut5);
  theCylinder->setMediumProperties(_theMPPixelOutside5);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,106));

  theDisk = new BoundDisk(PPIXBOut6,theRotation2,PIXBOut6);
  theDisk->setMediumProperties(_theMPPixelOutside6);
  _theCylinders.push_back(TrackerLayer(theDisk,true,107));
*/

  // Inner Barrel 
/*
  theCylinder = new BoundCylinder(thePosition,theRotation,TIB1);
  theCylinder->setMediumProperties(_theMPTIB1);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,6,0.032));

  theCylinder = new BoundCylinder(thePosition,theRotation,TIB2);
  theCylinder->setMediumProperties(_theMPTIB2);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,7,0.032));

  theCylinder = new BoundCylinder(thePosition,theRotation,TIB3);
  theCylinder->setMediumProperties(_theMPTIB3);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,8,0.032));

  theCylinder = new BoundCylinder(thePosition,theRotation,TIB4);
  theCylinder->setMediumProperties(_theMPTIB4);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,9,0.032));

  theDisk = new BoundDisk(PTIBEOut,theRotation2,TIBEOut);
  theDisk->setMediumProperties(_theMPTIBEOutside);
  _theCylinders.push_back(TrackerLayer(theDisk,true,108));

  theDisk = new BoundDisk(PTIBEOut2,theRotation2,TIBEOut2);
  theDisk->setMediumProperties(_theMPTIBEOutside);
  _theCylinders.push_back(TrackerLayer(theDisk,true,109));
*/

/*  
  // Inner Endcaps

  theDisk = new BoundDisk(PTID1,theRotation2,TID1);
  theDisk->setMediumProperties(_theMPInner);
  _theCylinders.push_back(TrackerLayer(theDisk,10,0.032,1,3));

  theDisk = new BoundDisk(PTID2,theRotation2,TID2);
  theDisk->setMediumProperties(_theMPInner);
  _theCylinders.push_back(TrackerLayer(theDisk,11,0.032,1,3));

  theDisk = new BoundDisk(PTID3,theRotation2,TID3);
  theDisk->setMediumProperties(_theMPInner);
  _theCylinders.push_back(TrackerLayer(theDisk,12,0.032,1,3));

  theDisk = new BoundDisk(PTIDEOut,theRotation2,TIDEOut);
  theDisk->setMediumProperties(_theMPTIDEOutside);
  _theCylinders.push_back(TrackerLayer(theDisk,true,110));
*/

  // Outer Barrel 
/*
  theCylinder = new BoundCylinder(thePosition,theRotation,TOBCIn);
  theCylinder->setMediumProperties(_theMPTOBBInside);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,111));

  theCylinder = new BoundCylinder(thePosition,theRotation,TOB1);
  theCylinder->setMediumProperties(_theMPTOB1);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,13,0.05));

  theCylinder = new BoundCylinder(thePosition,theRotation,TOB2);
  theCylinder->setMediumProperties(_theMPTOB2);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,14,0.05));

  theCylinder = new BoundCylinder(thePosition,theRotation,TOB3);
  theCylinder->setMediumProperties(_theMPTOB3);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,15,0.05));

  theCylinder = new BoundCylinder(thePosition,theRotation,TOB4);
  theCylinder->setMediumProperties(_theMPTOB4);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,16,0.05));

  theCylinder = new BoundCylinder(thePosition,theRotation,TOB5);
  theCylinder->setMediumProperties(_theMPTOB5);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,17,0.05));

  theCylinder = new BoundCylinder(thePosition,theRotation,TOB6);
  theCylinder->setMediumProperties(_theMPTOB6);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,18,0.05));

  theDisk = new BoundDisk(PTOBEOut,theRotation2,TOBEOut);
  theDisk->setMediumProperties(_theMPTOBEOutside);
  _theCylinders.push_back(TrackerLayer(theDisk,true,112));
*/

  // Outer Endcaps

  theDisk = new BoundDisk(PTEC1,theRotation2,TEC1);
  theDisk->setMediumProperties(_theMPEndcap);
  _theCylinders.push_back(TrackerLayer(theDisk,19,0.032,1,7));

  theDisk = new BoundDisk(PTEC2,theRotation2,TEC2);
  theDisk->setMediumProperties(_theMPEndcap);
  _theCylinders.push_back(TrackerLayer(theDisk,20,0.032,1,7));

  theDisk = new BoundDisk(PTEC3,theRotation2,TEC3);
  theDisk->setMediumProperties(_theMPEndcap);
  _theCylinders.push_back(TrackerLayer(theDisk,21,0.032,1,7));

  theDisk = new BoundDisk(PTEC4,theRotation2,TEC4);
  theDisk->setMediumProperties(_theMPEndcap);
  _theCylinders.push_back(TrackerLayer(theDisk,22,0.032,2,7));

  theDisk = new BoundDisk(PTEC5,theRotation2,TEC5);
  theDisk->setMediumProperties(_theMPEndcap);
  _theCylinders.push_back(TrackerLayer(theDisk,23,0.032,2,7));

  theDisk = new BoundDisk(PTEC6,theRotation2,TEC6);
  theDisk->setMediumProperties(_theMPEndcap);
  _theCylinders.push_back(TrackerLayer(theDisk,24,0.032,2,7));

  theDisk = new BoundDisk(PTEC7,theRotation2,TEC7);
  theDisk->setMediumProperties(_theMPEndcap);
  _theCylinders.push_back(TrackerLayer(theDisk,25,0.032,3,7));

  theDisk = new BoundDisk(PTEC8,theRotation2,TEC8);
  theDisk->setMediumProperties(_theMPEndcap);
  _theCylinders.push_back(TrackerLayer(theDisk,26,0.032,3,7));

  theDisk = new BoundDisk(PTEC9,theRotation2,TEC9);
  theDisk->setMediumProperties(_theMPEndcap);
  _theCylinders.push_back(TrackerLayer(theDisk,27,0.032,4,7));

  // Tracker Outside

  theCylinder = new BoundCylinder(thePosition,theRotation,TBOut);
  theCylinder->setMediumProperties(_theMPBarrelOutside);
  _theCylinders.push_back(TrackerLayer(theCylinder,false,113));

  theDisk = new BoundDisk(PTEOut,theRotation2,TEOut);
  theDisk->setMediumProperties(_theMPEndcapOutside);
  _theCylinders.push_back(TrackerLayer(theDisk,true,114));

  theDisk = new BoundDisk(PTEOut2,theRotation2,TEOut2);
  theDisk->setMediumProperties(_theMPEndcapOutside2);
  _theCylinders.push_back(TrackerLayer(theDisk,true,115));

  // Check overall compatibility of cylinder dimensions
  // (must be nested cylinders)
  // Throw an exception if the test fails
  double zin, rin;
  double zout, rout;
  unsigned nCyl=0;
  std::list<TrackerLayer>::iterator cyliterOut=cylinderBegin();
  // Inner cylinder dimensions
  if ( cyliterOut->forward() ) {
    zin = cyliterOut->disk()->position().z();
    rin = cyliterOut->disk()->outerRadius();
  } else {
    zin = cyliterOut->cylinder()->bounds().length()/2.;
    rin = cyliterOut->cylinder()->bounds().width()/2.;
  }
  // Go to the next cylinder
  ++cyliterOut;
  // And loop over all cylinders
  while ( cyliterOut != cylinderEnd() ) {
    // Outer cylinder dimensions
    if ( cyliterOut->forward() ) {
      zout = cyliterOut->disk()->position().z();
      rout = cyliterOut->disk()->outerRadius();
    } else {
      zout = cyliterOut->cylinder()->bounds().length()/2.;
      rout = cyliterOut->cylinder()->bounds().width()/2.;
    }

    nCyl++;
    if ( zout < zin || rout < rin ) { 
      throw cms::Exception("FastSimulation/TrackerInteractionGeometry ") 
	<< " WARNING with cylinder number " << nCyl 
	<< " (Active Layer Number = " <<  cyliterOut->layerNumber() 
	<< " Forward ? " <<  cyliterOut->forward() << " ) "
	<< " has dimensions smaller than previous cylinder : " << std::endl
	<< " zout/zin = " << zout << " " << zin << std::endl
	<< " rout/rin = " << rout << " " << rin << std::endl;
    } else {
      
      std::cout << " Cylinder number " << nCyl 
		<< " (Active Layer Number = " <<  cyliterOut->layerNumber() 
		<< " Forward ? " <<  cyliterOut->forward() << " ) "
		<< " has dimensions of : " 
		<< " zout = " << zout << "; " 
		<< " rout = " << rout << std::endl;
      
    }
    // Go to the next cylinder
    cyliterOut++;
    // Inner cylinder becomes outer cylinder
    zin = zout;
    rin = rout;
    // End test
  } 
    
}

TrackerInteractionGeometry::~TrackerInteractionGeometry()
{
  _theCylinders.clear();
  //  _theRings.clear();

  // The Beam pipe
  delete _theMPBeamPipe;
  // The pixel barrel layers
  delete _theMPPixelBarrel;
  // The pixel endcap disks
  delete _theMPPixelEndcap;
  // The various cables thicnesses for each layer / disks
  delete _theMPPixelOutside1;
  delete _theMPPixelOutside2;
  delete _theMPPixelOutside3;
  delete _theMPPixelOutside4;
  delete _theMPPixelOutside;
  delete _theMPPixelOutside5;
  delete _theMPPixelOutside6;
  // The tracker inner barrel layers
  delete _theMPTIB1;
  delete _theMPTIB2;
  delete _theMPTIB3;
  delete _theMPTIB4;
  // The tracker outer barrel layers
  delete _theMPTOB1;
  delete _theMPTOB2;
  delete _theMPTOB3;
  delete _theMPTOB4;
  delete _theMPTOB5;
  delete _theMPTOB6;
  // The tracker inner disks
  delete _theMPInner;
  // The tracker endcap disks
  delete _theMPEndcap;
  // Various cable thicknesses 
  delete _theMPTOBBInside;
  delete _theMPTIBEOutside;
  delete _theMPTIDEOutside;
  delete _theMPTOBEOutside;
  delete _theMPBarrelOutside;
  delete _theMPEndcapOutside;
  delete _theMPEndcapOutside2;
}
