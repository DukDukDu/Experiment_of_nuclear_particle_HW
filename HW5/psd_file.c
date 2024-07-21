#include <iostream>
#include <fstream>
 
using namespace std;

void BaseCorrection(int gindex, TGraph *gb, TFile *f)
{
    //从文件指针*f中得到name为“g00020”的TGraph指针，Form()函数用法与c语言中的printf类似;
    TGraph *g=(TGraph*)f->Get(Form("g%05d",gindex));
    double *y=g->GetY();//将TGraph的y轴数值按顺序填入数组y;
    double yb[250];
    double base=0; 
    for(int i=0;i<40;i++) {
        base += y[i];
    }
    base /=40.;
    gb->Clear();
    gb->SetTitle(Form("gindex=%d",gindex));
    for(int i=0;i<250;i++) {
        yb[i]=y[i]-base;
        gb->SetPoint(i,i,yb[i]);
    } 
}
void TimeCorrection(TGraph *gb, TGraph *gc){

    gc->Clear();
    double *y = gb->GetY();
    int xmax = TMath::LocMax(250, y);
    int dx = 59 - xmax;

    for(int i = 0; i < 250; i++ ){
        double x, y;
        gb->GetPoint(i, x, y);
        if(x + dx < 0) {
            x = 250 - i;
            gc->SetPoint(x, x, y);
        }
        if(x + dx >= 250) {
            x = x + dx -(i+1);
            gc->SetPoint(x, x, y);
        }
        else gc->SetPoint(x+dx, x+dx, y);
    }
}

double Fit(TH1F *g, double height, double middle, double right, double left, int choose){
    TF1 *fit = new TF1("fit", "[0]*exp(-((x-[1])/[2])*((x-[1])/[2]))");
    fit->SetParameter(0, height);
    fit->SetParameter(1, middle);
    fit->SetParameter(2, 0.05);

    g->Fit("fit", " ", " ", right, left);

    double m = fit->GetParameter(1);
    double delta = fit->GetParameter(2);

    if (choose == 0) return m;
    else if (choose == 1) return delta;
    else {
        cout << "change your choose number to 0 or 1" << endl;
        return 0;
        }
}

void psd_file(){

    TFile *input = new TFile("liquidpsd.root", "READ");

    double tail, fast, total, psd;
    TFile *output = new TFile("output.root", "RECREATE");
    TTree *tree = new TTree("tree", "correction");

    tree->Branch("tail", &tail, "tail/D");
    tree->Branch("fast", &fast, "fast/D");
    tree->Branch("total", &total, "total/D");
    tree->Branch("psd", &psd, "psd/D");

    int t0=50;//fast，total门的左边界
    int t1=70;//fast的右边界
    int t2=250;//total的右边界
    int count1 = 0;
    int count2 = 0;
    double y_n[250] = {0}, y_g[250] = {0};

    auto *gra_neutron = new TGraph;
    auto *gra_gamma = new TGraph;
    auto *gra_minus = new TGraph;

    ofstream dataFile;
    dataFile.open("dataFile.txt");
  
    for(int j = 0; j < 10000; j++){

        tail = 0, fast = 0, total = 0, psd = 0;

        //TGraph *gra_tem = (TGraph*)input->Get(Form("g%0d", j-1));
        auto *gra_baseline_corre = new TGraph;
        auto *gra_time_corre = new TGraph;
        BaseCorrection(j, gra_baseline_corre, input);
        TimeCorrection(gra_baseline_corre, gra_time_corre);

        double *y = gra_time_corre->GetY();

        for(int i=t0;i<t1;i++) fast += y[i];
        for(int i=t0;i<t2;i++) total += y[i];

        tail = total - fast;
        psd = tail/total;

        tree->Fill();
        
        if (psd > 0.3 && total >20000) {

            count1 += 1;
            if (count1 > 1000) break;
            else {
                gra_time_corre->Scale(1/gra_time_corre->Integral());
                double *neu = gra_time_corre->GetY();
                for(int i_n = 0; i_n < 250; i_n++){
                    y_n[i_n] += neu[i_n];
                }
            }

        }

        for(int j_n = 0; j_n < 250; j_n++){
                    gra_neutron->SetPoint(j_n, j_n, y_n[j_n]/1000);
                }

        if (psd < 0.2 && total >20000){

            count2 +=1;
            if (count2 > 1000) break;
            else {
                gra_time_corre->Scale(1/gra_time_corre->Integral());
                double *gam = gra_time_corre->GetY();
                for(int i_g = 0; i_g < 250; i_g++){
                    y_g[i_g] += gam[i_g];
                }
            }
        }

        for(int j_g = 0; j_g < 250; j_g++){
                    gra_gamma->SetPoint(j_g, j_g, y_g[j_g]/1000);
                }

        dataFile << tail << " " << total << " " << fast << " " << psd << endl;
        
        //gra_baseline_corre->Clear();
        //gra_time_corre->Clear();
    }

    tree->Write();
    output->Close();
    dataFile.close();

    TFile *input1 = new TFile("output.root", "READ");
    TTree *new_tree = (TTree*)input1->Get("tree");
    TH1F *h_psd = new TH1F("h_psd", " ", 70, 0, 0.7);

    TCanvas *c = new TCanvas("c", " ", 800, 600);
    new_tree->Draw("tail:total>>h0(1000, 0, 140000, 1000, -10000, 140000)", "tail>-10000 ", "colz");
    c->Draw();
    
    TCanvas *c0 = new TCanvas("c0", " ", 800, 600);
    new_tree->Draw("psd:total>>h1(200, 0, 140000, 100, -0.1, 0.7)", "psd>0", "colz");
    c0->Draw();

    TCanvas *c1 = new TCanvas("c1", " ", 800, 600);
    new_tree->Draw("psd>>h_psd");
    
    double mid_gamma = Fit(h_psd, 500, 0.07, 0.05, 0.1, 0);
    double mid_neutron = Fit(h_psd, 200, 0.3, 0.25, 0.35, 0);
    double mid_diff = mid_neutron - mid_gamma;
    cout << mid_diff << endl;
    double delta_g = Fit(h_psd, 500, 0.07, 0.05, 0.1, 1);
    double delta_n = Fit(h_psd, 200, 0.3, 0.25, 0.35, 1);
    cout << delta_g << " " << delta_n << endl;
    cout << "FoM = " << mid_diff/(2*sqrt(2*log(2))*(delta_g + delta_n)) << endl;

    c1->Draw();
    
    auto *gra_gamma_corr = new TGraph;//由于未知的效应导致最大值产生偏差，因此进行修正
    auto *gra_neutron_corr = new TGraph;
    TimeCorrection(gra_gamma, gra_gamma_corr), TimeCorrection(gra_neutron, gra_neutron_corr);
    double *y_g_corr = gra_gamma_corr->GetY();
    double *y_n_corr = gra_neutron_corr->GetY();

    for (int i_m = 0; i_m < 250; i_m ++){

        gra_minus->SetPoint(i_m, i_m, -y_n_corr[i_m] + y_g_corr[i_m]);
    }

    TCanvas *c2 = new TCanvas("c2", " ", 800, 600);
    gra_gamma_corr->SetLineColor(kRed);
    //gra_gamma->SetMaximum(0.1);
    //gra_gamma->SetMinimum(-0.1);
    gra_gamma_corr->Draw();
    gra_neutron_corr->SetLineColor(kBlue);
    gra_neutron_corr->Draw("same");
    gra_minus->SetLineColor(kBlack);
    gra_minus->SetLineWidth(3);
    gra_minus->Draw("same");

    TLegend *legend = new TLegend(0.8, 0.8, 0.9, 0.9);
    legend->AddEntry(gra_gamma_corr, "Gamma", "l");
    legend->AddEntry(gra_neutron_corr, "Neutron", "l");
    legend->AddEntry(gra_minus, "Diff", "l");
    legend->Draw();

    c2->Draw();
    //double *diff_y = gra_minus->GetY();
    //int diff_min = TMath::LocMin(250, diff_y);
    //int diff_max = TMath::LocMax(250, diff_y);
    //cout << diff_max << " " << diff_min << endl;
    cout << TMath::LocMax(250, y_g_corr) << " " << TMath::LocMax(250, y_n_corr) << endl;

}

