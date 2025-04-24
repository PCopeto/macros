void AnalyzeReactions() {
  // Open the ROOT file
  TFile *file = TFile::Open("RpcHits.root");
  if (!file || file->IsZombie()) {
    std::cerr << "Error: cannot open RpcHits.root" << std::endl;
    return;
  }

  // Get the tree
  TTree *tree = (TTree*)file->Get("evt");
  if (!tree) {
    std::cerr << "Error: Tree 'evt' not found in the file." << std::endl;
    return;
  }

  // Link branches
  std::vector<double> *frag_A = nullptr;
  std::vector<double> *frag_Z = nullptr;
  tree->SetBranchAddress("frag_A", &frag_A);
  tree->SetBranchAddress("frag_Z", &frag_Z);

  // Periodic table: Z -> element symbol
  std::vector<std::string> elements = {
    "n",  // Z=0 (placeholder for neutron)
    "H",  "He", "Li", "Be", "B",  "C",  "N",  "O",  "F",  "Ne",
    "Na", "Mg", "Al", "Si", "P",  "S",  "Cl", "Ar", "K",  "Ca",
    "Sc", "Ti", "V",  "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn",
    "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y",  "Zr",
    "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn",
    "Sb", "Te", "I",  "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd"
  };

  std::map<std::string, int> reaction_counts;
  std::map<std::string, int> fragment_counts;

  Long64_t nEvents = tree->GetEntries();
  for (Long64_t i = 0; i < nEvents; ++i) {
    tree->GetEntry(i);

    int n_neutrons = 0;
    int n_protons = 0;
    std::vector<std::string> nuclei;

    for (size_t j = 0; j < frag_A->size(); ++j) {
      int A = static_cast<int>(frag_A->at(j));
      int Z = static_cast<int>(frag_Z->at(j));

      if (A == 0 && Z == 1) {
        n_neutrons++;
        fragment_counts["n"]++;
      } else if (A == 1 && Z == 1) {
        n_protons++;
        fragment_counts["1H"]++;
      } else {
        std::string symbol = (Z >= 0 && Z < elements.size()) ? elements[Z] : "X";
        std::string frag = std::to_string(A) + symbol;
        nuclei.push_back(frag);
        fragment_counts[frag]++;
      }
    }

    std::sort(nuclei.begin(), nuclei.end());

    std::stringstream ss;
    if (n_neutrons > 0) ss << n_neutrons << "n ";
    if (n_protons > 0) ss << n_protons << "p ";
    for (const auto& nuc : nuclei) ss << nuc << " ";

    std::string reaction = ss.str();
    reaction_counts[reaction]++;
  }

  // Sort and print reactions
  std::vector<std::pair<std::string, int>> sorted_reactions(reaction_counts.begin(), reaction_counts.end());
  std::sort(sorted_reactions.begin(), sorted_reactions.end(),
            [](const auto &a, const auto &b) {
              return b.second < a.second;
            });

  std::cout << "\n=== Reaction Statistics (Sorted) ===\n";
  for (const auto& entry : sorted_reactions) {
    std::cout << entry.second << "x  " << entry.first << "\n";
  }

  // Sort and print fragments
  std::vector<std::pair<std::string, int>> sorted_fragments(fragment_counts.begin(), fragment_counts.end());
  std::sort(sorted_fragments.begin(), sorted_fragments.end(),
            [](const auto &a, const auto &b) {
              return b.second < a.second;
            });

  std::cout << "\n=== Fragment Counts (Sorted) ===\n";
  for (const auto& frag : sorted_fragments) {
    std::cout << frag.second << "x  " << frag.first << "\n";
  }

  // Filter and print reactions that include fragments with Z=6 (Carbon) or Z=7 (Nitrogen)
  std::cout << "\n=== Reactions with Z=6 (Carbon) or Z=7 (Nitrogen) ===\n";
  for (const auto& entry : sorted_reactions) {
    bool has_C_or_N = false;
    
    std::stringstream reaction_stream(entry.first);
    std::string fragment;
    
    while (reaction_stream >> fragment) {
      // Skip neutrons and protons
      if (fragment == "n" || fragment == "p") {
        continue;
      }
      
      // Check for fragments with Z=6 (Carbon) or Z=7 (Nitrogen)
      if (fragment.find("C") != std::string::npos || fragment.find("N") != std::string::npos) {
        has_C_or_N = true;
        break;
      }
    }

    // If the reaction has Carbon (C) or Nitrogen (N), print it
    if (has_C_or_N) {
      std::cout << entry.second << "x  " << entry.first << "\n";
    }
  }

  // === Now analyzing multiplicity 1 reactions ===
  std::vector<double> *frag_A_Multi1 = nullptr;
  std::vector<double> *frag_Z_Multi1 = nullptr;
  tree->SetBranchAddress("frag_A_Multi1", &frag_A_Multi1);
  tree->SetBranchAddress("frag_Z_Multi1", &frag_Z_Multi1);
  
  std::map<std::string, int> reaction_counts_m1;
  std::map<std::string, int> fragment_counts_m1;
  
  for (Long64_t i = 0; i < nEvents; ++i) {
    tree->GetEntry(i);
  
    int n_neutrons = 0;
    int n_protons = 0;
    std::vector<std::string> nuclei;
  
    for (size_t j = 0; j < frag_A_Multi1->size(); ++j) {
      int A = static_cast<int>(frag_A_Multi1->at(j));
      int Z = static_cast<int>(frag_Z_Multi1->at(j));
  
      if (A == 0 && Z == 1) {
        n_neutrons++;
        fragment_counts_m1["n"]++;
      } else if (A == 1 && Z == 1) {
        n_protons++;
        fragment_counts_m1["1H"]++;
      } else {
        std::string symbol = (Z >= 0 && Z < elements.size()) ? elements[Z] : "X";
        std::string frag = std::to_string(A) + symbol;
        nuclei.push_back(frag);
        fragment_counts_m1[frag]++;
      }
    }
  
    std::sort(nuclei.begin(), nuclei.end());
  
    std::stringstream ss;
    if (n_neutrons > 0) ss << n_neutrons << "n ";
    if (n_protons > 0) ss << n_protons << "p ";
    for (const auto& nuc : nuclei) ss << nuc << " ";
  
    std::string reaction = ss.str();
    reaction_counts_m1[reaction]++;
  }
  
  // Print full reactions (with counts)
  std::vector<std::pair<std::string, int>> sorted_reactions_m1(reaction_counts_m1.begin(), reaction_counts_m1.end());
  std::sort(sorted_reactions_m1.begin(), sorted_reactions_m1.end(),
            [](const auto &a, const auto &b) {
              return b.second < a.second;
            });
  
  std::cout << "\n=== [Multi1] Reaction Statistics (Sorted) ===\n";
  for (const auto& entry : sorted_reactions_m1) {
    std::cout << entry.second << "x  " << entry.first << "\n";
  }
  
  // Print fragments
  std::vector<std::pair<std::string, int>> sorted_fragments_m1(fragment_counts_m1.begin(), fragment_counts_m1.end());
  std::sort(sorted_fragments_m1.begin(), sorted_fragments_m1.end(),
            [](const auto &a, const auto &b) {
              return b.second < a.second;
            });
  
  std::cout << "\n=== [Multi1] Fragment Counts (Sorted) ===\n";
  for (const auto& frag : sorted_fragments_m1) {
    std::cout << frag.second << "x  " << frag.first << "\n";
  }
  
  // Print only reactions that include C or N
  std::cout << "\n=== [Multi1] Reactions with Z=6 (Carbon) or Z=7 (Nitrogen) ===\n";
  for (const auto& entry : sorted_reactions_m1) {
    bool has_C_or_N = false;
  
    std::stringstream reaction_stream(entry.first);
    std::string fragment;
  
    while (reaction_stream >> fragment) {
      if (fragment == "n" || fragment == "p") continue;
      if (fragment.find("C") != std::string::npos || fragment.find("N") != std::string::npos) {
        has_C_or_N = true;
        break;
      }
    }
  
    if (has_C_or_N) {
      std::cout << entry.second << "x  " << entry.first << "\n";
    }
  }


  // === Now analyzing Multi1_RPCandTofd reactions ===
  std::vector<double> *frag_A_Multi1_RPCandTofd = nullptr;
  std::vector<double> *frag_Z_Multi1_RPCandTofd = nullptr;
  tree->SetBranchAddress("frag_A_Multi1_RPCandTofd", &frag_A_Multi1_RPCandTofd);
  tree->SetBranchAddress("frag_Z_Multi1_RPCandTofd", &frag_Z_Multi1_RPCandTofd);

  std::map<std::string, int> reaction_counts_rpc;
  std::map<std::string, int> fragment_counts_rpc;

  for (Long64_t i = 0; i < nEvents; ++i) {
    tree->GetEntry(i);

    int n_neutrons = 0;
    int n_protons = 0;
    std::vector<std::string> nuclei;

    for (size_t j = 0; j < frag_A_Multi1_RPCandTofd->size(); ++j) {
      int A = static_cast<int>(frag_A_Multi1_RPCandTofd->at(j));
      int Z = static_cast<int>(frag_Z_Multi1_RPCandTofd->at(j));

      if (A == 0 && Z == 1) {
        n_neutrons++;
        fragment_counts_rpc["n"]++;
      } else if (A == 1 && Z == 1) {
        n_protons++;
        fragment_counts_rpc["1H"]++;
      } else {
        std::string symbol = (Z >= 0 && Z < elements.size()) ? elements[Z] : "X";
        std::string frag = std::to_string(A) + symbol;
        nuclei.push_back(frag);
        fragment_counts_rpc[frag]++;
      }
    }

    std::sort(nuclei.begin(), nuclei.end());

    std::stringstream ss;
    if (n_neutrons > 0) ss << n_neutrons << "n ";
    if (n_protons > 0) ss << n_protons << "p ";
    for (const auto& nuc : nuclei) ss << nuc << " ";

    std::string reaction = ss.str();
    reaction_counts_rpc[reaction]++;
  }

  // Sort and print reactions
  std::vector<std::pair<std::string, int>> sorted_reactions_rpc(reaction_counts_rpc.begin(), reaction_counts_rpc.end());
  std::sort(sorted_reactions_rpc.begin(), sorted_reactions_rpc.end(),
            [](const auto &a, const auto &b) {
              return b.second < a.second;
            });

  std::cout << "\n=== [Multi1_RPCandTofd] Reaction Statistics (Sorted) ===\n";
  for (const auto& entry : sorted_reactions_rpc) {
    std::cout << entry.second << "x  " << entry.first << "\n";
  }

  // Sort and print fragment counts
  std::vector<std::pair<std::string, int>> sorted_fragments_rpc(fragment_counts_rpc.begin(), fragment_counts_rpc.end());
  std::sort(sorted_fragments_rpc.begin(), sorted_fragments_rpc.end(),
            [](const auto &a, const auto &b) {
              return b.second < a.second;
            });

  std::cout << "\n=== [Multi1_RPCandTofd] Fragment Counts (Sorted) ===\n";
  for (const auto& frag : sorted_fragments_rpc) {
    std::cout << frag.second << "x  " << frag.first << "\n";
  }

  // Print only reactions that include C or N
  std::cout << "\n=== [Multi1_RPCandTofd] Reactions with Z=6 (Carbon) or Z=7 (Nitrogen) ===\n";
  for (const auto& entry : sorted_reactions_rpc) {
    bool has_C_or_N = false;

    std::stringstream reaction_stream(entry.first);
    std::string fragment;

    while (reaction_stream >> fragment) {
      if (fragment == "n" || fragment == "p") continue;
      if (fragment.find("C") != std::string::npos || fragment.find("N") != std::string::npos) {
        has_C_or_N = true;
        break;
      }
    }

    if (has_C_or_N) {
      std::cout << entry.second << "x  " << entry.first << "\n";
    }
  }
  }

