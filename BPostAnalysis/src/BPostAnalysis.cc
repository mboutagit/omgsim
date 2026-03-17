#include "BPostAnalysis.hh"
#include "DAQManager.hh"
#include "OM.hh"
#include "BWaveCatcher.hh"


using namespace tools;
using namespace H5;

namespace lactel
{
    std::vector<TCanvas*> getNPEPlot(TTree* tree, BAnalysisManager::DetectedEvent* evt, 
                                        histsByName& hists, 
                                        float exposure_time, float t_coin){
        std::vector<TCanvas*> vCanvas;
        float xmin=0.;
        float xmax=200.;
        float unitsPerBin=1.;
        const int nbins= (int)((xmax - xmin)/unitsPerBin);

        float xmin_amp=0.;
        float xmax_amp=2.4;
        float unitsPerBin_amp=10./1e3;
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
        OM om2 = OM("OMS0304");

        BWaveCatcher* wc1 = new BWaveCatcher("wc1");
        wc1->plugOM(0, om1);
        wc1->plugOM(1, om2);


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

            amp_om1->Fill(abs(ymin_a0)/1e3, 1/exposure_time);
            amp_om2->Fill(abs(ymin_a1)/1e3, 1/exposure_time);

            int nhits_om1 = 0;
            int nhits_om2 = 0;

            for(auto& hit : evt->hits){
                    if(hit.om_id == 1)
                    nhits_om1++;

                    if(hit.om_id == 2)
                    nhits_om2++;
            }
            npe_om1->Fill(nhits_om1, 1/exposure_time);
            npe_om2->Fill(nhits_om2, 1/exposure_time);

            if(evt->isACoincidence(t_coin)){
                int nhits_om1_coin = 0;
                int nhits_om2_coin = 0;

                ampCoin_om1->Fill(abs(ymin_a0)/1e3, 1/exposure_time);
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
                    
                    ampCoin_om1_cut->Fill(abs(ymin_a0)/1e3, 1/exposure_time);
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

        ampCoin_om1->SetStats(0);
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

        ampCoin_om1_cut->SetStats(0);
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

        amp_om1->SetStats(0);
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

        amp_om1->SetStats(0);
        TLegend* leg_amp_wwCoin = new TLegend(.6, .8, .8, .9);
        leg_amp_wwCoin->AddEntry(amp_om1, "OM0 - Upper", "l");
        leg_amp_wwCoin->AddEntry(amp_om2, "OM1 - Lower", "l");
        leg_amp_wwCoin->AddEntry(ampCoin_om1, "OM0 - Upper - coincidence", "l");
        leg_amp_wwCoin->AddEntry(ampCoin_om2, "OM1 - Lower - coincidence", "l");
        leg_amp_wwCoin->Draw();

        // plot npe with and without coincidence
        TCanvas* canvas_npe_wwCoin = new TCanvas("canvas_npe_wwCoin", "NPE", 800, 600);
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

        BWaveCatcher* wc1 = new BWaveCatcher("wc1");
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


    std::vector<TCanvas*>  getRealDataPlots(histsByName& hists){
        float xmin=0.;
        float xmax=2.4;
        float unitsPerBin=10/1e3;
        const int nbins= (int)((xmax - xmin)/unitsPerBin);
        std::string prefix="data_";

        std::cout << "\r\033[45C" << " -- xmin: " << xmin 
                                << " -- xmax: " << xmax 
                                << " -- unitsPerBin: " << unitsPerBin 
                                << " -- nbins: " << nbins 
                                << std::endl;

        std::vector<TCanvas*> canvas;
        H5File file_run3("/home/bouta-mohammed/work/cppm/lactel/realData/data/run_003/data.h5", H5F_ACC_RDONLY);
        H5File file_run4("/home/bouta-mohammed/work/cppm/lactel/realData/data/run_004/data.h5", H5F_ACC_RDONLY);

        DataSet dataset_run3 = file_run3.openDataSet("channel_data/peak");
        DataSet dataset_run4 = file_run4.openDataSet("channel_data/peak");

        DataSpace dataspace_run3 = dataset_run3.getSpace();
        DataSpace dataspace_run4 = dataset_run4.getSpace();

        hsize_t dims_run3[2], dims_run4[2];
        dataspace_run3.getSimpleExtentDims(dims_run3, NULL);
        dataspace_run4.getSimpleExtentDims(dims_run4, NULL);

        // std::cout << "Dimensions: " << dims_run3[0] << " x " << dims_run3[1] << std::endl;

        std::vector<double> data_run3(dims_run3[0]*dims_run3[1]);
        std::vector<double> data_run4(dims_run4[0]*dims_run4[1]);
        dataset_run3.read(data_run3.data(), PredType::NATIVE_DOUBLE);
        dataset_run4.read(data_run4.data(), PredType::NATIVE_DOUBLE);

        TH1F* hist0_run3= new TH1F("hist0_run3", "hist0_run3 amp OM0 run003",
                                nbins, xmin, xmax);
        TH1F* hist1_run3= new TH1F("hist1_run3", "hist1_run3 amp OM1 run003",
                                nbins, xmin, xmax);

        TH1F* hist0_run4= new TH1F("hist0_run4", "hist0_run4 amp OM0 run003",
                                nbins, xmin, xmax);
        TH1F* hist1_run4= new TH1F("hist1_run4", "hist1_run4 amp OM1 run003",
                                nbins, xmin, xmax);
        
        for(long i = 0; i < dims_run3[0]; i++){
                // std::cout << "line: " << i << " -- col0: " << data[i*dims_run3[1]] << endl; // " - col1: " << data[i*dims_run3[1]+1] << std::endl;
                hist0_run3->Fill(abs(data_run3[i*dims_run3[1]]), 1./410);
                hist1_run3->Fill(abs(data_run3[i*dims_run3[1]+1]), 1./410);
        }

        for(long i = 0; i < dims_run4[0]; i++){
                // std::cout << "line: " << i << " -- col0: " << data[i*dims_run3[1]] << endl; // " - col1: " << data[i*dims_run3[1]+1] << std::endl;
                hist0_run4->Fill(abs(data_run4[i*dims_run4[1]]), 1./301); // one minute of data
                hist1_run4->Fill(abs(data_run4[i*dims_run4[1]+1]), 1./301); // one minute of data
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

        container.push_back(std::pair("amp_ch0_run3", hist0_run3));
        container.push_back(std::pair("amp_ch1_run3", hist1_run3));
        container.push_back(std::pair("amp_ch0_run4", hist0_run4));
        container.push_back(std::pair("amp_ch1_run4", hist1_run4));

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

} // namespace lactel
