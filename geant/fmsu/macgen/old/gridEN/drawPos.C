void drawPos()
{
  system("grep position *.mac | sed 's/macro_//g;s/\.m.*n//g' | awk '{print $1\" \"$2\" \"$3}'>pos.dat");
  system("cat pos.dat");
  TTree *tr = new TTree();
  tr->ReadFile("pos.dat","macro:x:y");
  //new TCanvas(); tr->Draw("y:x","macro","text");
  new TCanvas(); tr->Draw("y:x","","col");
  system("rm pos.dat");
}
