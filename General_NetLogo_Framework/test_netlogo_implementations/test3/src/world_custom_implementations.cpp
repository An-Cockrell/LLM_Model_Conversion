// world_custom_implementations.cpp
#include "world.h"

std::shared_ptr<Turtle> World::create_inj_turtle(int x, int y, int id, int heading) {
   auto inj = std::make_shared<Turtle>(x, y, id, heading);
   inj->setLabel("inj");
   Patch& patch = get_patch(x, y);
   std::weak_ptr<Turtle> inj_weak_ptr = inj;

   all_injs.push_back(inj);
   all_turtles.push_back(inj_weak_ptr);
   patch.add_turtle(inj);

   return inj;
}

std::shared_ptr<Turtle> World::create_pmn_turtle(int x, int y, int id, int heading) {
   auto pmn = std::make_shared<Turtle>(x, y, id, heading);
   pmn->setLabel("pmn");
   Patch& patch = get_patch(x, y);
   std::weak_ptr<Turtle> pmn_weak_ptr = pmn;

   all_pmns.push_back(pmn);
   all_turtles.push_back(pmn_weak_ptr);
   patch.add_turtle(pmn);

   return pmn;
}

std::shared_ptr<Turtle> World::create_band_turtle(int x, int y, int id, int heading) {
   auto band = std::make_shared<Turtle>(x, y, id, heading);
   band->setLabel("band");
   Patch& patch = get_patch(x, y);
   std::weak_ptr<Turtle> band_weak_ptr = band;

   all_bands.push_back(band);
   all_turtles.push_back(band_weak_ptr);
   patch.add_turtle(band);

   return band;
}

std::shared_ptr<Turtle> World::create_pmn_marrow_turtle(int x, int y, int id, int heading) {
   auto pmn_marrow = std::make_shared<Turtle>(x, y, id, heading);
   pmn_marrow->setLabel("pmn-marrow");
   Patch& patch = get_patch(x, y);
   std::weak_ptr<Turtle> pmn_marrow_weak_ptr = pmn_marrow;

   all_pmn_marrows.push_back(pmn_marrow);
   all_turtles.push_back(pmn_marrow_weak_ptr);
   patch.add_turtle(pmn_marrow);

   return pmn_marrow;
}

std::shared_ptr<Turtle> World::create_mono_turtle(int x, int y, int id, int heading) {
   auto mono = std::make_shared<Turtle>(x, y, id, heading);
   mono->setLabel("mono");
   Patch& patch = get_patch(x, y);
   std::weak_ptr<Turtle> mono_weak_ptr = mono;

   all_monos.push_back(mono);
   all_turtles.push_back(mono_weak_ptr);
   patch.add_turtle(mono);

   return mono;
}

std::shared_ptr<Turtle> World::create_mono_marrow_turtle(int x, int y, int id, int heading) {
   auto mono_marrow = std::make_shared<Turtle>(x, y, id, heading);
   mono_marrow->setLabel("mono-marrow");
   Patch& patch = get_patch(x, y);
   std::weak_ptr<Turtle> mono_marrow_weak_ptr = mono_marrow;

   all_mono_marrows.push_back(mono_marrow);
   all_turtles.push_back(mono_marrow_weak_ptr);
   patch.add_turtle(mono_marrow);

   return mono_marrow;
}

std::shared_ptr<Turtle> World::create_gen_mono_marrow_turtle(int x, int y, int id, int heading) {
   auto gen_mono_marrow = std::make_shared<Turtle>(x, y, id, heading);
   gen_mono_marrow->setLabel("gen-mono-marrow");
   Patch& patch = get_patch(x, y);
   std::weak_ptr<Turtle> gen_mono_marrow_weak_ptr = gen_mono_marrow;

   all_gen_mono_marrows.push_back(gen_mono_marrow);
   all_turtles.push_back(gen_mono_marrow_weak_ptr);
   patch.add_turtle(gen_mono_marrow);

   return gen_mono_marrow;
}

std::shared_ptr<Turtle> World::create_th0_germ_turtle(int x, int y, int id, int heading) {
   auto th0_germ = std::make_shared<Turtle>(x, y, id, heading);
   th0_germ->setLabel("th0-germ");
   Patch& patch = get_patch(x, y);
   std::weak_ptr<Turtle> th0_germ_weak_ptr = th0_germ;

   all_th0_germs.push_back(th0_germ);
   all_turtles.push_back(th0_germ_weak_ptr);
   patch.add_turtle(th0_germ);

   return th0_germ;
}

std::shared_ptr<Turtle> World::create_th0_turtle(int x, int y, int id, int heading) {
   auto th0 = std::make_shared<Turtle>(x, y, id, heading);
   th0->setLabel("th0");
   Patch& patch = get_patch(x, y);
   std::weak_ptr<Turtle> th0_weak_ptr = th0;

   all_th0s.push_back(th0);
   all_turtles.push_back(th0_weak_ptr);
   patch.add_turtle(th0);

   return th0;
}

std::shared_ptr<Turtle> World::create_th1_turtle(int x, int y, int id, int heading) {
   auto th1 = std::make_shared<Turtle>(x, y, id, heading);
   th1->setLabel("th1");
   Patch& patch = get_patch(x, y);
   std::weak_ptr<Turtle> th1_weak_ptr = th1;

   all_th1s.push_back(th1);
   all_turtles.push_back(th1_weak_ptr);
   patch.add_turtle(th1);

   return th1;
}

std::shared_ptr<Turtle> World::create_th1_germ_turtle(int x, int y, int id, int heading) {
   auto th1_germ = std::make_shared<Turtle>(x, y, id, heading);
   th1_germ->setLabel("th1-germ");
   Patch& patch = get_patch(x, y);
   std::weak_ptr<Turtle> th1_germ_weak_ptr = th1_germ;

   all_th1_germs.push_back(th1_germ);
   all_turtles.push_back(th1_germ_weak_ptr);
   patch.add_turtle(th1_germ);

   return th1_germ;
}

std::shared_ptr<Turtle> World::create_th2_turtle(int x, int y, int id, int heading) {
  auto th2 = std::make_shared<Turtle>(x, y, id, heading);
  th2->setLabel("th2");
  Patch& patch = get_patch(x, y);
  std::weak_ptr<Turtle> th2_weak_ptr = th2;

  all_th2s.push_back(th2);
  all_turtles.push_back(th2_weak_ptr);
  patch.add_turtle(th2);

  return th2;
}

std::shared_ptr<Turtle> World::create_th2_germ_turtle(int x, int y, int id, int heading) {
  auto th2_germ = std::make_shared<Turtle>(x, y, id, heading);
  th2_germ->setLabel("th2-germ");
  Patch& patch = get_patch(x, y);
  std::weak_ptr<Turtle> th2_germ_weak_ptr = th2_germ;

  all_th2_germs.push_back(th2_germ);
  all_turtles.push_back(th2_germ_weak_ptr);
  patch.add_turtle(th2_germ);

  return th2_germ;
}

std::shared_ptr<Turtle> World::create_nk_turtle(int x, int y, int id, int heading) {
  auto nk = std::make_shared<Turtle>(x, y, id, heading);
  nk->setLabel("nk");
  Patch& patch = get_patch(x, y);
  std::weak_ptr<Turtle> nk_weak_ptr = nk;

  all_nks.push_back(nk);
  all_turtles.push_back(nk_weak_ptr);
  patch.add_turtle(nk);

  return nk;
}

// Sets up the initial patches in the world.
void World::setup_patches() {
   for (int x = 0; x < WORLD_WIDTH; ++x) {
       for (int y = 0; y < WORLD_HEIGHT; ++y) {
           add_patch(x, y);
       }
   }
}

// updates tracking vectors and gets rid of turtles that have died
void World::updateTurtleVectors(){
    // erasing the turtle weak pointers that were reset with kill()
    all_turtles.erase(std::remove_if(all_turtles.begin(), all_turtles.end(),
    [](const std::weak_ptr<Turtle>& wp) {     //lambda function to check if weak pointer is expired (aka killed and reset() has been called)
        return wp.expired();
      }),
    all_turtles.end());

   all_injs.erase(std::remove_if(
       all_injs.begin(), all_injs.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_injs.end());

   // Following tracking vectors' dead shared_ptr will be removed
   all_pmns.erase(std::remove_if(
       all_pmns.begin(), all_pmns.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_pmns.end());

   all_bands.erase(std::remove_if(
       all_bands.begin(), all_bands.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_bands.end());

   all_pmn_marrows.erase(std::remove_if(
       all_pmn_marrows.begin(), all_pmn_marrows.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_pmn_marrows.end());

   all_monos.erase(std::remove_if(
       all_monos.begin(), all_monos.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_monos.end());

   all_mono_marrows.erase(std::remove_if(
       all_mono_marrows.begin(), all_mono_marrows.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_mono_marrows.end());

   all_gen_mono_marrows.erase(std::remove_if(
       all_gen_mono_marrows.begin(), all_gen_mono_marrows.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_gen_mono_marrows.end());

   all_th0_germs.erase(std::remove_if(
       all_th0_germs.begin(), all_th0_germs.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_th0_germs.end());

   all_th0s.erase(std::remove_if(
       all_th0s.begin(), all_th0s.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_th0s.end());

   all_th1s.erase(std::remove_if(
       all_th1s.begin(), all_th1s.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_th1s.end());

   all_th1_germs.erase(std::remove_if(
       all_th1_germs.begin(), all_th1_germs.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_th1_germs.end());

   all_th2s.erase(std::remove_if(
       all_th2s.begin(), all_th2s.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_th2s.end());

   all_th2_germs.erase(std::remove_if(
       all_th2_germs.begin(), all_th2_germs.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_th2_germs.end());

   all_nks.erase(std::remove_if(
       all_nks.begin(), all_nks.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_nks.end());
}


void World::setup(){
  set_rng_seed(RNG_SEED);
  // This function initializes the global variables and setus up the world
  step = 0;
  time = 1;
  
  setup_patches(); // Initializing the patches
  
  global_ID_counter = 0;

  // create cell types like so, using PMN as an example
  /*
  for (int i = 0; i < NUM_CELLS_TO_ADD; i++) {
    // get random position to add new cell
    int random_x = RNG_Engine() % WORLD_WIDTH; //random X and Y position
    int random_y = RNG_Engine() % WORLD_HEIGHT;
    
    // function that creates the actual PMN turtle, adding it to the patch, and tracking vectors, and returns the pmn
    auto pmn = create_pmn_turtle(random_x, random_y, global_ID_counter++)
      
    // setting new cell variables
    pmn->setAge(rand() % 50); // setting age to random number no larger than 50
    pmn->setWbcRoll(1);
    pmn->setWbcStick(0);
    pmn->setWbcMigrate(0);
    pmn->setPmnPcd(10);
  
  }
  // repeat the above code block for all turtle types in setup
  */  
  
  // set world global variables at the end
  // eg, system_oxy = 10201.0;
}

void World::go() {
    // NetLogo "tick" equivalent in C++ is simply incrementing the time counter
    step++;

    // implement netlogo Go function here


    // end of go
    updateTurtleVectors(); // need to update turtle positions/delete dead turtles
}



void World::inj_function(Patch& patch) {
 float rand_inj = 0;
 float oxy = std::max(0.0f, patch.getOxy() - patch.getInfection());
 patch.setOxy(oxy);
 patch.setEndotoxin(patch.getEndotoxin() + (float)patch.getInfection() / 10);

 if (patch.getInfection() >= 100) {
   int random_heading = RNG_Engine() % 360;
   float displacement = 1;
   float target_x = patch.getX() + std::round(displacement * std::cos(random_heading * M_PI / 180.0));
   float target_y = patch.getY() + std::round(displacement * std::sin(random_heading * M_PI / 180.0));
   auto target_patch= get_patch(target_x, target_y);

   target_patch.setInfection(target_patch.getInfection() + 1);

   patch.setInfection(100);
 }

 if (patch.getInfection() > 0) {
   patch.setInfection(std::max(0.0f, patch.getInfection() - patch.getCytotox() + 0.1f));
 }

 if (patch.getInfection() > 50) {
   patch.setColor("grey");
 }
}


void World::ec_activate(Patch& patch) {
    patch.setEcRoll(patch.getEcRoll() + 1);
    patch.setEcStick(patch.getEcStick() + 1);
    patch.setPAF(patch.getPAF() + 1);
    patch.setIL_8(patch.getIL_8() + 1);
}

void World::patch_inj_spread(Patch& patch) {
   if (&patch == nullptr) {
       return;
   }

   float oxy = patch.getOxy();
   float cytotox = patch.getCytotox();
   float eRoll = patch.getEcRoll();
   float eStick = patch.getEcStick();
   float paf = patch.getPAF();

   oxy -= cytotox;

   if (oxy >= 30 && oxy < 60) { // Ischemia
       eRoll += 1;
       oxy -= 0.05;
       paf += 1;

       for (int dx = -1; dx <= 1; ++dx) {
           for (int dy = -1; dy <= 1; ++dy) {
               if (!(dx == 0 && dy == 0)) {
                   Patch& neighbor = get_patch(patch.getX() + dx, patch.getY() + dy);
                   if (neighbor.getOxy() >= 0) {
                       neighbor.setOxy(neighbor.getOxy() - 0.05);
                   }
               }
           }
       }
   } else if (oxy <= 30) { // Infarction
       eStick += 1;
       oxy -= 0.25;
       paf += 1;

       for (int dx = -1; dx <= 1; ++dx) {
           for (int dy = -1; dy <= 1; ++dy) {
               if (!(dx == 0 && dy == 0)) {
                   Patch& neighbor = get_patch(patch.getX() + dx, patch.getY() + dy);
                   if (neighbor.getOxy() >= 0) {
                       neighbor.setOxy(neighbor.getOxy() - 0.25);
                   }
               }
           }
       }
   }

   if (oxy < 0) { // Prevents negative values for oxy
       patch.setOxy(0);
   } else {
       patch.setOxy(oxy);
   }

   patch.setEcRoll(eRoll);
   patch.setEcStick(eStick);
   patch.setPAF(paf);
}


void World::ec_function(Patch& patch) {
   if (patch.getEndotoxin() >= 1.0f || patch.getOxy() < 60.0f) {
       patch.setEcActivation(1.0f);
   }

   if (patch.getEcActivation() == 1.0f) {
       ec_activate(patch);
       patch_inj_spread(patch);
   }
}

void World::pmn_burst(std::shared_ptr<Turtle> pmn) {
   if (!pmn) {
       return;
   }

   Patch& patch = get_patch(pmn->getX(), pmn->getY());

   float tnf = patch.getTNF();
   float cytotox = patch.getCytotox();
   cytotox += std::max(10.0f, tnf);
   patch.setCytotox(cytotox);

   float oxy = patch.getOxy();
   oxy = 100.0f;
   patch.setOxy(oxy);

   float eRoll = patch.getEcRoll();
   eRoll = 0.0f;
   patch.setEcRoll(eRoll);

   float eStick = patch.getEcStick();
   eStick = 0.0f;
   patch.setEcStick(eStick);

   float eMigrate = patch.getEcMigrate();
   eMigrate = 0.0f;
   patch.setEcMigrate(eMigrate);

   float infG = patch.getINF_g();
   float gcsf = patch.getGCSF();
   tnf += 1.0f;
   float il1 = patch.getIL_1();
   il1 += 1.0f;

   patch.setTNF(tnf);
   patch.setIL_1(il1);

   float pmn_age = pmn->getPmnAge();
   float pmn_pcd = pmn->getPmnPcd();
   pmn_age = pmn_pcd;
   pmn_pcd += -1.0f + std::max(0.0f, (tnf + infG + gcsf - patch.getIL_10()) / 100.0f);
   pmn->setPmnAge(pmn_age);
   pmn->setPmnPcd(pmn_pcd);

   if (pmn_age < 0) {
       kill(pmn);
   }
}


void World::pmn_sniff(std::shared_ptr<Turtle> pmn) {
   if (!pmn) {
       return;
   }

   const Patch& patchAhead = getPatchAhead(pmn);
   const Patch& patchRightAhead = getPatchAheadRight(pmn);
   const Patch& patchLeftAhead = getPatchAheadLeft(pmn);

   double pmnahead = patchAhead.getIL_8();
   double pmnright = patchRightAhead.getIL_8();
   double pmnleft = patchLeftAhead.getIL_8();

   double heading = pmn->getHeading() * (M_PI / 180.0);

   if (pmnright > pmnahead && pmnright > pmnleft) {
       heading -= 45;
   } else if (pmnleft > pmnahead) {
       heading += 45;
   }

   pmn->setHeading(heading * 180 / M_PI);

   move(pmn);
}

void World::pmn_function(std::shared_ptr<Turtle> pmn) {
   if (!pmn) {
       return;
   }

   Patch& patch = get_patch(pmn->getX(), pmn->getY());

   if (pmn->getWbcMigrate() > 0) {
       pmn_burst(pmn);
   }
   else {
       if ((patch.getEcRoll() > 3.0f) && (pmn->getWbcRoll() == 1.0f)) {
           pmn_sniff(pmn);
       }
       else {
           for (int i = 0; i < 2; i++) {
               pmn_sniff(pmn);
           }
       }

       if (patch.getTNF() + patch.getPAF() > 1.0f) {
           pmn->setWbcStick(patch.getIL_1());
           patch.setIL_1ra(patch.getIL_1ra() + 1.0f);
       }

       if ((pmn->getWbcStick() >= 1.0f) && (patch.getEcStick() >= 100.0f)) {
           float wbcMigrate = patch.getTNF() + patch.getIL_1() + patch.getGCSF() - patch.getIL_10();
           pmn->setWbcMigrate(wbcMigrate);
           pmn->setColor("yellow");
       }

       pmn->setPmnAge(pmn->getPmnAge() - 1.0f);
       if (pmn->getPmnAge() < 0) {
           kill(pmn);
       }
   }
}

void World::heal(Patch& patch) {
   patch.setOxy(100.0);
   patch.setEcRoll(0.0);
   patch.setEcStick(0.0);
   patch.setEcMigrate(0.0);
   patch.setInfection(0.0);
}

void World::mono_sniff(std::shared_ptr<Turtle> mono) {
   if (!mono) {
       return;
   }

   float monoahead = 0, monoright = 0, monoleft = 0;

   const Patch& patchAhead = getPatchAhead(mono, 1);
   monoahead = patchAhead.getPAF();

   const Patch& patchRightAhead = getPatchAheadRight(mono, 1);
   monoright = patchRightAhead.getPAF();

   const Patch& patchLeftAhead = getPatchAheadLeft(mono, 1);
   monoleft = patchLeftAhead.getPAF();

   if (monoright >= monoahead && monoright >= monoleft) {
       mono->setHeading(mono->getHeading() + 45);
   }
   else if (monoleft >= monoahead) {
       mono->setHeading(mono->getHeading() - 45);
   }

   move(mono); // Assuming you have implemented the move member function for World
}

void World::mono_function(std::shared_ptr<Turtle> mono) {
   if (!mono) {
       return;
   }

   Patch& patch = get_patch(mono->getX(), mono->getY());

   if (patch.getSTNFr() <= 100.0f) {
       patch.setTNF(std::min(100.0f, patch.getTNF() + patch.getSTNFr()));
   }
   else {
       patch.setTNF(std::min(100.0f, patch.getTNF() - patch.getSTNFr()));
   }

   mono->setIL_1r(std::min(100.0f, patch.getIL_1() - patch.getIL_1ra() - patch.getSIL_1r()));
   patch.setIL_1ra(patch.getIL_1ra() + patch.getIL_1() / 2);
   patch.setSTNFr(patch.getSTNFr() + patch.getTNF() / 2);
   patch.setSIL_1r(patch.getSIL_1r() + mono->getIL_1r() / 2);

   float endotoxin = patch.getEndotoxin();
   float PAF = patch.getPAF();
   float INF_g = patch.getINF_g();
   float IL_10 = patch.getIL_10();
   float activation = endotoxin + PAF + INF_g - IL_10;

   if (activation > 0) {
       float GCSF = patch.getGCSF();
       float TNF = patch.getTNF();
       float IL_1 = patch.getIL_1();
       float IL_8 = patch.getIL_8();
       float IL_12 = patch.getIL_12();

       GCSF += endotoxin + PAF + TNF + INF_g;
       IL_8 += TNF + IL_1;
       IL_12 += TNF + IL_1;
       IL_10 += TNF + IL_1;
       IL_1 += endotoxin + PAF + mono->getIL_1r() + TNF;
       TNF += endotoxin + PAF + mono->getTNFr() + INF_g;

       if (mono->getWbcStick() == 1 && patch.getEcStick() >= 100.0f) {
           mono->setWbcMigrate(1.0f);
       }

       if (mono->getWbcRoll() == 1) {
           mono->setWbcStick(1.0f);
       }

       mono->setWbcRoll(1.0f);
   }
   else if (activation < 0) {
       patch.setIL_10(patch.getIL_10() + patch.getTNF() + patch.getIL_1());
   }

   if (mono->getWbcMigrate() == 1.0f) {
       heal(patch);
   }

   int condition = (mono->getWbcRoll() == 1) ? 1 : 2;
   if (condition == 1) {
       mono_sniff(mono);
       move(mono);
   }
   else {
       for (int i = 0; i < 2; i++) {
           mono_sniff(mono);
           move(mono);
       }
   }


   if (activation > 20) {
       activation = 20.0f;
       mono->setActivation(activation);
   }

   mono->setMonoAge(mono->getMonoAge() - 1.0f);
   if (mono->getMonoAge() < 0) {
       kill(mono);
   }

}

