#include "take_data.h"

void bragg_curve(){

    TGraph *p_h2o = new TGraph;
    TGraph *c12_h2o = new TGraph;

    TCanvas *c1 = new TCanvas("c1", " ", 800, 600);

    double E_p = 133.122, E_c = 3021.97;
    double x1 = 0., x2 = 0.;

    for (int i = 0; i < 1e8; i++){
        
        if(E_p > 100.) {

            E_p = E_p - take_data_p(E_p)*10;
            x1 = x1+0.01;
        }
        else {

            E_p = E_p - take_data_p(E_p);
            x1 = x1+0.001;
        }
        
        p_h2o->SetPoint(i , x1, take_data_p(E_p)*1000);

        if( take_data_p(E_p) < 0.) break;
    }
    
    for (int j = 0; j < 1e8; j++){

        if(E_c > 2000.){

            E_c = E_c - take_data_c(E_c)*10;
            x2 = x2 + 0.01;
        }
        else{

            E_c = E_c -take_data_c(E_c)*1;
            x2 = x2 + 0.001;
        }
        
        c12_h2o->SetPoint(j, x2, take_data_c(E_c)*1000);

        if( take_data_c(E_c) < 0. ) break;
    }


    c1->SetLogy();

    p_h2o->SetMarkerStyle(0);
    p_h2o->SetLineColor(kGreen);
    p_h2o->GetXaxis()->SetTitle("mm");
    p_h2o->GetYaxis()->SetTitle("dE/dx(Mev/mm)");
    p_h2o->SetTitle("proton in H_{2}O(green) and C^{12} in H_{2}O(red)");
    p_h2o->GetXaxis()->SetLimits(0,200);
    p_h2o->SetLineWidth(2);
    p_h2o->SetMaximum(1e6);
    p_h2o->SetMinimum(1e-1);
    p_h2o->Draw();

    c12_h2o->SetLineColor(kRed);
    c12_h2o->SetLineWidth(2);
    c12_h2o->Draw("same");

    double x[4] = {127., 127., 127., 127.}, y[4] = {0., 10., 100., 1000.};
    TGraph *v_line = new TGraph(4, x, y);
       
    v_line->SetLineColor(kBlack);
    v_line->SetLineWidth(3);
    //v_line->Draw("Lsame"); 

 /*
    TLegend *legend = new TLegend(0.1, 0.8, 0.3, 0.9);
    legend->AddEntry("p_h2o", "proton in H_{2}O", "l");
    legend->AddEntry("c12_h2o", "C^{12} in H_{2}O", "l");
    legend->Draw();
*/
    c1->Draw();

}