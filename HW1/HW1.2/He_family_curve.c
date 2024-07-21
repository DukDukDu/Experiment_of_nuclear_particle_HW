void He_family_curve(){

    TGraph *he4_de = new TGraph("he4_si.txt");
    TGraph *he6_de = new TGraph("he6_si.txt");
    TGraph *he8_de = new TGraph("he8_si.txt");

    TCanvas *c1 = new TCanvas("c1", " ", 800, 600);

    he4_de->SetLineColor(kGreen);
    he4_de->GetXaxis()->SetTitle("E");
    he4_de->GetYaxis()->SetTitle("dE/dX");
    he4_de->Draw();

    he6_de->SetLineColor(kRed);
    he6_de->Draw("same");

    he8_de->SetLineColor(kBlack);
    he8_de->Draw("same");

    c1->SetLogx();
    c1->SetLogy();

    c1->Draw();//可以看出三条线重合的很好
}