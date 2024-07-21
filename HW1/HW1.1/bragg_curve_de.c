#include "take_data.h"

void bragg_curve_de(){

    TGraph *p_h2o_de = new TGraph;
    TGraph *c12_h2o_de = new TGraph;

    TCanvas *c1 = new TCanvas("c1", " ", 800, 600);

    double E_p_de = 133.122, E_c_de = 3021.97, deltaE1 = 0.01, deltaE2 = 0.1;
    double x, y;

    int n1 = 15000, n2 = 31000;

    for (int i = 0 ; i < n1 ; i++){

        E_p_de = E_p_de - deltaE1 ;
        x = x+deltaE1/take_data_p(E_p_de);
        p_h2o_de->SetPoint(i, x/1e3, take_data_p(E_p_de)*1e3);

        if (E_p_de < 0.) break;
    }
    
    for (int j = 0; j < 1e8; j++){

        E_c_de = E_c_de - deltaE2;
        y = y + deltaE2/take_data_c(E_c_de);
        c12_h2o_de->SetPoint(j, y/1e3, take_data_c(E_c_de)*1e3);

        if(E_c_de < 0. ) break;
    }
    c1->SetLogy();
    p_h2o_de->SetMarkerStyle(0);
    p_h2o_de->SetLineColor(kGreen);
    p_h2o_de->GetXaxis()->SetTitle("mm");
    p_h2o_de->GetYaxis()->SetTitle("dE/dx(Mev/mm)");
    p_h2o_de->SetTitle("proton in H_{2}O(green) and C^{12} in H_{2}O(red)");
    p_h2o_de->GetXaxis()->SetLimits(0,200);
    p_h2o_de->SetLineWidth(2);
    p_h2o_de->SetMaximum(1e3);
    p_h2o_de->SetMinimum(1e-1);
    p_h2o_de->Draw();

    c12_h2o_de->SetLineColor(kRed);
    c12_h2o_de->SetLineWidth(2);
    c12_h2o_de->Draw("same");

    double linex[4] = {127., 127., 127., 127.}, liney[4] = {0., 10., 100., 1000.};
    TGraph *v_line = new TGraph(4, linex, liney);
       
    v_line->SetLineColor(kBlack);
    v_line->SetLineWidth(3);
    //v_line->Draw("Lsame"); 

    c1->Draw();
}