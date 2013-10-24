#include "stock.h"


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

double stock::eval(double Dtime,double bypassBeta){
  m_shift=m_integral;

  m_betaVal=(m_beta->Uniform(-1,+1))*m_betaSigma;
  if(bypassBeta!=0) m_betaVal=bypassBeta;

  m_f_val = 0 ;
  // linear increase 
  m_f_val+=m_alpha * m_integral ;
  
  //random component
  m_f_val+=m_betaVal * m_integral;
  
  m_integral+=m_f_val ;
  m_iter++;
  m_time+=Dtime;
  m_int_time+=m_time;
  m_iter++;
  m_shift-=m_integral;
  
  m_hp->Fill(m_time,m_shift/m_integral);
  m_hpInt->Fill(m_time,m_integral);
  m_hcashout->Fill(m_time,m_potential);

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
