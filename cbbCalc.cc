#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"

using namespace std;

double getWeight(int seed1, int seed2, double roundMult){
  //return 0.5+0.033746*abs(seed1-seed2); //weight toward lower seeds
  TF1 *normal = new TF1("normal","gaus(0)",0,300);
  normal->SetParameters(0.5,0,25);
  normal->SetMaximum(1.0);
  double ret =  1-(normal->Eval(abs(seed1-seed2))*roundMult);
  delete normal;
  return ret;
  //return 0.56+0.0294*abs(seed1-seed2);
  //return 0.67+0.22*abs(seed1-seed2); //weight toward higher seeds
}

void cbbCalc() {
  
  bool debug = 1;
  bool supprOutput=0;
  int nTrials=1;

  if(nTrials>1) {debug = 0; supprOutput=1;}
  //aim for about 17.5 total upsets based on historical trend.
  int nUpsets[4]={0,0,0,0};
  
  std::pair <double,double> scatterp[63*nTrials];
  int k=0;
  srand (time(NULL));
  ifstream infile("cbb_teams.txt", ifstream::in);
  std::pair <int,string> teams_r[6][64];

  int m=0;
  if(!supprOutput) cout << "Teams:" << endl;
  while(m<64){
    int seed, ranking;
    string name;
    infile >> ranking >> seed >> name;
    teams_r[0][m] = std::make_pair(ranking,name);
    if(!supprOutput) cout << teams_r[0][m].second << endl;
    m++;
  }
  k=0;
  for(int itrials=0; itrials<nTrials; itrials++){
    if(supprOutput){ if((itrials)%(nTrials/10)==0 && itrials>0){ cout << (double)itrials/(double)nTrials*100 << "%" << endl;}}
  for(int rnd=0; rnd<6; rnd++){
    int j=0;
    if(!supprOutput) cout << endl << "*******************" << endl << "Round: " << (rnd+2) << endl << "*******************" << endl;
    for(int i=0; i<pow(2,6-rnd); i+=2){
      double diff = (double)rand() / (double)RAND_MAX;
      int hseed,lseed;
      double rndweight = 0.;
      if(rnd==0) rndweight = 1.4;
      else if(rnd==1) rndweight = 1.1;
      else rndweight = 1.0;
      double prob = getWeight(teams_r[rnd][i].first,teams_r[rnd][i+1].first,rndweight);
      lseed = teams_r[rnd][i].first > teams_r[rnd][i+1].first ? i : i+1;
      hseed = teams_r[rnd][i].first < teams_r[rnd][i+1].first ? i : i+1;
      scatterp[k++] = std::make_pair(prob,diff);
      if(debug){
	cout << "team 1: " << teams_r[rnd][i].first << " " << teams_r[rnd][i].second << " team 2: " << teams_r[rnd][i+1].first << " " << teams_r[rnd][i+1].second << endl;
	cout << "rand %: " << diff << " prob of higher seed win: " << prob << endl;
      }
      if(rnd==5){
	if(diff > prob){
	  if(!supprOutput) cout << "Champion: "<< teams_r[rnd][lseed].second << endl;
	}
	else if(!supprOutput) cout << "Champion: "<< teams_r[rnd][hseed].second << endl;
      }
      else{
	if(diff > prob){
	  teams_r[rnd+1][j] = std::make_pair(teams_r[rnd][lseed].first,teams_r[rnd][lseed].second);
	  int upsetCounter = rnd < 3 ? rnd : 3;
	  if(abs(teams_r[rnd][lseed].first-teams_r[rnd][hseed].first) > 2) nUpsets[upsetCounter]++;
	}
	else{ teams_r[rnd+1][j] = std::make_pair(teams_r[rnd][hseed].first,teams_r[rnd][hseed].second); }
	if(!supprOutput) cout << "winner: " << teams_r[rnd+1][j].second << endl;
	j++;
      }
    }
  }
  }
  double xp[63*nTrials];
  double yp[63*nTrials];
  for(int i=0; i<63*nTrials; i++){
    xp[i] = 1.-scatterp[i].first;
    yp[i] = scatterp[i].second;
  }
  
  /* TH1 *h1 = new TH1("h1","",300,0,1);
  h1->SetXTitle("Probability of upset");
  h1->SetYTitle("Random Seed");
  h1->Draw();*/
  TF1 *l1 = new TF1("l1","-1*x+1",0,1);
  l1->Draw("");
  TGraphErrors *sp = new TGraphErrors(63*nTrials,xp,yp,0,0);
  sp->Draw("Pa,same");


  for(int i=0; i<4; i++){
    nUpsets[i] = nUpsets[i]/(double)nTrials;
  }
  cout << "avg 1st round upsets: " << nUpsets[0] << endl;
  cout << "avg 2nd round upsets: " << nUpsets[1] << endl;
  cout << "avg 3rd round upsets: " << nUpsets[2] << endl;
  cout << "avg other upsets: " << nUpsets[3] << endl;
}

