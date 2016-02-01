
#include <cstdio>

#include <TChain.h>
#include <TSelector.h>
#include <TH2I.h>

#include <TS800.h>

class TS800Selector : public TSelector { 

  public:
    TS800Selector();
    virtual ~TS800Selector() { if(pid) pid->Delete(); }


  public:
    void Begin(TTree*)      ; 
    void Init(TTree*)       ; 
    bool Notify()           ; 
    bool Process(Long64_t)  ; 
    void Terminate()        ; 
  
  private:
    TH2I *pid;
    TS800 *s800;    
    //double ionsum;
    //double objtac;

    TChain *fChain=0;

  ClassDef(TS800Selector,0)
};
