#include "stock.h"

/*
 * Function to minimize
 */
Double_t baseEval (double Dtime,double &alpha, double & bypassBeta,bool store,double &cap){
  double  f_val = 0 ;
  // linear increase 
  f_val+= alpha * cap;
  //random component
  f_val+= bypassBeta * cap;
  return f_val;
}

Double_t function(Double_t *x, Double_t *p){
  double time=x[0];
  double step=p[0];
  double initCap=p[1];
  double beta=p[2];
  double bet=p[3];
  double alpha=p[4];
  
  double capital=0;   
  for( double t=0; t < time ; t+=step)
    capital+=baseEval(t,alpha,beta,false,capital)+bet*baseEval(t+step,alpha,beta,false,capital);
  
  return capital;   
}

double stock::fit(double Dt, double bet, double betRange, double beta){
  if(m_tf1func!=NULL) delete  m_tf1func;
  m_tf1func =new TF1("tf1func",function,0.,2000.,5);
  m_tf1func->SetParName(0,"#Delta t");
  m_tf1func->SetParName(1,"C");
  m_tf1func->SetParName(2,"#beta");
  m_tf1func->SetParName(3,"bet");
  m_tf1func->SetParName(4,"#alpha");
  
  
  // Initialize
  m_tf1func->FixParameter(0,Dt);
  m_tf1func->FixParameter(1,m_integral);
  m_tf1func->SetParameter(2,beta);
  m_tf1func->SetParameter(3,bet);
  m_tf1func->FixParameter(4,m_alpha);
  
  m_tf1func->SetParLimits(3,bet-bet*betRange,bet+bet*betRange);
  
  fresult=m_hpInt->Fit(m_tf1func,"LE","",0,m_time); 
  return m_tf1func->GetParameter(3);
}



double stock::buy(double bet) {
  m_integral+=bet;
  m_totalSpent-= m_bet;
  m_potential +=bet;
  htrans->Fill(bet);
  return m_integral;
}

double stock::sell(double bet){
  m_integral -=bet;
  m_totalSpent+=m_bet;
  m_potential +=fabs(bet); 
  htrans->Fill(-bet);
  return m_integral;
}

double stock::getVar(double start,double stop){
  return (m_integral + m_totalSpent ) / m_totalSpent ; 
}

double stock::getLastBin(bool integ){
  TProfile *h=integ ? m_hpInt:m_hp; 
  for ( int i=1;i<(int)h->GetNbinsX() ;i++) 
    if( h->GetBinContent(i+1)==0 ) return h->GetBinContent(i);
  return h->GetBinContent(h->GetNbinsX());
}

double stock::eval(double Dtime,double bypassBeta,bool store,double cap){
  if(store) 
    m_shift=m_integral;
  
  m_betaVal=(m_beta->Uniform(-1,+1))*m_betaSigma;
  if(bypassBeta!=0) m_betaVal=bypassBeta;

  double thisIntegral = cap==0 ? m_integral:cap;
  
  thisIntegral+=baseEval(Dtime,m_alpha,m_betaVal,false,thisIntegral);
  
  if(store) {
    m_integral=thisIntegral;
    m_iter++;
    m_time+=Dtime;
    m_int_time+=m_time;
    m_iter++;
    m_shift-=m_integral;
    m_hp->Fill(m_time,m_shift/m_integral);
    m_hpInt->Fill(m_time,m_integral);
    m_hcashout->Fill(m_time,m_potential);
  }


#ifdef DEBUG
  std::cout<<"Evaluating Evolution "<< m_f_val<<" "<<m_integral<<" "<<m_time<<std::endl;
#endif
  return m_f_val;
}

double stock::setBetaSigma(double betaSigma){
  m_betaSigma = betaSigma;
}


void stock::iterTime(double time,double step){
  int i=0;
  double ltime=0;
  while ( ltime < time) {
    ltime+=step;
    this->eval(step);
  } 
  
}
