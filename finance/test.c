#include "TApplication.h"
#include "TSystem.h"
#include "TROOT.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include "TFile.h"
#include "TH1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TF1.h"
#include "TFile.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TF1.h"

#include "RooWorkspace.h"
#include "RooAbsData.h"
#include "RooStats/NumberCountingUtils.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/FeldmanCousins.h"
#include "RooStats/ToyMCSampler.h"
#include "RooStats/PointSetInterval.h"
#include "RooStats/ConfidenceBelt.h"
#include "RooDataHist.h"
#include "RooGenericPdf.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooHistPdf.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "RooStats/HypoTestResult.h"
#include "RooStats/ConfInterval.h"
#include "RooStats/ProfileLikelihoodTestStat.h"
#include "RooStats/SamplingDistribution.h"
#include "RooStats/SamplingDistPlot.h"
#include "RooStats/RooStatsUtils.h"
#include "RooStats/HypoTestResult.h"
#include "RooStats/SamplingDistPlot.h"
#include "RooStats/ModelConfig.h"
#include "TMath.h"
#include <vector>
#include <stdio>
#include <time>

using namespace RooFit;
using namespace RooStats;
using namespace std;

TLegend *DefLeg(double x1=0.6,double x2=0.7,double x3=.75,double x4=0.85){
  TLegend* legend=new TLegend(x1,x2,x3,x4);
  legend->SetFillColor(kWhite);
  legend->SetBorderSize(0);
  legend->SetTextSize(0.04);
  legend->SetTextFont(42);
  return legend;
}


double function(double t=0,
		double alpha=0,
		double f_int=0,
		double bet=0,
		double beta=0){

  double f=0;
  // linear increase 
  f = alpha*f_int;
 
  //random component
  f+=beta*f_int;
  f+=bet;
  
  /*
  cout<<"linear "<<f<<endl;
  cout<<"random "<<beta*f_int<<endl;
  cout<<"bet "<<bet<<endl;
  cout<<"total "<< f <<endl;
  */
  return f;
}

void test(double time=100,double step=.01){


  // constant term 
  double alpha=0.001;
  // Random component 
  double betaVar=.10;
  TRandom3 *r=new TRandom();
  double t=0;
  double f_tot=0;

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


  TRandom *r2=new TRandom();
  double f=0;

  while ( t < time) {
    
    double rt=r->Uniform(-betaVar,+betaVar);
    double alphaR = alpha;//r->Uniform(0,alpha);
   
    double randomDay =0;
    double  bet=0;

    //Betting Stragies 
    // Bet every 10 days 
    if ( i % 10  == 0 )
      {   bet = 100; betDays++; } 
    
    // Bet if beta - random prediction is > 0 
    //if( rt - r->Uniform(-betaVar,+betaVar)  > 0 ){
    // bet =  100; 
    // cout<<"day "<< i <<" seems to be a good day betting " <<rt<<endl;
    //betDays++;
    //}
    
    // Bet Only if day money was won
    //if( f/f_tot > 0 ){  bet= 100; betDays++; } 
    
    //  cout<<"Day "<<i<<" Betting "<<bet<<endl;
    
    // Initial Bet 
    if( i==0 ) bet=100;
    f =function(t,alphaR,f_tot,bet,rt);
    
    f_tot+=f;
    spent_-=bet;
    // Retrive 10 % of gain 
  

    timev.push_back(t);
    fintv.push_back(f_tot);
    fv.push_back(f/f_tot);
    
    hp->Fill(t,f);
    hpint->Fill(t,f_tot);
    
    
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

 cout<<"alpha "<<alpha<<" beta "<< betaVar<<endl;
 cout<<"Initial "<<100<<endl;
 cout<<"cumulative f "<<f_tot<<endl;
 cout<<"total spent "<<spent_<<endl;
 cout<<"total bet days "<< betDays*100/i <<"  % " <<endl;
 cout<<"Gain "<<((f_tot+spent_)/-spent_)*100<<" % "<<endl;
 
 
}
