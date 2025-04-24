#include "libs.hh"
#include "R3BRpcPoint.h"
#include "R3BMCTrack.h"
#include "Nuke_Mass.h"

namespace fs = std::filesystem;

using namespace std;

struct incoming_beam
{
	TLorentzVector p;
	TVector3 Bx;
	TVector3 By;
	TVector3 Bz;
	TVector3 vertex;
	int mul;
};

struct reaction_prod
{
	double A;
	double Z;
	TLorentzVector p;
	double pos_x;
	double pos_y;
	double pos_z;
	double mass;
	int Track_Id;
	int forward;
};
const double SPEED_OF_LIGHT = 29.9792458; // cm/ns
const double AMU = 0.9314940038;          // GeV/c2

//some variable 
double me = 0.000511; 
double my_nan = numeric_limits<double>::quiet_NaN(); 
double mp = 1.00728*AMU;
double m10Be = 10.01134*AMU;
double m_12C = Nuke_Mass(12,6)/1000. - me*6; 

bool rpc_exist = false;

TClonesArray *MCTrackCA = new TClonesArray("R3BMCTrack", 5);
TClonesArray *traData = new TClonesArray("R3BTraPoint");
TClonesArray *rpcData = new TClonesArray("R3BRpcPoint");

Double_t mul_frag_vertex = my_nan;
Double_t mul_proton = my_nan;

Double_t open_angle_p_frag = my_nan;
Double_t e_rel_gen = my_nan;
Double_t e_rel_gen_proton_on = my_nan;
Double_t e_rel_reco = my_nan;
Double_t e_rel_reco_resol = my_nan;
Double_t e_rel_reco_perfect_frag = my_nan;
Double_t e_rel_reco_perfect_frag_resol = my_nan;

Double_t rpc_x = my_nan;
Double_t rpc_y = my_nan;
Double_t rpc_z = my_nan;
Double_t rpc_tof = my_nan;
Double_t rpc_x_resol = my_nan;
Double_t rpc_y_resol = my_nan;
Double_t rpc_z_resol = my_nan;
Double_t rpc_tof_resol = my_nan;
Double_t rpc_fp = my_nan;
Double_t rpc_Q = my_nan;
Double_t rec_angle_x = my_nan; 
Double_t rec_angle_y = my_nan;
Double_t rec_poq = my_nan;

Double_t mass_rem = my_nan;
Double_t q_rem = my_nan;
Double_t num_part = my_nan;
Int_t counter_proton = my_nan;
Double_t origin_1 = my_nan;
Double_t origin_2 = my_nan;
Double_t origin_n = my_nan;

Double_t Q_tree = my_nan;
Double_t AoZ_vertex = my_nan;

Double_t vertex_x = my_nan;
Double_t vertex_y = my_nan;
Double_t vertex_z = my_nan;

Short_t fParticles;
Short_t fOrigin[160];
Short_t fMass[160];
Short_t fCharge[160];
Int_t fMassRem[160];
Int_t fChargeRem[160];
Int_t fPdgCode[160];
Float_t fPzPrime[160];
Float_t fThetaPrime[160];
Float_t fPhi[160];

TVector3 poq_vec_vertex;

TLorentzVector p_proton_tree;
TLorentzVector p_proton_resol_perf_vertex;
TLorentzVector p_proton_resol_vertex;
TLorentzVector p_proton_gen;

TLorentzVector p_neutron_gen;

TLorentzVector pfrag_mdf_vertex_tree;
TLorentzVector pfrag_mdf_vertex_lab;
TLorentzVector pfrag_gen;

TLorentzVector pbeam_lab;
TLorentzVector pbeam_tree;

TH2F *gen_poq_tx_neutron = new TH2F("gen_poq_tx_neutron","gen_poq_tx_neutron",110,0.9,3.1,25,-0.1,0.1); 
TH2F *reach_neuland_poq_tx = new TH2F("reach_neuland_poq_tx","reach_neuland_poq_tx",110,0.9,3.1,25,-0.1,0.1); 
TH2F *ratio_poq_tx_neutron = new TH2F("ratio_poq_tx_neutron","ratio_poq_tx_neutron",110,0.9,3.1,25,-0.1,0.1); 

TH1F *gen_poq_neutron = new TH1F("gen_poq_neutron","gen_poq_neutron",50,0,3.5); 
TH1F *reach_neuland_poq = new TH1F("reach_neuland_poq","reach_neuland_poq",50,0,3.5); 

TH2F *gen_angles_neutron = new TH2F("gen_angles_neutron","gen_angles_neutron",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *reach_neuland_angles = new TH2F("reach_neuland_angles","reach_neuland_angles",25,-0.1,0.1,50,-0.1,0.1); 

TH2F *gen_poq_tx = new TH2F("gen_poq_tx","gen_poq_tx",110,0.9,3.1,25,-0.1,0.1); 
TH2F *reach_rpc_poq_tx = new TH2F("reach_rpc_poq_tx","reach_rpc_poq_tx",110,0.9,3.1,25,-0.1,0.1); 
TH2F *ratio_poq_tx = new TH2F("ratio_poq_tx","ratio_poq_tx",110,0.9,3.1,25,-0.1,0.1); 

TH2F *open_angle_p_frag_vs_p_mom = new TH2F("open_angle_p_frag_vs_p_mom","open_angle_p_frag_vs_p_mom",1000,0,-3,1000,20,-23); 

TH1F *gen_poq = new TH1F("gen_poq","gen_poq",50,0,3.5); 
TH1F *reach_rpc_poq = new TH1F("reach_rpc_poq","reach_rpc_poq",50,0,3.5); 

TH2F *gen_angles = new TH2F("gen_angles","gen_angles",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *reach_rpc_angles = new TH2F("reach_rpc_angles","reach_rpc_angles",25,-0.1,0.1,50,-0.1,0.1); 

TH2F *reach_rpc_angles0_0p5 = new TH2F("reach_rpc_angles0_0p5","reach_rpc_angles0_0p5",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *reach_rpc_angles0p5_1 = new TH2F("reach_rpc_angles0p5_1","reach_rpc_angles0p5_1",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *reach_rpc_angles1_1p5 = new TH2F("reach_rpc_angles1_1p5","reach_rpc_angles1_1p5",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *reach_rpc_angles1p5_2 = new TH2F("reach_rpc_angles1p5_2","reach_rpc_angles1p5_2",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *reach_rpc_angles2_2p5 = new TH2F("reach_rpc_angles2_2p5","reach_rpc_angles2_2p5",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *reach_rpc_angles2p5_3 = new TH2F("reach_rpc_angles2p5_3","reach_rpc_angles2p5_3",25,-0.1,0.1,50,-0.1,0.1); 

TH2F *gen_angles0_0p5 = new TH2F("gen_angles0_0p5","gen_angles0_0p5",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *gen_angles0p5_1 = new TH2F("gen_angles0p5_1","gen_angles0p5_1",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *gen_angles1_1p5 = new TH2F("gen_angles1_1p5","gen_angles1_1p5",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *gen_angles1p5_2 = new TH2F("gen_angles1p5_2","gen_angles1p5_2",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *gen_angles2_2p5 = new TH2F("gen_angles2_2p5","gen_angles2_2p5",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *gen_angles2p5_3 = new TH2F("gen_angles2p5_3","gen_angles2p5_3",25,-0.1,0.1,50,-0.1,0.1); 

TH2F *ratio_angles_neutron = new TH2F("ratio_angles_neutron","ratio_angles_neutron",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *ratio_angles = new TH2F("ratio_angles","ratio_angles",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *ratio_angles0_0p5 = new TH2F("ratio_angles0_0p5","ratio_angles0_0p5",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *ratio_angles0p5_1 = new TH2F("ratio_angles0p5_1","ratio_angles0p5_1",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *ratio_angles1_1p5 = new TH2F("ratio_angles1_1p5","ratio_angles1_1p5",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *ratio_angles1p5_2 = new TH2F("ratio_angles1p5_2","ratio_angles1p5_2",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *ratio_angles2_2p5 = new TH2F("ratio_angles2_2p5","ratio_angles2_2p5",25,-0.1,0.1,50,-0.1,0.1); 
TH2F *ratio_angles2p5_3 = new TH2F("ratio_angles2p5_3","ratio_angles2p5_3",25,-0.1,0.1,50,-0.1,0.1); 

TH2F *e_rel_response_matrix = new TH2F("e_rel_response_matrix","e_rel_response_matrix",100,0,50,100,0,50); 

TH1F *opa_gen = new TH1F("opa_gen","opa_gen",360,20,180); 

void reset_val(){

	Q_tree = my_nan;
	AoZ_vertex = my_nan;

	vertex_x = my_nan;
	vertex_y = my_nan;
	vertex_z = my_nan;

	poq_vec_vertex.SetXYZ(my_nan,my_nan,my_nan);
	poq_vec_vertex.SetMag(my_nan);

	pbeam_tree.SetXYZM(my_nan,my_nan,my_nan,my_nan);
	pfrag_mdf_vertex_tree.SetXYZM(my_nan,my_nan,my_nan,my_nan);

	pbeam_lab.SetXYZM(my_nan,my_nan,my_nan,my_nan);
	pfrag_mdf_vertex_lab.SetXYZM(my_nan,my_nan,my_nan,my_nan);
	pfrag_gen.SetXYZM(my_nan,my_nan,my_nan,my_nan);

	p_proton_tree.SetXYZM(my_nan,my_nan,my_nan,my_nan);
	p_proton_resol_perf_vertex.SetXYZM(my_nan,my_nan,my_nan,my_nan);
	p_proton_resol_vertex.SetXYZM(my_nan,my_nan,my_nan,my_nan);
	p_proton_gen.SetXYZM(my_nan,my_nan,my_nan,my_nan);

	p_neutron_gen.SetXYZM(my_nan,my_nan,my_nan,my_nan);

	rpc_x_resol = my_nan;
	rpc_y_resol = my_nan;
	rpc_z_resol = my_nan;
	rpc_tof_resol = my_nan;

	rpc_x = my_nan;
	rpc_y = my_nan;
	rpc_z = my_nan;
	rpc_tof = my_nan;
	rpc_fp = my_nan;
	rpc_Q = my_nan;
	rec_angle_x = my_nan; 
	rec_angle_y = my_nan;
	rec_poq = my_nan;

	mass_rem = my_nan;
	q_rem = my_nan;
	num_part = my_nan;
	origin_1 = my_nan;
	origin_2 = my_nan;
	origin_n = my_nan;

	mul_frag_vertex = my_nan;
	mul_proton = my_nan;
	open_angle_p_frag = my_nan;
	e_rel_gen = my_nan;
	e_rel_gen_proton_on = my_nan;
	e_rel_reco = my_nan;
	e_rel_reco_resol = my_nan;
	e_rel_reco_perfect_frag = my_nan;
}

void analysis_simu(){

	TString output_filename_hit = TString::Format("ana_simu.root");
	TFile *_file2 = TFile::Open(output_filename_hit,"RECREATE");
	TTree *outtree = new TTree("evt","General Tree");

	outtree->Branch("pbeam", &pbeam_tree);
	outtree->Branch("pbeam_lab", &pbeam_lab);
	outtree->Branch("pfrag_mdf_vertex", &pfrag_mdf_vertex_tree);
	outtree->Branch("pfrag_mdf_vertex_lab", &pfrag_mdf_vertex_lab);
	outtree->Branch("pfrag_gen", &pfrag_gen);

	outtree->Branch("Q",&Q_tree,"Q/D");

	outtree->Branch("AoZ_vertex",&AoZ_vertex,"AoZ_vertex/D");

	outtree->Branch("mass_rem",&mass_rem,"mass_rem/D");
	outtree->Branch("q_rem",&q_rem,"q_rem/D");
	outtree->Branch("origin_1",&origin_1,"origin_1/D");
	outtree->Branch("origin_2",&origin_2,"origin_2/D");
	outtree->Branch("origin_n",&origin_n,"origin_n/D");
	outtree->Branch("num_part",&num_part,"num_part/D");

	outtree->Branch("vertex_x",&vertex_x,"vertex_x/D");
	outtree->Branch("vertex_y",&vertex_y,"vertex_y/D");
	outtree->Branch("vertex_z",&vertex_z,"vertex_z/D");

	outtree->Branch("rpc_x_resol",&rpc_x_resol,"rpc_x_resol/D");
	outtree->Branch("rpc_y_resol",&rpc_y_resol,"rpc_y_resol/D");
	outtree->Branch("rpc_z_resol",&rpc_z_resol,"rpc_z_resol/D");
	outtree->Branch("rpc_tof_resol",&rpc_tof_resol,"rpc_tof_resol/D");

	outtree->Branch("rpc_x",&rpc_x,"rpc_x/D");
	outtree->Branch("rpc_y",&rpc_y,"rpc_y/D");
	outtree->Branch("rpc_z",&rpc_z,"rpc_z/D");
	outtree->Branch("rpc_Q",&rpc_Q,"rpc_Q/D");
	outtree->Branch("rpc_tof",&rpc_tof,"rpc_tof/D");
	outtree->Branch("rpc_fp",&rpc_fp,"rpc_fp/D");

	outtree->Branch("p_proton", &p_proton_tree);
	outtree->Branch("p_proton_resol_perf_vertex", &p_proton_resol_perf_vertex);
	outtree->Branch("p_proton_resol_vertex", &p_proton_resol_vertex);
	outtree->Branch("p_proton_gen", &p_proton_gen);

	outtree->Branch("p_neutron_gen", &p_neutron_gen);

	outtree->Branch("open_angle_p_frag",&open_angle_p_frag);

	outtree->Branch("e_rel_gen",&e_rel_gen,"e_rel_gen/D");
	outtree->Branch("e_rel_gen_proton_on",&e_rel_gen_proton_on,"e_rel_gen_proton_on/D");
	outtree->Branch("e_rel_reco",&e_rel_reco,"e_rel_reco/D");
	outtree->Branch("e_rel_reco_perfect_frag",&e_rel_reco_perfect_frag,"e_rel_reco_perfect_frag/D");
	outtree->Branch("e_rel_reco_perfect_frag_resol",&e_rel_reco_perfect_frag_resol,"e_rel_reco_perfect_frag_resol/D");
	outtree->Branch("e_rel_reco_resol",&e_rel_reco_resol,"e_rel_reco_resol/D");

	outtree->Branch("mul_frag_vertex",&mul_frag_vertex,"mul_frag_vertex/D");
	outtree->Branch("mul_proton",&mul_proton,"mul_proton/D");

	TFile *eventFile, *inclFile;
	TTree *eventTree, *inclTree;
	vector <incoming_beam> beam_array;

	std::vector<TString> fileList;
	fileList.push_back("sim.root");

	inclFile = TFile::Open("inputs/p_O22_630.root");
	inclTree = (TTree*)inclFile->Get("et");

	inclTree->SetBranchAddress("nParticles", &fParticles);
	inclTree->SetBranchAddress("origin", &fOrigin);
	inclTree->SetBranchAddress("A", &fMass);
	inclTree->SetBranchAddress("Z", &fCharge);
	inclTree->SetBranchAddress("ARem", &fMassRem);
	inclTree->SetBranchAddress("ZRem", &fChargeRem);
	inclTree->SetBranchAddress("pzPrime", &fPzPrime);
	inclTree->SetBranchAddress("thetaPrime", &fThetaPrime);
	inclTree->SetBranchAddress("phi", &fPhi);
	inclTree->SetBranchAddress("PDGCode", &fPdgCode);

	std::ifstream infile("inputs/Breackup_10Be_p_uni_50_MeV_e_rel_angle.txt");
	//std::ifstream infile("inputs/test_QFS_S509.txt");

	if (!infile.is_open()) {
		std::cerr << "Error opening file." << std::endl;
		return;
	}

	std::string line;

	//while (std::getline(infile, line)) {
	//	std::istringstream headerStream(line);
	//	int eventNumber, numLines, TX_in, TY_in;

	//	headerStream >> eventNumber >> numLines >> TX_in >> TY_in;

	//	TVector3 beam_vec(TX_in,TY_in,1);

	//	TVector3 Z_lab(0,0,1);
	//	TVector3 Bz = beam_vec.Unit();
	//	TVector3 By = Z_lab.Cross(beam_vec.Unit()).Unit();
	//	TVector3 Bx = By.Cross(Bz);
	//	beam_vec.SetMag(sqrt(pow(1.25*12+m_12C,2) - pow(m_12C,2)));

	//	TLorentzVector beam_TL;
	//	beam_TL.SetVectM(beam_vec,m_12C);

	//	incoming_beam beam;

	//	beam.p = beam_TL;
	//	beam.Bx = Bx;
	//	beam.By = By;
	//	beam.Bz = Bz;
	//	beam.mul = numLines;
	//	TVector3 vertex;
	//	for (int i = 0; i < numLines; ++i) {
	//		std::getline(infile, line);
	//		std::istringstream headerStream2(line);
	//		double a, b,c,d,e,f, vx, vy, vz, g;
	//		if(i==0){
	//			headerStream2 >> a >> b >> c >> d >> e >> f >> vx >> vy >> vz >> g;
	//			vertex.SetXYZ(vx,vy,vz);
	//		}

	//	}
	//	beam.vertex = vertex;
	//	beam_array.push_back(beam);
	//}
	for(int s = 0 ; s < fileList.size() ; s++){
		eventFile = TFile::Open(fileList.at(s));

		eventTree = (TTree*)eventFile->Get("evt");
		int nev = eventTree->GetEntries();

		// Data
		TBranch *branchMCTrack = eventTree->GetBranch("MCTrack");
		branchMCTrack->SetAddress(&MCTrackCA);

		TBranch *branchRpcData = eventTree->GetBranch("RpcPoint");
		branchRpcData->SetAddress(&rpcData);

		TRandom rand(time(0));

		int nMCtracks = 0, nRpc_point = 0, nNeuland_point =0, nTra_point;
		double mass_frag = 0, opa_beam = 0, opa_lab = 0, p1_mag=0, p2_mag=0;
		double exi_energy = 0;
		double beta_proton=0, beta_frag = 0, gamma_proton = 0, gamma_frag = 0;
		int counter1 =0, counter2=0, counter3 = 0, counter4 =0, counter5 =0;
		int counter_tracks = 0, counter_frags = 0, counter_10Be = 0, counter_proton = 0;
		cout << nev << endl;
		cout << inclTree->GetEntries() << endl;
		for(int t = 0; t< nev;t++){

			reset_val();
			eventTree->GetEntry(t);
			inclTree->GetEntry(t);

			mass_rem = fMassRem[0];
			q_rem = fChargeRem[0];
			num_part = fParticles;

			nMCtracks = MCTrackCA->GetEntries();
			nRpc_point = rpcData->GetEntries();
			nTra_point = traData->GetEntries();

			bool frag_exists =false, p_exists = false, n_exists = false, rpc_exist = false;
			vector <reaction_prod> generated_frags;
			TLorentzVector p_temp, n_temp;
			for (Int_t j = 0; j < nMCtracks; j++) {
				auto track = dynamic_cast<R3BMCTrack*>(MCTrackCA->At(j));
				if (track->GetMotherId() == -1) {
					counter_tracks++;
					int fZ_in = 0.; int fA_in = 0.;

					if (track->GetPdgCode()>1000010010){
						fZ_in = int(track->GetPdgCode() / 10000) - 100000.;
						fA_in = 0.1 * (track->GetPdgCode() - (100000 + fZ_in) * 10000.);
						mass_frag = Nuke_Mass(fA_in,fZ_in)/1000. - me*fZ_in; 
						p_temp.SetXYZM(track->GetPx(),track->GetPy(),track->GetPz(),mass_frag);

						reaction_prod frag;
						frag.A = fA_in;
						frag.Z = fZ_in;
						frag.p = p_temp;
						frag.pos_x = track->GetStartX();
						frag.pos_y = track->GetStartY();
						frag.pos_z = track->GetStartZ();
						frag.mass = mass_frag;
						frag.Track_Id = j;
						frag.forward = 1;
						generated_frags.push_back(frag);
					}
					else if(track->GetPdgCode()==2212){
						fZ_in = 1;
						fA_in = 1;
						mass_frag = Nuke_Mass(fA_in,fZ_in)/1000. - me*fZ_in; 
						p_temp.SetXYZM(track->GetPx(),track->GetPy(),track->GetPz(),mass_frag);

						reaction_prod proton;
						proton.A = fA_in;
						proton.Z = fZ_in;
						proton.p = p_temp;
						proton.pos_x = track->GetStartX();
						proton.pos_y = track->GetStartY();
						proton.pos_z = track->GetStartZ();
						proton.mass = mass_frag;
						proton.Track_Id = j;
						proton.forward = 1;
						generated_frags.push_back(proton);
					}
					else if(track->GetPdgCode()==2112){
						fZ_in = 0;
						fA_in = 1;
						mass_frag = Nuke_Mass(fA_in,fZ_in)/1000. - me*fZ_in; 
						p_temp.SetXYZM(track->GetPx(),track->GetPy(),track->GetPz(),mass_frag);

						reaction_prod neutron;
						neutron.A = fA_in;
						neutron.Z = fZ_in;
						neutron.p = p_temp;
						neutron.pos_x = track->GetStartX();
						neutron.pos_y = track->GetStartY();
						neutron.pos_z = track->GetStartZ();
						neutron.mass = mass_frag;
						neutron.Track_Id = j;
						neutron.forward = 0;
						generated_frags.push_back(neutron);
					}
				}
			}
			//		if(beam_array[nev].mul!=1){
			//			generated_frags[beam_array[nev].mul-2].forward = 1;
			//		}
			//		else {
			//			generated_frags[beam_array[nev].mul-1].forward = 1;
			//		}

			//		for( int jj =0; jj < generated_frags.size(); jj++){
			//			cout <<  generated_frags[jj].A << " " << generated_frags[jj].Z << " " << generated_frags[jj].mass << " " << generated_frags[jj].Track_Id  << " " << generated_frags[jj].forward << endl; 
			//		}
			int gen_proton_track_id = -1;
			int gen_neutron_track_id = -1;
			TLorentzVector p_proton_gen_1;
			TLorentzVector p_proton_gen_2;
			std::map<int, TVector3> foot_M;
			std::map<int, TVector3> foot_W;
			//cout << "new" << " " << mass_rem << " " << q_rem << " " << fParticles << endl;
			for (Int_t j = 0; j < fParticles; j++)
			{
				//cout << " avaiablep : " << j << " " << fPdgCode[j] << " " << fOrigin[j] << endl;
			}
			//cout << "sep :" << endl;

			for(int jj = 0; jj < generated_frags.size(); jj++){	
				//			if(generated_frags[jj].forward != 1){
				//				continue;
				//			}
				if(generated_frags[jj].A == 11 && generated_frags[jj].Z ==6){
					frag_exists = true;

					pfrag_mdf_vertex_lab.SetXYZM(generated_frags[jj].p.X() + gRandom->Gaus(0,0.01),generated_frags[jj].p.Y() + gRandom->Gaus(0,0.01),generated_frags[jj].p.Z() + gRandom->Gaus(0,0.0525),generated_frags[jj].mass);
					pfrag_gen = generated_frags[jj].p;
				}
			}

			for (Int_t j = 0; j < nRpc_point; j++) {
				auto rpc_point = dynamic_cast<R3BRpcPoint*>(rpcData->At(j));
				Int_t TrackId_rpc = rpc_point->GetTrackID();
				int gap = rpc_point->GetChannelId();
				if(gap != 6 || gen_proton_track_id != TrackId_rpc){
					continue;
				}

				rpc_exist = true;

				TVector3 rpc_pos; //cm
				TVector3 p_proton_temp;
				rpc_point->PositionIn(rpc_pos);
				rpc_tof = rpc_point->GetTime();	
				rpc_x = rpc_pos.X();
				rpc_y = rpc_pos.Y();
				rpc_z = rpc_pos.Z();

				rpc_tof_resol = rpc_point->GetTime() + gRandom->Gaus(0,0.1);	
				rpc_x_resol = rpc_pos.X() + gRandom->Gaus(0,1);
				rpc_y_resol = rpc_pos.Y() + gRandom->Gaus(0,1);
				rpc_z_resol = rpc_pos.Z() + gRandom->Gaus(0,1);

			}

			if(frag_exists && p_exists){
				//// Generated ////
				TLorentzVector LVtotal = p_proton_gen + pfrag_gen;

				e_rel_gen = (LVtotal.M() - pfrag_gen.M() - mp)*1000;
				open_angle_p_frag = p_proton_gen.Vect().Angle(pfrag_gen.Vect());

				//// Generated with proton on////
				if(rpc_exist){
					reach_rpc_angles->Fill(p_proton_gen.X()/p_proton_gen.Z(),p_proton_gen.Y()/p_proton_gen.Z());
					ratio_angles->Fill(p_proton_gen.X()/p_proton_gen.Z(),p_proton_gen.Y()/p_proton_gen.Z());
					reach_rpc_poq->Fill(p_proton_gen.P());
					reach_rpc_poq_tx->Fill(p_proton_gen.P(),p_proton_gen.X()/p_proton_gen.Z());
					ratio_poq_tx->Fill(p_proton_gen.P(),p_proton_gen.X()/p_proton_gen.Z());
					open_angle_p_frag_vs_p_mom->Fill(p_proton_gen.P(),p_proton_gen.Vect().Angle(pfrag_gen.Vect()));
					if(p_proton_gen.P()<0.5 && p_proton_gen.P()>=0){
						counter1++;
						ratio_angles0_0p5->Fill(p_proton_gen.X()/p_proton_gen.Z(),p_proton_gen.Y()/p_proton_gen.Z());
					}
					if(p_proton_gen.P()<1 && p_proton_gen.P()>=0.5){
						counter1++;
						ratio_angles0p5_1->Fill(p_proton_gen.X()/p_proton_gen.Z(),p_proton_gen.Y()/p_proton_gen.Z());
					}
					if(p_proton_gen.P()<1.5 && p_proton_gen.P()>=1){
						reach_rpc_angles1_1p5->Fill(p_proton_gen.X()/p_proton_gen.Z(),p_proton_gen.Y()/p_proton_gen.Z()); 
						ratio_angles1_1p5->Fill(p_proton_gen.X()/p_proton_gen.Z(),p_proton_gen.Y()/p_proton_gen.Z()); 
					}
					if(p_proton_gen.P()<2 && p_proton_gen.P()>=1.5){
						reach_rpc_angles1p5_2->Fill(p_proton_gen.X()/p_proton_gen.Z(),p_proton_gen.Y()/p_proton_gen.Z()); 
						ratio_angles1p5_2->Fill(p_proton_gen.X()/p_proton_gen.Z(),p_proton_gen.Y()/p_proton_gen.Z()); 
					}
					if(p_proton_gen.P()<2.5 && p_proton_gen.P()>=2){
						reach_rpc_angles2_2p5->Fill(p_proton_gen.X()/p_proton_gen.Z(),p_proton_gen.Y()/p_proton_gen.Z()); 
						ratio_angles2_2p5->Fill(p_proton_gen.X()/p_proton_gen.Z(),p_proton_gen.Y()/p_proton_gen.Z()); 
					}
					if(p_proton_gen.P()<3 && p_proton_gen.P()>=2.5){
						reach_rpc_angles2p5_3->Fill(p_proton_gen.X()/p_proton_gen.Z(),p_proton_gen.Y()/p_proton_gen.Z()); 
						ratio_angles2p5_3->Fill(p_proton_gen.X()/p_proton_gen.Z(),p_proton_gen.Y()/p_proton_gen.Z()); 
					}

					LVtotal = p_proton_gen + pfrag_gen;
					e_rel_gen_proton_on = (LVtotal.M() - pfrag_gen.M() - mp)*1000;

					//// MDF proton with perfect frag////
					LVtotal = p_proton_resol_perf_vertex + pfrag_gen;
					e_rel_reco_perfect_frag = (LVtotal.M() - pfrag_gen.M() - mp)*1000;

					//// MDF proton with resol frag////
					LVtotal = p_proton_resol_perf_vertex + pfrag_mdf_vertex_lab;
					e_rel_reco = (LVtotal.M() - pfrag_gen.M() - mp)*1000;

					//// MDF proton resol with perfect frag////
					LVtotal = p_proton_resol_vertex + pfrag_gen;
					e_rel_reco_perfect_frag_resol = (LVtotal.M() - pfrag_gen.M() - mp)*1000;

					//// MDF proton resol with perfect frag resol////
					LVtotal = p_proton_resol_vertex + pfrag_mdf_vertex_lab;
					e_rel_reco_resol = (LVtotal.M() - pfrag_gen.M() - mp)*1000;
					e_rel_response_matrix->Fill(e_rel_gen_proton_on,e_rel_reco_resol);
				}
			}
			outtree->Fill();
		}
		cout << counter1 << " " << counter2 << " " << counter3 << " " << counter4 << " " << counter5 << endl;
		cout << counter_tracks << " " << counter_frags << " " << counter_10Be << " " << counter_proton << endl;
	}

	// ratio hist
	ratio_angles_neutron->Divide(gen_angles_neutron); 
	ratio_poq_tx_neutron->Divide(gen_poq_tx_neutron); 

	ratio_poq_tx->Divide(gen_poq_tx); 
	ratio_angles->Divide(gen_angles); 
	ratio_angles1_1p5->Divide(gen_angles1_1p5); 
	ratio_angles1p5_2->Divide(gen_angles1p5_2); 
	ratio_angles2_2p5->Divide(gen_angles2_2p5); 
	ratio_angles2p5_3->Divide(gen_angles2p5_3); 

	TCanvas *c1_ratio_poq_tx_neutron = new TCanvas("c1_ratio_poq_tx_neutron", "c1_ratio_poq_tx_neutron", 800, 600);
	c1_ratio_poq_tx_neutron->cd();

	ratio_poq_tx_neutron->SetTitle("Acceptance neutron");
	ratio_poq_tx_neutron->GetXaxis()->SetTitle("P");
	ratio_poq_tx_neutron->GetYaxis()->SetTitle("TX");
	ratio_poq_tx_neutron->SetMinimum(-0.0001);
	ratio_poq_tx_neutron->Draw("colz");

	TCanvas *c1_ratio_poq_tx = new TCanvas("c1_ratio_poq_tx", "c1_ratio_poq_tx", 800, 600);
	c1_ratio_poq_tx->cd();

	ratio_poq_tx->SetTitle("Acceptance");
	ratio_poq_tx->GetXaxis()->SetTitle("P");
	ratio_poq_tx->GetYaxis()->SetTitle("TX");
	ratio_poq_tx->SetMinimum(-0.0001);
	ratio_poq_tx->Draw("colz");

	TCanvas *c1_ratio_angles_neutron = new TCanvas("c1_ratio_angles_neutron", "c1_ratio_angles_neutron", 800, 600);
	c1_ratio_angles_neutron->cd();

	ratio_angles_neutron->SetTitle("Acceptance neutron");
	ratio_angles_neutron->GetXaxis()->SetTitle("TX");
	ratio_angles_neutron->GetYaxis()->SetTitle("TY");
	ratio_angles_neutron->SetMinimum(-0.0001);
	ratio_angles_neutron->Draw("colz");

	TCanvas *c1_ratio_angles = new TCanvas("c1_ratio_angles", "c1_ratio_angles", 800, 600);
	c1_ratio_angles->cd();

	ratio_angles->SetTitle("Acceptance");
	ratio_angles->GetXaxis()->SetTitle("TX");
	ratio_angles->GetYaxis()->SetTitle("TY");
	ratio_angles->SetMinimum(-0.0001);
	ratio_angles->Draw("colz");

	TCanvas *c1_ratio_angles0_0p5 = new TCanvas("c1_ratio_angles0_0p5", "c1_ratio_angles0_0p5", 800, 600);
	c1_ratio_angles0_0p5->cd();

	ratio_angles0_0p5->SetTitle("Acceptance 0_0p5");
	ratio_angles0_0p5->GetXaxis()->SetTitle("TX");
	ratio_angles0_0p5->GetYaxis()->SetTitle("TY");
	ratio_angles0_0p5->SetMinimum(-0.0001);

	ratio_angles0_0p5->Draw("colz");

	TCanvas *c1_ratio_angles0p5_1 = new TCanvas("c1_ratio_angles0p5_1", "c1_ratio_angles0p5_1", 800, 600);
	c1_ratio_angles0p5_1->cd();

	ratio_angles0p5_1->SetTitle("Acceptance 0p5_1");
	ratio_angles0p5_1->GetXaxis()->SetTitle("TX");
	ratio_angles0p5_1->GetYaxis()->SetTitle("TY");
	ratio_angles0p5_1->SetMinimum(-0.0001);

	ratio_angles0p5_1->Draw("colz");

	TCanvas *c1_ratio_angles1_1p5 = new TCanvas("c1_ratio_angles1_1p5", "c1_ratio_angles1_1p5", 800, 600);
	c1_ratio_angles1_1p5->cd();

	ratio_angles1_1p5->SetTitle("Acceptance 1_1p5");
	ratio_angles1_1p5->GetXaxis()->SetTitle("TX");
	ratio_angles1_1p5->GetYaxis()->SetTitle("TY");
	ratio_angles1_1p5->SetMinimum(-0.0001);

	ratio_angles1_1p5->Draw("colz");
	TCanvas *c1_ratio_angles1p5_2 = new TCanvas("c1_ratio_angles1p5_2", "c1_ratio_angles1p5_2", 800, 600);
	c1_ratio_angles1p5_2->cd();

	ratio_angles1p5_2->SetTitle("Acceptance 1p5_2");
	ratio_angles1p5_2->GetXaxis()->SetTitle("TX");
	ratio_angles1p5_2->GetYaxis()->SetTitle("TY");
	ratio_angles1p5_2->SetMinimum(-0.0001);

	ratio_angles1p5_2->Draw("colz");
	TCanvas *c1_ratio_angles2_2p5 = new TCanvas("c1_ratio_angles2_2p5", "c1_ratio_angles2_2p5", 800, 600);

	c1_ratio_angles2_2p5->cd();
	ratio_angles2_2p5->SetTitle("Acceptance 2_2p5");
	ratio_angles2_2p5->GetXaxis()->SetTitle("TX");
	ratio_angles2_2p5->GetYaxis()->SetTitle("TY");
	ratio_angles2_2p5->SetMinimum(-0.0001);

	ratio_angles2_2p5->Draw("colz");
	TCanvas *c1_ratio_angles2p5_3 = new TCanvas("c1_ratio_angles2p5_3", "c1_ratio_angles2p5_3", 800, 600);
	c1_ratio_angles2p5_3->cd();

	ratio_angles2p5_3->SetTitle("Acceptance 2p5_3");
	ratio_angles2p5_3->GetXaxis()->SetTitle("TX");
	ratio_angles2p5_3->GetYaxis()->SetTitle("TY");
	ratio_angles2p5_3->SetMinimum(-0.0001);

	ratio_angles2p5_3->Draw("colz");

	_file2->cd();

	c1_ratio_angles_neutron->Write();
	c1_ratio_poq_tx_neutron->Write();

	c1_ratio_angles->Write();
	c1_ratio_poq_tx->Write();
	c1_ratio_angles0_0p5->Write();
	c1_ratio_angles0p5_1->Write();
	c1_ratio_angles1_1p5->Write();
	c1_ratio_angles1p5_2->Write();
	c1_ratio_angles2_2p5->Write();
	c1_ratio_angles2p5_3->Write();

	outtree->Write();
	open_angle_p_frag_vs_p_mom->Write();
	e_rel_response_matrix->Write();
	gen_poq_neutron->Write(); 
	gen_angles_neutron->Write(); 
	gen_poq_tx_neutron->Write(); 
	gen_poq->Write(); 
	gen_angles->Write(); 
	gen_poq_tx->Write(); 
	reach_neuland_angles->Write(); 
	reach_neuland_poq_tx->Write(); 
	reach_neuland_poq->Write(); 
	reach_rpc_angles->Write(); 
	reach_rpc_poq_tx->Write(); 
	reach_rpc_poq->Write(); 
	reach_rpc_angles0_0p5->Write(); 
	reach_rpc_angles0p5_1->Write(); 
	reach_rpc_angles1_1p5->Write(); 
	reach_rpc_angles1p5_2->Write(); 
	reach_rpc_angles2_2p5->Write(); 
	reach_rpc_angles2p5_3->Write(); 
	gen_angles0_0p5->Write(); 
	gen_angles0p5_1->Write(); 
	gen_angles1_1p5->Write(); 
	gen_angles1p5_2->Write(); 
	gen_angles2_2p5->Write(); 
	gen_angles2p5_3->Write(); 
	ratio_angles_neutron->Write(); 
	ratio_poq_tx_neutron->Write(); 
	ratio_angles->Write(); 
	ratio_poq_tx->Write(); 
	ratio_angles0_0p5->Write(); 
	ratio_angles0p5_1->Write(); 
	ratio_angles1_1p5->Write(); 
	ratio_angles1p5_2->Write(); 
	ratio_angles2_2p5->Write(); 
	ratio_angles2p5_3->Write(); 
	opa_gen->Write(); 

	delete _file2;
}

int main(Int_t argc, Char_t* argv[]){
	TStopwatch t1; 
	t1.Start(); 
	analysis_simu();
	cout << "\n-- Run time of analysis():\n";
	t1.Print();
	return 0;
}
