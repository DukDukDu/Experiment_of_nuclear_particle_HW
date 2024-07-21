#include "Z_1_8_dedx.h"

TGraph *ex_gra(string element, int A, int choose){

    auto *graph1 = new TGraph;
    auto *graph2 = new TGraph;
    int n = 1e8;
    double E_init = 1000., x = 0.;

    for (int i = 0; i < n; i++){

        E_init = E_init - 100*Z_1_8_dedx(element, A, E_init);
        x = x + 0.1;

        graph1->SetPoint(i, E_init, x);
        graph2->SetPoint(i, x, E_init);

        if (E_init < 0) break;
    }

    if (choose == 0) return graph1;
    else if (choose == 1) return graph2;
    else {
        return graph1;
        cout << "plz enter choose 1 or 0" << endl;
    }
}

double range_fun(string element, int A, double E, TGraph *g_range_fun){
    
    //if(Z > 8 || Z < 1 || Z > A) cout << "Plz enter right Z and A" << endl;

    TGraph *ele = g_range_fun;
   
    return (ele->Eval(0, 0, "S"))-(ele->Eval(E, 0, "S"));

}

double de_thick(string element, int A, double E, double thickness, TGraph *g1_de_thick, TGraph *g2_de_thick){
    
    double e_loss;

    if (range_fun( element, A, E, g1_de_thick) <= thickness ) {

        e_loss = E;
        //cout << "Lose all energy" << endl;
    }

    else {

        TGraph *e_x_gra = g1_de_thick;
        TGraph *x_e_gra = g2_de_thick;

        double x0, x_prime, E_prime;

        x0 = e_x_gra->Eval(E, 0, "S");
        E_prime = x_e_gra->Eval(x0+thickness, 0, "S");

        e_loss = E - E_prime;
    }

    return e_loss;

}

void test(){
    /*
    double de1, de2, de3;
    TRandom3 *ran = new TRandom3(0);
    TFile *file = new TFile("telescope.root","RECREATE");
    TTree *tree = new TTree("tree", "Ran_Events");

    tree->Branch("de1", &de1, "de1/D");
    tree->Branch("de2", &de2, "de2/D");
    tree->Branch("de3", &de3, "de3/D");

    for(int j = 0; j < 1000; j ++){

        int Z = ceil(ran->Uniform(0, 8));
        string res;
        stringstream ss;
        ss << Z, ss >> res;
        
        int A = 2*Z + ceil(ran->Uniform(-2, 2));

        auto *e_x_he = new TGraph;
        e_x_he->Clear();
        e_x_he = ex_gra(res, A, 0);
        auto *x_e_he = new TGraph;
        x_e_he->Clear();
        x_e_he = ex_gra(res, A, 1);

        for(int i =0; i < 1000; i++){
            
            double E1_he = 35*A*ran->Rndm();
            de1 = de_thick(res, A, E1_he, 1, e_x_he, x_e_he);
        
            double E2_he = E1_he - de1;
            de2 = de_thick(res, A, E2_he, 0.5, e_x_he, x_e_he);
        
            double E3_he = E2_he - de2;
            de3 = de_thick(res, A, E3_he, 1, e_x_he, x_e_he);

            tree->Fill();
    }

    }

    tree->Write();
    file->Close();
*/
    TFile *new_f = new TFile("telescope.root", "READ");
    TTree *new_tree = (TTree*)new_f->Get("tree");
    
    TCanvas *c = new TCanvas("c", " ", 800, 600);
    c->Divide(3,1);
    
    c->cd(1);
    new_tree->Draw("de1:de2>>h1(500,0,250,1000,0,500)", "de2 > 0", "colz");
    c->cd(2);
    new_tree->Draw("de2:de3>>h2(500,0,250,500,0,250)", "de3 > 0", "colz");
    c->cd(3);
    new_tree->Draw("de1:(de1+de2+de3)>>h3(500,0,600,500,0,500)", "de2>0&&de3>0", "colz");
}
