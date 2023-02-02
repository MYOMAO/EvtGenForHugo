#ifndef MACRO_G4BLACKHOLE_C
#define MACRO_G4BLACKHOLE_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>
#include <g4detectors/PHG4BlockSubsystem.h>

R__LOAD_LIBRARY(libg4detectors.so)

namespace Enable
{
  bool BLACKHOLE = false;
  bool BLACKHOLE_SAVEHITS = true;
  bool BLACKHOLE_FORWARD_SAVEHITS = true;
}  // namespace Enable

void BlackHoleInit() {}

void BlackHole(PHG4Reco *g4Reco, double radius)
{
  std::cout << "BLACK HOLE ZZ BRO" << std::endl;
  bool addEndcaps = true;

  g4Reco->set_field(0);  // field off
  g4Reco->SetWorldMaterial("G4_Galactic");
  g4Reco->SetWorldSizeX(500.);
  g4Reco->SetWorldSizeZ(500.);
  g4Reco->SetWorldSizeY(500.);
  g4Reco->save_DST_geometry(false);  // no geometry -> crashes when trying to save it
//  g4Reco->set_decayer_active(false);

  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("BH", 0);
  cyl->set_double_param("radius", 100.);
/*
  if(-1 >0)
    {
      cyl->set_string_param("material", "G4_Galactic");
      cyl->BlackHole();
    }
  else cyl->set_string_param("material", "G4_POLYSTYRENE");
*/
  // cyl->BlackHole();
  cyl->set_string_param("material", "G4_Galactic");
  cyl->set_double_param("thickness", 2);
  cyl->set_double_param("length", 100);

  cyl->BlackHole();
  cyl->SetActive();
  cyl->OverlapCheck(true);
 
  g4Reco->registerSubsystem(cyl);


  if(addEndcaps)
    {
      PHG4BlockSubsystem *box = new PHG4BlockSubsystem("Rightside1");
      box->set_double_param("size_x",210);
      box->set_double_param("size_y",210.);
      box->set_double_param("size_z",2);
      box->set_double_param("place_x",0.);
      box->set_double_param("place_y",0.);
      box->set_double_param("place_z",52.);
      box->set_double_param("rot_x",180.);
      box->set_double_param("rot_y",180.);
      box->set_double_param("rot_z",180.);
 /*
	  if(-1 >0)
	{
	  box ->set_string_param("material", "G4_Galactic"); 
	  box -> BlackHole();
	}
 
	else box->set_string_param("material","G4_POLYSTYRENE"); // material of box
	//  box -> BlackHole();    
	  box->SetActive(); // it is an active volume - save G4Hits
 */
	  box->BlackHole();    
	  box->SetActive(); // it is an active volume - save G4Hits
	  box->OverlapCheck(true);
	 
	  g4Reco->registerSubsystem(box);

      PHG4BlockSubsystem *box2 = new PHG4BlockSubsystem("Leftside1");
      box2->set_double_param("size_x",210);
      box2->set_double_param("size_y",210.);
      box2->set_double_param("size_z",2);
      box2->set_double_param("place_x",0.);
      box2->set_double_param("place_y",0.);
      box2->set_double_param("place_z",-52.);
      box2->set_double_param("rot_x",180.);
      box2->set_double_param("rot_y",180.);
      box2->set_double_param("rot_z",180.);
/*
	  if(-1 >0)
	{
	  box2 ->set_string_param("material", "G4_Galactic"); 
	  box2 -> BlackHole();
	}
      else box2->set_string_param("material","G4_POLYSTYRENE"); // material of box
	//  box2 -> BlackHole();
	  box2->SetActive(); // it is an active volume - save G4Hits

	  g4Reco->registerSubsystem(box2);
  */
	  box2->BlackHole();    
	  box2->SetActive(); // it is an active volume - save G4Hits
	  box2->OverlapCheck(true);
	
	  g4Reco->registerSubsystem(box2);

	}


 


  return;
}
#endif
