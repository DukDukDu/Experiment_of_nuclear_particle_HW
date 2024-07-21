double Ran_Random(TRandom *rn){

    double rx = rn->Landau();
    //if(rx < 0.5) return rx*0.6;
    //else return (rx-0.5)*0.6 + 0.7;
    if(rx <= 100) return rx;
    else return -1;
    
}

void central_limit(){
    TRandom3 *rn = new TRandom3(0);
    TCanvas *c1 = new TCanvas("c1", " ", 800, 600);

    TH1F *h0=new TH1F("h0","random distribution",100,0,100);
    for(int i=0;i<10000;i++) {

        h0->Fill(Ran_Random(rn)*100);
    }
    gPad->SetLogy(0);
    h0->Draw();
    c1->Draw();

    TH1F *h2=new TH1F("h2","sum1-2",70,0,700);
    TH1F *h5=new TH1F("h5","sum1-5",70,0,700);
    TH1F *h10=new TH1F("h10","sum1-10",70,0,700);
    TH1F *h20=new TH1F("h20","sum1-20",70,0,700);
    TH1F *h100=new TH1F("h100","sum1-100",70,0,700);
    TCanvas *c2 = new TCanvas("c2", " ", 800, 600);
    c2->Divide(3, 2);

    for(int i = 0; i < 1e6; i++){
        double rx = 0;
        for(int j = 0; j < 1000; j++){
            rx += Ran_Random(rn);
            if(j==1) h2->Fill(rx*50);
            if(j==4) h5->Fill(rx*20);
            if(j==9) h10->Fill(rx*10);
            if(j==19) h20->Fill(rx*5);
            if(j==999) h100->Fill(rx/10);
        }
    }
    c2->cd(1);
    h2->Draw();
    c2->cd(2);
    h5->Draw();
    c2->cd(3);
    h10->Draw();
    c2->cd(4);
    h20->Draw();
    c2->cd(5);
    h100->Draw();
    h100->Fit("gaus");
}