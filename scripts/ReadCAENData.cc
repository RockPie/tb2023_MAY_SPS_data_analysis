#include <fstream>
#include <string>
#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>
#include <array>
#include <cstdio>
#include <numeric>
#include <optional>
#include <chrono>
#include <typeinfo>
#include <cstdlib>
#include <ctime>


int nboard = 0;
int nchannelb = 0;
int nchannel[200];

int evID;
int tCh;
int tChB0;
int tChB1;
int evcount=0;
//int linenum;


typedef struct 
{
  int ib;
  uint32_t q[64];
  double ts;
  int trgID;
  char type[16];
  int nfired;
} event_t;
		
struct his_s 
{
  TFile *outFile;
  TH1F *hNCh;
  TH1F *hCharge[64];
  TH1F *hCharge1[64];
  TH1F *hTotCharge;
  TH2F *hTotChargeMap;
  TH2F *hShowerPosition;
  TH1F *hTotChargeB0;
  TH1F *hTotChargeB1;
  TH2F *hChargeMap;
  TH1F *hTotChargeEvent[10];
  TH1F *hTotChargeMatched;
  TH1F *hTotChargeMatchedB0;
  TH1F *hTotChargeMatchedB1;
} his;

struct Board 
{
  unsigned long int nboards;
  double ts;
  unsigned long int trigID;
  std::map<int, int> charge;
};

struct Event 
{
  int id;
  double ts0;
  double ts1;
  std::map<int, int> charge0;
  std::map<int, int> charge1;
};


std::ostream &operator<<( std::ostream &os, const Board &b )
{
  char cts[20];
  sprintf( cts, "%16.3lf", b.ts );
  os << b.nboards << cts << "  " << b.trigID << "  " << b.charge.size() << '\n';
  for( const auto &[k, v] : b.charge )
    os << k << "  " << v << '\n'; //k = kanal, v = charge
  // for( const auto &m: b.charge)
  //   m.first = kanal, m.second = charge
  return os;
}


std::ostream &operator<<( std::ostream &os, const Event &ev )
{
  char cts0[20];
  sprintf( cts0, "%16.3lf", ev.ts0 );
  char cts1[20];
  sprintf( cts1, "%16.3lf", ev.ts1 );
  char ctsd[20];
  sprintf( ctsd, "%16.3lf", fabs( ev.ts0 - ev.ts1 ) );
  os << ev.id << cts0 << cts1 << ctsd << '\n';
  os << "0 " << ev.charge0.size() << '\n';
  for( const auto &[k, v] : ev.charge0 )
    os << k << "  " << v << '\n';
  os << "1 " << ev.charge1.size() << '\n';
  for( const auto &[k, v] : ev.charge1 )
    os << k << "  " << v << '\n';
  return os;
}


std::optional<Board> readboard( std::ifstream &fdata )
{
  std::string line;
  Board board;
  
  while( getline( fdata, line ) )
  { 
    if( line.front() == '/' )
      continue;
    else if( line.substr( 0, 5 ) == "Board" )
    {
      //printf("nboards: %s\n", (line.substr( 6, 1 )).c_str());
      board.nboards = stoul( line.substr( 6, 1 ) );
    }
    else if( line.substr( 0, 2 ) == "TS" ) {
      int start = line.find( "=" );
      int end = line.find( " " );
      board.ts = stod( line.substr( start + 1, end - start ) );
    }
    else if( line.substr( 0, 5 ) == "TrgID" )
    {
      //printf("trigID: %s\n", (line.substr( 6 )).c_str());
      board.trigID = stoul( line.substr( 6 ) );
    }
    else if( line.substr( 0, 2 ) == "CH" )
      continue;
    else if( line.front() == '-' ) 
      {
	return board;
      }
    else
    {
      const unsigned long int channel = stoul( line.substr( 0, 2 ) );
      
      if ( line.substr( 2 ).length() == 0 )
      {
	printf( "\nBad Entry: Board %lu, TrigID: %lu.\n Skipping...\n", board.nboards, board.trigID );
	continue;
      }
      
      const unsigned long int charge = stoul( line.substr( 2 ) );
      board.charge.insert( { channel, charge } );
    }
    //linenum++;
  }
  
  return std::nullopt;
}



void readdata( const std::string &filename )
{
  char ename[64];
  char etitle[64];
  
  std::ifstream fdata( filename );
  if( !fdata.good() ) 
  {
    std::cout << "Cannot open the file " << filename << '\n';
    return;
  }
  std::array<std::vector<Board>, 2> boards;
  while( true )
  {
    auto optb = readboard( fdata );
    if( !optb ) break;
    Board board = optb.value();
    const int idx = board.nboards == 0 ? 1 : 0;
    auto it = std::find_if(
			   boards[idx].begin(), boards[idx].end(),
			   [trig = board.trigID]( const Board &b ) { return b.trigID == trig; } );
    if( it == boards[idx].end() )
      boards[board.nboards].push_back( board );
    else 
    {
      Event event;
      event.id = board.trigID;
      if( board.nboards == 0 ) 
      {
	event.ts0 = board.ts;
	event.ts1 = it->ts;
	event.charge0 = std::move( board.charge );
	event.charge1 = std::move( it->charge );
      }
      else 
      {
	event.ts0 = it->ts;
	event.ts1 = board.ts;
	event.charge0 = std::move( it->charge );
	event.charge1 = std::move( board.charge );
      }
      /*
      std::cout << event.id << "  "
		<< std::accumulate(
				   event.charge0.begin(), event.charge0.end(), 0,
				   []( int acc, const auto &m ) { return acc + m.second; } ) +
	           std::accumulate(
				   event.charge1.begin(), event.charge1.end(), 0,
				   []( int acc, const auto &m ) { return acc + m.second; } )
		<< '\n';
      */
      evID = event.id;
      
      tCh = std::accumulate(
			    event.charge0.begin(), event.charge0.end(), 0,
			    []( int acc, const auto &m ) { return acc + m.second; } ) +
	    std::accumulate(
			    event.charge1.begin(), event.charge1.end(), 0,
			    []( int acc, const auto &m ) { return acc + m.second; } );

      tChB0 = std::accumulate(
			    event.charge0.begin(), event.charge0.end(), 0,
			    []( int acc, const auto &m ) { return acc + m.second; } );

      tChB1 = std::accumulate(
			    event.charge1.begin(), event.charge1.end(), 0,
			    []( int acc, const auto &m ) { return acc + m.second; } );

      if ( tCh > 0 )
      {
	his.hTotChargeMatched->Fill(tCh);
      }

      if ( tChB0 > 0 )
      {
	his.hTotChargeMatchedB0->Fill(tChB0);
      }
	    
      if ( tChB1 > 0 )
      {
	his.hTotChargeMatchedB1->Fill(tChB1);
      }
      
      boards[idx].erase( it );
    }
    //linenum++;
  }
} 	 



std::map<std::pair<int, int>, std::pair<int, int>>
load_table( const std::string &filename )
{
  int b, c, x, y;
  std::map<std::pair<int, int>, std::pair<int, int>> table;
  std::ifstream fdata( filename.c_str() );
  while( fdata >> b >> c >> x >> y ) 
  {
    table[{ b, c }] = { x, y };
  }
  return table;
}



const auto table = load_table ("mapping.txt");

void histoInit(const char *fname) 
{
  char name[64];
  char title[64];
  char name1[64];
  char title1[64];
  
  his.outFile = new TFile(TString(fname)+".root","RECREATE");
  his.outFile->cd();

  his.hNCh = new TH1F("hNCh","Number of fired channels",64,0.0,64);
  
  for (int i = 0; i<64;i++) 
  {
    sprintf(name,"hCharge-%2d",i);
    sprintf(title,"Charge distribution in channel %d",i);
    his.hCharge[i] = new TH1F(name,title,1025,0.0,4100);
  }  
  
  for (int i = 0; i<64;i++) 
  {
    sprintf(name1,"hCharge1-%2d",i+100);
    sprintf(title1,"Charge distribution in channel %d",i+100);
    his.hCharge1[i] = new TH1F(name1,title1,1025,0.0,4100);
  }
  
  his.hTotCharge = new TH1F("hTotCharge","Total charge in the event",1000,0.0,60000);
  
  his.hTotChargeB0 = new TH1F("hTotChargeB0","Total charge in the event",1000,0.0,60000);
  
  his.hTotChargeB1 = new TH1F("hTotChargeB1","Total charge in the event",1000,0.0,60000);
    
  his.hTotChargeMap = new TH2F("hTotChargeMap","Map of the charge distribution",7,0.0,7.0,7,0.0,7.0);

  his.hChargeMap = new TH2F("hChargeMap", "Map of the charge distribution for outter part", 15,0.0,15.0,15,0.0,15.0);
  
  his.hShowerPosition = new TH2F("hShowerPosition","Impact position of the particle",7,0.0,7.0,7,0.0,7.0);

  his.hTotChargeMatched = new TH1F( "hTotChargeMatched", "Total charge for matched events", 1000, 0.0, 80000 );

  his.hTotChargeMatchedB0 = new TH1F("hTotChargeMatchedB0","Total charge in the matched events",1000,0.0,60000);
  
  his.hTotChargeMatchedB1 = new TH1F("hTotChargeMatchedB1","Total charge in the matched events",1000,0.0,60000);
}



void histoExit() 
{
  his.outFile->Write();
  his.outFile->Close();
}



ssize_t getEvent(event_t &evt,std::ifstream &infile) 
{
  std::string line;
  int bf = 0;
  int tsf = 0;
  int trigf = 0;
  int typef = 0;
  char buf[256];
  int nfired = 0;
  
  ssize_t res = 0;
  while (std::getline(infile, line)) 
  {
    //check the individual cases
    if(line.find("//") == 0) continue; //few lines of comments in the header
    if(line.find("---------------------------------") == 0) 
    {
      evt.nfired=nfired; //this event is over
      return 1;
    }
      
    
    if(line.find("Board 1") != string::npos) 
    {
      nboard = 1;
      std::istringstream iss(line);
      iss >> buf >> evt.ib;
      bf=1;
      continue;
    }
    
    
    if(line.find("Board 0") != string::npos) 
    {
      nboard = 0;
      std::istringstream iss(line);
      iss >> buf >> evt.ib;
      bf=1;
      continue;
    }
    
    
    if(line.find("TS") != string::npos) 
    {
      std::istringstream iss(line.substr(3));
      iss >> evt.ts;
      tsf = 1;
      continue;
    }  
    if(line.find("TrgID") != string::npos) 
    {
      std::istringstream iss(line.substr(6));
      iss >> evt.trgID;
      trigf = 1;
      continue;
    }
    if(line.find("CH") != string::npos) 
    {
      std::istringstream iss(line);
      iss >> buf >> evt.type;
      typef = 1;
      continue;
    }  
    if (bf == 1 && tsf == 1 && trigf == 1 && typef == 1) 
    {
      std::istringstream iss(line);
	  
      int ich;
      int charge;
      
      if (!(iss >> ich >> charge)) 
      {	
	break;
      } // error
      //std::cout << "Channel:   " << ich << "   Charge: " << charge << std::endl;
      // process pair (a,b)
      
      if(ich >=0 && ich < 64) 
      {
	evt.q[ich] = charge;
	nchannel[nfired] = ich;
	nfired++;
      }
    } 
    else 
    {
      continue;
    }
  }
  

  return 0;
}



void clearchannel()
{
  for (int i=0;i<200;i++)
  {
    nchannel[i]=0;
  }
}



int analyzeEvent(event_t &evt) 
{
  his.hNCh->Fill(evt.nfired);
  
  float totCharge = 0.;
  float totChargeB0 = 0.;
  float totChargeB1 = 0.;

  float xmean = 0.;
  float ymean = 0.;
  int channelid = 0;

  //TTree Tree_hCharge( "hCharge", "hCharge Histo Tree", 128000 );
  //TTree Tree_hCharge1( "hCharge1", "hCharge1 Histo Tree", 128000 );
  
  for (int i = 0; i < 64; i++) 
  {
    if (evt.q[i]<=0) 
    {
      continue;
    }

    //char hname;
    //char hname1;

    //sprintf( hname, "hCharge-%2d",i);
    //sprintf( hname1, "hCharge1-%2d",i);
    
    //Tree_hCharge.Branch( ("hCharge-%d",i), "TH1F", &hname );
    //Tree_hCharge.Branch( ("hCharge1-%d",i), "TH1F", &hname1 );

    auto [x, y] = table.at (std::make_pair(nboard, i));    
    totCharge += 1.*evt.q[i];
    
    if (nboard == 1) 
    {
      his.hCharge1[i]->Fill(1.*evt.q[i]);
      his.hChargeMap->Fill(x, 14-y, evt.q[i]);      
      totChargeB1 +=1.*evt.q[i];
    }
    
    if (nboard == 0) 
    {
      his.hCharge[i]->Fill(1.*evt.q[i]);
      totChargeB0 +=1.*evt.q[i];
      
      if (i>48)
      {
	his.hChargeMap->Fill(x, 14-y, evt.q[i]);
      }
      
      else { his.hTotChargeMap->Fill(x,6-y,1.*evt.q[i]);}	
    }

    //Tree_hCharge->Fill();
    //Tree_hCharge1->Fill();
  }
  
  if(totCharge>0)
  {
    his.hTotCharge->Fill(totCharge);
  }
  
  if(totChargeB0>0){
    his.hTotChargeB0->Fill(totChargeB0);}
  
  if(totChargeB1>0){
    his.hTotChargeB1->Fill(totChargeB1);}  
  
  return 0;
}



void clearEvent(event_t *ev)
{
  memset(ev,0,sizeof(event_t));
}



void ReadCAENData(const char *fname)
{
  auto begin = std::chrono::high_resolution_clock::now();
    
  event_t evt;
  histoInit(fname);
  readdata(fname);
  std::ifstream infile(fname);
  int nev = 0;

  clearEvent(&evt);
  while (getEvent(evt,infile)) 
  {
    nev++;
    //std::cout << "=============== New event =================: " << nev << std::endl;
    analyzeEvent(evt);
    clearEvent(&evt);
  }
  clearchannel();
  histoExit();

  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end-begin);

  printf("\nRuntime: %ld  seconds\n", elapsed.count());
}
