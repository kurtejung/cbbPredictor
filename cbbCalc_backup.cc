#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "TF1.h"
#include "TGraphErrors.h"

using namespace std;

double getWeight(int seed1, int seed2, double roundMult){
  //return 0.5+0.033746*abs(seed1-seed2); //weight toward lower seeds
  TF1 *normal = new TF1("normal","gaus(0)",0,300);
  normal->SetParameters(0.5,0,25);
  //normal->SetParameter(0,1./normal->Integral(-3000,3000));
  normal->SetMaximum(1.0);
  //normal->Draw();
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

  //aim for about 17.5 total upsets based on historical trend.
  int nUpsets[4]={0,0,0,0};
  
  std::pair <int,double> scatterp[63];
  int k=0;
  srand (time(NULL));
  ifstream infile("cbb_teams.txt", ifstream::in);;
  std::pair <int,string> teams_r1[64];
  std::pair <int,string> teams_r2[32];
  std::pair <int,string> teams_r3[16];
  std::pair <int,string> teams_r4[8];
  std::pair <int,string> teams_r5[4];
  std::pair <int,string> teams_r6[2];
  int m=0;
  if(!supprOutput) cout << "Teams:" << endl;
  while(m<64){
    int seed, ranking;
    string name;
    infile >> ranking >> seed >> name;
    teams_r1[m] = std::make_pair(ranking,name);
    if(!supprOutput) cout << teams_r1[m].second << endl;
    m++;
  }
  for(int itrials=0; itrials<nTrials; itrials++){
    if(supprOutput){ if((itrials)%(nTrials/10)==0 && itrials>0){ cout << (double)itrials/(double)nTrials*100 << "%" << endl;}}
    k=0;
  int j=0;
  if(!supprOutput) cout << endl << "Second Round:" << endl;
  for(int i=0; i<64; i+=2){
    double diff = (double)rand() / (double)RAND_MAX;
    int hseed,lseed;
    lseed = teams_r1[i].first > teams_r1[i+1].first ? i : i+1;
    hseed = teams_r1[i].first < teams_r1[i+1].first ? i : i+1;
    scatterp[k++] = std::make_pair(abs(teams_r1[i].first-teams_r1[i+1].first),diff);
    if(diff > getWeight(teams_r1[i].first,teams_r1[i+1].first,2.0)){
      teams_r2[j] = std::make_pair(teams_r1[lseed].first,teams_r1[lseed].second);
      if(abs(teams_r1[lseed].first-teams_r1[hseed].first) > 2) nUpsets[0]++;
    }
    else{ teams_r2[j] = std::make_pair(teams_r1[hseed].first,teams_r1[hseed].second); }
    if(!supprOutput) cout << teams_r2[j].second << endl;
    j++;
  }

  j=0;
  if(!supprOutput) cout << endl << "Sweet 16:" << endl;
  for(int i=0; i<32; i+=2){
    double diff = (double)rand() / (double)RAND_MAX;
    int hseed,lseed;
    lseed = teams_r2[i].first > teams_r2[i+1].first ? i : i+1;
    hseed = teams_r2[i].first < teams_r2[i+1].first ? i : i+1;
    scatterp[k++] = std::make_pair(abs(teams_r2[i].first-teams_r2[i+1].first),diff);
    if(debug){
      cout << "team 1: " << teams_r2[i].first << " " << teams_r2[i].second << " team 2: " << teams_r2[i+1].first << " " << teams_r2[i+1].second << endl;
      cout << "rand %: " << diff << " prob of higher seed win: " << getWeight(teams_r2[i].first,teams_r2[i+1].first,1) << endl;
    }
    if(diff > getWeight(teams_r2[i].first,teams_r2[i+1].first,1.2)){
      teams_r3[j] = std::make_pair(teams_r2[lseed].first,teams_r2[lseed].second);
      if(abs(teams_r2[lseed].first-teams_r2[hseed].first) > 2) nUpsets[1]++;
    }
    else{ teams_r3[j] = std::make_pair(teams_r2[hseed].first,teams_r2[hseed].second); }
    if(!supprOutput) cout << teams_r3[j].second << endl;
    j++;
  }

  j=0;
  if(!supprOutput) cout << endl << "Elite 8:" << endl;
  for(int i=0; i<16; i+=2){
    double diff = (double)rand() / (double)RAND_MAX;
    int hseed,lseed;
    lseed = teams_r3[i].first > teams_r3[i+1].first ? i : i+1;
    hseed = teams_r3[i].first < teams_r3[i+1].first ? i : i+1;
    scatterp[k++] = std::make_pair(abs(teams_r3[i].first-teams_r3[i+1].first),diff);
    if(debug){
      cout << "team 1: " << teams_r3[i].first << " " << teams_r3[i].second << " team 2: " << teams_r3[i+1].first << " " << teams_r3[i+1].second << endl;
      cout << "rand %: " << diff << " prob: " << getWeight(teams_r3[i].first,teams_r3[i+1].first,1) << endl;
    }
    if(diff > getWeight(teams_r3[i].first,teams_r3[i+1].first,1)){
      teams_r4[j] = std::make_pair(teams_r3[lseed].first,teams_r3[lseed].second);
      if(abs(teams_r3[lseed].first-teams_r3[hseed].first) > 2) nUpsets[2]++;
    }
    else{ teams_r4[j] = std::make_pair(teams_r3[hseed].first,teams_r3[hseed].second); }
    if(!supprOutput) cout << teams_r4[j].second << endl;
    j++;
  }

  j=0;
  if(!supprOutput) cout << endl << "Final 4:" << endl;
  for(int i=0; i<8; i+=2){
    double diff = (double)rand() / (double)RAND_MAX;
    int hseed,lseed;
    lseed = teams_r4[i].first > teams_r4[i+1].first ? i : i+1;
    hseed = teams_r4[i].first < teams_r4[i+1].first ? i : i+1;
    scatterp[k++] = std::make_pair(abs(teams_r4[i].first-teams_r4[i+1].first),diff);
    if(debug){
      cout << "team 1: " << teams_r4[i].first << " " << teams_r4[i].second << " team 2: " << teams_r4[i+1].first << " " << teams_r4[i+1].second << endl;
      cout << "rand %: " << diff << " prob: " << getWeight(teams_r4[i].first,teams_r4[i+1].first,1) << endl;
    }
    if(diff > getWeight(teams_r4[i].first,teams_r4[i+1].first,1)){
      teams_r5[j] = std::make_pair(teams_r4[lseed].first,teams_r4[lseed].second);
      if(abs(teams_r4[lseed].first-teams_r4[hseed].first) > 2) nUpsets[3]++;
    }
    else{ teams_r5[j] = std::make_pair(teams_r4[hseed].first,teams_r4[hseed].second); }
    if(!supprOutput) cout << teams_r5[j].second << endl;
    j++;
  }

  j=0;
  if(!supprOutput) cout << endl << "Final 2:" << endl;
  for(int i=0; i<4; i+=2){
    double diff = (double)rand() / (double)RAND_MAX;
    int hseed,lseed;
    lseed = teams_r5[i].first > teams_r5[i+1].first ? i : i+1;
    hseed = teams_r5[i].first < teams_r5[i+1].first ? i : i+1;
    scatterp[k++] = std::make_pair(abs(teams_r5[i].first-teams_r5[i+1].first),diff);
    if(debug){
      cout << "team 1: " << teams_r5[i].first << " " << teams_r5[i].second << " team 2: " << teams_r5[i+1].first << " " << teams_r5[i+1].second << endl;
      cout << "rand %: " << diff << " prob: " << getWeight(teams_r5[i].first,teams_r5[i+1].first,1) << endl;
    }
    if(diff > getWeight(teams_r5[i].first,teams_r5[i+1].first,1)){
      teams_r6[j] = std::make_pair(teams_r5[lseed].first,teams_r5[lseed].second);
      if(abs(teams_r5[lseed].first-teams_r5[hseed].first) > 2) nUpsets[3]++;
    }
    else{ teams_r6[j] = std::make_pair(teams_r5[hseed].first,teams_r5[hseed].second); }
    if(!supprOutput) cout << teams_r6[j].second << endl;
    j++;
  }

  double diff = (double)rand() / (double)RAND_MAX;
  int hseed,lseed;
  lseed = teams_r6[0].first > teams_r6[1].first ? 0 : 1;
  hseed = teams_r6[0].first < teams_r6[1].first ? 0 : 1;
  scatterp[k++] = std::make_pair(abs(teams_r6[0].first-teams_r6[1].first),diff);
  if(debug){
    cout << "team 1: " << teams_r6[0].first << " " << teams_r6[0].second << " team 2: " << teams_r6[1].first << " " << teams_r6[1].second << endl;
    cout << "rand %: " << diff << " prob: " << getWeight(teams_r6[0].first,teams_r6[1].first,1) << endl;
  }
  if(diff > getWeight(teams_r6[0].first,teams_r6[1].first,1)){
    if(abs(teams_r6[lseed].first-teams_r6[hseed].first) > 2) nUpsets[3]++;
    if(!supprOutput) cout << "Winner: " << teams_r6[lseed].second << endl;
  }
  else{ if(!supprOutput) cout << "Winner: " << teams_r6[hseed].second << endl;}
  }

  double xp[63];
  double yp[63];
  for(int i=0; i<63; i++){
    xp[i] = scatterp[i].first;
    yp[i] = scatterp[i].second;
  }
  
  TGraphErrors *sp = new TGraphErrors(63,xp,yp,0,0);
  sp->Draw("Pa");

  for(int i=0; i<4; i++){
    nUpsets[i] = nUpsets[i]/(double)nTrials;
  }
  cout << "avg 1st round upsets: " << nUpsets[0] << endl;
  cout << "avg 2nd round upsets: " << nUpsets[1] << endl;
  cout << "avg 3rd round upsets: " << nUpsets[2] << endl;
  cout << "avg other upsets: " << nUpsets[3] << endl;
}

