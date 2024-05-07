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


void World::setup() {
    set_rng_seed(RNG_SEED);

    step = 0;
    time = 1;
    system_oxy = 0;
    oxy_deficit = 0;
    total_infection = 0;
    total_TNF = 0;
    total_sTNFr = 0;
    total_IL_10 = 0;
    total_GCSF = 0;
    total_pro_TH1 = 0;
    total_pro_TH2 = 0;

    setup_patches(); // Initializing the patches

    global_ID_counter = 0;

    // Adding various turtles and adding them to the different tracking vectors

    system_oxy = 10201.0;
    oxy_deficit = 0;
}

void World::go() {
    // NetLogo "tick" equivalent in C++ is simply incrementing the time counter
    step++;

    // implement netlogo Go function here


    // end of go
    updateTurtleVectors(); // need to update turtle positions/delete dead turtles
}

void World::update_system_oxy() {
    system_oxy = 0;
    oxy_deficit = 10201.0;
    total_infection = 0;
    total_TNF = 0;
    total_sTNFr = 0;
    total_IL_10 = 0;
    total_pro_TH1 = 0;
    total_pro_TH2 = 0;
    total_GCSF = 0;

    // Calculate system oxy
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            system_oxy += all_patches[x][y].getOxy();
        }
    }
    system_oxy /= 100;
    oxy_deficit = 10201.0 - system_oxy;

    // Calculate total infections and cytokines
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            total_infection += all_patches[x][y].getInfection();
            total_TNF += all_patches[x][y].getTNF();
            total_sTNFr += all_patches[x][y].getSTNFr();
            total_IL_10 += all_patches[x][y].getIL_10();
            total_GCSF += all_patches[x][y].getGCSF();
        }
    }
    total_infection /= 100;
    total_TNF /= 100;
    total_sTNFr /= 100;
    total_IL_10 /= 100;
    total_GCSF /= 100;

    // Calculate total pro-TH1 and pro-TH2
    for (const auto& turtle : all_turtles) {
        if (turtle.lock()) { // Check if turtle is still alive
            total_pro_TH1 += turtle.lock()->getProTH1();
            total_pro_TH1 += turtle.lock()->getProTH1();
            total_pro_TH2 += turtle.lock()->getProTH2();
        }
    }
    total_pro_TH1 /= 100;
    total_pro_TH2 /= 100;
}

void World::set_injury_infection(int inj_number, double radius) {
   std::default_random_engine generator;
   std::uniform_real_distribution<double> distribution(0.0, 1.0);

   for (int i = 0; i < inj_number; ++i) {
       double random_angle = 2 * M_PI * distribution(generator);
       double random_distance = std::pow(distribution(generator), 1.0 / 2) * radius;

       int random_x = (WORLD_WIDTH - 1) / 2 + std::round(random_distance * std::cos(random_angle));
       int random_y = (WORLD_HEIGHT - 1) / 2 + std::round(random_distance * std::sin(random_angle));

       if (random_x >= 0 && random_x < WORLD_WIDTH && random_y >= 0 && random_y < WORLD_HEIGHT) {
           all_patches[random_x][random_y].setInfection(100);
       }
   }
}

void World::injure_sterile(int inj_number) {
   std::default_random_engine generator;
   std::uniform_real_distribution<double> distribution(0.0, 1.0);

   for (int i = 0; i < inj_number; ++i) {
       double random_radius = std::sqrt(std::abs(i));
       int random_x = (WORLD_WIDTH - 1) / 2 + std::round(distribution(generator) * random_radius);
       int random_y = (WORLD_HEIGHT - 1) / 2 + std::round(distribution(generator) * random_radius);

       if (random_x >= 0 && random_x < WORLD_WIDTH && random_y >= 0 && random_y < WORLD_HEIGHT) {
           all_patches[random_x][random_y].setOxy(0);
           all_patches[random_x][random_y].setEcRoll(3);
           all_patches[random_x][random_y].setEcStick(100);
       }
   }
}