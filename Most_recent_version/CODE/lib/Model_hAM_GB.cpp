// Source code associated with  ===========================  //
// "Multi-scale cardiac simulation framework" =============  //
// For simulation of cardiac cellular and tissue dynamics =  //
// from the spatial cellular to full organ scales. ========  //
// With implementation of multiple, published cell models =  //
// as well as novel models developed in my lab. ===========  //
// ========================================================  //
// This file: Implementation of the human atrial cell model  //
// by Grandi-Bers: E. Grandi, et al. Circ Res 2011; =======  //
// 109(9):1055-66. And updated by Chang-Trayanova: K.C. ===  //
// Chang, J.D. Bayer and N.A. Trayanova PLOS Comp. Biol. ==  //
// 2014; 10(12):e1004011 ==================================  //
// Using Ito and IKur from M.M. Maleckar et al. Am. J. ====  //
// Physiol Heart Circ Physiol 2009;297(4):H1398-410 =======  //
// Implemented based on C/C++ code by Dominic Whittaker and  //
// Haibo Ni, 2017-2018. ===================================  //
// In-code identifier: hAM_GB =============================  //
// ========================================================  //
// GNU 3 LICENSE TEXT =====================================  //
// COPYRIGHT (C) 2016-2019 MICHAEL A. COLMAN ==============  //
// THIS PROGRAM IS FREE SOFTWARE: YOU CAN REDISTRIBUTE IT =  //
// AND/OR MODIFY IT UNDER THE TERMS OF THE GNU GENERAL ====  //
// PUBLIC LICENSE AS PUBLISHED BY THE FREE SOFTWARE =======  //
// FOUNDATION, EITHER VERSION 3 OF THE LICENSE, OR (AT YOUR  //
// OPTION) ANY LATER VERSION. =============================  //
// THIS PROGRAM IS DISTRIBUTED IN THE HOPE THAT IT WILL BE=  //
// USEFUL, BUT WITHOUT ANY WARRANTY; WITHOUT EVEN THE =====  //
// IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR A ===  //
// PARTICULAR PURPOSE.  SEE THE GNU GENERAL PUBLIC LICENSE=  //
// FOR MORE DETAILS. ======================================  //
// YOU SHOULD HAVE RECEIVED A COPY OF THE GNU GENERAL =====  //
// PUBLIC LICENSE ALONG WITH THIS PROGRAM.  IF NOT, SEE ===  //
// <https://www.gnu.org/licenses/>. =======================  //
// ========================================================  //
// ADDITIONAL LICENSE TEXT ================================  //
// THIS SOFTWARE IS PROVIDED OPEN SOURCE AND MAY BE FREELY=  //
// USED, DISTRIBUTED AND UPDATED, PROVIDED: ===============  //
//  (i) THE APPROPRIATE WORK(S) IS(ARE) CITED. THIS =======  //
//      PERTAINS TO THE CITATION OF COLMAN 2019 PLOS COMP =  //
//      BIOL (FOR THIS IMPLEMTATION) AND ALL WORKS ========  //
//      ASSOCIATED WITH THE SPECIFIC MODELS AND COMPONENTS=  //
//      USED IN PARTICULAR SIMULATIONS. IT IS THE USER'S ==  //
//      RESPONSIBILITY TO ENSURE ALL RELEVANT WORKS ARE ===  //
//      CITED. PLEASE SEE FULL DOCUMENTATION AND ON-SCREEN=  //
//      DISCLAIMER OUTPUTS FOR A GUIDE. ===================  //
//  (ii) ALL OF THIS TEXT IS RETAINED WITHIN OR ASSOCIATED=  //
//      WITH THE SOURCE CODE AND/OR BINARY FORM OF THE ====  //
//      SOFTWARE. =========================================  //
// ========================================================  //
// ANY INTENDED COMMERCIAL USE OF THIS SOFTWARE MUST BE BY   //
// EXPRESS PERMISSION OF MICHAEL A COLMAN ONLY. IN NO EVENT  //
// ARE THE COPYRIGHT HOLDERS LIABLE FOR ANY DIRECT, =======  //
// INDIRECT INCIDENTAL, SPECIAL, EXEMPLARY OR CONSEQUENTIAL  //
// DAMAGES ASSOCIATED WITH USE OF THIS SOFTWARE ===========  //
// ========================================================  //
// THIS SOFTWARE CONTAINS IMPLEMENTATIONS OF MODELS AND ===  //
// COMPONENTS WHICH I (MICHAEL COLMAN) DID NOT DEVELOP.====  //
// ALL OF THESE COMPONENTS HAVE BEEN CODED FROM PROVIDED ==  //
// SOURCE CODE OR INFORMATION IN THE PUBLICATIONS. ========  //
// I CLAIM NO RIGHTS OR INTELLECTUAL PROPERTY OWNERSHIP ===  //
// FOR THESE MODELS AND COMPONENTS, OTHER THAN THEIR ======  //
// SPECIFIC IMPLEMENTATION IN THIS CODE PACKAGE. FURTHER TO  //
// THE ABOVE STATEMENT, ANY INDTENDED COMMERCIAL USE OF ===  //
// THOSE COMPONENTS MUST BE BY EXPRESS PERMISSION OF THE ==  //
// ORIGINAL COPYRIGHT HOLDERS. ============================  //
// WHERE IMPLEMENTED FROM PROVIDED CODE, ANY DISCLAIMERS ==  //
// PRESENT IN THE ORIGINAL CODE HAVE BEEN RETAINED IN THE =  //
// RELEVANT FILE. =========================================  //
// ========================================================  //
// Contact: m.a.colman@leeds.ac.uk ========================  //
// For updates, corrections etc, please check: ============  //
// 1. http://physicsoftheheart.com/ =======================  //
// 2. https://github.com/michaelcolman ====================  //
// ========================================================  //

#include "Model.h"
#include "Structs.h"

// Parameters and specific settings =============================================================\\|
// Set model dependent parameters 
void set_parameters_native_hAM_GB(Cell_parameters *p)
{
	// dt
	p->dt				= 0.01; 	// ms

	// Capacitance and cell structure
	p->Cm				= 110;		// pF
	p->Cm_F				= 1.10e-10;	// F

	double radius, length;
	radius				= 10.25; 	// uM
	length				= 100;		// uM

	p->Vcell			= 3.14159265358979 * pow(radius, 2.0) * length * 1.0e-15;	// L
	p->Vjunc			= 0.0539    * 0.01 * p->Vcell;
	p->Vcyto			= 0.65 		* p->Vcell;
	p->VnSR				= 0.035		* p->Vcell;
	p->Vsl				= 0.02		* p->Vcell;

	p->Fjunc			= 0.11;
	p->Fjunc_ICaL		= 0.9;

	// Concentrations (constant OR initial condition)
	p->Nai				= 9.136;	// mM
	p->Nao				= 140;		// mM
	p->Ki				= 120;		// mM
	p->Ko				= 5.4;		// mM
	p->Cai				= 0.000102; // mM 
	p->Cao				= 1.8;		// mM

	// Current parameters
	p->gNa				= 7.8; 			// s/mF
	p->gNaL				= 0;//*0.0025;	// s/mF
	p->gto				= 0.1652;		// s/mF
	p->pCaL				= 2.7e-4; 		// cm/s
	p->pCaL_K			= 1.35e-7;		// cm/s
	p->pCaL_Na			= 0.75e-8;		// cm/s
	p->gKur				= 0.045; 		// s/mF 
	p->gK1				= 0.0525;		// s/mF
	p->gNab				= 0.597e-3; 	// s/mF
	p->gCab				= 6.0643e-4; 	// s/mF
	p->gKb				= 0.002;		// s/mF
	p->gClCa			= 0.5 * 0.109625;	// s/mF
	p->gKr				= 0.035 * sqrt(p->Ko / 5.4);
	p->gKs				= 0.0035;		// s/mV

	p->INCX_bar			= 3.15;		// pA/pF
	p->INCX_kNao		= 87.5;		// mM
	p->INCX_kCao		= 1.3;		// mM
	p->INCX_kNai		= 12.3;		// mM
	p->INCX_kCai		= 3.59e-3;	// mM
	p->INCX_k			= 0.1;
	p->INCX_gamma		= 0.35;
	p->INCX_kda			= 0.384e-3;	// mM
	p->INCX_ksat		= 0.27;

	p->INaK_bar			= 1.26; 		// pA/pF
	p->INaK_kK			= 1.5;			// mM
	p->INaK_kNa			= 11.0;			// mM

	p->ICaP_bar			= 0.0471;		// pA/pF
	p->ICaP_kCa			= 0.5e-3; 		// mM

	p->IClCa_kd			= 100.0e-3;

	// Structure and environment
	p->DcaJuncSL            = 1.64e-6;		// cm^2/s
	p->DcaSLcyto            = 1.22e-6;		// cm^2/s
	p->DnaJuncSL            = 1.09e-5;		// cm^2/s
	p->DnaSLcyto            = 1.79e-5;	 	// cm^2/s	
	p->junctionLength       = 160.0e-3;		// um
	p->junctionRadius       = 15.0e-3;		// um
	p->distSLcyto           = 0.45;			// um
	p->distJuncSL           = 0.5;			// um
	p->SAjunc               = 20150.0 * 3.14159265358979 * 2.0 * p->junctionLength * p->junctionRadius; // um^2
	p->SAsl                 = 3.14159265358979 * 2.0 * radius * length;									// um^2
	p->J_ca_juncsl          = 8.2413e-13;	// L/ms
	p->J_ca_slmyo           = 3.7243e-12;	// L/ms
	p->J_na_juncsl			= 1.8313e-14;	// L/ms
	p->J_na_slmyo		 	= 1.6386e-12;	// L/ms

	// Transport - SR
	p->ks                   = 25.0;				// ms^-1
	p->J_SERCA_max          = 5.3114e-3;		// mM/ms
	p->Kmf                  = 2.5*0.246e-3;		// mM
	p->Kmr                  = 1.7;				// mM
	p->hillSRCaP            = 1.787;			
	p->koCa                 = 10.0;				// ms^-1 mM^-1
	p->kiCa                 = 0.5;				// ms^-1 mM^-1
	p->kim                  = 0.005;			// ms^-1
	p->kom                  = 0.06;				// ms^-1
	p->ec50SR               = 0.45;				// mM
	p->MaxSR                = 15.0;				// mM (?)
	p->MinSR                = 1.0;				// mM (?)

	// Buffering
	p->Bmax_SLlowj          = 4.6e-3 * p->Vcyto / p->Vjunc * 0.1;			// mM
	p->koff_sll             = 1300.0e-3;									// ms^-1
	p->kon_sll              = 100.0;										// ms^-1 mM^-1
	p->Bmax_SLhighj         = 1.65e-3 * p->Vcyto / p->Vjunc * 0.1;			// mM
	p->kon_slh              = 100.0;										// ms^-1 mM^-1
	p->koff_slh             = 30.0e-3;										// ms^-1
	p->Bmax_SLlowsl         = 37.4e-3 * p->Vcyto / p->Vsl;					// mM
	p->Bmax_SLhighsl        = 13.4e-3 * p->Vcyto / p->Vsl;					// mM
	p->koff_myoca           = 0.46e-3;										// ms^-1
	p->kon_myoca            = 13.8;											// ms^-1 mM^-1
	p->Bmax_myosin          = 140.0e-3;										// mM
	p->Mgi                  = 1.0;
	p->kon_myomg            = 0.0157;										// ms^-1 mM^-1
	p->koff_myomg           = 0.057e-3;										// ms^-1
	p->kon_tnchca           = 2.37;											// ms^-1 mM^-1
	p->koff_tnchca          = 0.032e-3;										// ms^-1
	p->Bmax_TnChigh         = 140.0e-3;										// mM
	p->kon_tnchmg           = 3.0e-3;										// ms^-1 mM^-1
	p->koff_tnchmg          = 3.33e-3;										// ms^-1
	p->kon_tncl             = 32.7;											// ms^-1 mM^-1
	p->koff_tncl            = 19.6e-3;										// ms^-1
	p->Bmax_TnClow          = 70.0e-3;										// mM	
	p->Bmax_CaM             = 24.0e-3;										// mM
	p->kon_cam              = 34.0;											// ms^-1 mM^-1
	p->koff_cam             = 238.0e-3;										// ms^-1
	p->Bmax_SR              = 19.0 * 0.9e-3;								// mM
	p->kon_sr               = 100.0;										// ms^-1 mM^-1
	p->koff_sr              = 60.0e-3;										// ms^-1
	p->Bmax_Csqn            = 140.0e-3 * p->Vcyto / p->VnSR;				// mM
	p->kon_csqn             = 100.0;										// ms^-1 mM^-1
	p->koff_csqn            = 65.0;											// ms^-1
	p->Bmax_Naj				= 7.561;										// mM
	p->Bmax_Nasl			= 1.65;											// mM
	p->koff_na				= 1.0e-3;										// ms^-1
	p->kon_na				= 0.1e-3;										// ms^-1 mM^-1

	// Stimulus parameters
	p->stimduration 		= 5.0;      // ms
	p->stimmag      		= -12.5;    // pA/pF

	// Default celltype
	p->Celltype				= "RA";
	p->ISO_model			= "GB";

	// Ihyp
	p->AIhyp				= 0.63; 	// pA/pF
}

// Initial conditions
void initial_conditions_native_hAM_GB(State_variables *s, Cell_parameters p)
{
	s->Vm      			= -73.503667;
	s->INa_va  			= 0.010085;
	s->INa_vi_1			= 0.829053;
	s->INa_vi_2			= 0.849974;
	s->INaL_va 			= 0.000102;
	s->INaL_vi			= 0.050442;
	s->Ito_va  			= 0.07;
	s->Ito_vi  			= 0.99;
	s->Ito_vi_s			= 0.361531;
	s->ICaL_va			= 0.0;
	s->ICaL_vi			= 1.0;
	s->ICaL_ci			= 0;//1.0;
	s->ICaL_ci_j		= 0;//1.0;
	s->IKur_va			= 0.0;
	s->IKur_vi			= 1.0;
	s->IKr_va			= 0.000024;
	s->IKs_va			= 0;

	// Ca handling
	s->CajSR			= 0.52;		// mM
	s->CanSR			= 0.52;		// mM
	s->RyRo             = 0.000002;
	s->RyRi             = 0.0;
	s->RyRr             = 0.841294;
	s->Myo_c            = 0.003737;
	s->Myo_m            = 0.135752;
	s->Tn_CHc           = 0.127315;
	s->Tn_CHm           = 0.005947;
	s->Tn_CL            = 0.017693;

	// Assign state from param (even if constant)
	s->Nai				= p.Nai;
	s->Nai_j			= p.Nai;
	s->Nai_sl			= p.Nai;
	s->Nao				= p.Nao;
	s->Ki				= p.Ki;
	s->Ko				= p.Ko;
	s->Cai				= p.Cai;
	s->Cai_j			= p.Cai;
	s->Cai_sl			= p.Cai;
	s->Cao				= p.Cao;
}
// end Parameters and specific settings =========================================================//|

// Heterogeneity and modulation =================================================================\\|
void set_het_mod_hAM_GB(Cell_parameters *p)
{
	if (                                       p->Het_set_ref == 0)         set_celltype_native_hAM_GB(p);
	if (p->ISO > 0.0                        && p->ISO_set_ref == 0)         set_modulation_ISO_native_hAM_GB(p);
	if (strcmp(p->Agent, "none") != 0       && p->Agent_set_ref == 0)       set_modulation_Agent_native_hAM_GB(p);
	if (strcmp(p->Remodelling, "none") != 0 && p->Remodelling_set_ref == 0) set_modulation_Remodelling_native_hAM_GB(p);
	if (strcmp(p->Mutation, "none") != 0    && p->Mutation_set_ref == 0)    set_modulation_Mutation_native_hAM_GB(p);
	if (p->ACh > 0.0                        && p->ACh_set_ref == 0)         set_modulation_ACh_hAM_GB(p);
}

// Celltype/Heterogeneity
void set_celltype_native_hAM_GB(Cell_parameters *p)
{
	// Can modify parameters (e.g. conductance) directly, or scaling factors.
	// (conductance denoted "g", scale factor "G")
	// In general, use the scale factors as these are multiplicative/additive throughout
	// the code - if a parameter is set (rather than modified) it will overwrite previous
	// settings; this may or may not be desired

	// NOTE:: Most het has been set in Model.c as common to multiple human atrial cell models. Please see that function first.
	// set_celltype_hAM() in lib/Model.c

	if (strcmp(p->Celltype, "RA") == 0);    // Do nothing for baseline celltype
	else if (strcmp(p->Celltype, "WL_scaling") == 0)    // Scaling of current magntiudes to fit WL_2006 data
	{
		p->gto              *= 0.93;//0.7*0.1652;   // s/mF           
		p->pCaL             *= 0.61;   // cm/s
		p->gKur             *= 1.48;// s/mF 
		p->gK1              = 2*0.0525;       // s/mF
	}
	else if (strcmp(p->Celltype, "WL_scaling2") == 0)    // Scaling of current magntiudes to fit WL_2006 data
	{
		p->gto              = 0.7*0.1652;   // s/mF           
		p->gKur             = 1.8*0.9*0.9*0.045;// s/mF 
		p->gK1              = 2*0.0525;       // s/mF
	}
	// testing exmaple illustration of model-specific implementation
	else if (strcmp(p->Celltype, "test") == 0)
	{
		p->Gto              *= 2;       // Scale factor = Multiplies previous settings
		p->gKur             = 0.003;    // Actual conductance explicitly set. Will overwrite any previous settings of g, but not scale factor mods. 
		p->IKr_va_ss_kscale *= 1.25;    // Multiplies gradient parameter for voltage-activation steady-state. Multiplies previous settings
		p->ICaL_vi_ss_shift += 5;       // Shifts the voltage dependence of the steady state of inactivation gate. Summed to previous settings
		p->IKs_va_tau_scale *= 0.75;    // Multiplies time constant of voltage activation. Multiplies previous settings.
		p->Gup              *= 2;       // Scales intracellular upatke rate. Multiplies previous settings.
	}
	// Add new celltypes here: else if (strcmp(p->Celltype, "X") == 0) {   }
	else 
	{
		printf("ERROR: \"%s\" is not a valid Celltype for the hAM_GB models. Please check Model.c and Model_hAM_GB.cpp for options\n\n", p->Celltype);
		exit(1);
	}
}

// ISO
void set_modulation_ISO_native_hAM_GB(Cell_parameters *p)
{
	// Can modify parameters (e.g. conductance) directly, or scaling factors.
	// (conductance denoted "g", scale factor "G")
	// In general, use the scale factors as these are multiplicative/additive throughout
	// the code - if a parameter is set (rather than modified) it will overwrite previous
	// settings; this may or may not be desired

	// "ISO" is 0-1 representing 0 to saturating solution i.e. describes the proportion of full ISO scaling to apply
	// This is thus a linear and homogeneous application. If desired, rescale this according to a non-linear
	// function to apply conc dependency etc up to saturating solution. If so, rescale here. 
	// "ISO_model" may be used also to implement different implementations of ISO. If not, will be set to "Default"

	// NOTE: ISO has been set in Model.c set_ISO_hAM() as commong to many human atrial models
	// Place only a model-specific ISO here

	// testing exmaple illustration of model-specific implementation
	if (strcmp(p->ISO_model, "test") == 0)
	{
		p->GKr              *= (1.0 + p->ISO*(2.0 - 1.0)); // x2
		p->GLTCC_kva1_va2   *= (1.0 + p->ISO*(2.0 - 1.0)); // x2
		p->Gup              *= (1.0 + p->ISO*(1.75- 1.0)); // x1.75
	}
	// Add new ISO_models here: else if (strcmp(p->ISO_model, "X") == 0) {   }
	else
	{
		printf("ERROR: \"%s\" is not a valid ISO model for the hAM_GB models. Please check Model.c and Model_hAM_GB.cpp for options\n\n", p->ISO_model);
		exit(1);
	}
}

// Pharmacological modulation
void set_modulation_Agent_native_hAM_GB(Cell_parameters *p)
{
	// Can modify parameters (e.g. conductance) directly, or scaling factors.
	// (conductance denoted "g", scale factor "G")
	// In general, use the scale factors as these are multiplicative/additive throughout
	// the code - if a parameter is set (rather than modified) it will overwrite previous
	// settings; this may or may not be desired

	// For Agents: "Agent_prop" is 0-1 which describes the proportion of full agent scaling to apply
	// This is thus a linear and homogeneous application. If desired, rescale this according to a non-linear
	// function to apply conc dependency etc up to saturating solution. If so, rescale here.

	// Note: Many Agents will be common to multiple or all models, and thus should be defined in
	// lib/Model.c. Only model-specific drugs should be here

	if (strcmp(p->Agent, "none") == 0); // do nothing
	// testing exmaple illustration of model-specific implementation
	else if (strcmp(p->Agent, "test") == 0)
	{
		p->Gto              *= (1.0 + p->Agent_prop*(2.0  -1));     // Scale factor = Multiplies previous settings  | 2 is scale factor, 2-1 is additive scale factor
		p->IKr_va_ss_kscale *= (1.0 + p->Agent_prop*(1.25 -1));     // Multiplies gradient parameter for voltage-activation steady-state. Multiplies previous settings
		p->IKs_va_tau_scale *= (1.0 + p->Agent_prop*(0.75 -1));     // Multiplies time constant of voltage activation. Multiplies previous settings.
		p->Gup              *= (1.0 + p->Agent_prop*(2.0  -1));     // Scales intracellular upatke rate. Multiplies previous settings.
		p->ICaL_vi_ss_shift += 5*p->Agent_prop;                     // Shifts the voltage dependence of the steady state of inactivation gate. Summed to previous settings
		p->gKur             = 0.003;   
	}
	// Add new pharmacological agent here: else if (strcmp(p->Agent, "X") == 0) {   }
	else
	{
		printf("ERROR: \"%s\" is not a valid pharmacological agent for the hAM_GB models. Please check Model.c and Model_hAM_GB.cpp for options\n\n", p->Agent);
		exit(1);
	}
}

// Remodelling
void set_modulation_Remodelling_native_hAM_GB(Cell_parameters *p)
{
	// Can modify parameters (e.g. conductance) directly, or scaling factors.
	// (conductance denoted "g", scale factor "G")
	// In general, use the scale factors as these are multiplicative/additive throughout
	// the code - if a parameter is set (rather than modified) it will overwrite previous
	// settings; this may or may not be desired

	// For remodelling: "Remodelling_prop" is 0-1 which describes the proportion of full agent scaling to apply
	// This is thus a linear and homogeneous application. If desired, rescale this according to a non-linear
	// function. This may have less/no meaning for remodelling (compared to what it means in Agent/ISO).
	// In general, this can be used for continuous gradients between healthy and remodelling regions

	// NOTE: AF remodelling from both Grandi 2011 and Colman 2013, common
	// to many human atrial models, is in Model.c set_remodelling_hAM().
	// Add only a model-specific remodelling here

	if (strcmp(p->Remodelling, "none") == 0); // do nothing
	// testing exmaple illustration of model-specific implementation
	else if (strcmp(p->Remodelling, "test") == 0)
	{
		p->Gto              *= (1.0 + p->Remodelling_prop*(2.0  -1));     // Scale factor = Multiplies previous settings  | 2 is scale factor, 2-1 is additive scale factor
		p->IKr_va_ss_kscale *= (1.0 + p->Remodelling_prop*(1.25 -1));     // Multiplies gradient parameter for voltage-activation steady-state. Multiplies previous settings
		p->IKs_va_tau_scale *= (1.0 + p->Remodelling_prop*(0.75 -1));     // Multiplies time constant of voltage activation. Multiplies previous settings.
		p->Gup              *= (1.0 + p->Remodelling_prop*(2.0  -1));     // Scales intracellular upatke rate. Multiplies previous settings.
		p->ICaL_vi_ss_shift += 5*p->Remodelling_prop;                     // Shifts the voltage dependence of the steady state of inactivation gate. Summed to previous settings
		p->gKur             = 0.003;  
	}
	// Add new Remodelling here: else if (strcmp(p->Remodelling, "X") == 0) {   }
	else
	{
		printf("ERROR: \"%s\" is not a valid Remodelling model for the hAM_GB models. Please check Model.c and Model_hAM_GB.cpp for options\n\n", p->Remodelling);
		exit(1);
	}
}

// Mutation
void set_modulation_Mutation_native_hAM_GB(Cell_parameters *p)
{
	// Can modify parameters (e.g. conductance) directly, or scaling factors.
	// (conductance denoted "g", scale factor "G")
	// In general, use the scale factors as these are multiplicative/additive throughout
	// the code - if a parameter is set (rather than modified) it will overwrite previous
	// settings; this may or may not be desired

	// NOTE: Many mutations may be common to multiple models and should be in Model.c
	// Add only a model-specific remodelling here

	if (strcmp(p->Mutation, "none") == 0); // do nothing
	// testing exmaple illustration of model-specific implementation
	else if (strcmp(p->Mutation, "test") == 0)
	{
		p->Gto              *= 2;       // Scale factor = Multiplies previous settings
		p->gKur             = 0.003;    // Actual conductance explicitly set. Will overwrite any previous settings of g, but not scale factor mods. 
		p->IKr_va_ss_kscale *= 1.25;    // Multiplies gradient parameter for voltage-activation steady-state. Multiplies previous settings
		p->ICaL_vi_ss_shift += 5;       // Shifts the voltage dependence of the steady state of inactivation gate. Summed to previous settings
		p->IKs_va_tau_scale *= 0.75;    // Multiplies time constant of voltage activation. Multiplies previous settings.
		p->Gup              *= 2;       // Scales intracellular upatke rate. Multiplies previous settings.
	}
	// Add new mutation here: else if (strcmp(p->mutation, "X") == 0) {   }
	else
	{
		printf("ERROR: \"%s\" is not a valid Mutation model for the hAM_GB models. Please check Model.c and Model_hAM_GB.cpp for options\n\n", p->Mutation);
		exit(1);
	}
}

// ACh ===================================\\|
void set_modulation_ACh_hAM_GB(Cell_parameters *p)
{
	// Can modify parameters (e.g. conductance) directly, or scaling factors.
	// (conductance denoted "g", scale factor "G")
	// In general, use the scale factors as these are multiplicative/additive throughout
	// the code - if a parameter is set (rather than modified) it will overwrite previous
	// settings; this may or may not be desired

	// Note: setting X_set_ref to 1 and then 0 if not set is important for error checking
	// and for being able to jump into global/common AND specific functions properly

	// NOTE: if the model does not have IKACh (or If) in, then modifying them won't make a difference!
	/*if (strcmp(p->ACh_model, "default") == 0)
	  {
	// Put actual things in here
	}
	else*/ if (strcmp(p->ACh_model, "test") == 0)  // just an example of how to implement: NOT an actual implementation
	{
		p->gKACh    = p->gKACh_max * ( pow(p->ACh, 1.5) / ( pow(2.8e-1, 1.5) + pow(p->ACh, 1.5) ) );
		p->If_va_ss_shift += -7.2*p->ACh;
	}
	else
	{
		printf("ERROR: \"%s\" is not a valid ACh for the hAM_GB model. Please check Model_hAM_GB.cpp for options\n\n", p->ACh_model);
		exit(1);
	}

}
// End ACh ===============================//|
// End heterogeneity and modulation =============================================================//|

// Compute model functions ======================================================================\\|
void compute_model_hAM_GB_native(Cell_parameters p, Model_variables *var, State_variables *s, double Vm, double dt)
{
	compute_reversal_potentials(p, var, s);
	set_gate_rates_hAM_GB_native(p, var, Vm, s->Cai);
	update_gating_variables_hAM_GB_native(p, var, s, Vm, dt);
	compute_Itot_hAM_GB_native(p, var, s, Vm);
	comp_homeostasis_hAM_GB(p, var, s, Vm, dt);
}

void set_gate_rates_hAM_GB_native(Cell_parameters p, Model_variables *var, double Vm, double Cai)
{
	set_INa_LR_rates(p, var, Vm);						// lib/Model.c
	set_INaL_hAM_GB_rates(p, var, Vm);					
	set_IKr_hAM_GB_rates(p, var, Vm);	
	set_IKs_hAM_GB_rates(p, var, Vm);	
	set_IK1_hAM_GB_variables(p, var, Vm);
	set_Ito_hAM_GB_rates(p, var, Vm);
	set_IKur_hAM_MT_rates(p, var, Vm);	
	set_ICaL_hAM_GB_rates(p, var, Vm, Cai);
}

void update_gating_variables_hAM_GB_native(Cell_parameters p, Model_variables *var, State_variables *s, double Vm, double dt)
{
	update_gates_INa_LR(p, var, s, Vm, dt); 		// lib/Model.c
	update_gates_INaL_hAM_GB(p, var, s, Vm, dt); 	
	update_gates_IKr_hAM_GB(p, var, s, Vm, dt);
	update_gates_IKs_hAM_GB(p, var, s, Vm, dt);
	update_gates_ICaL_hAM_GB(p, var, s, Vm, dt);
	update_gates_Ito_hAM_MT(p, var, s, Vm, dt);
	update_gates_IKur_hAM_MT(p, var, s, Vm, dt);
}

void compute_Itot_hAM_GB_native(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	var->Itot   = 0;

	compute_INa_hAM_GB(p, var, s, Vm);			
	compute_INaL_hAM_GB(p, var, s, Vm);		
	compute_IK1_hAM_GB(p, var, s, Vm);
	compute_INab_hAM_GB(p, var, s, Vm);
	compute_IKb_hAM_GB(p, var, s, Vm);
	compute_ICab_hAM_GB(p, var, s, Vm);
	compute_ICaP_hAM_GB(p, var, s, Vm);
	compute_INCX_hAM_GB(p, var, s, Vm);
	compute_ICaL_hAM_GB(p, var, s, Vm);
	compute_IKr_hAM_GB(p, var, s, Vm);
	compute_IKs_hAM_GB(p, var, s, Vm);
	compute_INaK_hAM_GB(p, var, s, Vm);
	compute_IClCa_hAM_GB(p, var, s, Vm);
	compute_IClb_hAM_GB(p, var, s, Vm);
	compute_Ito_hAM_MT(p, var, s, Vm);
	compute_IKur_hAM_MT(p, var, s, Vm);
	compute_ICaL_hAM_GB(p, var, s, Vm);

	if (strcmp(p.environment, "isolated") == 0)
	{
		compute_IK1_hAM_WL_isolated(p, var, s, Vm); // overwrites previous calculation
		var->IKr = var->IKs = 0;
	}

	var->Itot   = var->INa + var->IK1 + var->INab + var->IKb + var->ICab + var->ICaP + var->INCX + var->INaL + var->Ito + var->ICaL + var->IKur + var->INaK + var->IClCa + var->IClb + var->IKr + var->IKs;
	if (strcmp(p.environment, "isolated") == 0) var->Itot +=  p.AIhyp;
}
// End Compute model functions ==================================================================//|

// Current formulations =========================================================================\\|
// INa ======================================================================\\|
// Rates and update gates same as LR model, lib/Model.c
void compute_INa_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	var->INa_sl    		= (1 - p.Fjunc) * p.gNa * pow(s->INa_va, 3) * s->INa_vi_1 * s->INa_vi_2 * (Vm - var->ENa_sl);
	var->INa_j  		= (    p.Fjunc) * p.gNa * pow(s->INa_va, 3) * s->INa_vi_1 * s->INa_vi_2 * (Vm - var->ENa_j);
	var->INa_sl			*= p.GNa;
	var->INa_j			*= p.GNa;
	var->INa			= var->INa_sl + var->INa_j;
}
// End INa ==================================================================//|

// INaL =====================================================================\\|
void set_INaL_hAM_GB_rates(Cell_parameters p, Model_variables *var, double Vm)
{
	double Vm_ac        = Vm - p.INaL_va_shift;  // Shift of the voltage used to calculate alpha and beta, activation
	double Vm_inac      = Vm - p.INaL_vi_shift;  // Shift of the voltage used to calculate alpha and beta, inactivation

	// Voltage activation
	var->INaL_va_al             = 0.32 * (Vm_ac + 47.13) / (1.0 - exp(-0.1 * (Vm_ac + 47.13)));
	var->INaL_va_bet            = 8 * exp((-Vm_ac) / 11.0);

	var->INaL_va_ss             = var->INaL_va_al/(var->INaL_va_al + var->INaL_va_bet); // al/(al+b)
	var->INaL_va_tau       		= 1.0/(var->INaL_va_al + var->INaL_va_bet);
	var->INaL_va_tau			*= p.INaL_va_tau_scale;	

	// Voltage inactivation
	var->INaL_vi_ss     	  	= 1.0 / (1.0 + exp((Vm_inac - (-91.0)) / 6.1));
	var->INaL_vi_tau 	      	= 600.0;
	var->INaL_vi_tau			*= p.INaL_vi_tau_scale;	
}

void update_gates_INaL_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm, double dt)
{
	s->INaL_va              = rush_larsen(s->INaL_va, var->INaL_va_ss, var->INaL_va_tau, dt);
	s->INaL_vi              = rush_larsen(s->INaL_vi, var->INaL_vi_ss, var->INaL_vi_tau, dt);
}

void compute_INaL_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	var->INaL_sl        = (1 - p.Fjunc)  * p.gNaL * pow(s->INaL_va, 3) * s->INaL_vi * (Vm - var->ENa_sl);
	var->INaL_j         = (    p.Fjunc)  * p.gNaL * pow(s->INaL_va, 3) * s->INaL_vi * (Vm - var->ENa_j);
	var->INaL_sl        *= p.GNaL;
	var->INaL_j         *= p.GNaL;
	var->INaL           = var->INaL_sl + var->INaL_j;
}
// End INaL =================================================================//|

// Ito ======================================================================\\|
// MT formulation lib/Model_hAM_MT.cpp (with slight different tau vi and V1/2 ac ss -1 not +1)
void set_Ito_hAM_GB_rates(Cell_parameters p, Model_variables *var, double Vm)
{
	double Vm_ac_ss         = Vm - p.Ito_va_ss_shift;   // Voltage modified by shift applied to activation steady state
	double Vm_inac_ss       = Vm - p.Ito_vi_ss_shift;   // Voltage modified by shift applied to inactivation steady state
	double Vm_ac_tau        = Vm - p.Ito_va_tau_shift;  // Voltage modified by shift applied to activation time constant
	double Vm_inac_tau      = Vm - p.Ito_vi_tau_shift;  // Voltage modified by shift applied to inactivation time constant

	// Voltage activation   
	var->Ito_va_ss          = sigmoid(Vm_ac_ss, -1.0, -11.0*p.Ito_va_ss_kscale); // Vm, V1/2, k  1/(1+exp((V-V1/2)/k)
	var->Ito_va_tau         = 1000*(0.0035 * exp(-(Vm_ac_tau/30.0)*(Vm_ac_tau/30.0)) + 0.0015);
	var->Ito_va_tau         *= p.Ito_va_tau_scale;

	// Voltage inactivation
	var->Ito_vi_ss          = sigmoid(Vm_inac_ss, -40.5, 11.5*p.Ito_vi_ss_kscale); // Vm, V1/2, k  1/(1+exp((V-V1/2)/k)
	var->Ito_vi_tau         = 1000*(0.025635 * exp (-((Vm_inac_tau+52.45)/15.8827)*((Vm_inac_tau+52.45)/15.8827)) + 0.02414);
	var->Ito_vi_tau         *= p.Ito_vi_tau_scale;
}
// End Ito ==================================================================//|

// ICaL =====================================================================\\|
void set_ICaL_hAM_GB_rates(Cell_parameters p, Model_variables *var, double Vm, double Cai)
{
	double Vm_ac_ss         = Vm - p.ICaL_va_ss_shift;    // Voltage modified by shift applied to activation steady state
	double Vm_inac_ss       = Vm - p.ICaL_vi_ss_shift;    // Voltage modified by shift applied to inactivation steady state
	double Vm_ac_tau        = Vm - p.ICaL_va_tau_shift;   // Voltage modified by shift applied to activation time constant
	double Vm_inac_tau      = Vm - p.ICaL_vi_tau_shift;   // Voltage modified by shift applied to inactivation time constant

	set_ICaL_hAM_GB_va_rates(p, &var->ICaL_va_ss, &var->ICaL_va_tau, Vm_ac_ss, Vm_ac_tau, p.ICaL_va_ss_kscale);
	var->ICaL_va_tau        *= p.ICaL_va_tau_scale;

	set_ICaL_hAM_GB_vi_rates(p, &var->ICaL_vi_ss, &var->ICaL_vi_tau, Vm_inac_ss, Vm_inac_tau, p.ICaL_vi_ss_kscale);
	var->ICaL_vi_tau        *= p.ICaL_vi_tau_scale;

	//var->ICaL_ci_al       = 1.7; // Set explicitly in update gates function
	//var->ICaL_ci_bet      = 11.9e-3;
}

void set_ICaL_hAM_GB_va_rates(Cell_parameters p, double *va_ss, double *va_tau, double Vm_ss, double Vm_tau, double kscale)
{
	*va_ss                  = sigmoid(Vm_ss, -9.0, -6.0*kscale);  // V, V1/2, k
	*va_tau                 = 1.0 * (*va_ss) * (1.0 - exp(-(Vm_tau - (-9.0)) / 6.0)) / (0.035 * (Vm_tau - (-9.0)));
}

void set_ICaL_hAM_GB_vi_rates(Cell_parameters p, double *vi_ss, double *vi_tau, double Vm_ss, double Vm_tau, double kscale)
{
	*vi_ss                  = 1.0 / (1.0 + exp((Vm_ss - (-30.0)) / 7.0)) + 0.2 / (1.0 + exp((50.0 - Vm_ss) / 20.0));
	*vi_tau                 = 1.0 / (0.0197 * exp(-pow(0.0337 * (Vm_tau - (-25.0)), 2.0)) + 0.02);
}

void update_gates_ICaL_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm, double dt)
{
	s->ICaL_va       		= rush_larsen(s->ICaL_va, var->ICaL_va_ss, var->ICaL_va_tau, dt);
	s->ICaL_vi    			= rush_larsen(s->ICaL_vi, var->ICaL_vi_ss, var->ICaL_vi_tau, dt);
	s->ICaL_ci				+= dt*(1.7 * s->Cai_sl * (1.0 - s->ICaL_ci) - 11.9e-3 * s->ICaL_ci);
	s->ICaL_ci_j			+= dt*(1.7 * s->Cai_j * (1.0 - s->ICaL_ci_j) - 11.9e-3 * s->ICaL_ci_j);
}

double compute_ICaL_bar_hAM_GB(Cell_parameters p, Model_variables *var, double Vm, double Cai, double Cao)
{
	double ICaL_bar, Vm_in;
	Vm_in = Vm;
	if (Vm == 0) Vm_in = 1e-10;
	ICaL_bar	= 	0.45* 4.0 * Vm_in * 1000*p.F * (p.FoRT) * (0.341 * Cai * exp(2.0 * Vm_in * p.FoRT) - 0.341 * Cao) / (exp(2.0 * Vm_in * p.FoRT) - 1.0);
	return ICaL_bar;
}

double compute_ICaL_bar_Na_hAM_GB(Cell_parameters p, Model_variables *var, double Vm, double Nai, double Nao)
{
	double ICaL_bar, Vm_in;
	Vm_in = Vm;
	if (Vm == 0) Vm_in = 1e-10;
	ICaL_bar	=	0.45 * Vm_in * 1000*p.F * p.FoRT * (0.75 * Nai * exp(Vm_in * p.FoRT) - 0.75 * Nao) / (exp(Vm_in * p.FoRT) - 1.0);
	return ICaL_bar;
}

double compute_ICaL_bar_K_hAM_GB(Cell_parameters p, Model_variables *var, double Vm, double Ki, double Ko)
{
	double ICaL_bar, Vm_in;
	Vm_in = Vm;
	if (Vm == 0) Vm_in = 1e-10;
	ICaL_bar	= 	0.45 * Vm_in * 1000*p.F * p.FoRT * (0.75 * Ki * exp(Vm_in * p.FoRT) - 0.75 * Ko) / (exp(Vm_in * p.FoRT) - 1.0);
	return ICaL_bar;
}

void compute_ICaL_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	// Ca component
	var->ICaL_bar_sl		= compute_ICaL_bar_hAM_GB(p, var, Vm, s->Cai_sl, s->Cao); 
	var->ICaL_bar_j			= compute_ICaL_bar_hAM_GB(p, var, Vm, s->Cai_j, s->Cao); 

	var->ICaL_Ca_sl   		=  (1 - p.Fjunc_ICaL) * p.pCaL * var->ICaL_bar_sl 	* s->ICaL_va * s->ICaL_vi * (1 - s->ICaL_ci);
	var->ICaL_Ca_j   		=  (    p.Fjunc_ICaL) * p.pCaL * var->ICaL_bar_j 	* s->ICaL_va * s->ICaL_vi * (1 - s->ICaL_ci_j);
	var->ICaL_Ca_sl			*= p.GCaL;
	var->ICaL_Ca_j			*= p.GCaL;
	var->ICaL				= var->ICaL_Ca_sl + var->ICaL_Ca_j;

	// Na and K components
	var->ICaL_bar_Na_sl		= compute_ICaL_bar_Na_hAM_GB(p, var, Vm, s->Nai_sl, s->Nao);
	var->ICaL_bar_Na_j		= compute_ICaL_bar_Na_hAM_GB(p, var, Vm, s->Nai_j, s->Nao);

	var->ICaL_Na_sl			= (1 - p.Fjunc_ICaL) * p.pCaL_Na * var->ICaL_bar_Na_sl * s->ICaL_va * s->ICaL_vi * (1 - s->ICaL_ci);
	var->ICaL_Na_j			= (    p.Fjunc_ICaL) * p.pCaL_Na * var->ICaL_bar_Na_j  * s->ICaL_va * s->ICaL_vi * (1 - s->ICaL_ci_j);
	var->ICaL_Na_sl         *= p.GCaL;
	var->ICaL_Na_j          *= p.GCaL;

	var->ICaL_bar_K			= compute_ICaL_bar_K_hAM_GB(p, var, Vm, s->Ki, s->Ko);
	var->ICaL_K				= p.pCaL_K * var->ICaL_bar_K * s->ICaL_va * s->ICaL_vi * (p.Fjunc_ICaL*(1 - s->ICaL_ci_j) + (1 - p.Fjunc_ICaL)*(1 - s->ICaL_ci));
	var->ICaL_K         	*= p.GCaL;

	var->ICaL				+= var->ICaL_Na_sl + var->ICaL_Na_j + var->ICaL_K;
}
// End ICaL =================================================================//|

// IKur =====================================================================\\|
// MT formulation lib/Model_hAM_MT.cpp
// End IKur =================================================================//|

// IKr ======================================================================\\|
void set_IKr_hAM_GB_rates(Cell_parameters p, Model_variables *var, double Vm)
{
	double Vm_ac_ss         = Vm - p.IKr_va_ss_shift;   // Voltage modified by shift applied to activation steady state
	double Vm_ac_tau        = Vm - p.IKr_va_tau_shift;  // Voltage modified by shift applied to activation time constant

	// Voltage activation   
	var->IKr_va_ss         	= sigmoid(Vm_ac_ss, -10, -5.0*p.IKr_va_ss_kscale); // Vm, V1/2, k  1/(1+exp((V-V1/2)/k)
	var->IKr_va_tau        	=  550.0 / (1.0 + exp((-22.0 - Vm_ac_tau) / 9.0)) * 6.0 / (1.0 + exp((Vm_ac_tau - (-11.0)) / 9.0))
		+ 230.0 / (1.0 + exp((Vm_ac_tau - (-40.0)) / 20.0));// ms
	var->IKr_va_tau         *= p.IKr_va_tau_scale;

	// Time-independant gate
	var->IKr_vi_ti			= sigmoid(Vm_ac_ss, -74, 24);
}

void update_gates_IKr_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm, double dt)
{
	s->IKr_va              = rush_larsen(s->IKr_va, var->IKr_va_ss, var->IKr_va_tau, dt);
}

void compute_IKr_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	var->IKr               =  p.gKr * s->IKr_va * var->IKr_vi_ti * (Vm - var->EK);
	var->IKr               *= p.GKr;
}
// End IKr ==================================================================//|

// IKs ======================================================================\\|
void set_IKs_hAM_GB_rates(Cell_parameters p, Model_variables *var, double Vm)
{
	double Vm_ac_ss         = Vm - p.IKs_va_ss_shift;   // Voltage modified by shift applied to activation steady state
	double Vm_ac_tau        = Vm - p.IKs_va_tau_shift;  // Voltage modified by shift applied to activation time constant

	// Voltage activation   
	var->IKs_va_ss          = sigmoid(Vm_ac_ss, -3.8, -14.25*p.IKs_va_ss_kscale); // Vm, V1/2, k  1/(1+exp((V-V1/2)/k)
	var->IKs_va_tau         = 990.1 / (1.0 + exp(-(Vm_ac_tau - (-2.436)) / 14.12));	// ms
	var->IKs_va_tau         *= p.IKs_va_tau_scale;
}

void update_gates_IKs_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm, double dt)
{
	s->IKs_va              = rush_larsen(s->IKs_va, var->IKs_va_ss, var->IKs_va_tau, dt);
}

void compute_IKs_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	var->IKs               	=  p.gKs * s->IKs_va * s->IKs_va * (Vm - var->EK);
	var->IKs               	*= p.GKs;
	var->IKs_sl				= (1 - p.Fjunc)*var->IKs;
	var->IKs_j				= (	   p.Fjunc)*var->IKs;
}
// End IKs ==================================================================//|

// IK1 ======================================================================\\|
void set_IK1_hAM_GB_variables(Cell_parameters p, Model_variables *var, double Vm)
{
	double Vm_in 		= Vm - p.IK1_va_shift;
	double ak1 			= 1.02 / (1.0 + exp(0.2385 * (Vm_in - var->EK - 59.215)));
	double bk1 			= (0.49124 * exp(0.08032 * (Vm_in + 5.476 - var->EK)) + exp(0.06175 * (Vm_in - var->EK - 594.31))) / (1.0 + exp(-0.5143 * (Vm_in - var->EK + 4.753)));
	double kiss 		= ak1 / (ak1 + bk1);
	var->IK1_va_ti      = sqrt(p.Ko / 5.4)*kiss;
}

void compute_IK1_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	var->IK1               = p.gK1 * (Vm - var->EK - p.IK1_Erev_shift) * var->IK1_va_ti;
	var->IK1               *= p.GK1;
}
// End IK1 ==================================================================//|

// Ca2+ handling, background and pump currents ==============================\\|
// INCX
void compute_INCX_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	// Variables to be calculated, but only needed locally
	double Ka, s1, s2, s3;

	// Create some local variables to make equation easier to read
	double Cai,Cao, Nai, Nao;

	Cao					= s->Cao;
	Nao					= s->Nao;

	// Junctional
	Cai		 			= s->Cai_j;
	Nai					= s->Nai_j;
	Ka					= 1.0 / (1.0 + pow(p.INCX_kda/s->Cai_j, 2.0)); 
	s1					= exp(p.INCX_gamma * Vm * p.FoRT) * pow(Nai, 3.0) * Cao; 
	s2					= exp((p.INCX_gamma - 1.0) * Vm * p.FoRT) * pow(Nao, 3.0) * Cai;
	s3					= p.INCX_kCai * pow(Nao, 3.0) * (1.0 + pow(Nai / p.INCX_kNai, 3.0))
		+ pow(p.INCX_kNao, 3.0) * Cai * (1.0 + Cai / p.INCX_kCai) + p.INCX_kCao * pow(Nai, 3.0)
		+ pow(Nai, 3.0) * Cao + pow(Nao, 3.0) * Cai;

	var->INCX_j			= p.Fjunc * p.INCX_bar * Ka * (s1 - s2) / s3 / (1.0 + p.INCX_ksat * exp((p.INCX_gamma - 1.0) * Vm * p.FoRT));

	// Sub-sarcolemmal
	Cai                 = s->Cai_sl;
	Nai                 = s->Nai_sl;
	Ka                  = 1.0 / (1.0 + pow(p.INCX_kda/s->Cai_sl, 2.0));
	s1                  = exp(p.INCX_gamma * Vm * p.FoRT) * pow(Nai, 3.0) * Cao;
	s2                  = exp((p.INCX_gamma - 1.0) * Vm * p.FoRT) * pow(Nao, 3.0) * Cai;
	s3                  = p.INCX_kCai * pow(Nao, 3.0) * (1.0 + pow(Nai / p.INCX_kNai, 3.0))
		+ pow(p.INCX_kNao, 3.0) * Cai * (1.0 + Cai / p.INCX_kCai) + p.INCX_kCao * pow(Nai, 3.0)
		+ pow(Nai, 3.0) * Cao + pow(Nao, 3.0) * Cai;

	var->INCX_sl        = (1 - p.Fjunc) * p.INCX_bar * Ka * (s1 - s2) / s3 / (1.0 + p.INCX_ksat * exp((p.INCX_gamma - 1.0) * Vm * p.FoRT));

	var->INCX_sl        *= p.GNCX;
	var->INCX_j         *= p.GNCX;
	var->INCX			= var->INCX_sl + var->INCX_j;
}

// INaK
void compute_INaK_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	double sigma		= (exp(s->Nao/67.3)-1.0)/7.0;
	double FNaK			= pow(1.0+0.1245*exp(-0.1*p.F*Vm/(p.R*p.T))+0.0365*sigma*exp(-Vm*p.FoRT ), -1.0);
	var->INaK_sl		= (1 - p.Fjunc) * p.INaK_bar * FNaK * (1.0/(1.0+pow((p.INaK_kNa /s->Nai_sl),4.0))) * (s->Ko/(s->Ko + p.INaK_kK));
	var->INaK_j			= (    p.Fjunc) * p.INaK_bar * FNaK * (1.0/(1.0+pow((p.INaK_kNa /s->Nai_j),4.0))) * (s->Ko/(s->Ko + p.INaK_kK));
	var->INaK_sl        *= p.GNaK;
	var->INaK_j         *= p.GNaK;
	var->INaK			= var->INaK_sl + var->INaK_j;
}

// ICaP
void compute_ICaP_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	var->ICaP_sl		= (1 - p.Fjunc) * p.ICaP_bar * pow(s->Cai_sl, 1.6)/(pow(p.ICaP_kCa, 1.6) + pow(s->Cai_sl, 1.6));
	var->ICaP_j			= (    p.Fjunc) * p.ICaP_bar * pow(s->Cai_j, 1.6)/(pow(p.ICaP_kCa, 1.6) + pow(s->Cai_j, 1.6));
	var->ICaP_sl		*= p.GCaP;
	var->ICaP_j			*= p.GCaP;
	var->ICaP			= var->ICaP_sl + var->ICaP_j; 
}

// INab
void compute_INab_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	var->INab_sl		= (1 - p.Fjunc) * p.gNab * (Vm - var->ENa_sl);	
	var->INab_j 		= (    p.Fjunc) * p.gNab * (Vm - var->ENa_j);	
	var->INab_sl		*= p.GNab;
	var->INab_j			*= p.GNab;
	var->INab			= var->INab_sl + var->INab_j;
}

// IKb
void compute_IKb_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	var->IKb           	= p.gKb * (Vm - var->EK) * (1.0 / (1.0 + exp(7.488 - Vm / 5.98)));
	var->IKb           *= p.GKb;
}

// ICab
void compute_ICab_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	var->ICab_sl		= (1 - p.Fjunc) * p.gCab * (Vm - var->ECa_sl);	
	var->ICab_j 		= (    p.Fjunc) * p.gCab * (Vm - var->ECa_j);	
	var->ICab_sl		*= p.GCab;
	var->ICab_j			*= p.GCab;
	var->ICab			= var->ICab_sl + var->ICab_j;
}

// IClCa
void compute_IClCa_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	var->IClCa_sl 	 	= (1 - p.Fjunc) * p.gClCa / (1.0 + p.IClCa_kd / s->Cai_sl) * (Vm - var->ECl);
	var->IClCa_j 	 	= (    p.Fjunc) * p.gClCa / (1.0 + p.IClCa_kd / s->Cai_j ) * (Vm - var->ECl);
	var->IClCa_sl		*= p.GClCa;
	var->IClCa_j		*= p.GClCa;
	var->IClCa			= var->IClCa_sl + var->IClCa_j;
}

// IClb
void compute_IClb_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm)
{
	var->IClb			= 1.0 * 9.0e-3 * (Vm - var->ECl);
}
// End Ca2+ handling currents ===============================================//|

// Homeostasis ==============================================================\\|
void comp_homeostasis_hAM_GB(Cell_parameters p, Model_variables *var, State_variables *s, double Vm, double dt)
{
	// Ca current totals in each compartment
	double ICa_tot_sl	= var->ICaL_Ca_sl + var->ICab_sl + var->ICaP_sl - 2.0*var->INCX_sl;
	double ICa_tot_j	= var->ICaL_Ca_j  + var->ICab_j  + var->ICaP_j  - 2.0*var->INCX_j;

	double beta;
	double dCai_j, dCai_sl, dCai, dCaSR;

	// Junctional space =======================\\|
	// Buffering
	beta				= 1.0 + p.Bmax_SLlowj * p.koff_sll / p.kon_sll / (p.koff_sll / p.kon_sll + s->Cai_j) / (p.koff_sll / p.kon_sll + s->Cai_j) + p.Bmax_SLhighj * p.koff_slh  / p.kon_slh / (p.koff_slh / p.kon_slh + s->Cai_j) / (p.koff_slh / p.kon_slh + s->Cai_j);
	beta				= 1.0;///beta;

	// Jrel and Jleak
	var->J_rel			= p.ks * (s->RyRo)/ 1.0 * (s->CanSR - s->Cai_j);
	var->J_rel			*= p.Grel;
	var->J_leak			= 5.348e-6 * (s->CanSR - s->Cai_j);
	var->J_leak			*= p.Gleak;

	// Update Cai_j
	//s->Cai_j += (dt * beta * ( (-ICa_tot_j * p.Cm_F / (p.Vjunc * 2.0 * 1000*p.F) + p.J_ca_juncsl / p.Vjunc * (s->Cai_sl - s->Cai_j) + var->J_rel * p.VnSR / p.Vjunc + var->J_leak * p.Vcyto / p.Vjunc) ));
	dCai_j = /*beta */ (-ICa_tot_j * p.Cm_F / (p.Vjunc * 2.0 * 1000*p.F) + p.J_ca_juncsl / p.Vjunc * (s->Cai_sl - s->Cai_j) + var->J_rel * p.VnSR / p.Vjunc + var->J_leak * p.Vcyto / p.Vjunc);
	// End Junctional space ====================//|

	// sub-sarcolemmal space ===================\\|
	beta 				= 1.0 + p.Bmax_SLlowsl * p.koff_sll / p.kon_sll / (p.koff_sll / p.kon_sll + s->Cai_sl) / (p.koff_sll / p.kon_sll + s->Cai_sl) + p.Bmax_SLhighsl * p.koff_slh / p.kon_slh / (p.koff_slh / p.kon_slh + s->Cai_sl) / (p.koff_slh / p.kon_slh + s->Cai_sl);
	beta				= 1.0/beta;

	// Update Cai_sl
	//s->Cai_sl  += dt * beta * ( (-ICa_tot_sl * p.Cm_F / (p.Vsl * 2.0 * 1000*p.F) + p.J_ca_juncsl / p.Vsl * (s->Cai_j - s->Cai_sl) + p.J_ca_slmyo / p.Vsl * (s->Cai - s->Cai_sl))  );
	dCai_sl  = beta * ( (-ICa_tot_sl * p.Cm_F / (p.Vsl * 2.0 * 1000*p.F) + p.J_ca_juncsl / p.Vsl * (s->Cai_j - s->Cai_sl) + p.J_ca_slmyo / p.Vsl * (s->Cai - s->Cai_sl))  );
	// End sub-sarcolemmal space ===============//|

	// Bulk cytosol ============================\\|
	// Jup
	var->J_SERCA  = p.Gup * p.J_SERCA_max * (pow(s->Cai / p.Kmf, p.hillSRCaP) - pow(s->CanSR / p.Kmr, p.hillSRCaP))
		/ (1.0 + pow(s->Cai / p.Kmf, p.hillSRCaP) + pow(s->CanSR / p.Kmr, p.hillSRCaP));

	// Buffering
	var->dMyo_c   = p.kon_myoca * s->Cai * (p.Bmax_myosin - s->Myo_c - s->Myo_m) - p.koff_myoca * s->Myo_c;
	var->dMyo_m   = p.kon_myomg * p.Mgi * (p.Bmax_myosin - s->Myo_c - s->Myo_m) - p.koff_myomg * s->Myo_m;
	var->dTn_CHc  = p.kon_tnchca * s->Cai * (p.Bmax_TnChigh - s->Tn_CHc - s->Tn_CHm) - p.koff_tnchca * s->Tn_CHc;
	var->dTn_CHm  = p.kon_tnchmg * p.Mgi * (p.Bmax_TnChigh - s->Tn_CHc - s->Tn_CHm) - p.koff_tnchmg * s->Tn_CHm;
	var->dTn_CL   = p.kon_tncl * s->Cai * (p.Bmax_TnClow - s->Tn_CL) - p.koff_tncl * s->Tn_CL;

	s->Tn_CHm   += dt * var->dTn_CHm;
	s->Tn_CHc   += dt * var->dTn_CHc;
	s->Myo_m    += dt * var->dMyo_m;
	s->Myo_c    += dt * var->dMyo_c;
	s->Tn_CL    += dt * var->dTn_CL;

	var->J_CaB_cytosol = var->dTn_CL + var->dTn_CHc + var->dTn_CHm + var->dMyo_c + var->dMyo_m;

	beta     = 1.0 + p.Bmax_CaM * p.koff_cam / p.kon_cam / (p.koff_cam / p.kon_cam + s->Cai) / (p.koff_cam / p.kon_cam + s->Cai) + p.Bmax_SR * p.koff_sr / p.kon_sr / (p.koff_sr / p.kon_sr + s->Cai) / (p.koff_sr / p.kon_sr + s->Cai);
	beta     = 1.0/beta;

	// Update Cai
	//s->Cai      += dt * beta * ( (- var->J_SERCA * p.VnSR / p.Vcyto - var->J_CaB_cytosol + p.J_ca_slmyo / p.Vcyto * (s->Cai_sl - s->Cai)) );
	dCai      = beta * ( (- var->J_SERCA * p.VnSR / p.Vcyto - var->J_CaB_cytosol + p.J_ca_slmyo / p.Vcyto * (s->Cai_sl - s->Cai)) );
	// End Bulk cytosol ========================//|

	// Casr ====================================\\|
	beta     = 1.0 + p.Bmax_Csqn * p.koff_csqn / p.kon_csqn / (p.koff_csqn / p.kon_csqn + s->CanSR) / (p.koff_csqn / p.kon_csqn + s->CanSR);
	beta     = 1.0/beta;
	//s->CanSR     += dt * beta * ( (var->J_SERCA - (var->J_leak * p.Vcyto / p.VnSR + var->J_rel) ) );
	dCaSR     = beta * ( (var->J_SERCA - (var->J_leak * p.Vcyto / p.VnSR + var->J_rel) ) );
	// End Casr ================================//|

	// RyR model ===============================\\|
	var->kCaSR    = p.MaxSR - (p.MaxSR - p.MinSR) / (1.0 + pow(p.ec50SR / s->CanSR, 2.5));
	var->koSRCa   = p.koCa / var->kCaSR;
	var->kiSRCa   = p.kiCa * var->kCaSR;
	var->RI       = 1 - s->RyRr - s->RyRo - s->RyRi;

	s->RyRr     += dt* ( p.kim * var->RI - var->kiSRCa * s->Cai_j * s->RyRr - (var->koSRCa * pow(s->Cai_j, 2.0) * s->RyRr - p.kom * s->RyRo) );
	s->RyRo     += dt* ( var->koSRCa * pow(s->Cai_j, 2.0) * s->RyRr - p.kom * s->RyRo - (var->kiSRCa * s->Cai_j * s->RyRo - p.kim * s->RyRi) );
	s->RyRi     += dt* ( var->kiSRCa * s->Cai_j * s->RyRo - p.kim * s->RyRi - (p.kom * s->RyRi - var->koSRCa * pow(s->Cai_j, 2.0) * var->RI) );
	// End RyR model ===========================//|

	// Sodium ==================================\\|
	double INa_tot_j 	=	 var->INa_j + var->INaL_j + var->INab_j + 3.0*var->INCX_j + 3.0 * var->INaK_j + var->ICaL_Na_j;
	double INa_tot_sl 	=	 var->INa_sl + var->INaL_sl + var->INab_sl + 3.0*var->INCX_sl + 3.0 * var->INaK_sl + var->ICaL_Na_sl;

	beta 	= 1.0 + p.Bmax_Naj * p.koff_na / p.kon_na / (p.koff_na / p.kon_na + s->Nai_j) / (p.koff_na / p.kon_na + s->Nai_j);
	beta 	= 1.0 / beta;
	s->Nai_j	+= dt*beta*(-INa_tot_j * p.Cm_F / (p.Vjunc * 1000*p.F) + p.J_na_juncsl / p.Vjunc * (s->Nai_sl - s->Nai_j));

	beta    = 1.0 + p.Bmax_Nasl * p.koff_na / p.kon_na / (p.koff_na / p.kon_na + s->Nai_sl) / (p.koff_na / p.kon_na + s->Nai_sl);
	beta    = 1.0 / beta;
	s->Nai_sl	+= dt*beta*(-INa_tot_sl * p.Cm_F / (p.Vsl * 1000*p.F) + p.J_na_juncsl / p.Vsl * (s->Nai_j - s->Nai_sl) + p.J_na_slmyo / p.Vsl * (s->Nai - s->Nai_sl));

	s->Nai		+= dt*(p.J_na_slmyo / p.Vcyto * (s->Nai_sl - s->Nai));
	// End Sodium ==============================//|

	// Potassium ===============================\\|
	//s->Ki		+= dt*(var->Ito + var->IKr + var->IKs + var->ICaL_K + var->IKb + var->IKur - 2*var->INaK)*(p.Cm_F / (p.Vcyto * 1000*p.F)); // Cm/VF added by me
	// dKi set to zero in code. Correct??
	// End potassium ===========================//|

	// And finally update Ca ===================\\|
	s->Cai_j 	+= dt*dCai_j;
	s->Cai_sl 	+= dt*dCai_sl;
	s->Cai 		+= dt*dCai;
	s->CanSR 	+= dt*dCaSR;
	// And finally update Ca ===================//|
}
// End Homeostasis ==========================================================//|
// End Current formulations =====================================================================//|
