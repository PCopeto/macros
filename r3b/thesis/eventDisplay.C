void eventDisplay()
{
  FairRunAna *fRun= new FairRunAna();
  
  FairRuntimeDb* rtdb = fRun->GetRuntimeDb();
  FairParRootFileIo*  parIo1 = new FairParRootFileIo();
  parIo1->open("par.root");
  rtdb->setFirstInput(parIo1);
  rtdb->print();
  
  fRun->SetSource(new FairFileSource("sim.root"));
  fRun->SetOutputFile("test.root");
  
  R3BEventManager *fMan= new R3BEventManager();
  R3BMCTracks *Track =  new R3BMCTracks("Monte-Carlo Tracks");
//  FairMCTracksDraw *Track =  new FairMCTracksDraw ("Monte-Carlo Tracks");
//
  FairMCPointDraw *LandPoints = new FairMCPointDraw ("LandPoint",kOrange,  kFullSquare);
//  
//  FairMCPointDraw *RpcPoints = new FairMCPointDraw("RpcPoint", kRed, kFullSquare);
//  fMan->AddTask(RpcPoints);
//
//
  fMan->AddTask(Track);
  
//  fMan->AddTask(LandPoints);
  
  fMan->Init();
}
