#include "BPostAnalysis.hh"
#include "DAQManager.hh"
#include "OM.hh"
#include "BWaveCatcher.hh"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>


using namespace tools;
using namespace H5;

namespace lactel
{
    // std::vector<stack> container;

    // Parse ISO 8601 (UTC "Z")
    std::time_t parse_iso8601(const std::string& s) {
        std::tm tm = {};
        std::istringstream ss(s);

        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

        // Convert as UTC
        #ifdef _WIN32
            return _mkgmtime(&tm);
        #else
            return timegm(&tm);
        #endif
    }


    #include <filesystem>
    #include <stdexcept>

    namespace fs = std::filesystem;

    std::vector<std::string> find_h5_files(const std::string& path = ".") {
        std::vector<std::string> h5_files;

        try {
            // 🔹 Check if path exists
            if (!fs::exists(path)) {
                throw std::runtime_error("Path does not exist: " + path);
            }

            // 🔹 Check if it's a directory
            if (!fs::is_directory(path)) {
                throw std::runtime_error("Path is not a directory: " + path);
            }

            // 🔹 Recursive traversal
            for (const auto& entry : fs::recursive_directory_iterator(
                    path, fs::directory_options::skip_permission_denied)) {

                try {
                    if (entry.is_regular_file()) {
                        if (entry.path().extension() == ".h5") {
                            h5_files.push_back(entry.path().string());
                        }
                    }
                } catch (const fs::filesystem_error&) {
                    // Skip problematic files silently
                    continue;
                }
            }

            // 🔹 No files found
            if (h5_files.empty()) {
                throw std::runtime_error("No .h5 files found in: " + path);
            }

        } catch (const fs::filesystem_error& e) {
            throw std::runtime_error(std::string("Filesystem error: ") + e.what());
        }

        return h5_files;
    }

    std::vector<TCanvas*> getNPEPlot(TTree* tree, 
                                        histsByName& hists, 
                                        float exposure_time, float t_coin, float thr){

        auto* evt= static_cast<BAnalysisManager::DetectedEvent*>(nullptr);
        tree->SetBranchAddress("DetectedEvent", &evt);

        std::vector<TCanvas*> vCanvas;
        float xmin=0.;
        float xmax=200.;
        float unitsPerBin=1.;
        const int nbins= (int)((xmax - xmin)/unitsPerBin);

        float xmin_amp=0.;
        float xmax_amp=2.4;
        // float unitsPerBin_amp=20./1e3;
        float unitsPerBin_amp = BPostAnalysis::mVperBin/1e3;
        const int nbins_amp= (int)((xmax_amp - xmin_amp)/unitsPerBin_amp);

        std::cout << "\r\033[45C" << " -- xmin: " << xmin 
                                << " -- xmax: " << xmax 
                                << " -- unitsPerBin: " << unitsPerBin 
                                << " -- nbins: " << nbins 
                                << std::endl;
        std::cout << "\r\033[45C" << " -- xmin_amp: " << xmin_amp 
                                << " -- xmax_amp: " << xmax_amp 
                                << " -- unitsPerBin_amp: " << unitsPerBin_amp 
                                << " -- nbins_amp: " << nbins_amp 
                                << std::endl;

        OM om1 = OM("OMS0321");
        om1.setID(1);
        OM om2 = OM("OMS0304");
        om2.setID(2);


        BWaveCatcher* wc1 = new BWaveCatcher("wc1", 1);
        wc1->plugOM(0, om1);
        wc1->plugOM(1, om2);
        wc1->setTriggerThreshold(thr);
        wc1->getConfiguration().setInputDynamicRange(6);
        wc1->print();

        TH1F* npe_om1 = new TH1F("npe_om1", 
                                        "NPE; Number of PE; Rate for 1 PE per bin [Hz]",
                                        nbins, xmin, xmax
                                    );
        TH1F* npe_om2 = new TH1F("npe_om2", 
                                        "NPE; Number of PE; Rate for 1 PE per bin [Hz]",
                                        nbins, xmin, xmax
                                    );

        TH1F* npeCoin_om1 = new TH1F("npeCoin_om1", 
                                        ("Coincidence #Deltat=" + std::to_string((int)t_coin) + " ns; Number of PE; Rate for 1 PE per bin [Hz]").c_str(),
                                        nbins, xmin, xmax
                                    );
        TH1F* npeCoin_om2 = new TH1F("npeCoin_om2", 
                                        ("Coincidence #Deltat=" + std::to_string((int)t_coin) + " ns; Number of PE; Rate for 1 PE per bin [Hz]").c_str(),
                                        nbins, xmin, xmax
                                    );
                               
        TH1F* npe_om1_cut = new TH1F("npe_om1_cut", 
                                        ("Coincidence #Deltat=" + std::to_string((int)t_coin) + " ns; Number of PE; Rate for 1 PE per bin [Hz]").c_str(),
                                        nbins, xmin, xmax
                                    );
        TH1F* npe_om2_cut = new TH1F("npe_om2_cut", 
                                        ("Coincidence #Deltat=" + std::to_string((int)t_coin) + " ns; Number of PE; Rate for 1 PE per bin [Hz]").c_str(),
                                        nbins, xmin, xmax
                                    );
        TH1F* npeCoin_om1_cut = new TH1F("npeCoin_om1_cut", 
                                        ("Coincidence #Deltat=" + std::to_string((int)t_coin) + " ns; Number of PE; Rate for 1 PE per bin [Hz]").c_str(),
                                        nbins, xmin, xmax
                                    );
        TH1F* npeCoin_om2_cut = new TH1F("npeCoin_om2_cut", 
                                        ("Coincidence #Deltat=" + std::to_string((int)t_coin) + " ns; Number of PE; Rate for 1 PE per bin [Hz]").c_str(),
                                        nbins, xmin, xmax
                                    );


        TH1F* amp_om1 = new TH1F("amp_om1", 
                                        "Amplitude; Amplitude [V]; Rate for 10 mV per bin [Hz]",
                                        nbins_amp, xmin_amp, xmax_amp
                                    );
        TH1F* amp_om2 = new TH1F("amp_om2", 
                                        "Amplitude; Amplitude [V]; Rate for 10 mV per bin [Hz]",
                                        nbins_amp, xmin_amp, xmax_amp
                                    );

        TH1F* ampCoin_om1 = new TH1F("ampCoin_om1", 
                                        ("Amplitude Coincidence #Deltat"+ std::to_string((int)t_coin) +" ns; Amplitude [V]; Rate for 10 mV per bin [Hz]").c_str(),
                                        nbins_amp, xmin_amp, xmax_amp
                                    );
        TH1F* ampCoin_om2 = new TH1F("ampCoin_om2", 
                                        ("Amplitude Coincidence #Deltat"+ std::to_string((int)t_coin) +" ns; Amplitude [V]; Rate for 10 mV per bin [Hz]").c_str(),
                                        nbins_amp, xmin_amp, xmax_amp
                                    );
        
        TH1F* ampCoin_om1_cut = new TH1F("ampCoin_om1_cut", 
                                        ("Amplitude Coincidence #Deltat"+ std::to_string((int)t_coin) +" ns; Amplitude [V]; Rate for 10 mV per bin [Hz]").c_str(),
                                        nbins_amp, xmin_amp, xmax_amp
                                    );
        TH1F* ampCoin_om2_cut = new TH1F("ampCoin_om2_cut", 
                                        ("Amplitude Coincidence #Deltat"+ std::to_string((int)t_coin) +" ns; Amplitude [V]; Rate for 10 mV per bin [Hz]").c_str(),
                                        nbins_amp, xmin_amp, xmax_amp
                                    );

        TH2F* npeCoin_oms_cut = new TH2F("npeCoin_oms_cut", 
                                        ("NPE - Coincidence #Deltat"+ std::to_string((int)t_coin) +" ns; nPE OM0; nPE OM1").c_str(),
                                        nbins, xmin, xmax,
                                        nbins, xmin, xmax
                                    );


        for(long long i = 0; i < tree->GetEntries(); i++){
            tree->GetEntry(i);

            // apply DAQ
            std::vector<ADC_record> adcs_om1 = wc1->digitizeEvent(0, *evt);
            std::vector<ADC_record> adcs_om2 = wc1->digitizeEvent(1, *evt);

            // std::cout << "\r\033[10C" << "Processing event: " << evt->event_id << " Total Events: " << tree->GetEntries() << std::endl;

            std::vector<Waveform> wfs_om1 = wc1->waveformFromADC(adcs_om1);
            std::vector<Waveform> wfs_om2 = wc1->waveformFromADC(adcs_om2);

            Waveform wf_om1 = wc1->joinWavefroms(0, wfs_om1);
            Waveform wf_om2 = wc1->joinWavefroms(1, wfs_om2);

            // std::cout << "\r\033[10C" << "wf1 min " << wf_om1.getMinAmplitude() << "  ---  " 
            //                             << "wf2 min " << wf_om2.getMinAmplitude() << "  ---  " 
            //                             << std::endl;

            TGraph totA0 = wc1->drawWaveform(wf_om1, wc1->getConfiguration().eventBufferDepth);
            TGraph totA1 = wc1->drawWaveform(wf_om2, wc1->getConfiguration().eventBufferDepth);
        

            double xmin_a0, xmax_a0, ymin_a0, ymax_a0;
            totA0.ComputeRange(xmin_a0, ymin_a0, xmax_a0, ymax_a0);

            double xmin_a1, xmax_a1, ymin_a1, ymax_a1;
            totA1.ComputeRange(xmin_a1, ymin_a1, xmax_a1, ymax_a1);
            

            // std::cout << "\r\033[10C" << "ymina0: " << ymin_a0 << "  -- ymina1: " << ymin_a1 << std::endl;
            if(abs(ymin_a0/1e3) >= thr){
                amp_om1->Fill(abs(ymin_a0)/1e3, 1/exposure_time);
                // std::cout << "\r\033[10C" << "amp ch0: " << abs(ymin_a0)/1e3 << " - nEntries: " << totA0.GetN() << std::endl;
            }
                
            if(abs(ymin_a1/1e3) >= thr){
                amp_om2->Fill(abs(ymin_a1)/1e3, 1/exposure_time);
                // std::cout << "\r\033[10C" << "amp ch1: " << abs(ymin_a1)/1e3 <<" - nEntries: " << totA0.GetN() << std::endl;
            }

            int nhits_om1 = 0;
            int nhits_om2 = 0;

            for(auto& hit : evt->hits){
                    if(hit.om_id == 1)
                        nhits_om1++;

                    if(hit.om_id == 2)
                        nhits_om2++;
            }
            // std::cout << string(20, '-') << " DebugTest " << string(20, '-') << " N° 1.5" << std::endl;

            npe_om1->Fill(nhits_om1, 1/exposure_time);
            npe_om2->Fill(nhits_om2, 1/exposure_time);

            if(evt->isACoincidence(t_coin)){
                int nhits_om1_coin = 0;
                int nhits_om2_coin = 0;

                if(abs(ymin_a0/1e3) >= thr)
                    ampCoin_om1->Fill(abs(ymin_a0)/1e3, 1/exposure_time);
                if(abs(ymin_a1/1e3) >= thr)
                    ampCoin_om2->Fill(abs(ymin_a1)/1e3, 1/exposure_time);

                for(auto& hit : evt->hits){
                    if(hit.om_id == 1)
                        nhits_om1_coin++;

                    if(hit.om_id == 2)
                        nhits_om2_coin++;
                }

                npeCoin_oms_cut->Fill(nhits_om1_coin, nhits_om2_coin, 1/exposure_time);

                npeCoin_om1->Fill(nhits_om1_coin, 1/exposure_time);
                npeCoin_om2->Fill(nhits_om2_coin, 1/exposure_time); 
                if(nhits_om1_coin >= 25 && nhits_om2_coin >= 25){ // coicidence and npe cut
                    npeCoin_om1_cut->Fill(nhits_om1_coin, 1/exposure_time);
                    npeCoin_om2_cut->Fill(nhits_om2_coin, 1/exposure_time); 
                    
                    if(abs(ymin_a0/1e3) >= thr)
                        ampCoin_om1_cut->Fill(abs(ymin_a0)/1e3, 1/exposure_time);
                    if(abs(ymin_a1/1e3) >= thr)
                        ampCoin_om2_cut->Fill(abs(ymin_a1)/1e3, 1/exposure_time);
                }         
            }
        }


        TCanvas* canvas = new TCanvas("canvas", "Coincidence", 800, 600);
        canvas->Divide(2);
        canvas->cd(1);
        gPad->SetLogy();
        npeCoin_om1->SetLineColor(1);
        npeCoin_om2->SetLineColor(2);
        npeCoin_om1->Draw("hist");
        npeCoin_om2->Draw("hist same");

        npeCoin_om1->SetStats(0);
        TLegend* leg = new TLegend(.6, .8, .8, .9);
        leg->AddEntry(npeCoin_om1, "OM0 - Upper", "l");
        leg->AddEntry(npeCoin_om2, "OM1 - Lower", "l");
        leg->Draw();

        canvas->cd(2);
        gPad->SetLogy();
        npeCoin_om1_cut->SetLineColor(1);
        npeCoin_om2_cut->SetLineColor(2);
        npeCoin_om1_cut->Draw("hist");
        npeCoin_om2_cut->Draw("hist same");

        npeCoin_om1_cut->SetStats(0);
        TLegend* leg_cut = new TLegend(.6, .8, .8, .9);
        leg_cut->AddEntry(npeCoin_om1_cut, "OM0 - Upper", "l");
        leg_cut->AddEntry(npeCoin_om2_cut, "OM1 - Lower", "l");
        leg_cut->Draw();


        TCanvas* canvas1 = new TCanvas("canvas1", "Amplitude", 800, 600);
        canvas1->Divide(2);
        canvas1->cd(1);

        gPad->SetLogy();
        ampCoin_om1->SetLineColor(1);
        ampCoin_om2->SetLineColor(2);
        ampCoin_om1->Draw("hist");
        ampCoin_om2->Draw("hist same");

        TLegend* leg1 = new TLegend(.6, .8, .8, .9);
        leg1->AddEntry(ampCoin_om1, "OM0 - Upper", "l");
        leg1->AddEntry(ampCoin_om2, "OM1 - Lower", "l");
        leg1->Draw();

        canvas1->cd(2);
        gPad->SetLogy();
        ampCoin_om1_cut->SetLineColor(1);
        ampCoin_om2_cut->SetLineColor(2);
        ampCoin_om1_cut->Draw("hist");
        ampCoin_om2_cut->Draw("hist same");

        TLegend* leg1_cut = new TLegend(.6, .8, .8, .9);
        leg1_cut->AddEntry(ampCoin_om1_cut, "OM0 - Upper", "l");
        leg1_cut->AddEntry(ampCoin_om2_cut, "OM1 - Lower", "l");
        leg1_cut->Draw();




        // plot number of p.e.
        TCanvas* canvas_npe = new TCanvas("canvas_npe", "NPE", 800, 600);
        canvas_npe->SetLogy();
        canvas_npe->SetGrid();
        npe_om1->SetLineColor(1);
        npe_om2->SetLineColor(2);
        npe_om1->Draw("hist");
        npe_om2->Draw("hist same");

        npe_om1->SetStats(0);
        TLegend* leg_npe = new TLegend(.6, .8, .8, .9);
        leg_npe->AddEntry(npeCoin_om1, "OM0 - Upper", "l");
        leg_npe->AddEntry(npe_om2, "OM1 - Lower", "l");
        leg_npe->Draw();

        // plot amplitude without coincidence
        TCanvas* canvas_amp = new TCanvas("canvas_amp", "NPE", 800, 600);
        canvas_amp->SetLogy();
        canvas_amp->SetGrid();
        amp_om1->SetLineColor(1);
        amp_om2->SetLineColor(2);
        amp_om1->Draw("hist");
        amp_om2->Draw("hist same");

        TLegend* leg_amp = new TLegend(.6, .8, .8, .9);
        leg_amp->AddEntry(npeCoin_om1, "OM0 - Upper", "l");
        leg_amp->AddEntry(amp_om1, "OM1 - Upper", "l");
        leg_amp->Draw();

        // plot amplitude with and without coincidence
        TCanvas* canvas_amp_wwCoin = new TCanvas("canvas_amp_wwCoin", "NPE", 800, 600);
        canvas_amp_wwCoin->SetLogy();
        amp_om1->SetLineColor(1);
        amp_om2->SetLineColor(2);
        ampCoin_om1->SetLineColor(3);
        ampCoin_om2->SetLineColor(4);
        amp_om1->Draw("hist");
        amp_om2->Draw("hist same");
        ampCoin_om1->Draw("hist same");
        ampCoin_om2->Draw("hist same");

        TLegend* leg_amp_wwCoin = new TLegend(.6, .8, .8, .9);
        leg_amp_wwCoin->AddEntry(amp_om1, "OM0 - Upper", "l");
        leg_amp_wwCoin->AddEntry(amp_om2, "OM1 - Lower", "l");
        leg_amp_wwCoin->AddEntry(ampCoin_om1, "OM0 - Upper - coincidence", "l");
        leg_amp_wwCoin->AddEntry(ampCoin_om2, "OM1 - Lower - coincidence", "l");
        leg_amp_wwCoin->Draw();

        // plot npe with and without coincidence
        TCanvas* canvas_npe_wwCoin = new TCanvas("canvas_npe_wwCoin", "NPE", 800, 600);
        canvas_npe_wwCoin->SetLogy();
        npe_om1->SetLineColor(1);
        npe_om2->SetLineColor(2);
        npeCoin_om1->SetLineColor(3);
        npeCoin_om2->SetLineColor(4);
        npe_om1->Draw("hist");
        npe_om2->Draw("hist same");
        npeCoin_om1->Draw("hist same");
        npeCoin_om2->Draw("hist same");

        npe_om1->SetStats(0);
        TLegend* leg_npe_wwCoin = new TLegend(.6, .8, .8, .9);
        leg_npe_wwCoin->AddEntry(npe_om1, "OM0 - Upper", "l");
        leg_npe_wwCoin->AddEntry(npe_om2, "OM1 - Lower", "l");
        leg_npe_wwCoin->AddEntry(npeCoin_om1, "OM0 - Upper - coincidence", "l");
        leg_npe_wwCoin->AddEntry(npeCoin_om2, "OM1 - Lower - coincidence", "l");
        leg_npe_wwCoin->Draw();


        // plot npe OM1 vs OM2 without coincidence
        TCanvas* canvas_npeOms = new TCanvas("canvas_npeOms", "NPE", 800, 600);
        npeCoin_oms_cut->SetLineColor(2);
        npeCoin_oms_cut->Draw("colz");
        npeCoin_oms_cut->SetStats(0);
        // TLegend* leg_amp = new TLegend(.6, .8, .8, .9);
        // leg_amp->AddEntry(npeCoin_om1, "OM", "l");
        // leg_amp->Draw();

        TGraph* ampRatio_om1 = ratio(amp_om1, ampCoin_om1); //new TGraph();
        TGraph* ampRatio_om2 = ratio(amp_om2, ampCoin_om2); //new TGraph();
        // for(int i = 0; i < amp_om1->GetNbinsX(); i++){
        //     if(amp_om1->GetBinContent(i) != 0)
        //         ampRatio_om1->SetPoint(i, amp_om1->GetBinCenter(i), ampCoin_om1->GetBinContent(i)/amp_om1->GetBinContent(i));  
                
        //     if(amp_om2->GetBinContent(i))
        //         ampRatio_om2->SetPoint(i, amp_om2->GetBinCenter(i), ampCoin_om2->GetBinContent(i)/amp_om2->GetBinContent(i));
        // }

        TCanvas* canvas_ampRatio = new TCanvas("Ratio", "Amp", 800, 600);
        ampRatio_om1->SetMarkerColor(1);
        ampRatio_om2->SetMarkerColor(2);
        ampRatio_om1->SetMarkerStyle(28);
        ampRatio_om2->SetMarkerStyle(30);
        ampRatio_om1->Draw("AP");
        ampRatio_om2->Draw("P same");
        ampRatio_om1->SetStats(0);

        TLegend* leg_ampRatio = new TLegend(.6, .8, .8, .9);
        leg_ampRatio->AddEntry(ampRatio_om1, "OM0 - Upper", "p");
        leg_ampRatio->AddEntry(ampRatio_om2, "OM1 - Lower", "p");
        leg_ampRatio->Draw();
        vCanvas.push_back(canvas_ampRatio);

        vCanvas.push_back(canvas_npeOms);
        vCanvas.push_back(canvas);
        vCanvas.push_back(canvas1);
        vCanvas.push_back(canvas_amp);
        vCanvas.push_back(canvas_npe);
        vCanvas.push_back(canvas_amp_wwCoin);
        vCanvas.push_back(canvas_npe_wwCoin);


        hists.push_back(std::pair("mc_npe_ch0_run3", npe_om1));
        hists.push_back(std::pair("mc_npe_ch1_run3", npe_om2));
        hists.push_back(std::pair("mc_npe_ch0_run4", npeCoin_om1));
        hists.push_back(std::pair("mc_npe_ch1_run4", npeCoin_om2));
        hists.push_back(std::pair("mc_npeCut_ch0_run3", npe_om1_cut));
        hists.push_back(std::pair("mc_npeCut_ch0_run3", npe_om2_cut));
        hists.push_back(std::pair("mc_npeCut_ch0_run4", npeCoin_om1_cut));
        hists.push_back(std::pair("mc_npeCut_ch0_run4", npeCoin_om2_cut));
        hists.push_back(std::pair("mc_amp_ch0_run3", amp_om1));
        hists.push_back(std::pair("mc_amp_ch1_run3", amp_om2));
        hists.push_back(std::pair("mc_amp_ch0_run4", ampCoin_om1));
        hists.push_back(std::pair("mc_amp_ch1_run4", ampCoin_om2));
        hists.push_back(std::pair("mc_ampCut_ch0_run4", ampCoin_om1_cut)); 
        hists.push_back(std::pair("mc_ampCut_ch1_run4", ampCoin_om2_cut)); 
        
        return vCanvas;
    }

    TCanvas* getDAQParameter(TTree* tree, BAnalysisManager::DetectedEvent* evt, std::string par){
        
        if(par != "Amplitude")
            return new TCanvas;

        OM om1 = OM("OMS0321");
        OM om2 = OM("OMS0304");

        BWaveCatcher* wc1 = new BWaveCatcher("wc1", 1);
        wc1->plugOM(0, om1);
        wc1->plugOM(1, om2);

        for(long long i = 0; i < tree->GetEntries(); i++){
            tree->GetEntry(i);

            std::vector<ADC_record> adcs_om1 = wc1->digitizeEvent(0, *evt);
            std::vector<ADC_record> adcs_om2 = wc1->digitizeEvent(1, *evt);

            std::vector<Waveform> wfs_om1 = wc1->waveformFromADC(adcs_om1);
            std::vector<Waveform> wfs_om2 = wc1->waveformFromADC(adcs_om2);

            Waveform wf_om1 = wc1->joinWavefroms(0, wfs_om1);
            Waveform wf_om2 = wc1->joinWavefroms(1, wfs_om2);

            TGraph totA0 = wc1->drawWaveform(wf_om1, wc1->getConfiguration().eventBufferDepth);
            TGraph totA1 = wc1->drawWaveform(wf_om2, wc1->getConfiguration().eventBufferDepth);
        

            double xmin_a0, xmax_a0, ymin_a0, ymax_a0;
            totA0.ComputeRange(xmin_a0, ymin_a0, xmax_a0, ymax_a0);

            double xmin_a1, xmax_a1, ymin_a1, ymax_a1;
            totA1.ComputeRange(xmin_a1, ymin_a1, xmax_a1, ymax_a1);

            if(ymin_a0==0 && ymin_a1)
                std::cout << "\r\033[44C" << "amps: (" << ymin_a0 << "," << ymin_a1 << ")" << std::endl;


        }
        return new TCanvas;
    }


    std::vector<TCanvas*>  getRealDataPlots(histsByName& hists, std::string file1, std::string file2){
        float xmin=0.;
        float xmax=2.4;
        float unitsPerBin = BPostAnalysis::mVperBin/1e3;
        const int nbins= (int)((xmax - xmin)/unitsPerBin);

        float attenuator=0.5;
        std::string prefix="data_";

        std::vector<std::string> data ={"Amplitude", "NPE"};
        std::vector<int> runs = {3, 4};

        std::cout << "\r\033[45C" << " -- xmin: " << xmin 
                                << " -- xmax: " << xmax 
                                << " -- unitsPerBin: " << unitsPerBin 
                                << " -- nbins: " << nbins 
                                << std::endl;

        std::vector<TCanvas*> canvas;
        H5File file_run3(file1, H5F_ACC_RDONLY);
        H5File file_run4(file2, H5F_ACC_RDONLY);

            // 🔹 Read start_time
            H5::Attribute startAttr_run3 = file_run3.openAttribute("start_time");
            H5::Attribute stopAttr_run3 = file_run3.openAttribute("stop_time");

            H5::Attribute startAttr_run4 = file_run4.openAttribute("start_time");
            H5::Attribute stopAttr_run4 = file_run4.openAttribute("stop_time");

            std::string start_str_run3, stop_str_run3, start_str_run4, stop_str_run4;
            startAttr_run3.read(startAttr_run3.getStrType(), start_str_run3);
            stopAttr_run3.read(stopAttr_run3.getStrType(), stop_str_run3);

            startAttr_run4.read(startAttr_run4.getStrType(), start_str_run4);
            stopAttr_run4.read(stopAttr_run4.getStrType(), stop_str_run4);

            // std::cout << "Start: " << start_str_run3 << "\n";
            // std::cout << "End:   " << end_str_run3 << "\n";

            // 🔹 Convert to time_t
            std::time_t t1_run3 = parse_iso8601(start_str_run3);
            std::time_t t2_run3 = parse_iso8601(stop_str_run3);

            std::time_t t1_run4 = parse_iso8601(start_str_run4);
            std::time_t t2_run4 = parse_iso8601(stop_str_run4);

            // 🔹 Compute difference in seconds
            double diff_run3 = std::difftime(t2_run3, t1_run3);
            double diff_run4 = std::difftime(t2_run4, t1_run4);

            std::cout << "Difference run3: " << diff_run3 << " seconds = " << diff_run3/60. << " min\n";
            std::cout << "Difference run4: " << diff_run4 << " seconds = " << diff_run4/60. << " min\n";

        DataSet dataset_run3 = file_run3.openDataSet("channel_data/peak");
        DataSet dataset_run4 = file_run4.openDataSet("channel_data/peak");

        DataSet charge_dataset_run3 = file_run3.openDataSet("channel_data/peak");
        DataSet charge_dataset_run4 = file_run4.openDataSet("channel_data/peak");

        DataSpace dataspace_run3 = dataset_run3.getSpace();
        DataSpace dataspace_run4 = dataset_run4.getSpace();
        
        DataSpace charge_dataspace_run3 = charge_dataset_run3.getSpace();
        DataSpace charge_dataspace_run4 = charge_dataset_run4.getSpace();



        hsize_t dims_run3[2], dims_run4[2];
        dataspace_run3.getSimpleExtentDims(dims_run3, NULL); // same dim
        dataspace_run4.getSimpleExtentDims(dims_run4, NULL);

        // std::cout << "Dimensions: " << dims_run3[0] << " x " << dims_run3[1] << std::endl;

        std::vector<double> data_run3(dims_run3[0]*dims_run3[1]);
        std::vector<double> data_run4(dims_run4[0]*dims_run4[1]);
        std::vector<double> charge_data_run3(dims_run3[0]*dims_run3[1]); // same dim
        std::vector<double> charge_data_run4(dims_run4[0]*dims_run4[1]);


        dataset_run3.read(data_run3.data(), PredType::NATIVE_DOUBLE);
        dataset_run4.read(data_run4.data(), PredType::NATIVE_DOUBLE);

        charge_dataset_run3.read(charge_data_run3.data(), PredType::NATIVE_DOUBLE);
        charge_dataset_run4.read(charge_data_run4.data(), PredType::NATIVE_DOUBLE);

        TH1F* hist0_run3= new TH1F("hist0_run3", "hist0_run3 amp OM0 run003",
                                nbins, xmin, xmax);
        TH1F* hist1_run3= new TH1F("hist1_run3", "hist1_run3 amp OM1 run003",
                                nbins, xmin, xmax);

        TH1F* hist0_run4= new TH1F("hist0_run4", "hist0_run4 amp OM0 run004",
                                nbins, xmin, xmax);
        TH1F* hist1_run4= new TH1F("hist1_run4", "hist1_run4 amp OM1 run004",
                                nbins, xmin, xmax);

        const int nhists = runs.size() * data.size();
        TH1* histograms[nhists];
        for(int i = 0; i < runs.size(); i++){
            for(int j = 0; j < data.size(); j++){
                histograms[i+j] = new TH1F(("hist" + std::to_string(i) + "_" + std::to_string(j)).c_str(), 
                                        "hist0_run3 amp OM0 run003",
                                        nbins, xmin, xmax);
            }
        }
        // TH1F* hist0_run3= new TH1F("hist0_run3", "hist0_run3 amp OM0 run003",
        //                         nbins, xmin, xmax);
        // TH1F* hist1_run3= new TH1F("hist1_run3", "hist1_run3 amp OM1 run003",
        //                         nbins, xmin, xmax);

        // TH1F* hist0_run4= new TH1F("hist0_run4", "hist0_run4 amp OM0 run003",
        //                         nbins, xmin, xmax);
        // TH1F* hist1_run4= new TH1F("hist1_run4", "hist1_run4 amp OM1 run003",
        //                         nbins, xmin, xmax);

        
        for(long i = 0; i < dims_run3[0]; i++){
                // std::cout << "line: " << i << " -- col0: " << data[i*dims_run3[1]] << endl; // " - col1: " << data[i*dims_run3[1]+1] << std::endl;
                hist0_run3->Fill(abs(data_run3[i*dims_run3[1]])/ attenuator, 1./diff_run3); // weating 
                hist1_run3->Fill(abs(data_run3[i*dims_run3[1]+1])/ attenuator, 1./diff_run3);
        }

        for(long i = 0; i < dims_run4[0]; i++){
                // std::cout << "line: " << i << " -- col0: " << data[i*dims_run3[1]] << endl; // " - col1: " << data[i*dims_run3[1]+1] << std::endl;
                hist0_run4->Fill(abs(data_run4[i*dims_run4[1]])/ attenuator, 1./diff_run4); // one minute of data
                hist1_run4->Fill(abs(data_run4[i*dims_run4[1]+1])/ attenuator, 1./diff_run4); // one minute of data
        }



        TCanvas* can = new TCanvas("can", "can ", 800, 600);
        can->SetLogy();
        hist0_run3->SetLineColor(1);
        hist1_run3->SetLineColor(2);
        hist0_run4->SetLineColor(3);
        hist1_run4->SetLineColor(4);
        hist0_run3->Draw("hist");
        hist1_run3->Draw("hist same");
        hist0_run4->Draw("hist same");
        hist1_run4->Draw("hist same");

        hist0_run3->SetStats(0);

        TLegend* leg = new TLegend(.6, .8, .8, .9);
        leg->AddEntry(hist0_run3, "OM0 - Upper", "l");
        leg->AddEntry(hist1_run3, "OM1 - Lower", "l");
        leg->AddEntry(hist0_run4, "C - OM0 - Upper", "l");
        leg->AddEntry(hist1_run4, "C - OM1 - Lower", "l");
        leg->Draw();

        canvas.push_back(can);

        hists.push_back(std::pair("amp_ch0_run3", hist0_run3));
        hists.push_back(std::pair("amp_ch1_run3", hist1_run3));
        hists.push_back(std::pair("amp_ch0_run4", hist0_run4));
        hists.push_back(std::pair("amp_ch1_run4", hist1_run4));

        // container.h1["amp_ch0_run3"] = hist0_run3;
        // container.h1["amp_ch0_run3"] = hist0_run3;
        // container.h1["amp_ch1_run3"] = hist1_run3;
        // container.h1["amp_ch0_run4"] = hist0_run4;
        // container.h1["amp_ch1_run4"] = hist1_run4;

        return canvas;
    }

    TGraph* ratio(TH1* h1, TH1* h2){
        int bins_h1 = h1->GetNbinsX();
        int bins_h2 = h2->GetNbinsX();
        if(bins_h1 != bins_h2 ){
            std::cout << "\r\033[20C" << "Error: bins mismatch." << std::endl;
            return nullptr;
        }

        TGraph* graph = new TGraph();
            
        for(int i = 0; i < bins_h1; i++){
            if(h1->GetBinContent(i) != 0)
                graph->SetPoint(i, h1->GetBinCenter(i), h2->GetBinContent(i)/h1->GetBinContent(i));  
        }
        return graph;
    }

    TH1* getHistByName(const histsByName& vec, std::string key) {
        return std::find_if(vec.begin(), vec.end(),
                        [key](const auto& p) { return p.first == key; })->second;
    }

    TCanvas* getVertexPositions(TTree* tree){

        auto* evt= static_cast<BAnalysisManager::DetectedEvent*>(nullptr);
        tree->SetBranchAddress("DetectedEvent", &evt);
        
        float xoWT1 = 0, // origin on axis 
                yoWT1 = 0,
                zoWT1 = -1500;
        float xoWT2 = 0,
                yoWT2 = 0,
                zoWT2 = -4000;
        float xw = 1000, // width on axis;
                yw = 1000,
                zw = 1000;
        
        const int nWT = 2;

        // std::vector<WaterTank> WTs= {WaterTank(0, 0, 0, -1500, 1000, 1000, 1000),
        //                             WaterTank(0, 0, 0, -3500, 1000, 1000, 1000)
        //                         };

        // const std::vector<string> plan = {"xy", "xz", "yz"};
        // TH2* Hist[WTs.size()][plan.size()];
        
        BSquare* xySquareWT1 = new BSquare(xoWT1 - xw, xoWT1 + xw, yoWT1 - yw, yoWT1 + yw);
        BSquare* xzSquareWT1 = new BSquare(xoWT1 - xw, xoWT1 + xw, zoWT1 - zw, zoWT1 + zw);
        BSquare* yzSquareWT1 = new BSquare(yoWT1 - yw, yoWT1 + yw, zoWT1 - zw, zoWT1 + zw);

        BSquare* xySquareWT2 = new BSquare(xoWT2 - xw, xoWT2 + xw, yoWT2 - yw, yoWT2 + yw);
        BSquare* xzSquareWT2 = new BSquare(xoWT2 - xw, xoWT2 + xw, zoWT2 - zw, zoWT2 + zw);
        BSquare* yzSquareWT2 = new BSquare(yoWT2 - yw, yoWT2 + yw, zoWT2 - zw, zoWT2 + zw);

        // for(int i = 0; i < WTs.size(); i++){
        //     for(int j=0; j < plan.size(); j++){
        //         Hist[i][j] = new TH2 (plan[j] + "_" + WTs[i]; "Hist projection on " + axis[j]; 100, WTs[i].xo - WTs[i].xo)
        //     }
        // }

        TH2F* xyHistWT1 = new TH2F("xyHistWT1", "Projection Vertex position XY plan - WT1; X (mm); Y(mm)", 300, -6000, 6000,
                                                                300, -6000, 6000
                                                            );
        TH2F* xzHistWT1 = new TH2F("xzHistWT1", "Projection Vertex position XZ plan - WT1; X (mm); Z(mm)", 300, -6000, 6000,
                                                                300, -6000, 0
                                                            );
        TH2F* yzHistWT1 = new TH2F("yzHistWT1", "Projection Vertex position YZ plan - WT1; Y (mm); Z(mm)", 300, -6000, 6000,
                                                                300, -6000, 0
                                                            );


        TH2F* xyHistWT2 = new TH2F("xyHistWT2", "Projection Vertex position XY plan - WT2; X (mm); Y (mm)", 300, -6000, 6000,
                                                                300, -6000, 6000
                                                            );
        TH2F* xzHistWT2 = new TH2F("xzHistWT2", "Projection Vertex position XZ plan - WT2; X (mm); Z (mm)", 300, -6000, 6000,
                                                                300, -6000, 0
                                                            );
        TH2F* yzHistWT2 = new TH2F("yzHistWT2", "Projection Vertex position YZ plan - WT2; Y (mm); Z (mm)", 300, -6000, 6000,
                                                                300, -6000, 0
                                                            );


        TGraph* xyGrWT1 = new TGraph();
        TGraph* xzGrWT1 = new TGraph();
        TGraph* yzGrWT1 = new TGraph();
        TGraph* xyGrWT2 = new TGraph();
        TGraph* xzGrWT2 = new TGraph();
        TGraph* yzGrWT2 = new TGraph();


        for(long long i = 0; i < tree->GetEntries(); i++){
            // std::cout << "\r\033[20C" << "Entry: " << i << std::endl;
            tree->GetEntry(i);
            for(auto& hit : evt->hits){

                int i_om1 = 0, i_om2 = 0;
                if(hit.om_id == 1){
                    xyHistWT1->Fill(hit.vrx, hit.vry);
                    xzHistWT1->Fill(hit.vrx, hit.vrz);
                    yzHistWT1->Fill(hit.vry, hit.vrz);
                    // xyGrWT1->SetPoint(i_om1, hit.vry, hit.vrx);
                    // xzGrWT1->SetPoint(i_om1, hit.vrz, hit.vrx);
                    // yzGrWT1->SetPoint(i_om1, hit.vrz, hit.vry);
                    i_om1++;
                }
                
                if(hit.om_id == 2){
                    xyHistWT2->Fill(hit.vrx, hit.vry);
                    xzHistWT2->Fill(hit.vrx, hit.vrz);
                    yzHistWT2->Fill(hit.vry, hit.vrz);
                    // xyGrWT2->SetPoint(i_om2, hit.vry, hit.vrx);
                    // xzGrWT2->SetPoint(i_om2, hit.vrz, hit.vrx);
                    // yzGrWT2->SetPoint(i_om2, hit.vrz, hit.vry);
                    i_om2++;
                }
            }
        }


        TCanvas* posCanvas = new TCanvas("PosCanvas", "Vertex position canvas");
        posCanvas->Divide(3, 2);

        posCanvas->cd(1);
        xyHistWT1->Draw();
        xySquareWT1->Draw();
        posCanvas->cd(2);
        xzHistWT1->Draw();
        xzSquareWT1->Draw();
        posCanvas->cd(3);
        yzHistWT1->Draw();
        yzSquareWT1->Draw();

        posCanvas->cd(4);
        xyHistWT2->Draw();
        xySquareWT2->Draw();
        posCanvas->cd(5);
        xzHistWT2->Draw();
        xzSquareWT2->Draw();
        posCanvas->cd(6);
        yzHistWT2->Draw();
        yzSquareWT2->Draw();


        return posCanvas;
    }

        void getRealDataByInFile(histsByName& hists, std::string inFile, std::string dataset, float xmin, float xmax, float unitsPerBin, float attenuator){
            const int nbins= (int)((xmax - xmin)/unitsPerBin);

            std::string prefix="data_";

            std::cout << "\r\033[45C"   << " -- xmin: " << xmin 
                                        << " -- xmax: " << xmax 
                                        << " -- unitsPerBin: " << unitsPerBin 
                                        << " -- nbins: " << nbins 
                                        << std::endl;

            std::vector<TCanvas*> canvas;
            H5File file(inFile, H5F_ACC_RDONLY);

            // 🔹 Read start_time
            H5::Attribute startTime = file.openAttribute("start_time");
            H5::Attribute endTime = file.openAttribute("stop_time");

            std::string start, end;
            startTime.read(startTime.getStrType(), start);
            endTime.read(endTime.getStrType(), end);

            // 🔹 Convert to time_t
            std::time_t t1_file = parse_iso8601(start);
            std::time_t t2_file = parse_iso8601(end);

            // 🔹 Compute difference in seconds
            double time = std::difftime(t2_file, t1_file);

            std::cout << "Difference file: " << time << " seconds = " << time/60. << " min\n";

            DataSet dataSet = file.openDataSet(dataset);
            DataSpace dataspace = dataSet.getSpace(); // get shape
        
            hsize_t dims[2]; // 2D data, Nlines == values, NColumns == Channels
            dataspace.getSimpleExtentDims(dims, NULL); // fill dimension vector

            std::cout << "Dimensions: nlines: " << dims[0] << "  - ncolumns: " << dims[1] << std::endl;

            std::vector<double> data(dims[0]*dims[1]); // data has a length of dim[0], => nLines
            // std::vector<pair<double, double>> data(dims_file[0]);

            dataSet.read(data.data(), PredType::NATIVE_DOUBLE);

            
            TH1F* hist_amp_ch0= new TH1F("hist_amp_ch0", "hist Amplitude amp channel 0",
                                        nbins, xmin, xmax);

            TH1F* hist_amp_ch1= new TH1F("hist_amp_ch1", "hist Amplitude amp channel 1",
                                        nbins, xmin, xmax);

            for(long i = 0; i < dims[0]; i++){
                hist_amp_ch0->Fill(abs(data[ i*dims[1] + 0]) / attenuator, 1/time); // abs to positive signal convert to Vols and divide by the attenuator and scale with the time of data taking
                hist_amp_ch1->Fill(abs(data[ i*dims[1] + 1]) / attenuator, 1/time);
            }

            hists.push_back(std::pair("amp_ch0", hist_amp_ch0));
            hists.push_back(std::pair("amp_ch1", hist_amp_ch1));
            

            TCanvas* ccc = new TCanvas("ccc", "Test Plot");
            ccc->SetLogy();
            hist_amp_ch0->Draw("hist");
            hist_amp_ch1->SetLineColor(2);
            hist_amp_ch1->Draw("hist same");

        }




    TCanvas* makeRatioPlot(TH1* h1, TH1* h2){
        TCanvas* canvas = new TCanvas("canvas", "Ratio Canvas", 900, 800);
        canvas->Divide(1,2);

        canvas->cd(1);
        gPad->SetPad(0, 0, 1, 0.3);
        canvas->cd(2);
        gPad->SetPad(0, 0.3, 1, 1);
        h1->Draw("hist");
        h2->Draw("hist same");
        gPad->SetFrameLineColor(0);
        gPad->SetMargin(0.1, 0.1, 0.1, 0.01);      

        canvas->cd(1);
        h1->Sumw2(1);
        h2->Sumw2(1);
        TH1* ratio = (TH1*)h1->Clone("ratio");
        ratio->Divide(h2);
        ratio->Draw("hist same");
        gPad->SetFrameLineColor(0);
        gPad->SetMargin(0.1, 0.1, 0.1, 0.01);


        return canvas;
    }


    TCanvas* DrawWithRatio(TH1* h1, TH1* h2, const char* canvasName){
        if (!h1 || !h2)
            return nullptr;

        // Canvas
        TCanvas* c = new TCanvas(canvasName, canvasName, 800, 800);

        //--------------------------------------------------
        // Pads
        //--------------------------------------------------
        TPad* padTop = new TPad("padTop", "", 0.0, 0.40, 1.0, 1.0);
        TPad* padBot = new TPad("padBot", "", 0.0, 0.00, 1.0, 0.40);

        padTop->SetBottomMargin(0.02);

        padBot->SetTopMargin(0.03);
        padBot->SetBottomMargin(0.30);

        padTop->Draw();
        padBot->Draw();

        //--------------------------------------------------
        // Top pad
        //--------------------------------------------------
        padTop->cd();

        TH1* h1Draw = (TH1*)h1->Clone("h1Draw");
        TH1* h2Draw = (TH1*)h2->Clone("h2Draw");

        h1Draw->SetDirectory(nullptr);
        h2Draw->SetDirectory(nullptr);

        h1Draw->SetLineColor(kBlack);
        h1Draw->SetLineWidth(2);

        h2Draw->SetLineColor(kRed + 1);
        h2Draw->SetLineWidth(2);

        double ymax = std::max(h1Draw->GetMaximum(), h2Draw->GetMaximum());
        h1Draw->SetMaximum(1.25 * ymax);

        h1Draw->Draw("HIST");
        h2Draw->Draw("HIST SAME");

        // Hide x-axis
        h1Draw->GetXaxis()->SetLabelSize(0.);
        h1Draw->GetXaxis()->SetTitleSize(0.);

        TLegend* leg = new TLegend(0.65, 0.75, 0.88, 0.88);
        leg->AddEntry(h1Draw, h1Draw->GetTitle(), "l");
        leg->AddEntry(h2Draw, h2Draw->GetTitle(), "l");
        leg->Draw();

        //--------------------------------------------------
        // Bottom pad
        //--------------------------------------------------
        padBot->cd();

        TH1* ratio = (TH1*)h1->Clone("ratio");
        ratio->SetDirectory(nullptr);
        ratio->Divide(h2);

        ratio->SetStats(false);
        ratio->SetLineColor(kBlack);
        ratio->SetMarkerStyle(20);
        ratio->SetMarkerSize(0.9);

        ratio->GetYaxis()->SetTitle("Ratio");
        ratio->GetYaxis()->CenterTitle();

        ratio->GetYaxis()->SetNdivisions(505);

        ratio->GetYaxis()->SetTitleSize(0.09);
        ratio->GetYaxis()->SetTitleOffset(0.55);
        ratio->GetYaxis()->SetLabelSize(0.08);

        ratio->GetXaxis()->SetTitleSize(0.10);
        ratio->GetXaxis()->SetLabelSize(0.09);
        ratio->GetXaxis()->SetTitleOffset(1.1);

        ratio->SetMinimum(0.5);
        ratio->SetMaximum(1.5);

        ratio->Draw("EP");

        TLine* line = new TLine(
            ratio->GetXaxis()->GetXmin(),
            1.0,
            ratio->GetXaxis()->GetXmax(),
            1.0);

        line->SetLineStyle(2);
        line->Draw();

        int nbins = ratio->GetNbinsX();
        TGraphAsymmErrors *ratioBand = new TGraphAsymmErrors(nbins);
        for (int i = 1; i <= nbins; ++i) {
            double c = ratio->GetBinContent(i);
            double e = ratio->GetBinError(i);

            ratioBand->SetPoint(i-1, ratio->GetBinCenter(i), 1.0);

            double rel = (c > 0.) ? e/c : 0.0;

            ratioBand->SetPointError(i-1,
                                    ratio->GetBinWidth(i)/2.,
                                    ratio->GetBinWidth(i)/2.,
                                    rel, rel);
        }

        ratioBand->Draw("E2 same");

        c->cd();
        c->Update();

        return c;
    }

    TCanvas* DrawWithRatio(std::vector<std::pair<TH1*, TH1*>> hists, const char* canvasName, bool errorBand){
        for (auto& hist : hists)
            if (!hist.first || !hist.second)
                return nullptr;

        // Canvas
        TCanvas* c = new TCanvas(canvasName, canvasName, 800, 800);

        //--------------------------------------------------
        // Pads
        //--------------------------------------------------
        TPad* padTop = new TPad("padTop", "", 0.0, 0.40, 1.0, 1.0);
        TPad* padBot = new TPad("padBot", "", 0.0, 0.00, 1.0, 0.40);

        padTop->SetBottomMargin(0.02);

        padBot->SetTopMargin(0.03);
        padBot->SetBottomMargin(0.30);

        padTop->Draw();
        padBot->Draw();

        //--------------------------------------------------
        // Top pad
        //--------------------------------------------------
        padTop->cd();
        padTop->SetLogy();

        int i=0;
        TH1* h1Draw[hists.size()];
        TH1* h2Draw[hists.size()];
        
        TLegend* leg = new TLegend(0.65, 0.75, 0.88, 0.88);
        for(auto& hist : hists){
            h1Draw[i] = (TH1*)hist.first->Clone(("h1Draw" + to_string(i)).c_str());
            h2Draw[i] = (TH1*)hist.second->Clone(("h2Draw" + to_string(i)).c_str());

            h1Draw[i]->SetDirectory(nullptr);
            h2Draw[i]->SetDirectory(nullptr);

            h1Draw[i]->SetLineColor(i+1);
            h1Draw[i]->SetLineWidth(2);
            h1Draw[i]->SetFillColorAlpha(i+1, 0.4);

            h2Draw[i]->SetLineColor(i+1);
            h2Draw[i]->SetMarkerColor(i+1);
            h2Draw[i]->SetLineWidth(2);
            h2Draw[i]->SetMarkerStyle(20);
            h2Draw[i]->SetMarkerSize(0.8);

            double ymax = std::max(h1Draw[i]->GetMaximum(), h2Draw[i]->GetMaximum());
            h1Draw[i]->SetMaximum(1.25 * ymax);

            if(i == 0)
                h1Draw[i]->Draw("HIST");
            else
                h1Draw[i]->Draw("HIST same");
            h2Draw[i]->Draw("P SAME E");

            // Hide x-axis
            h1Draw[i]->GetXaxis()->SetLabelSize(0.);
            h1Draw[i]->GetXaxis()->SetTitleSize(0.);

            leg->AddEntry(h1Draw[i], h1Draw[i]->GetTitle(), "l");
            leg->AddEntry(h2Draw[i], h2Draw[i]->GetTitle(), "P");
            i++;
        }
        leg->Draw();
        

        //--------------------------------------------------
        // Bottom pad
        //--------------------------------------------------
        padBot->cd();

        i=0;
        TH1* ratio[hists.size()];
        TGraphAsymmErrors *ratioBand[hists.size()];

        for(auto& hist : hists){
            ratio[i] = (TH1*)hist.first->Clone(("ratio" + to_string(i)).c_str());
            ratio[i]->SetDirectory(nullptr);
            ratio[i]->Divide(hist.second);

            ratio[i]->SetStats(false);
            ratio[i]->SetLineColor(i+1);
            ratio[i]->SetMarkerColor(i+1);
            ratio[i]->SetMarkerStyle(20);
            ratio[i]->SetMarkerSize(0.9);

            ratio[i]->GetYaxis()->SetTitle("Ratio");
            ratio[i]->GetYaxis()->CenterTitle();

            ratio[i]->GetYaxis()->SetNdivisions(505);

            ratio[i]->GetYaxis()->SetTitleSize(0.09);
            ratio[i]->GetYaxis()->SetTitleOffset(0.55);
            ratio[i]->GetYaxis()->SetLabelSize(0.08);

            ratio[i]->GetXaxis()->SetTitleSize(0.10);
            ratio[i]->GetXaxis()->SetLabelSize(0.09);
            ratio[i]->GetXaxis()->SetTitleOffset(1.1);

            ratio[i]->SetMinimum(0);
            ratio[i]->SetMaximum(2);

            ratio[i]->Draw("EP same");

            if(errorBand){
                int nbins = ratio[i]->GetNbinsX();
                ratioBand[i] = new TGraphAsymmErrors(nbins);
                for (int j = 1; j <= nbins; j++) {
                    double c = ratio[i]->GetBinContent(j);
                    double e = ratio[i]->GetBinError(j);

                    ratioBand[i]->SetPoint(j-1, ratio[i]->GetBinCenter(j), 1.0);

                    double rel = (c > 0.) ? e/c : 0.0;

                    ratioBand[i]->SetPointError(j-1,
                                            ratio[i]->GetBinWidth(j)/2.,
                                            ratio[i]->GetBinWidth(j)/2.,
                                            rel, rel);
                }

                ratioBand[i]->SetFillColorAlpha(i+1, 0.2);
                ratioBand[i]->Draw("E2 same");
            }
            

            i++;
        }
        
        // TLine* line = new TLine(
        //     ratio->GetXaxis()->GetXmin(),
        //     1.0,
        //     ratio->GetXaxis()->GetXmax(),
        //     1.0);

        // line->SetLineStyle(2);
        // line->Draw();


   

        c->cd();
        c->Update();

        return c;
    }



    // TH1* getHistByName(histsByName container, std::string hist_name){
    //     for(auto a : container){
    //         if(a.first == hist_name)
    //             return a.second;
    //     }
    //     return nullptr;
    // }




} // namespace lactel
