/*
 * Compile with g++ interaction.cxx -o interaction `root-config --cflags --libs`
 */

#include "lib/stock.h"
#include "lib/stock.cxx"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TProfile.h"
#include "TApplication.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TSystem.h"
#include "TMultiGraph.h"
#include "TROOT.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include<libgen.h>

using namespace std;

double SimpleMiniMizer(stock *a,double beta,
		       double betRangeLow=0, 
		       double betRangeUp=0,
		       double betaLow=0,
		       double betUp=0){
  
  double b=0; 
  double capital=a->getIntegral(); 
  double step=0.01*(betRangeUp- betRangeLow) ;
  double best=0;
  
  for( b = betRangeLow ; b < betRangeUp ; b+=step) 
    if( capital*beta + beta*capital*(b+step) > capital*beta + beta*capital*b )
      best = b; 
  
  cout<<"Best Bet will be "<< best <<" in range "<<betRangeLow<<" "<<betRangeUp<<endl;
  return best;
}


TLegend *DefLeg(double x1=0.6,double x2=0.7,double x3=.75,double x4=0.85){
  
  TLegend* legend=new TLegend(x1,x2,x3,x4);
  legend->SetFillColor(kWhite);
  legend->SetBorderSize(0);
  legend->SetTextSize(0.04);
  legend->SetTextFont(42);
  return legend;
}

int main(int argc, char *argv[]){

  double time = 100;
  double t = 0;
  double alpha = 0.001;
  double betaVal = .1;
  double step = .1; 
  
  // Plots & stuff
  // Graph
  vector <double> timev;
  vector <double> fintv;
  vector <double> fv;
  
  TProfile *hp=new TProfile("hp","",int(time*.1/step),0,time,-1e5,+1e5,"S");
  hp->GetXaxis()->SetTitle("time [a.u.]");
  hp->GetYaxis()->SetTitle("capital [a.u.]");
  hp->SetMarkerStyle(8);
  hp->SetMarkerColor(kBlack);
  hp->SetStats(0);

  TProfile *hpint=new TProfile("hpint","",int(time*.1/step),0,time,-1e5,+1e5,"S");
  hpint->GetXaxis()->SetTitle("time [a.u.]");
  hpint->GetYaxis()->SetTitle("capital [a.u.]");
  hpint->SetMarkerStyle(22);
  hpint->SetMarkerColor(kRed);
  hpint->SetStats(0);
  int  i=0;
  double spent_=0;
  int betDays=0;
  double f=0;
  
  stock *myStock= new stock(100,alpha,betaVal);
  myStock->setZeroOfPotential(100);
  // define a second correlated stock
  stock *corrStock=new stock(100,-alpha,betaVal);
  
  
  while ( t < time) {
    
    double alphaR = 0;//r->Uniform(0,alpha);
    double randomDay =0;
    double  bet=0;

    //Betting Stragies 
    // Bet every 10 days 
    // if ( i % 10  == 0 )
    // {      myStock->buy(100);   }     
    
    corrStock->eval(step);
    
    double buyVal=  SimpleMiniMizer(myStock,corrStock->getBetaVal(), 
				    -.99*myStock->getIntegral(),0.1*myStock->getPotential());
    
    myStock->buy(buyVal);   

    double f= myStock->eval(step,corrStock->getBetaVal());
    
    cout<<"Prediction "<<buyVal<<" change "<<
      myStock->getShift()*100/myStock->getIntegral()<<" % capital "<<myStock->getIntegral()<<endl;
    

    timev.push_back(t);
    fintv.push_back(myStock->getIntegral());
    fv.push_back(f);
    
    hp->Fill(t,f);
    hpint->Fill(t,myStock->getIntegral());
    
    
    t+=step;
    i++;
  } 



  TGraph *gr1=new TGraph((int)timev.size(),&timev[0],&fv[0]);
  gr1->SetMarkerStyle(8);
  gr1->SetMarkerColor(kBlack);

  TGraph *grint=new TGraph((int)timev.size(),&timev[0],&fintv[0]);
  grint->SetMarkerStyle(22);
  grint->SetMarkerColor(kRed);
  
  TMultiGraph *multi=new TMultiGraph("multi","");
  multi->Add(gr1);
  multi->Add(grint);


  TLegend *leg=(TLegend*)DefLeg()->Clone("leg");
  leg->AddEntry(gr1,"f","P");
  leg->AddEntry(grint,"cumulative f","P");
   
  TCanvas *c1=new TCanvas("c1","");{
    c1->cd();
    c1->SetTicks();
    c1->SetRightMargin(0.02);
    c1->SetTopMargin(0.02);
    multi->Draw("AP");
    multi->GetXaxis()->SetTitle("time [a.u]");
    multi->GetYaxis()->SetTitle("capital [a.u.]");
    leg->Draw();
    c1->Update();
  }
   



TCanvas *c2=new TCanvas("c2","");{
    c2->cd();
    c2->SetTicks();
    c2->SetRightMargin(0.02);
    c2->SetTopMargin(0.02);
    hp->Draw("P");
    hpint->Draw("P same");
    leg->Draw();
    c1->Update();
 }

 TFile *file= new TFile("interaction.root","RECREATE");
 file->cd();
 c1->Write();
 c2->Write();
 gr1->Write();
 grint->Write();
 hp->Write();
 hpint->Write();
 myStock->getHpotential()->Write();
 myStock->getTrans()->Write();
 myStock->getHist(false)->Write("f");
 myStock->getHist(true)->Write("cumulative");
 corrStock->getHist(false)->Write("f_c");
 corrStock->getHist(true)->Write("cumulative_c");

 TCanvas *c3 = new TCanvas("c3","");{

   
   TPad*    stack = new TPad("stack", "", 0, 0.25, 1, 1);   
   TPad*    comparison = new TPad("comparison", "", 0, 0, 1, 0.29);
   comparison->SetBottomMargin(0.35);
   stack->SetRightMargin(0.02);
   comparison->SetTopMargin(0.08);
   comparison->SetRightMargin(0.02);
   stack->SetTicks();
   stack->Draw();
   comparison->SetTicks();
   comparison->Draw();
   stack->cd();
   myStock->getHist(true)->Scale(1/fabs(myStock->getHist(true)->Integral()));
   myStock->getHist(true)->GetXaxis()->SetTitle(" ");
   myStock->getHist(true)->Draw("PE");
   corrStock->getHist(true)->SetMarkerStyle(22);
   corrStock->getHist(true)->SetMarkerColor(kRed);
   corrStock->getHist(true)->Scale(1/fabs(corrStock->getHist(true)->Integral()));
   corrStock->getHist(true)->Draw("SAME PE");

   TLegend *leg2=(TLegend*)DefLeg()->Clone("leg2");
   leg2->AddEntry(myStock->getHist(true),"#int_{0}^{t}f(t,x|#alpha,#beta)","P");
   leg2->AddEntry(corrStock->getHist(true),"input stock","P");

   leg2->Draw();
   
   comparison->cd();
   TProfile *h= myStock->getHist(false);
   h->SetTitle("");
   h->SetTitleSize(0);
   h->SetMarkerStyle(8);
   h->GetXaxis()->SetLabelSize(0.12);
   h->GetYaxis()->SetLabelSize(0.15);
   h->GetXaxis()->SetTitleSize(0.15);
   h->GetYaxis()->SetTitleSize(0.15);
   h->GetXaxis()->SetTitleOffset(1);
   h->GetYaxis()->SetTitleOffset(0.3);
   h->GetYaxis()->SetTitle("Daily Shift");
   h->GetYaxis()->SetNdivisions(505);
   h->GetYaxis()->SetRangeUser(-betaVal,+betaVal);
   h->Draw("PE");

 }

 c3->Write();


 cout<<"alpha "<<alpha<<" beta "<< betaVal<<endl;
 cout<<"Initial "<<100<<endl;
 cout<<"cumulative f "<<myStock->getIntegral()<<endl;
 cout<<"total spent "<<myStock->getSpent() <<endl;
 cout<<"total cahsout "<<myStock->getPotential()<<endl;
 cout<<"Gain "<<myStock->getVar()<<endl;

 
 return 1;
}
