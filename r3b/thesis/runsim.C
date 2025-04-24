/** --------------------------------------------------------------------
 **
 **  Define the simulation setup for the S522 experiment
 **  Author: <joseluis.rodriguez.sanchez@usc.es>
 **
 **  Last Update: 10/05/2022
 **  Comments:
 **         - 22/07/21 : Initial setup
 **
 **  Configuration:
 **  (1) Select the right generator "fGenerator"
 **  (2) Select the detectors that you wish for the simulation, for instance,
 *"fCalifa = true"
 **  (3) Look at the file "Setup.par" that the positions of your detectors
 *are right
 **
 **  Execute it as follows:
 **  root -l 'runsim.C(1000)'
 **  where 1000 means the number of events
 **
 **/

void runsim(Int_t nEvents = 1) {
	// ----------- Configuration area ----------------------------------
	const Double_t Target_to_GLAD_flange = 106.2;
	const Double_t Target_to_TP          = Target_to_GLAD_flange + 165.0;
	const Double_t Target_to_RP          = Target_to_GLAD_flange +  54.05 - 0.55580;

	//TString OutFile = "/lustre/land/mxarepe/sim_out/sim_QFS_11B_old_geo_no_cali_rand.root"; // Output file for data
	//TString ParFile = "/lustre/land/mxarepe/sim_out/par_QFS_11B_old_geo_no_cali_rand.root"; // Output file for params

	//TString OutFile = "/lustre/land/mxarepe/sim_out/sim_Mario_old_califa_no_target.root"; // Output file for data
	//TString ParFile = "/lustre/land/mxarepe/sim_out/par_Mario_old_califa_no_target.root"; // Output file for params

	//TString OutFile = "/lustre/land/mxarepe/sim_out/sim_INCL_10Be_more_1M_no_frag_arm_no_cali_rand.root"; // Output file for data
	//TString ParFile = "/lustre/land/mxarepe/sim_out/par_INCL_10Be_more_1M_no_frag_arm_no_cali_rand.root"; // Output file for params
	
	TString OutFile = "/lustre/land/mxarepe/sim_out/sim_MDF_like_protons.root"; // Output file for data
	TString ParFile = "/lustre/land/mxarepe/sim_out/par_MDF_like_protons.root"; // Output file for params

	//TString OutFile = "/lustre/land/mxarepe/sim_out/sim_src_pp_mega_strip.root"; // Output file for data
	//TString ParFile = "/lustre/land/mxarepe/sim_out/par_src_pp_mega_strip.root"; // Output file for params
	
	//TString OutFile = "/lustre/land/mxarepe/sim_out/sim_src_all_500000_4_scx.root"; // Output file for data
	//TString ParFile = "/lustre/land/mxarepe/sim_out/par_src_all_500000_4_scx.root"; // Output file for params

	//TString OutFile = "/lustre/land/mxarepe/for_christine/sim_16C_MF_fragment_v2.root"; // Output file for data
	//TString ParFile = "/lustre/land/mxarepe/for_christine/par_16C_MF_fragment_v2.root"; // Output file for params

	//TString OutFile = "/lustre/land/mxarepe/sim_out/sim_10Be_plus_proton_yosoi.root"; // Output file for data
	//TString ParFile = "/lustre/land/mxarepe/sim_out/par_10Be_plus_proton_yosoi.root"; // Output file for params

	//TString OutFile = "/lustre/land/mxarepe/sim_out/sim_Breackup_10Be_p_uni_50_MeV_e_rel_angle_10M.root"; // Output file for data
	//TString ParFile = "/lustre/land/mxarepe/sim_out/par_Breackup_10Be_p_uni_50_MeV_e_rel_angle_10M.root"; // Output file for params

//	TString OutFile = "/lustre/land/mxarepe/sim_out/sim_10Be_plus_proton_uni_e_rel_rpc_only_mega_strip.root"; // Output file for data
//	TString ParFile = "/lustre/land/mxarepe/sim_out/par_10Be_plus_proton_uni_e_rel_rpc_only_mega_strip.root"; // Output file for params

	TString fSetupFile = "Setup.par"; // Input file with the detector positions

	FairLogger::GetLogger()->SetLogScreenLevel("INFO");
	FairLogger::GetLogger()->SetColoredLog(true);

	Bool_t fVis = false;             // Store tracks for visualization
	Bool_t fUserPList = false;      // Use of R3B special physics list
	Bool_t fR3BMagnet = true;      // Magnetic field definition
	Bool_t fCalifaDigitizer = true; // Apply hit digitizer task
	Bool_t fCalifaHitFinder = true; // Apply hit finder task
	Bool_t fDetDigitizer = false; // Apply hit digitizer task

	// MonteCarlo engine: TGeant3, TGeant4, TFluka  --------------------
	TString fMC = "TGeant4";

	// Event generator type: box for particles or ascii&inclroot for p2p-fission
	TString generator1 = "box";
	TString generator2 = "ascii";
	TString generator3 = "inclroot";
	TString generator4 = "qfs";
	TString fGenerator = generator2;

	// Input event file in the case of ascii generator
	TString fEventFile;
	if (fGenerator.CompareTo("ascii") == 0)
		//fEventFile = "inputs/example_pdg_flag.txt";
		//fEventFile = "inputs/example_propagator_yosoi.txt";
		//fEventFile = "inputs/src_sim_pp_pairs.txt";
		//fEventFile = "inputs/src_sim_allpairs.txt";
		//fEventFile = "/u/land/hangqi/GCF_12C_AV18_SCX.txt";
		//fEventFile = "inputs/ascii_from_Mario.txt";
		fEventFile = "inputs/MDF_like_protons.txt";
		//fEventFile = "inputs/Breackup_10Be_p_uni_50_MeV_e_rel_angle_10M.txt";
		//fEventFile = "inputs/Proton_generator_uni_frag.txt";
		//fEventFile = "inputs/example_pdg_flag_2_e_rel.txt";
	else if (fGenerator.CompareTo("inclroot") == 0)
		fEventFile = "inputs/p_C12_B11_1250_1950132.root";
		//fEventFile = "/u/land/martinaff/p_C12_1250_large.root";

	Int_t fFieldMap = 1;          // Magentic field map selector
	Double_t fMeasCurrent = -2545.; // Magnetic field current
	//Double_t fMeasCurrent = -3420.; // Magnetic field current
	Float_t fFieldScale = -0.8;   // Magnetic field scale factor

	// ---------  Detector selection: true - false ---------------------
	// ---- R3B detectors as well as passive elements

	Bool_t fMwpc0 = false; // MWPC0 Detector
	TString fMwpc0Geo = "mwpc_0.geo.root";

	Bool_t fTwim = false; // Twin-Music Detector
	TString fTwimGeo = "twinmusic_v21.geo.root";

	Bool_t fMwpc1 = false; // MWPC1 Detector
	TString fMwpc1Geo = "mwpc_1.geo.root";

	Bool_t fTracker = true;//true; // FOOT-Tracker + Vacuum chamber + LH2 target
	//Bool_t fTracker = true;//true; // FOOT-Tracker + Vacuum chamber + LH2 target
	TString fTrackerGeo = "H2_target_area_2024_geo_522_MX_v23.geo.root";
//	TString fTrackerGeo = "targetvacuumchamber_foot_LiH_AL.geo.root";
	//TString fTrackerGeo = "targetvacuumchamber_foot_LiH.geo_2023.root";
	//TString fTrackerGeo = "create_target_area_2023_geo_509_v23.geo.root";

	Bool_t fCalifa = true; // Califa Calorimeter
	//TString fCalifaGeo = "califa_v2021.old.geo.root";
	TString fCalifaGeo = "califa_v2021.3.geo.root";
	Int_t fCalifaGeoVer = 2021;
	Double_t fCalifaCryTh = 0.001; // In GeV: 0.000080 means 80 keV per crystal
	Double_t fCalifaHitEnergyTh = 0.0; // Threshold in GeV per cluster hit
	Double_t fCalifaNonU = 0.5; // Non-uniformity: 1 means +-1% max deviation

	Bool_t fGlad = true; // Glad Magnet
	TString fGladGeo = "glad_v2023.1.geo.root";

	Bool_t fFi30 = false; // Fi30 detector
	TString fFi30Geo = "fi30_v22a.geo.root";

	Bool_t fFi31 = false; // Fi31 detector
	TString fFi31Geo = "fi31_v22a.geo.root";

	Bool_t fFi32 = false; // Fi32 detector
	TString fFi32Geo = "fi32_v22a.geo.root";

	Bool_t fFi33 = false; // Fi33 detector
	TString fFi33Geo = "fi33_v22a.geo.root";

	Bool_t fRpc = true; // RPC detector
	TString fRpcGeo = "tof_rpc_mega_strip.geo.root";

	Bool_t fTofD = false; // TofD detector
	TString fTofDGeo = "tofd_v22a.geo.root";

	Bool_t fNeuLand = false; // NeuLand Detector
	TString fNeuLandGeo = "neuland_v3_13dp.geo.root";

	// ---- End of Configuration area   ---------------------------------------

	// ---- Stable part   -----------------------------------------------------
	TString dir = gSystem->Getenv("VMCWORKDIR");
	TString pardir = dir + "/../R3BParams_S522/";
	pardir.ReplaceAll("//", "/");

	TString r3b_geomdir = dir + "/geometry/";
	gSystem->Setenv("GEOMPATH", r3b_geomdir.Data());
	r3b_geomdir.ReplaceAll("//", "/");

	TString r3b_pardir = pardir + "/geometry/";
	r3b_pardir.ReplaceAll("//", "/");

	TString r3b_confdir = dir + "/gconfig/";
	gSystem->Setenv("CONFIG_DIR", r3b_confdir.Data());
	r3b_confdir.ReplaceAll("//", "/");

	// ----    Debug option   -------------------------------------------------
	gDebug = 0;

	// -----   Create simulation run   ----------------------------------------
	FairRunSim *run = new FairRunSim();
	run->SetName(fMC);                           // Transport engine
	run->SetSink(new FairRootFileSink(OutFile)); // Output file

	// -----   Runtime data base   --------------------------------------------
	FairRuntimeDb *rtdb = run->GetRuntimeDb();

	// -----   Load detector parameters    ------------------------------------
	FairParAsciiFileIo *parIo1 = new FairParAsciiFileIo();
	parIo1->open((r3b_pardir + fSetupFile).Data(), "in");
	rtdb->setFirstInput(parIo1);
	rtdb->print();

	// ----- Containers
	auto mwpc0Par = (R3BTGeoPar*)rtdb->getContainer("Mwpc0GeoPar");
	auto musliPar = (R3BTGeoPar*)rtdb->getContainer("MusicGeoPar");
	auto mwpc1Par = (R3BTGeoPar*)rtdb->getContainer("Mwpc1GeoPar");
	auto targetPar = (R3BTGeoPar*)rtdb->getContainer("TargetGeoPar");
	auto califaPar = (R3BTGeoPar*)rtdb->getContainer("CalifaGeoPar");
	auto fi30Par = (R3BTGeoPar*)rtdb->getContainer("Fi30GeoPar");
	auto fi31Par = (R3BTGeoPar*)rtdb->getContainer("Fi31GeoPar");
	auto fi32Par = (R3BTGeoPar*)rtdb->getContainer("Fi32GeoPar");
	auto fi33Par = (R3BTGeoPar*)rtdb->getContainer("Fi33GeoPar");
	auto rpcPar = (R3BTGeoPar *)rtdb->getContainer("RpcGeoPar");
	auto tofdPar = (R3BTGeoPar*)rtdb->getContainer("tofdGeoPar");

	UInt_t runId = 1;
	rtdb->initContainers(runId);

	// -----   R3B Special Physics List in G4 case
	if ((fUserPList) && (fMC.CompareTo("TGeant4") == 0)) {
		run->SetUserConfig("g4R3bConfig.C");
		run->SetUserCuts("SetCuts.C");
	}

	// -----   Create media   -------------------------------------------------
	run->SetMaterials("media_r3b.geo"); // Materials

	// -----   Create R3B geometry --------------------------------------------

	// Cave definition
	FairModule *cave = new R3BCave("CAVE");
	cave->SetGeometryFileName("r3b_cave.geo");
	//cave->SetGeometryFileName("r3b_cave_vacuum.geo");
	run->AddModule(cave);

	// MWPC0 definition
	if (fMwpc0) {
		if (mwpc0Par) {
			mwpc0Par->printParams();
			auto rmwpc0 = new TGeoRotation("Mwpc0rot");
			rmwpc0->RotateX(mwpc0Par->GetRotX());
			rmwpc0->RotateY(mwpc0Par->GetRotY());
			rmwpc0->RotateZ(mwpc0Par->GetRotZ());
			run->AddModule(
					new R3BMwpc0(fMwpc0Geo, {mwpc0Par->GetPosX(), mwpc0Par->GetPosY(),
						mwpc0Par->GetPosZ(), rmwpc0}));
		} else
			run->AddModule(new R3BMwpc0(fMwpc0Geo, {0., 0., -190.}));
		auto mw0_digitizer = new R3BMwpcDigitizer("Mwpc0", 1);
		if (fDetDigitizer)
			run->AddTask(mw0_digitizer);
	}

	// Twim definition (Musli for S522)
	if (fTwim) {
		if (musliPar) {
			musliPar->printParams();
			auto rtwim = new TGeoRotation("Twimrot");
			rtwim->RotateX(musliPar->GetRotX());
			rtwim->RotateY(musliPar->GetRotY());
			rtwim->RotateZ(musliPar->GetRotZ());
			run->AddModule(
					new R3BTwim(fTwimGeo, {musliPar->GetPosX(), musliPar->GetPosY(),
						musliPar->GetPosZ(), rtwim}));
		} else
			run->AddModule(new R3BTwim(fTwimGeo, {-5.05, 5., -190.}));
		auto twim_digitizer = new R3BTwimDigitizer("Twim", 1);
		if (fDetDigitizer)
			run->AddTask(twim_digitizer);
	}

	// MWPC1 definition
	if (fMwpc1) {
		if (mwpc1Par) {
			mwpc1Par->printParams();
			auto rmwpc1 = new TGeoRotation("Mwpc1rot");
			rmwpc1->RotateX(mwpc1Par->GetRotX());
			rmwpc1->RotateY(mwpc1Par->GetRotY());
			rmwpc1->RotateZ(mwpc1Par->GetRotZ());
			run->AddModule(
					new R3BMwpc1(fMwpc1Geo, {mwpc1Par->GetPosX(), mwpc1Par->GetPosY(),
						mwpc1Par->GetPosZ(), rmwpc1}));
		} else
			run->AddModule(new R3BMwpc1(fMwpc1Geo, {0., 0., -190.}));
		auto mw1_digitizer = new R3BMwpcDigitizer("Mwpc1", 1);
		if (fDetDigitizer)
			run->AddTask(mw1_digitizer);
	}

	//// Tracker, vacuum chamber and LH2 target definitions
	if (fTracker) {
		if (targetPar) {
			targetPar->printParams();
			TGeoRotation *rtarget = new TGeoRotation("Targetrot");
			rtarget->RotateX(targetPar->GetRotX());
			rtarget->RotateY(targetPar->GetRotY());
			rtarget->RotateZ(targetPar->GetRotZ());
			R3BTra *tra =
				new R3BTra(fTrackerGeo, {targetPar->GetPosX(), targetPar->GetPosY(),
						targetPar->GetPosZ(), rtarget});
			tra->SetEnergyCut(1e-6); // 1 keV
			run->AddModule(tra);
		} else {
			R3BTra *tra = new R3BTra(fTrackerGeo, {0., 0., 2.0});
			tra->SetEnergyCut(1e-6); // 1 keV
			run->AddModule(tra);
		}
	}

	// CALIFA Calorimeter
	if (fCalifa) {
		if (califaPar) {
			califaPar->printParams();
			std::cout << "bbb" << califaPar->GetPosZ()<< std::endl;
			auto rcalifa = new TGeoRotation("Califarot");
			rcalifa->RotateX(califaPar->GetRotX());
			rcalifa->RotateY(califaPar->GetRotY());
			rcalifa->RotateZ(califaPar->GetRotZ());
			auto califa =
				new R3BCalifa(fCalifaGeo, {califaPar->GetPosX(), califaPar->GetPosY(),
						califaPar->GetPosZ(), rcalifa});
			califa->SelectGeometryVersion(fCalifaGeoVer);
			run->AddModule(califa);
		} else { std::cout << "aaa" << std::endl;
			auto califa = new R3BCalifa(fCalifaGeo, {0., 0., -65.});
			califa->SelectGeometryVersion(fCalifaGeoVer);
			run->AddModule(califa);
		}
	}

	// Glad Magnet definition
	if (fGlad) {
		fFieldMap = 1;
		fR3BMagnet = true;
		run->AddModule(new R3BGladMagnet(fGladGeo));
	}

	if (fFi30){
		if (fi30Par) {
			cout << "here 30" << endl;
			fi30Par->printParams();
			auto rfib30 = new TGeoRotation("Fi30rot");
			rfib30->RotateX(fi30Par->GetRotX());
			rfib30->RotateY(fi30Par->GetRotY());
			rfib30->RotateZ(fi30Par->GetRotZ());
			run->AddModule(new R3BFiber("Fi30", fFi30Geo, DetectorId::kFI30, {fi30Par->GetPosX(), fi30Par->GetPosY(),
						fi30Par->GetPosZ(), rfib30}));
		} else {
			run->AddModule(new R3BFiber("Fi30", fFi30Geo, DetectorId::kFI30, {0., 0., 890.}));
		}
	}

	if (fFi31){
		if (fi31Par) {
			cout << "here 31" << endl;
			fi31Par->printParams();
			auto rfib31 = new TGeoRotation("Fi31rot");
			rfib31->RotateX(fi31Par->GetRotX());
			rfib31->RotateY(fi31Par->GetRotY());
			rfib31->RotateZ(fi31Par->GetRotZ());
			run->AddModule(new R3BFiber("Fi31", fFi31Geo, DetectorId::kFI31, {fi31Par->GetPosX(), fi31Par->GetPosY(),
						fi31Par->GetPosZ(), rfib31}));
		} else {
			run->AddModule(new R3BFiber("Fi31", fFi31Geo, DetectorId::kFI31, {-15., 0., 900.}));
		}
	}

	if (fFi32){
		if (fi32Par) {
			cout << "here 32" << endl;
			fi32Par->printParams();
			auto rfib32 = new TGeoRotation("Fi32rot");
			rfib32->RotateX(fi32Par->GetRotX());
			rfib32->RotateY(fi32Par->GetRotY());
			rfib32->RotateZ(fi32Par->GetRotZ());
			run->AddModule(new R3BFiber("Fi32", fFi32Geo, DetectorId::kFI32, {fi32Par->GetPosX(), fi32Par->GetPosY(),
						fi32Par->GetPosZ(), rfib32}));
		} else {
			run->AddModule(new R3BFiber("Fi32", fFi32Geo, DetectorId::kFI32, {-30., 0., 920.}));
		}
	}

	if (fFi33){
		if (fi33Par) {
			cout << "here 33" << endl;
			fi33Par->printParams();
			auto rfib33 = new TGeoRotation("Fi33rot");
			rfib33->RotateX(fi33Par->GetRotX());
			rfib33->RotateY(fi33Par->GetRotY());
			rfib33->RotateZ(fi33Par->GetRotZ());
			run->AddModule(new R3BFiber("Fi33", fFi33Geo, DetectorId::kFI33, {fi33Par->GetPosX(), fi33Par->GetPosY(),
						fi33Par->GetPosZ(), rfib33}));
		} else {
			run->AddModule(new R3BFiber("Fi33", fFi33Geo, DetectorId::kFI33, {-30., 0., 920.}));
		}
	}

	// TofD
	if (fTofD) {
		if (tofdPar) {
			tofdPar->printParams();
			auto rtofd = new TGeoRotation("Tofdrot");
			rtofd->RotateX(tofdPar->GetRotX());
			rtofd->RotateY(tofdPar->GetRotY());
			rtofd->RotateZ(tofdPar->GetRotZ());
			run->AddModule(new R3BTofD(fTofDGeo, {tofdPar->GetPosX(), tofdPar->GetPosY(),
						tofdPar->GetPosZ(), rtofd}));
		} else {
			run->AddModule(new R3BTofD(fTofDGeo,
						{ -1373.84*TMath::Sin(18* TMath::DegToRad()), 0, 271.4+1373.84*TMath::Cos(18* TMath::DegToRad()) },
						{ "", -90., +18, 90. }));
		}
		auto tofd_digitizer = new R3BTofDDigitizer();
		if (fDetDigitizer)
			run->AddTask(tofd_digitizer);
	}

	if (fRpc) {
		if (rpcPar) {
			rpcPar->printParams();
			auto rrpc = new TGeoRotation("Rpcrot");
			rrpc->RotateX(rpcPar->GetRotX());
			rrpc->RotateY(rpcPar->GetRotY());
			rrpc->RotateZ(rpcPar->GetRotZ());
			run->AddModule(new R3BRpc(fRpcGeo, {rpcPar->GetPosX(), rpcPar->GetPosY(),
						rpcPar->GetPosZ(), rrpc}));
		} else {
			run->AddModule(
					new R3BRpc(fRpcGeo,
						{-1073.84 * TMath::Sin(22 * TMath::DegToRad()), 0,
						371.4 + 1073.84 * TMath::Cos(22 * TMath::DegToRad())},
						{"", -90., +22, 90.}));
		}
	}
	// NeuLand Scintillator Detector
	if (fNeuLand) {
		auto const nDP = 13;
		auto const neulandGeoTrans = TGeoTranslation{ 0., 0., 1542.3 + 65};
		auto neuland = std::make_unique<R3BNeuland>(nDP, neulandGeoTrans);
		run->AddModule(neuland.release());
		//run->AddModule(new R3BNeuland(fNeuLandGeo,neulandGeoTrans));
	}

	// ----- Create R3B  magnetic field ---------------------------------------
	// If the Global Position of the Magnet is changed
	// the Field Map has to be transformed accordingly
	if (fFieldMap == 0) {
		R3BAladinFieldMap *magField = new R3BAladinFieldMap("AladinMaps");
		magField->SetCurrent(fMeasCurrent);
		magField->SetScale(fFieldScale);
		if (fR3BMagnet == kTRUE) {
			run->SetField(magField);
			R3BFieldPar *fieldPar = (R3BFieldPar *)rtdb->getContainer("R3BFieldPar");
			fieldPar->SetParameters(magField);
			fieldPar->setChanged();
		} else {
			run->SetField(NULL);
		}
	}
	else if (fFieldMap == 1) {
    		R3BGladFieldMap* magField = new R3BGladFieldMap("test_field","R");
		magField->SetPosition(0.7871, 1.75-1.526, Target_to_GLAD_flange + 54.05 - 0.55580);//x,y,z in cm 
		magField->SetXAngle(-0.113); //deg
		magField->SetYAngle(-14.08); //deg
		magField->SetZAngle(-0.83); //deg
    		magField->SetScale(fMeasCurrent/3583.81);
		//magField->SetFieldfromCurrent(fMeasCurrent);
		if (fR3BMagnet == kTRUE) {
			run->SetField(magField);
			R3BFieldPar *fieldPar = (R3BFieldPar *)rtdb->getContainer("R3BFieldPar");
			fieldPar->SetParameters(magField);
			fieldPar->setChanged();
		} else {
			run->SetField(NULL);
		}
	}
	// ---- End of field map section

	// -----   Create PrimaryGenerator   --------------------------------------

	// 1 - Create the Main API class for the Generator
	FairPrimaryGenerator *primGen = new FairPrimaryGenerator();

	if (fGenerator.CompareTo("box") == 0) {
		// Define the BOX generator
		Int_t pdgId = 2212;      // proton beam
		Double32_t theta1 = 22.; // polar angle distribution
		Double32_t theta2 = 90.;
		Double32_t momentum = 0.8;
		FairBoxGenerator *boxGen = new FairBoxGenerator(pdgId, 1);
		boxGen->SetThetaRange(theta1, theta2);
		boxGen->SetPRange(momentum, 2.0 * momentum);
		boxGen->SetPhiRange(0., 360.);
		boxGen->SetXYZ(0.0, 0.0, 0.0);
		// primGen->AddGenerator(boxGen);

		// 19-N fragment
		FairIonGenerator *ionGen =
			//new FairIonGenerator(8, 18, 7, 1, 0., 0., 1.30, 0., 0., -257.3);
			new FairIonGenerator(18, 40, 18, 1, 0., 0., 0.8045, 0., 0., 25.6);
		primGen->AddGenerator(ionGen);
	}

	if (fGenerator.CompareTo("ascii") == 0) {
		R3BAsciiGenerator *gen =
			new R3BAsciiGenerator(fEventFile.Data());
		//gen->SetXYZ(targetPar->GetPosX(), targetPar->GetPosY(),
		//		targetPar->GetPosZ());
		//gen->SetDxDyDz(0., 0., 0.);
		primGen->AddGenerator(gen);
	}

	if (fGenerator.CompareTo("inclroot") == 0) {
		R3BINCLRootGenerator *gen =
			new R3BINCLRootGenerator(fEventFile.Data());
		//new R3BINCLRootGenerator((fEventFile).Data());

		//gen->SetXYZ(targetPar->GetPosX(), targetPar->GetPosY(), targetPar->GetPosZ()+2.5);
		gen->SetXYZ(0.5, -0.35, 0.5);
		gen->SetDxDyDz(0.4, 0.19, 5);
		primGen->AddGenerator(gen);
	}

	run->SetGenerator(primGen);

	//-------Set visualisation flag to true------------------------------------
	run->SetStoreTraj(fVis);

	FairLogger::GetLogger()->SetLogVerbosityLevel("LOW");
	//FairLogger::GetLogger()->SetLogScreenLevel("ERROR");

	// ----- Initialize CalifaDigitizer task (from Point Level to Cal Level)
	if (fCalifa && fCalifaDigitizer) {
		R3BCalifaDigitizer *califaDig = new R3BCalifaDigitizer();
		califaDig->SetNonUniformity(fCalifaNonU);
		califaDig->SetExpEnergyRes(5.0); // 5. means 5% at 1 MeV
		califaDig->SetComponentRes(5.0);
		califaDig->SetDetectionThreshold(fCalifaHitEnergyTh);
		run->AddTask(califaDig);
	}

	// ----- Initialize Califa HitFinder task (from CrystalCal Level to Hit Level)
	if (fCalifa && fCalifaHitFinder) {
		R3BCalifaCrystalCal2Cluster *califaHF = new R3BCalifaCrystalCal2Cluster();
		califaHF->SetCrystalThreshold(fCalifaCryTh);
		califaHF->SetRoundWindow(0.25);
		califaHF->SetGammaClusterThreshold(0.001); 
		califaHF->SetProtonClusterThreshold(0.05);
		//califaHF->SetRandomization(kTRUE);
		//califaHF->SetRandomizationFile("/u/land/martinaff/angular_histograms.root");

		califaHF->IsSimulation(kTRUE);
		run->AddTask(califaHF);
	}

	// -----   Initialize simulation run   ------------------------------------
	run->Init();

	// -----   Runtime database   ---------------------------------------------
	Bool_t kParameterMerged = kTRUE;
	FairParRootFileIo *parOut = new FairParRootFileIo(kParameterMerged);
	parOut->open(ParFile.Data());
	rtdb->setOutput(parOut);
	rtdb->saveOutput();
	rtdb->print();
	// -----   Timer   --------------------------------------------------------
	TStopwatch timer;
	timer.Start();


	// -----   Start run   ----------------------------------------------------
	if (nEvents > 0)
		run->Run(nEvents);

	// -----   Finish   -------------------------------------------------------
	timer.Stop();
	Double_t rtime = timer.RealTime();
	Double_t ctime = timer.CpuTime();
	cout << endl << endl;
	cout << "Macro finished succesfully." << endl;
	cout << "Output file is " << OutFile << endl;
	cout << "Parameter file is " << ParFile << endl;
	cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl
		<< endl;

	cout << " Test passed" << endl;
	cout << " All ok " << endl;
	gApplication->Terminate();
}
