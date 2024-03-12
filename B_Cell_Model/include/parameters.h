// parameters.h
#ifndef PARAMETERS_H
#define PARAMETERS_H

const bool RENDERING = true;
const bool SAVING_DATA = false;
const bool TURTLE_CONTINOUS_MOVEMENT = true;
const bool TOROIDAL_WORLD = false;

const int RNG_SEED=108;
const int NUM_STEPS = 10000;


const static int WORLD_WIDTH = 101;
const static int WORLD_HEIGHT = 101;
const int PATCH_MAX_CAPACITY = 10000;
const int MAX_RANDOM_DISTANCE = 100;

// this netlgoo implementation specific variables
const bool RandomRuns = false;
const bool AUTOINOCULATE = true;
const int AUTOINOCULATE_FIRST_TIME = 480;
const int AUTOINOCULATE_SECOND_TIME = 2880;
const int STOP_EXPOSURE_TIME = 4800;
const int FIRST_EXPOSURE_AMOUNT = 50;
const int SECOND_EXPOSURE_AMOUNT = 50;
const int BACTERIA_SIZE = 2;

const int BACTERIA_EPITOPE_TYPE = 30;
const int NUMBER_OF_TI_EPITOPES = 10;
const int NUMBER_OF_TD_EPITOPES = 10;

const int BCELL_CD21_ACTIVATION_THRESHOLD =101;
const int BCELL_TNFA_APOP_THRESHOLD = 277;
const int BCELL_BREG_DIFF_THRESHOLD = 181;

const int PHAG_IL6_BURST=0;

const int BACKGROUND_TNFA = 0;
const int BACKGROUND_IL6 = 0;

static const int CXCL13_LEVEL_DIFFUSION_FACTOR = 1.0;
static const int S1P_LEVEL_DIFFUSION_FACTOR = 1.0;
static const int CCL19_LEVEL_DIFFUSION_FACTOR = 1.0;
static const int EBI2_LEVEL_DIFFUSION_FACTOR = 1.0;
static const int IL2_DIFFUSION_FACTOR = 1.0;
static const int IL4_DIFFUSION_FACTOR = 1.0;
static const int IL6_DIFFUSION_FACTOR = 1.0;
static const int IL10_DIFFUSION_FACTOR = 1.0;
static const int IL12_DIFFUSION_FACTOR = 1.0;
static const int IL15_DIFFUSION_FACTOR = 1.0;
static const int IL21_DIFFUSION_FACTOR = 1.0;
static const int IF_G_DIFFUSION_FACTOR = 1.0;
static const int IF_A_DIFFUSION_FACTOR = 1.0;
static const int TNF_A_DIFFUSION_FACTOR = 1.0;
static const int TGF_B_DIFFUSION_FACTOR = 1.0;

static const double CXCL13_EVAPORATION_FACTOR = 0.9;
static const double S1P_EVAPORATION_FACTOR = 0.9;
static const double CCL19_EVAPORATION_FACTOR = 0.9;
static const double EBI2_EVAPORATION_FACTOR = 0.9;
static const double IL2_EVAPORATION_FACTOR = 0.9;
static const double IL4_EVAPORATION_FACTOR = 0.9;
static const double IL6_EVAPORATION_FACTOR = 0.95;
static const double IL10_EVAPORATION_FACTOR = 0.95;
static const double IL12_EVAPORATION_FACTOR = 0.9;
static const double IL15_EVAPORATION_FACTOR = 0.9;
static const double IL21_EVAPORATION_FACTOR = 0.9;
static const double IF_G_EVAPORATION_FACTOR = 0.9;
static const double IF_A_EVAPORATION_FACTOR = 0.9;
static const double TNF_A_EVAPORATION_FACTOR = 0.9;
static const double TGF_B_EVAPORATION_FACTOR = 0.9;

#endif
