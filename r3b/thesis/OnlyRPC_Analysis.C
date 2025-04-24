//#include "Nuclear_Mass.h"
#include <unordered_map>
#include <utility>

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


TH2F* h_Z_vs_AoZ = new TH2F("h_Z_vs_AoZ", "Z vs A/Z;A/Z;Z", 100, 1, 4, 120, 0, 10);
TH2F* h_Z_vs_AoZ_gen = new TH2F("h_Z_vs_AoZ_gen", "gen Z vs A/Z;A/Z;Z", 100, 1, 4, 120, 0, 10);
TH2F* RPCPosHit = new TH2F("RPCPosHit", "Hit Position in RPC;x;y", 500, -340, -200, 300, 120, -120);
TH1F* Multiplicity = new TH1F("Multiplicity", "Multiplicity in RPC;Multiplicity", 10, 0, 10);
TH1F* NeutronMulti = new TH1F("Neutron_Multiplicity", "Neutron Multiplicity in RPCevents ;Neutron_Multiplicity", 10, 0, 10);
TH1F* ChargedMulti = new TH1F("Charged_Particles_Multiplicity", "Charged Particles Multiplicity in RPCevents ;Charged_Particles_Multiplicity", 10, 0, 10);
TH2F* Multi2RPC = new TH2F("Multi2RPC", "A1_vs_A2_Multi2;A1;A2", 100, 1, 4, 120, 0, 10);
TH2F* ZvsAoZMulti1RPC = new TH2F("ZvsAoZMulti1RPC", "h_Z_vs_AoZ_Multi1;A/Z;Z", 100, 1, 4, 120, 0, 10);
TH2F* ZvsAoZMulti2RPC = new TH2F("ZvsAoZMulti2RPC", "h_Z_vs_AoZ_Multi2;A/Z;Z", 100, 1, 4, 120, 0, 10);
TH2F* h_Z_vs_AoZ_non_RPC = new TH2F("h_Z_vs_AoZ_non_RPC", "Z vs A/Z not in RPC;A/Z;Z", 100, 1, 4, 120, 0, 10);
TH2F* Carbons = new TH2F("Hits in RPC when there are Carbons out", "Z vs A/Z in RPC Carbons out;A/Z;Z", 100, 1, 4, 120, 0, 10);
TH2F* HeORD = new TH2F("Frags out RPC when there are He or D in", "Z vs A/Z not in RPC He or D in;A/Z;Z", 100, 1, 4, 120, 0, 10);
TH2F* RPCMultivsNeutrons = new TH2F("RPCMultivsNeutron", "RPC Multi vs Neutrons;RPC multi;Neutron multi", 10, 0, 10, 10, 0, 10);
TH2F* RPCMultivsCP = new TH2F("RPCMultivsCP", "RPC Multi vs Charged Particles;RPC multi;CP multi", 10, 0, 10, 10, 0, 10);
TH2F* CarbonEvRPCNeutronMulti = new TH2F("CarbonEvRPCNeutronMulti", "RPC Multi vs Neutrons in Carbon events;RPC multi;Neutron multi", 10, 0, 10, 10, 0, 10);
TH1F* DeltaY_Multi2 = new TH1F("DeltaY_Multi2", "delta Y for multi 2", 300, 120, -120);


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

void OnlyRPC_Analysis(){

  std::vector<double> frag_A_vec;
  std::vector<double> frag_Z_vec;
  std::vector<double> RPC_A;
  std::vector<double> RPC_Z;
  std::vector<double> frag_A_Multi1_vec;
  std::vector<double> frag_Z_Multi1_vec;
  std::vector<double> PosY_Multi2;


  TString output_filename_hit = TString::Format("RpcOnlyHits.root");
  TFile *_file2 = TFile::Open(output_filename_hit,"RECREATE");
  TTree *outtree = new TTree("evt","General Tree");

  outtree->Branch("frag_A", &frag_A_vec);
  outtree->Branch("frag_Z", &frag_Z_vec);
  outtree->Branch("frag_A_Multi1", &frag_A_Multi1_vec);
  outtree->Branch("frag_Z_Multi1", &frag_Z_Multi1_vec);


  TFile *eventFile, *inclFile;
  TTree *eventTree, *inclTree;
  vector <incoming_beam> beam_array;

  std::vector<TString> fileList;
  fileList.push_back("sim_RPC_only.root");

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

  std::string line;


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
      std::cout << "\r" << t << "/" << nev << std::flush;
      std::unordered_map<int, std::pair<double, double>> trackIdToAzMap;

      //reset_val();

      eventTree->GetEntry(t);
      inclTree->GetEntry(t);

      mass_rem = fMassRem[0];
      q_rem = fChargeRem[0];
      num_part = fParticles;

      nMCtracks = MCTrackCA->GetEntries();
      nRpc_point = rpcData->GetEntries();

      bool frag_exists =false, p_exists = false, n_exists = false, rpc_exist = false;
      TLorentzVector p_temp, n_temp;
      for (Int_t j = 0; j < nMCtracks; j++) {
        auto track = dynamic_cast<R3BMCTrack*>(MCTrackCA->At(j));
        int trackId = j;
        //int trackID = track->GetID();
        if (track->GetMotherId() == -1) {  // Only consider particles with mother ID -1
          double fZ_in = 0;
          double fA_in = 0;

          if (track->GetPdgCode() > 1000010010) {
            fZ_in = int(track->GetPdgCode() / 10000) - 100000;
            fA_in = 0.1 * (track->GetPdgCode() - (100000 + fZ_in) * 10000);
          }
          else if (track->GetPdgCode() == 2212) {  // Proton case
            fZ_in = 1;
            fA_in = 1;
          }
          else if (track->GetPdgCode() == 2112) {  // Neutron case
            fZ_in = 1;
            fA_in = 0;
          }
          // Store the A and Z values in the map using the track ID
          trackIdToAzMap[trackId] = std::make_pair(fA_in, fZ_in);
          h_Z_vs_AoZ_gen->Fill(fA_in/fZ_in, fZ_in);
          //std::cout << "Track ID: " << trackId << " has A: " << fA_in << " and Z: " << fZ_in << std::endl;
        }
      }
    
      int multi = 0;
      //int neutronMulti = 0;
      //int chargedMulti = 0;
      RPC_A.clear();
      RPC_Z.clear();
      PosY_Multi2.clear();

      bool RPC_Flag = false;


      for (Int_t k = 0; k < nRpc_point; k++) {
        auto rpcPoint = dynamic_cast<R3BRpcPoint*>(rpcData->At(k));

        // Retrieve the track ID for the current RPC point
        Int_t rpcTrackId = rpcPoint->GetTrackID();

        // Check if this track ID exists in the map and if the mother track ID is -1
        if (trackIdToAzMap.find(rpcTrackId) != trackIdToAzMap.end()) {
          RPC_Flag = true;
          multi +=1;
          const auto& [A, Z] = trackIdToAzMap[rpcTrackId];  // Get A and Z values from the map
          //if (rpcPoint->GetMotherId() == -1) {
          // Use A and Z values as needed, for example:
          //std::cout << "Track ID: " << rpcTrackId << " has A: " << A << " and Z: " << Z << std::endl;
          h_Z_vs_AoZ->Fill(A/Z, Z);

          RPC_A.push_back(A);
          RPC_Z.push_back(Z);

          TVector3 rpc_pos; //cm
          rpcPoint->PositionIn(rpc_pos);

          rpc_x = rpc_pos.X();
          rpc_y = rpc_pos.Y();

          RPCPosHit->Fill(rpc_x,rpc_y);

          PosY_Multi2.push_back(rpc_y);

          frag_A_vec.clear();
          frag_Z_vec.clear();
          frag_A_Multi1_vec.clear();
          frag_Z_Multi1_vec.clear();


          //std::cout << "Number of Charged Particles: " << chargedMulti << std::endl;
        
          //Multiplicity->Fill(multi);
/*
          if (multi == 1){
            ZvsAoZMulti1RPC->Fill(RPC_A[0]/RPC_Z[0], RPC_Z[0]);
            for (const auto& entry : trackIdToAzMap) {
              frag_A_Multi1_vec.push_back(entry.second.first);
              frag_Z_Multi1_vec.push_back(entry.second.second);
            }
          }
          if (multi == 2){
            Multi2RPC->Fill(RPC_A[0],RPC_A[1]);
            ZvsAoZMulti2RPC->Fill(RPC_A[0]/RPC_Z[0], RPC_Z[0]);
            ZvsAoZMulti2RPC->Fill(RPC_A[1]/RPC_Z[1], RPC_Z[1]);
          }
*/
          if (Z==2 || (A==2 && Z==1)){
            for (const auto& entry : trackIdToAzMap){
              HeORD->Fill(entry.second.first/entry.second.second, entry.second.second);
            }
          }
/*
          RPCMultivsNeutrons->Fill(multi, neutronMulti);
          RPCMultivsCP->Fill(multi, chargedMulti);
*/            
        }
        else{
          for (const auto& entry : trackIdToAzMap){
            h_Z_vs_AoZ_non_RPC->Fill(entry.second.first/entry.second.second, entry.second.second);
          }
        }  
      }

      //END OF RPC POINTS LOOP
      
      int neutronMulti = 0;
      int chargedMulti = 0;
      if (RPC_Flag){
          for (const auto& entry : trackIdToAzMap) {
            int A_gen = static_cast<int>(entry.second.first);
            int Z_gen = static_cast<int>(entry.second.second);

            if (A_gen == 0 && Z_gen == 1){
              neutronMulti +=1;
            }

            if (A_gen>0){
              chargedMulti +=1;
            }

            frag_A_vec.push_back(A_gen);
            frag_Z_vec.push_back(Z_gen);
          }
          NeutronMulti->Fill(neutronMulti);
          ChargedMulti->Fill(chargedMulti);
          RPCMultivsNeutrons->Fill(multi, neutronMulti);
          RPCMultivsCP->Fill(multi, chargedMulti);
      }

      if (multi>0) Multiplicity->Fill(multi);
      //ChargedMulti->Fill(chargedMulti);

      if (multi == 1){
        ZvsAoZMulti1RPC->Fill(RPC_A[0]/RPC_Z[0], RPC_Z[0]);
        for (const auto& entry : trackIdToAzMap) {
          frag_A_Multi1_vec.push_back(entry.second.first);
          frag_Z_Multi1_vec.push_back(entry.second.second);
        }
      }
      if (multi == 2){
        Multi2RPC->Fill(RPC_A[0],RPC_A[1]);
        ZvsAoZMulti2RPC->Fill(RPC_A[0]/RPC_Z[0], RPC_Z[0]);
        ZvsAoZMulti2RPC->Fill(RPC_A[1]/RPC_Z[1], RPC_Z[1]);
        if (PosY_Multi2.size() != 2){
          std::cout << "Error in PosY_Multi2 vector!" << endl;
        }
        else DeltaY_Multi2->Fill(PosY_Multi2[0] - PosY_Multi2[1]);
      }

      for (const auto& entry : trackIdToAzMap) {
        if (multi ==0) break;
        else if (entry.second.second == 6){
          for (int i = 0; i < RPC_A.size(); i++){
            Carbons->Fill(RPC_A[i]/RPC_Z[i], RPC_Z[i]);
            CarbonEvRPCNeutronMulti->Fill(multi,neutronMulti);
          }
        }
      }
      
      outtree->Fill();
    }

    _file2->cd();
    h_Z_vs_AoZ->Write();
    h_Z_vs_AoZ_gen->Write();
    RPCPosHit->Write();
    Multiplicity->Write();
    NeutronMulti->Write();
    ChargedMulti->Write();
    Multi2RPC->Write();
    ZvsAoZMulti1RPC->Write();
    ZvsAoZMulti2RPC->Write();
    h_Z_vs_AoZ_non_RPC->Write(); 
    Carbons->Write();
    HeORD->Write();
    RPCMultivsNeutrons->Write();
    RPCMultivsCP->Write(); 
    CarbonEvRPCNeutronMulti->Write();
    DeltaY_Multi2->Write();
    outtree->Write();
    delete _file2;

  }
}
