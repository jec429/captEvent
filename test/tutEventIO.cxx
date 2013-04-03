/////////////////////////////////////////////////////////////
// Provide tests of the event input and output.  This also tests many of the
// associated classes.
/////////////////////////////////////////////////////////////
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <tut.h>

#include "TND280Event.hxx"
#include "TRootInput.hxx"
#include "TRootOutput.hxx"
#include "TMCHit.hxx"
#include "TMultiHit.hxx"
#include "TG4VHit.hxx"
#include "TG4HitSegment.hxx"
#include "TAlgorithmResult.hxx"
#include "THandleHack.hxx"
#include "TReconBase.hxx"
#include "TReconCluster.hxx"
#include "TReconShower.hxx"
#include "TReconTrack.hxx"
#include "ND280GeomId.hxx"

namespace {
    class TTestG4VHit : public CP::TG4HitSegment {
    public:
        TTestG4VHit() {};
        virtual ~TTestG4VHit() {};
        void SetPrimaryId(int i) {fPrimaryId = i;}
        void SetEnergyDeposit(double d) {fEnergyDeposit = d;}
    };
    
    int gIndexNumber = 1;
    std::string IndexNumber(int i=-1) {
        std::stringstream tmp;
        if (i<0) tmp << gIndexNumber++;
        else tmp << i;
        return tmp.str();
    }

    void CreateTG4Hits(CP::TDataVector& truth, const char* name) {
        if (!truth.FindDatum("g4Hits")) {
            truth.push_back(new CP::TDataVector("g4Hits"));
        }

        // Create the hit container and addit to the event.
        CP::TG4HitContainer* hits = new CP::TG4HitContainer(name);
        truth.Get<CP::TDataVector>("g4Hits")->push_back(hits);

        // Create hits for the event.
        for (int i = 1; i< 6; ++i) {
            TTestG4VHit* hit = new TTestG4VHit();
            hit->SetPrimaryId(i);
            hit->SetEnergyDeposit(10.0 + 1.0*i);
            hits->push_back(hit);
        }
    }

    void CreateTruth(CP::TND280Event& event) {
        event.push_back(new CP::TDataVector("truth"));
        CP::THandle<CP::TDataVector> truth 
            = event.Get<CP::TDataVector>("truth");
        CreateTG4Hits(*truth,"p0d");
        CreateTG4Hits(*truth,"fgd");
    }

    void CreateHits(CP::TND280Event& event, std::string name) {
        std::string g4Name = "truth/g4Hits/" + name;
        CP::THandle<CP::TG4HitContainer> g4Hits 
            = event.Get<CP::TG4HitContainer>(g4Name);
        if (!g4Hits) return;

        std::string containerTitle("Hits for the "
                                   + name 
                                   + " detector");

        CP::THitSelection *hits = new CP::THitSelection(name.c_str(),
                                                        containerTitle.c_str());
        event.Get<CP::TDataVector>("hits")->push_back(hits);

        for (CP::TG4HitContainer::iterator g4Hit = g4Hits->begin();
             g4Hit != g4Hits->end();
             ++g4Hit) {
            CP::TWritableMCHit mcHit;
            CP::TG4HitSegment* hitSeg
                = dynamic_cast<CP::TG4HitSegment*>(*g4Hit);
            mcHit.SetGeomId(CP::GeomId::P0D::Bar(20,0,50));
            mcHit.SetCharge(hitSeg->GetPrimaryId());
            mcHit.SetTime(hitSeg->GetPrimaryId());
            mcHit.GetContributors().push_back(hitSeg);
            mcHit.GetContributors().push_back(NULL);
            hits->push_back(CP::THandle<CP::TMCHit>(new CP::TMCHit(mcHit)));
        }
    }

    void CreateWaveform(CP::TND280Event& event, std::string name) {
        std::string g4Name = "truth/g4Hits/" + name;
        CP::THandle<CP::TG4HitContainer> g4Hits 
            = event.Get<CP::TG4HitContainer>(g4Name);
        if (!g4Hits) return;

        std::string containerTitle("Hits for the "
                                   + name 
                                   + " detector");

        CP::THitSelection *hits = new CP::THitSelection(name.c_str(),
                                                        containerTitle.c_str());
        event.Get<CP::TDataVector>("hits")->push_back(hits);

        
        // Create maxWaveforms worth of TMultiHits.
        int maxWaveforms = 20;
        std::vector< CP::THandle<CP::TSingleHit> > wave(600);
        for (int i = 0; i< maxWaveforms; ++i) {
            // Create a waveform.
            wave.clear();
            for (int j = 0; j<512; ++j) {
                CP::TWritableMCHit tmpHit;
                tmpHit.SetGeomId(CP::GeomId::FGD::Bar(1,3,0,40));
                tmpHit.SetCharge(1.0);
                tmpHit.SetTime(j);
                wave.push_back(CP::THandle<CP::TSingleHit>(new CP::TMCHit(tmpHit)));
            }
            hits->push_back(
                CP::THandle<CP::THit>(new CP::TMultiHit(wave.begin(),wave.end())));
        }
    }

    void CreateResults(CP::TND280Event& event) {
        // Create a simple algorithm result.
        CP::TAlgorithmResult *result
            = new CP::TAlgorithmResult("firstResult",
                                       "Test Result");
        CP::THandle<CP::THitSelection> hits = event.GetHitSelection("p0d");
        CP::THitSelection *newHits = new CP::THitSelection("newHits");
        std::copy(hits->begin(), hits->end(), std::back_inserter(*newHits));
        result->AddHitSelection(newHits);
        event.AddFit(result);

        // Create a second algorithm result.
        result = new CP::TAlgorithmResult("secondResult",
                                          "Test Result");
        hits = event.GetHitSelection("p0d");
        CP::THitSelection* moreHits = new CP::THitSelection("moreHits");
        std::copy(hits->begin(),hits->end(),std::back_inserter(*moreHits));
        result->AddHitSelection(moreHits);
        event.AddFit(result);

        /// Create an CP::TAlgorithmResult with reconstruction objects.
        ND280Debug("Create Recon Object");
        result = new CP::TAlgorithmResult("reconResult",
                                          "Test Recon Object Result");
        CP::TReconObjectContainer* recObjs
            = new CP::TReconObjectContainer("reconObjects");
        result->AddResultsContainer(recObjs);
        // Add a cluster.
        CP::THandle<CP::TReconCluster> cluster(new CP::TReconCluster);
        CP::THandle<CP::TClusterState> clusterState = cluster->GetState();
        clusterState->SetEDeposit(10.0);
        recObjs->push_back(cluster);
        // Add a shower.
        CP::THandle<CP::TReconShower> shower(new CP::TReconShower);
        recObjs->push_back(shower);
        // Add a track.
        CP::THandle<CP::TReconTrack> track(new CP::TReconTrack);
        recObjs->push_back(track);
        event.AddFit(result);
    }

    void CreateTemporary(CP::TND280Event& event) {
        CP::TDatum* d1 = new CP::TDatum("temporaryObject");
        event.AddDatum(d1);
    }

    void FillEvent(CP::TND280Event& event) {
        CreateTruth(event);
        CreateHits(event,"p0d");
        CreateWaveform(event,"fgd");
        CreateResults(event);
        CreateTemporary(event);
    }
}

namespace tut {
    struct baseEventIO {
        static std::vector<CP::TND280Event*> outputEvents;
        static std::vector<CP::TND280Event*> inputEvents;
        baseEventIO() {
            // Run before each test.
            const char* fileName = "./tutEventIO.root";
            // Write events to an output file and save them in a local vector.
            try {
                if (outputEvents.size()<1) {
                    CP::TRootOutput* output 
                        = new CP::TRootOutput(fileName,"RECREATE");
                    for (int i=0; i<5; ++i) {
                        CP::TND280Event* event = new CP::TND280Event();
                        event->SetRunId(1);
                        event->SetEventId(i);
                        ND280Trace("Write event");
                        outputEvents.push_back(event);
                        FillEvent(*event);
                        output->WriteEvent(*event);
                    }
                    output->Close();
                    delete output;
                }
            }
            catch (std::exception& e) {
                ND280Error("Error writing output file");
                ND280Error("exception: " << e.what());
                return;
            }
            // Read the same events from the file and save them in another
            // local vector.
            try {
                if (inputEvents.size()<1) {
                    // Read the event back.
                    CP::TRootInput* input = new CP::TRootInput(fileName,"OLD");
                    for (CP::TND280Event* event = input->FirstEvent();
                         !input->EndOfFile();
                         event = input->NextEvent()) {
                        ND280Trace("Read event");
                        inputEvents.push_back(event);
                    }
                    input->Close();
                    delete input;
                }
            }
            catch (std::exception& e) {
                ND280Error("Error reading input file");
                ND280Error("exception: " << e.what());
                return;
            }
        }
        ~baseEventIO() {
            // Run after each test.
        }
    };

    std::vector<CP::TND280Event*> baseEventIO::outputEvents;
    std::vector<CP::TND280Event*> baseEventIO::inputEvents;

    void EventIO_Finalization(void) {
        // Delete the events saved in the input and output event vectors.
        for (std::vector<CP::TND280Event*>::iterator e 
                 = baseEventIO::outputEvents.begin();
             e != baseEventIO::outputEvents.end();
             ++e) delete (*e);

        for (std::vector<CP::TND280Event*>::iterator e
                 = baseEventIO::inputEvents.begin();
             e != baseEventIO::inputEvents.end();
             ++e) delete (*e);

        if (!CP::CleanHandleRegistry()) {
            std::cout << "tutEventIO:: EVENT HANDLES NOT DELETED" << std::endl;
        }
    }

    // Declare the test group.
    typedef test_group<baseEventIO>::object testEventIO;
    test_group<baseEventIO> groupEventIO("EventIO", EventIO_Finalization);

    // Test that events were actual written, and then read back in.
    template<> template<>
    void testEventIO::test<1> () {
        ensure("Event IO events written", (0<outputEvents.size()));
        ensure_equals("Event IO events read", 
                      inputEvents.size(), outputEvents.size());
    }

    // Test that event number and run number match.
    template<> template<>
    void testEventIO::test<2> () {
        std::vector<CP::TND280Event*>::iterator out
            = baseEventIO::outputEvents.begin();
        std::vector<CP::TND280Event*>::iterator in
            = baseEventIO::inputEvents.begin();

        for (;out != baseEventIO::outputEvents.end();
             ++out, ++in) {
            ensure_equals("Input and output event IDs match",
                          (*in)->GetEventId(), (*out)->GetEventId());
            ensure_equals("Input and output run IDs match",
                          (*in)->GetRunId(), (*out)->GetRunId());
        }
    }

    // Test that the basic event structure was created.
    template<> template<>
    void testEventIO::test<3> () {
        std::vector<CP::TND280Event*>::iterator out
            = baseEventIO::outputEvents.begin();
        std::vector<CP::TND280Event*>::iterator in
            = baseEventIO::inputEvents.begin();

        for (;out != baseEventIO::outputEvents.end();
             ++out, ++in) {
            CP::THandle<CP::TDataVector> outHits=(*out)->Get<CP::TDataVector>("~/hits");
            CP::THandle<CP::TDataVector> inHits=(*in)->Get<CP::TDataVector>("~/hits");
            ensure("Hits vector exists in output event",outHits);
            ensure("Hits vector exists in input event",inHits);

            CP::THandle<CP::TDataVector> outFits=(*out)->Get<CP::TDataVector>("~/fits");
            CP::THandle<CP::TDataVector> inFits=(*in)->Get<CP::TDataVector>("~/fits");
            ensure("Fits vector exists in output event",outFits);
            ensure("Fits vector exists in input event",inFits);
        }
    }

    // Test the input TG4Hits match the output TG4Hits.
    template<> template<>
    void testEventIO::test<4> () {
        std::vector<CP::TND280Event*>::iterator out
            = baseEventIO::outputEvents.begin();
        std::vector<CP::TND280Event*>::iterator in
            = baseEventIO::inputEvents.begin();

        for (;out != baseEventIO::outputEvents.end();
             ++out, ++in) {
            CP::THandle<CP::TG4HitContainer> outP0D
                = (*out)->Get<CP::TG4HitContainer>("~/truth/g4Hits/p0d");
            CP::THandle<CP::TG4HitContainer> inP0D
                = (*in)->Get<CP::TG4HitContainer>("~/truth/g4Hits/p0d");
            ensure("P0D G4 hit container exists in output event",outP0D);
            ensure("P0D G4 hit container exists in input event",inP0D);

            ensure_equals("P0D G4 hit containers are same length",
                          outP0D->size() , inP0D->size());

            CP::TG4HitContainer::iterator out = outP0D->begin();
            CP::TG4HitContainer::iterator in = inP0D->begin();
            for (;out != outP0D->end(); ++out, ++in) {
                ensure("Output CP::TG4VHit pointer exists", (*out));
                ensure("Input CP::TG4VHit pointer exists", (*in));
                const CP::TG4HitSegment *inSeg
                    = dynamic_cast<const CP::TG4HitSegment*>(*in);
                const CP::TG4HitSegment *outSeg
                    = dynamic_cast<const CP::TG4HitSegment*>(*out);
                ensure("Output is a TG4HitSegment", outSeg);
                ensure("Input is a TG4HitSegment", inSeg);
                ensure_equals("P0D data fields match",
                              inSeg->GetPrimaryId(),
                              outSeg->GetPrimaryId());
            }
        }
    }

    // Check that the hits match in the output and input objects.
    template<> template<>
    void testEventIO::test<5> () {
        std::vector<CP::TND280Event*>::iterator out
            = baseEventIO::outputEvents.begin();
        std::vector<CP::TND280Event*>::iterator in
            = baseEventIO::inputEvents.begin();

        for (;out != baseEventIO::outputEvents.end();
             ++out, ++in) {
            CP::THandle<CP::THitSelection> outP0D
                = (*out)->Get<CP::THitSelection>("~/hits/p0d");
            CP::THandle<CP::THitSelection> inP0D
                = (*in)->Get<CP::THitSelection>("~/hits/p0d");
            ensure("P0D hit selection exists in output event",outP0D);
            ensure("P0D hit selection exists in input event",inP0D);
            
            CP::THitSelection::iterator out = outP0D->begin();
            CP::THitSelection::iterator in = inP0D->begin();
            for (;out != outP0D->end(); ++out, ++in) {
                CP::THandle<CP::THit> outHit = (*out);
                CP::THandle<CP::TMCHit> outMCHit = outHit;
                CP::THandle<CP::THit> inHit = (*in);
                CP::THandle<CP::TMCHit> inMCHit = inHit;

                ensure_greaterthan("P0D hit GeomId filled",
                                   inHit->GetGeomId().AsInt(), 0);

                ensure_equals("P0D hit GeomIds match",
                              inHit->GetGeomId(), outHit->GetGeomId());
                ensure_equals("P0D hit charges match",
                              inHit->GetCharge(), outHit->GetCharge());
                ensure_equals("P0D hit times match",
                              inHit->GetTime(), outHit->GetTime());

                ensure_equals("P0D MC and regular hit GeomIds match",
                              inHit->GetGeomId(), outMCHit->GetGeomId());
                ensure_equals("P0D MC and regular hit charges match",
                              inHit->GetCharge(), outMCHit->GetCharge());
                ensure_equals("P0D MC and regular hit times match",
                              inHit->GetTime(), outMCHit->GetTime());

                ensure_equals("P0D MC hit GeomIds match",
                              inMCHit->GetGeomId(), 
                              outMCHit->GetGeomId());
                ensure_equals("P0D MC hit charges match",
                              inMCHit->GetCharge(), outMCHit->GetCharge());
                ensure_equals("P0D MC hit times match",
                              inMCHit->GetTime(), outMCHit->GetTime());

                const CP::TMCHit::ContributorContainer& inContrib 
                    = inMCHit->GetContributors();
                const CP::TMCHit::ContributorContainer& outContrib 
                    = outMCHit->GetContributors();
                
                ensure_greaterthan("P0D contributors exist",
                                   inContrib.size(),
                                   (unsigned) 0);
                ensure_equals("P0D number of contributors match",
                              inContrib.size(), outContrib.size());
                
                CP::TG4VHit* inG4VHit = inContrib[0];
                CP::TG4VHit* outG4VHit = inContrib[0];

                ensure("P0D input CP::TG4VHit exists", inG4VHit);
                ensure("P0D output CP::TG4VHit exists", outG4VHit);
                
                CP::TG4HitSegment* inG4HitSegment 
                    = dynamic_cast<CP::TG4HitSegment*>(inG4VHit);
                
                CP::TG4HitSegment* outG4HitSegment 
                    = dynamic_cast<CP::TG4HitSegment*>(outG4VHit);
                
                ensure("P0D input contributor is a CP::TG4HitSegment",
                       inG4HitSegment);
                ensure("P0D output contributor is a CP::TG4HitSegment",
                       outG4HitSegment);
                
                ensure_equals("P0D primary particle identifiers match",
                              inG4HitSegment->GetPrimaryId(),
                              outG4HitSegment->GetPrimaryId());

                ensure_greaterthan("P0D primary particle identifiers not zero",
                                   inG4HitSegment->GetPrimaryId(),0);
                    
                ensure_distance("P0D energy deposits match",
                                inG4HitSegment->GetEnergyDeposit(),
                                outG4HitSegment->GetEnergyDeposit(), 0.001);

                double inPrim = 10.0 + 1.0*inG4HitSegment->GetPrimaryId();
                ensure_distance("P0D energy deposits match id",
                                inG4HitSegment->GetEnergyDeposit(),
                                inPrim, 0.0001);

                ensure_greaterthan("P0D energy deposit not zero",
                                   inG4HitSegment->GetEnergyDeposit(),5.0);

                inG4VHit = inContrib[1];
                outG4VHit = inContrib[1];

                ensure("P0D NULL CP::TG4VHits are saved",inG4VHit == NULL);
                ensure("P0D NULL CP::TG4VHits are saved",outG4VHit == NULL);
                       
            }
        }
    }

    // Check that the hit selections in different CP::TAlgorithmResult objects
    // share the same hits. 
    template<> template<>
    void testEventIO::test<8> () {
        std::vector<CP::TND280Event*>::iterator out
            = baseEventIO::outputEvents.begin();
        std::vector<CP::TND280Event*>::iterator in
            = baseEventIO::inputEvents.begin();

        for (;out != baseEventIO::outputEvents.end();
             ++out, ++in) {
            CP::THandle<CP::TAlgorithmResult> r1 = (*in)->GetFit("firstResult");
            CP::THandle<CP::TAlgorithmResult> r2 = (*in)->GetFit("secondResult");
            ensure("The first algorithm result exists in input",r1);
            ensure("The second algorithm result exists in input",r2);
            CP::THandle<CP::THitSelection> h1 = r1->GetHitSelection();
            CP::THandle<CP::THitSelection> h2 = r2->GetHitSelection();
            ensure("The first hit selection was  found",h1);
            ensure("There are hits in the first hit selection", h1->size()>0);
            ensure("The second hit selection was found",h2);
            ensure_equals("The selections have the same number of hits",
                          h2->size(), h1->size());
            CP::THitSelection::iterator hit1 = h1->begin();
            CP::THitSelection::iterator hit2 = h2->begin();
            for (; hit1 != h1->end(); ++hit1, ++hit2) {
                ensure("First hit handle is valid", *hit1);
                ensure("Second hit handle is valid", *hit2);
                ensure_equals("Hit pointers match", 
                              GetPointer(*hit1), GetPointer(*hit2));
            }
        }        
    }

    // Test that a simple CP::TAlgorithmResult object was correctly saved.
    template<> template<>
    void testEventIO::test<9> () {
        for (std::vector<CP::TND280Event*>::iterator in
                 = baseEventIO::inputEvents.begin();
             in != baseEventIO::inputEvents.end();
             ++in) {
            CP::THandle<CP::TAlgorithmResult> s = (*in)->GetFit("reconResult");
            ensure("The algorithm result exists in input",s);
            CP::THandle<CP::TReconObjectContainer> d = s->GetResultsContainer();
            ensure("The default recon objects were found",d);
            ensure_equals("Recon object container length",d->size(),
                          (unsigned) 3);

            // Check object 0
            CP::THandle<CP::TReconBase> elem = (*d)[0];
            ensure("State 0 is valid", elem);
            CP::THandle<CP::TReconCluster> cluster = elem;
            ensure("State 0 is a valid cluster", cluster);
            CP::THandle<CP::TReconTrack> track = elem;
            ensure("State 0 is not a valid track", !track);
            CP::THandle<CP::TClusterState> clusterState = cluster->GetState();
            ensure("State 0 cluster has a cluster state",clusterState);
            CP::THandle<CP::TTrackState> trackState = cluster->GetState();
            ensure("State 0 cluster does not have a track state",!trackState);
            ensure_distance("State 0 cluster state values are saved",
                            clusterState->GetEDeposit(),10.0,0.001);

            // Check object 1
            elem = (*d)[1];
            ensure("State 1 is valid", elem);
            CP::THandle<CP::TReconShower> shower = elem;
            ensure("State 1 is a valid shower", shower);
            track = elem;
            ensure("State 1 is not a valid track", !track);
            CP::THandle<CP::TShowerState> showerState = shower->GetState();
            ensure("State 1 shower has a shower state",showerState);
            trackState = shower->GetState();
            ensure("State 1 shower does not have a track state",!trackState);

            // Check object 2
            elem = (*d)[2];
            ensure("State 2 is valid", elem);
            track = elem;
            ensure("State 2 is a valid track", track);
            cluster = elem;
            ensure("State 2 is not a valid cluster", !cluster);
            trackState = track->GetState();
            ensure("State 2 track has a track state",trackState);
            clusterState = track->GetState();
            ensure("State 2 track does not have a cluster state",!clusterState);
        }
    }

    // Test that a temporary objects are not saved in output file.
    template<> template<>
    void testEventIO::test<10> () {
        for (std::vector<CP::TND280Event*>::iterator out
                 = baseEventIO::outputEvents.begin();
             out != baseEventIO::outputEvents.end();
             ++out) {
            CP::THandle<CP::TDatum> tmp 
                = (*out)->Get<CP::TDatum>("temporaryObject");
            ensure("Temporary object exists in output",tmp);
        }
        for (std::vector<CP::TND280Event*>::iterator in
                 = baseEventIO::inputEvents.begin();
             in != baseEventIO::inputEvents.end();
             ++in) {
            CP::THandle<CP::TDatum> tmp 
                = (*in)->Get<CP::TDatum>("temporaryObject");
            ensure("Temporary object does not exist in input",tmp);
        }
    }

    // Test that a simple CP::TAlgorithmResult object was correctly in the
    // output object.
    template<> template<>
    void testEventIO::test<11> () {
        for (std::vector<CP::TND280Event*>::iterator in
                 = baseEventIO::outputEvents.begin();
             in != baseEventIO::outputEvents.end();
             ++in) {
            CP::THandle<CP::TAlgorithmResult> s = (*in)->GetFit("reconResult");
            ensure("The algorithm result exists in input",s);
            CP::THandle<CP::TReconObjectContainer> d = s->GetResultsContainer();
            ensure("The default recon objects were found",d);
            ensure_equals("Recon object container length",d->size(),
                          (unsigned) 3);

            // Check object 0
            CP::THandle<CP::TReconBase> elem = (*d)[0];
            ensure("State 0 is valid", elem);
            CP::THandle<CP::TReconCluster> cluster = elem;
            ensure("State 0 is a valid cluster", cluster);
            CP::THandle<CP::TReconTrack> track = elem;
            ensure("State 0 is not a valid track", !track);
            CP::THandle<CP::TClusterState> clusterState = cluster->GetState();
            ensure("State 0 cluster has a cluster state",clusterState);
            CP::THandle<CP::TTrackState> trackState = cluster->GetState();
            ensure("State 0 cluster does not have a track state",!trackState);
            ensure_distance("State 0 cluster state values are saved",
                            clusterState->GetEDeposit(),10.0,0.001);

            // Check object 1
            elem = (*d)[1];
            ensure("State 1 is valid", elem);
            CP::THandle<CP::TReconShower> shower = elem;
            ensure("State 1 is a valid shower", shower);
            track = elem;
            ensure("State 1 is not a valid track", !track);
            CP::THandle<CP::TShowerState> showerState = shower->GetState();
            ensure("State 1 shower has a shower state",showerState);
            trackState = shower->GetState();
            ensure("State 1 shower does not have a track state",!trackState);

            // Check object 2
            elem = (*d)[2];
            ensure("State 2 is valid", elem);
            track = elem;
            ensure("State 2 is a valid track", track);
            cluster = elem;
            ensure("State 2 is not a valid cluster", !cluster);
            trackState = track->GetState();
            ensure("State 2 track has a track state",trackState);
            clusterState = track->GetState();
            ensure("State 2 track does not have a cluster state",!clusterState);
        }
    }


};
