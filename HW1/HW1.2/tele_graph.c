#include "Z_1_8_dedx.h"
#include "math.h"

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

TH2I *ele_family(string element, int A, TH2I *hist, int choose){

    TRandom3 *ran = new TRandom3(0);

    auto *e_x_he = new TGraph;
    e_x_he->Clear();
    e_x_he = ex_gra(element, A, 0);
    auto *x_e_he = new TGraph;
    x_e_he->Clear();
    x_e_he = ex_gra(element, A, 1);

    double de1, de2, de3;

    for (int i = 0; i < 1000; i++){

        double E1_he = 35*A*ran->Rndm();
        de1 = de_thick(element, A, E1_he, 1, e_x_he, x_e_he);
        double E2_he = E1_he - de1;// + 1.18*sqrt(0.1*3.6*1e-6*de1)*ran->Gaus();
        if(E2_he >= 0){
            de2 = de_thick(element, A, E2_he, 0.5, e_x_he, x_e_he);
            double E3_he = E2_he - de2;// + 1.18*sqrt(0.1*3.6*1e-6*de2)*ran->Gaus();
            if(E3_he >= 0){
                de3 = de_thick(element, A, E3_he, 1, e_x_he, x_e_he);
            }
            else de3 = 0;
        }

        else de2 = 0, de3 = 0;

        double de1_blur, de2_blur, de3_blur;
        de1_blur = ran->Gaus(de1, 0.01/2.35*sqrt(5*de1));
        de2_blur = ran->Gaus(de2, 0.01/2.35*sqrt(5*de2));
        de3_blur = ran->Gaus(de3, 0.01/2.35*sqrt(5*de3));
        
        if (choose == 0 ){
            if (de2_blur > 0.)
            hist->Fill(de2_blur, de1_blur);
        }

        if (choose == 1){
            if (de2_blur > 0. && de3_blur > 0.)
            hist->Fill(de3_blur, de2_blur);
        }

        if (choose == 2){
            if(de2_blur > 0)
            hist->Fill(de1_blur+de2_blur+de3_blur, de1_blur);
        }
    }

    return hist;
}

void tele_graph(){

    TH2I *he4 = new TH2I("he4", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *he6 = new TH2I("he6", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *he8 = new TH2I("he8", "de1_de2", 600, 0, 600, 600, 0, 600);

    TH2I *n12 = new TH2I("n12", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *n13 = new TH2I("n13", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *n14 = new TH2I("n14", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *n15 = new TH2I("n15", "de1_de2", 600, 0, 600, 600, 0, 600);

    TH2I *c11 = new TH2I("c11", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *c12 = new TH2I("c12", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *c13 = new TH2I("c13", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *c14 = new TH2I("c14", "de1_de2", 600, 0, 600, 600, 0, 600);

    TH2I *b10 = new TH2I("b10", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *b11 = new TH2I("b11", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *b12 = new TH2I("b12", "de1_de2", 600, 0, 600, 600, 0, 600);

    TH2I *be7 = new TH2I("be7", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *be9 = new TH2I("be9", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *be10 = new TH2I("be10", "de1_de2", 600, 0, 600, 600, 0, 600);

    TH2I *li6 = new TH2I("li6", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *li7 = new TH2I("li7", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *li8 = new TH2I("li8", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *li9 = new TH2I("li9", "de1_de2", 600, 0, 600, 600, 0, 600);

    TH2I *h1 = new TH2I("h1", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *h2 = new TH2I("h2", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *h3 = new TH2I("h3", "de1_de2", 600, 0, 600, 600, 0, 600);

    TH2I *o14 = new TH2I("o14", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *o15 = new TH2I("o15", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *o16 = new TH2I("o16", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *o17 = new TH2I("o17", "de1_de2", 600, 0, 600, 600, 0, 600);
    TH2I *o18 = new TH2I("o18", "de1_de2", 600, 0, 600, 600, 0, 600);



    TCanvas *c1 = new TCanvas("c1", " ", 800, 600);
    ele_family("2", 4, he4, 2)->Draw("colz");
    ele_family("2", 6, he6, 2)->Draw("same colz");
    ele_family("2", 8, he8, 2)->Draw("same colz");

    ele_family("8", 14, o14, 2)->Draw("same colz");
    ele_family("8", 15, o15, 2)->Draw("same colz");
    ele_family("8", 16, o16, 2)->Draw("same colz");
    ele_family("8", 17, o17, 2)->Draw("same colz");
    ele_family("8", 18, o18, 2)->Draw("same colz");

    ele_family("7", 12, n12, 2)->Draw("same colz");
    ele_family("7", 13, n13, 2)->Draw("same colz");
    ele_family("7", 14, n14, 2)->Draw("same colz");
    ele_family("7", 15, n15, 2)->Draw("same colz");

    ele_family("6", 11, c11, 2)->Draw("same colz");
    ele_family("6", 12, c12, 2)->Draw("same colz");
    ele_family("6", 13, c13, 2)->Draw("same colz");
    ele_family("6", 14, c14, 2)->Draw("same colz");

    ele_family("5", 10, b10, 2)->Draw("same colz");
    ele_family("5", 11, b11, 2)->Draw("same colz");
    ele_family("5", 12, b12, 2)->Draw("same colz");

    ele_family("4", 7, be7, 2)->Draw("same colz");
    ele_family("4", 9, be9, 2)->Draw("same colz");
    ele_family("4", 10, be10, 2)->Draw("same colz");

    ele_family("3", 6, li6, 2)->Draw("same colz");
    ele_family("3", 7, li7, 2)->Draw("same colz");
    ele_family("3", 8, li8, 2)->Draw("same colz");
    ele_family("3", 9, li9, 2)->Draw("same colz");

    ele_family("1", 1, h1, 2)->Draw("same colz");
    ele_family("1", 2, h2, 2)->Draw("same colz");
    ele_family("1", 3, h3, 2)->Draw("same colz");

    c1->SetLogz();
    c1->Draw();

    //cout << range_fun(4, 9, 10, ex_gra(4, 9, 0)) << endl;


   
}

